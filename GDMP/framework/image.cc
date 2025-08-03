#include "image.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

#include "GDMP/util/image.h"

void MediaPipeImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_gpu_image"), &MediaPipeImage::is_gpu_image);
	ClassDB::bind_method(D_METHOD("convert_to_cpu"), &MediaPipeImage::convert_to_cpu);
	ClassDB::bind_method(D_METHOD("get_image"), &MediaPipeImage::get_godot_image);
	ClassDB::bind_method(D_METHOD("set_image", "image"), &MediaPipeImage::set_godot_image);
	ClassDB::bind_method(D_METHOD("get_packet"), &MediaPipeImage::get_packet);
	ClassDB::bind_method(D_METHOD("get_image_frame_packet"), &MediaPipeImage::get_image_frame_packet);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, Image::get_class_static()), "set_image", "get_image");
}

bool MediaPipeImage::_register_packet_types = register_packet_types();

bool MediaPipeImage::register_packet_types() {
#if !MEDIAPIPE_DISABLE_GPU
	std::function<Variant(const mediapipe::Packet &)> get_gpu_buffer = [](const mediapipe::Packet &packet) {
		const mediapipe::GpuBuffer &gpu_buffer = packet.Get<mediapipe::GpuBuffer>();
		return memnew(MediaPipeImage(gpu_buffer));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::GpuBuffer>, get_gpu_buffer });
#endif
	std::function<Variant(const mediapipe::Packet &)> get_image = [](const mediapipe::Packet &packet) {
		const mediapipe::Image &image = packet.Get<mediapipe::Image>();
		return memnew(MediaPipeImage(image));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::Image>, get_image });
	std::function<Variant(const mediapipe::Packet &)> get_image_frame = [](const mediapipe::Packet &packet) {
		auto image_frame = std::make_unique<mediapipe::ImageFrame>();
		image_frame->CopyFrom(packet.Get<mediapipe::ImageFrame>(), mediapipe::ImageFrame::kDefaultAlignmentBoundary);
		return memnew(MediaPipeImage(std::move(image_frame)));
	};
	MediaPipePacket::add_packet_type({ MediaPipePacket::validate_packet_type<mediapipe::ImageFrame>, get_image_frame });
	return true;
};

MediaPipeImage::MediaPipeImage() = default;

MediaPipeImage::MediaPipeImage(mediapipe::Image image) {
	this->image = image;
}

MediaPipeImage::MediaPipeImage(mediapipe::ImageFrameSharedPtr image_frame) {
	this->image = mediapipe::Image(image_frame);
}

#if !MEDIAPIPE_DISABLE_GPU
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
MediaPipeImage::MediaPipeImage(CVPixelBufferRef pixel_buffer) {
	this->image = mediapipe::Image(pixel_buffer);
}
#endif
MediaPipeImage::MediaPipeImage(mediapipe::GpuBuffer gpu_buffer) {
	this->image = mediapipe::Image(gpu_buffer);
}
#endif

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
	return memnew(MediaPipePacket(packet));
}

Ref<MediaPipePacket> MediaPipeImage::get_image_frame_packet() {
	ERR_FAIL_NULL_V(image, nullptr);
	mediapipe::ImageFrameSharedPtr image_frame_ptr = image.GetImageFrameSharedPtr();
	auto image_frame = std::make_unique<mediapipe::ImageFrame>();
	image_frame->CopyFrom(*image_frame_ptr, mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	return memnew(MediaPipePacket(mediapipe::Adopt(image_frame.release())));
}

mediapipe::Image MediaPipeImage::get_mediapipe_image() {
	return image;
}
