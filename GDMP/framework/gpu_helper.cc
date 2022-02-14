#include "gpu_helper.h"

#include "mediapipe/gpu/gpu_buffer.h"

#include "../util/image.h"

using namespace godot;

void GPUHelper::_register_methods() {
	register_method("get_gpu_frame", &GPUHelper::get_gpu_frame);
	register_method("make_packet_from_image", &GPUHelper::make_packet_from_image);
}

GPUHelper *GPUHelper::_new(mediapipe::GpuResources *gpu_resource) {
	GPUHelper *helper = GPUHelper::_new();
	helper->gpu_helper.InitializeForTest(gpu_resource);
	return helper;
}

void GPUHelper::_init() {}

Ref<Image> GPUHelper::get_gpu_frame(Ref<Packet> packet) {
	Ref<Image> image;
	gpu_helper.RunInGlContext([this, &packet, &image]() -> void {
		auto image_frame = packet->get_packet().Get<mediapipe::GpuBuffer>().AsImageFrame();
		image = to_image(*image_frame);
	});
	return image;
}

Ref<Packet> GPUHelper::make_packet_from_image(Ref<Image> image) {
	mediapipe::ImageFrame image_frame = to_image_frame(image);
	return make_packet_from_image_frame(image_frame);
}

Ref<Packet> GPUHelper::make_packet_from_image_frame(const mediapipe::ImageFrame &image_frame) {
	Ref<Packet> packet;
	gpu_helper.RunInGlContext([this, &image_frame, &packet]() -> void {
		auto texture = gpu_helper.CreateSourceTexture(image_frame);
		auto gpu_frame = texture.GetFrame<mediapipe::GpuBuffer>();
		glFlush();
		texture.Release();
		packet = Ref<Packet>(Packet::_new(mediapipe::Adopt(gpu_frame.release())));
	});
	return packet;
}
