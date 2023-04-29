#include "gpu_helper.h"

#include "mediapipe/gpu/gpu_buffer.h"
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

#include "GDMP/util/image.h"

using namespace godot;

void GPUHelper::_register_methods() {
	register_method("initialize", &GPUHelper::initialize);
	register_method("get_gpu_frame", &GPUHelper::get_gpu_frame);
	register_method("make_packet_from_image", &GPUHelper::make_packet_from_image);
}

#if !MEDIAPIPE_DISABLE_GPU
GPUHelper *GPUHelper::_new(mediapipe::GpuResources *gpu_resource) {
	GPUHelper *helper = GPUHelper::_new();
	helper->gpu_helper.InitializeForTest(gpu_resource);
	return helper;
}
#endif

void GPUHelper::_init() {}

void GPUHelper::initialize(Ref<GPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND(gpu_resources.is_null());
	gpu_helper.InitializeForTest(gpu_resources->get_gpu_resources().get());
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

Ref<Image> GPUHelper::get_gpu_frame(Ref<Packet> packet) {
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

Ref<Packet> GPUHelper::make_packet_from_image(Ref<Image> image) {
	return make_packet_from_image_frame(std::move(to_image_frame(image)));
}

Ref<Packet> GPUHelper::make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame) {
	Ref<Packet> packet;
#if !MEDIAPIPE_DISABLE_GPU
	auto gpu_frame = gpu_helper.GpuBufferWithImageFrame(std::move(image_frame));
	packet = Ref<Packet>(Packet::_new(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame)));
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return packet;
}
