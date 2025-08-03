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
}

MediaPipeImage *MediaPipeImage::_new(mediapipe::Image image) {
	MediaPipeImage *i = MediaPipeImage::_new();
	i->image = image;
	return i;
}

bool MediaPipeImage::_register_packet_types = register_packet_types();

bool MediaPipeImage::register_packet_types() {
#if !MEDIAPIPE_DISABLE_GPU
	std::function<Variant(const mediapipe::Packet &)> get_gpu_buffer = [](const mediapipe::Packet &packet) {
		const mediapipe::GpuBuffer &gpu_buffer = packet.Get<mediapipe::GpuBuffer>();
		return Ref(MediaPipeImage::_new(gpu_buffer));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::GpuBuffer>, get_gpu_buffer });
#endif
	std::function<Variant(const mediapipe::Packet &)> get_image = [](const mediapipe::Packet &packet) {
		const mediapipe::Image &image = packet.Get<mediapipe::Image>();
		return Ref(MediaPipeImage::_new(image));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::Image>, get_image });
	std::function<Variant(const mediapipe::Packet &)> get_image_frame = [](const mediapipe::Packet &packet) {
		auto image_frame = std::make_unique<mediapipe::ImageFrame>();
		image_frame->CopyFrom(packet.Get<mediapipe::ImageFrame>(), mediapipe::ImageFrame::kDefaultAlignmentBoundary);
		return Ref(MediaPipeImage::_new(std::move(image_frame)));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::ImageFrame>, get_image_frame });
	return true;
};

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
