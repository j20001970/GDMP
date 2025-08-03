#include "image.h"

#include "Defs.hpp"

#include "GDMP/util/image.h"

void MediaPipeImage::_register_methods() {
	register_method("is_gpu_image", &MediaPipeImage::is_gpu_image);
	register_method("convert_to_cpu", &MediaPipeImage::convert_to_cpu);
	register_method("get_image", &MediaPipeImage::get_godot_image);
	register_method("set_image", &MediaPipeImage::set_godot_image);
	register_method("get_packet", &MediaPipeImage::get_packet);
	register_method("get_image_frame_packet", &MediaPipeImage::get_image_frame_packet);
	register_method("set_image_from_packet", &MediaPipeImage::set_image_from_packet);
}

MediaPipeImage *MediaPipeImage::_new(mediapipe::Image image) {
	MediaPipeImage *i = MediaPipeImage::_new();
	i->image = image;
	return i;
}

MediaPipeImage *MediaPipeImage::_new(mediapipe::ImageFrameSharedPtr image_frame) {
	MediaPipeImage *i = MediaPipeImage::_new();
	i->image = mediapipe::Image(image_frame);
	return i;
}

#if !MEDIAPIPE_DISABLE_GPU
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
MediaPipeImage *MediaPipeImage::_new(CVPixelBufferRef pixel_buffer) {
	MediaPipeImage *i = MediaPipeImage::_new();
	i->image = mediapipe::Image(pixel_buffer);
	return i;
}
#endif
MediaPipeImage *MediaPipeImage::_new(mediapipe::GpuBuffer gpu_buffer) {
	MediaPipeImage *i = MediaPipeImage::_new();
	i->image = mediapipe::Image(gpu_buffer);
	return i;
}
#endif

void MediaPipeImage::_init() {}

bool MediaPipeImage::is_gpu_image() {
	return image.UsesGpu();
}

void MediaPipeImage::convert_to_cpu() {
	image.ConvertToCpu();
}

Ref<Image> MediaPipeImage::get_godot_image() {
	if (image == nullptr) {
		return nullptr;
	}
	if (is_gpu_image()) {
		convert_to_cpu();
	}
	return util::get_image(image);
}

void MediaPipeImage::set_godot_image(Ref<Image> image) {
	if (image.is_null()) {
		this->image = nullptr;
		return;
	}
	this->image = util::get_image(image);
}

void MediaPipeImage::set_image_from_packet(Ref<MediaPipePacket> packet) {
	ERR_FAIL_COND(packet.is_null());
	mediapipe::Packet p = packet->get_packet();
	if (p.ValidateAsType<mediapipe::Image>().ok()) {
		auto image = p.Get<mediapipe::Image>();
		this->image = image;
	} else if (p.ValidateAsType<mediapipe::ImageFrame>().ok()) {
		auto &image_frame = p.Get<mediapipe::ImageFrame>();
		auto image = std::make_shared<mediapipe::ImageFrame>();
		image->CopyFrom(image_frame, image_frame.kGlDefaultAlignmentBoundary);
		this->image = mediapipe::Image(image);
#if !MEDIAPIPE_DISABLE_GPU
	} else if (p.ValidateAsType<mediapipe::GpuBuffer>().ok()) {
		auto gpu_buffer = p.Get<mediapipe::GpuBuffer>();
		this->image = mediapipe::Image(gpu_buffer);
#endif
	} else
		ERR_PRINT("Unsupported packet type.");
}

Ref<MediaPipePacket> MediaPipeImage::get_packet() {
	mediapipe::Packet packet = mediapipe::MakePacket<mediapipe::Image>(image);
	return Ref(MediaPipePacket::_new(packet));
}

Ref<MediaPipePacket> MediaPipeImage::get_image_frame_packet() {
	ERR_FAIL_NULL_V(image, nullptr);
	mediapipe::ImageFrameSharedPtr image_frame_ptr = image.GetImageFrameSharedPtr();
	auto image_frame = std::make_unique<mediapipe::ImageFrame>();
	image_frame->CopyFrom(*image_frame_ptr, mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	return Ref(MediaPipePacket::_new(mediapipe::Adopt(image_frame.release())));
}

mediapipe::Image MediaPipeImage::get_mediapipe_image() {
	return image;
}
