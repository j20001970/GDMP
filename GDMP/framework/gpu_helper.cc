#include "gpu_helper.h"

#include "mediapipe/gpu/gpu_buffer.h"
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

#include "../util/image.h"

using namespace godot;

void GPUHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize"), &GPUHelper::initialize);
	ClassDB::bind_method(D_METHOD("get_gpu_frame"), &GPUHelper::get_gpu_frame);
	ClassDB::bind_method(D_METHOD("make_packet_from_image"), &GPUHelper::make_packet_from_image);
}

GPUHelper::GPUHelper() = default;

GPUHelper::GPUHelper(mediapipe::GpuResources *gpu_resource) {
	gpu_helper.InitializeForTest(gpu_resource);
}

GPUHelper::~GPUHelper() = default;

void GPUHelper::initialize(Ref<Graph> graph) {
	gpu_helper.InitializeForTest(graph->get_gpu_resources().get());
}

Ref<Image> GPUHelper::get_gpu_frame(Ref<Packet> packet) {
	Ref<Image> image;
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
	return image;
}

Ref<Packet> GPUHelper::make_packet_from_image(Ref<Image> image) {
	return make_packet_from_image_frame(std::move(to_image_frame(image)));
}

Ref<Packet> GPUHelper::make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame) {
	auto gpu_frame = gpu_helper.GpuBufferWithImageFrame(std::move(image_frame));
	return new Packet(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame));
}
