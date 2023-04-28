#include "gpu_helper.h"

#include "godot_cpp/core/class_db.hpp"

#include "mediapipe/gpu/gpu_buffer.h"
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

#include "mediapipe/GDMP/util/image.h"

using namespace godot;

void MediaPipeGPUHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize"), &MediaPipeGPUHelper::initialize);
	ClassDB::bind_method(D_METHOD("get_gpu_frame"), &MediaPipeGPUHelper::get_gpu_frame);
	ClassDB::bind_method(D_METHOD("make_packet_from_image"), &MediaPipeGPUHelper::make_packet_from_image);
}

MediaPipeGPUHelper::MediaPipeGPUHelper() = default;

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeGPUHelper::MediaPipeGPUHelper(mediapipe::GpuResources *gpu_resource) {
	gpu_helper.InitializeForTest(gpu_resource);
}
#endif

MediaPipeGPUHelper::~MediaPipeGPUHelper() = default;

void MediaPipeGPUHelper::initialize(Ref<MediaPipeGPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND(gpu_resources.is_null());
	gpu_helper.InitializeForTest(gpu_resources->get_gpu_resources().get());
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

Ref<Image> MediaPipeGPUHelper::get_gpu_frame(Ref<MediaPipePacket> packet) {
	Ref<Image> image;
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND_V(!packet->get_packet().ValidateAsType<mediapipe::GpuBuffer>().ok(), image);
	auto &gpu_frame = packet->get_packet().Get<mediapipe::GpuBuffer>();
	std::unique_ptr<mediapipe::ImageFrame> image_frame;
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
	image_frame = CreateImageFrameForCVPixelBuffer(mediapipe::GetCVPixelBufferRef(gpu_frame));
#else
	ERR_FAIL_COND_V(!gpu_helper.Initialized(), image);
	gpu_helper.RunInGlContext([this, &gpu_frame, &image_frame]() -> void {
		auto texture = gpu_helper.CreateSourceTexture(gpu_frame);
		image_frame = std::make_unique<mediapipe::ImageFrame>(
				mediapipe::ImageFormatForGpuBufferFormat(gpu_frame.format()),
				gpu_frame.width(), gpu_frame.height(),
				mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
		gpu_helper.BindFramebuffer(texture);
		const auto info = mediapipe::GlTextureInfoForGpuBufferFormat(
				gpu_frame.format(), 0, gpu_helper.GetGlVersion());
		glReadPixels(0, 0, texture.width(), texture.height(), info.gl_format,
				info.gl_type, image_frame->MutablePixelData());
		glFlush();
		// We explicitly unbind framebuffer here, otherwise it can crash GPU inference on Android.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		texture.Release();
	});
#endif
	image = to_image(*image_frame);
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return image;
}

Ref<MediaPipePacket> MediaPipeGPUHelper::make_packet_from_image(Ref<Image> image) {
	return make_packet_from_image_frame(std::move(to_image_frame(image)));
}

Ref<MediaPipePacket> MediaPipeGPUHelper::make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame) {
	Ref<MediaPipePacket> packet;
#if !MEDIAPIPE_DISABLE_GPU
	auto gpu_frame = gpu_helper.GpuBufferWithImageFrame(std::move(image_frame));
	packet = Ref<MediaPipePacket>(memnew(MediaPipePacket(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame))));
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return packet;
}
