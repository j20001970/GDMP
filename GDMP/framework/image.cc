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
	ClassDB::bind_method(D_METHOD("set_image_from_packet", "packet"), &MediaPipeImage::set_image_from_packet);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, Image::get_class_static()), "set_image", "get_image");
}

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
	Ref<MediaPipePacket> packet;
	auto image_frame = image.GetImageFrameSharedPtr();
	ERR_FAIL_COND_V(image_frame == nullptr, packet);
	auto image = std::make_unique<mediapipe::ImageFrame>();
	image->CopyFrom(*image_frame, mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	packet = Ref(memnew(MediaPipePacket(mediapipe::Adopt(image.release()))));
	return packet;
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

mediapipe::Image MediaPipeImage::get_mediapipe_image() {
	return image;
}
