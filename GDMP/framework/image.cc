#include "image.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_gpu_image"), &MediaPipeImage::is_gpu_image);
	ClassDB::bind_method(D_METHOD("get_image"), &MediaPipeImage::get_godot_image);
	ClassDB::bind_method(D_METHOD("set_image"), &MediaPipeImage::set_godot_image);
	ClassDB::bind_method(D_METHOD("make_packet"), &MediaPipeImage::make_packet);
	ClassDB::bind_method(D_METHOD("make_image_frame_packet"), &MediaPipeImage::make_image_frame_packet);
	ClassDB::bind_method(D_METHOD("set_image_from_packet"), &MediaPipeImage::set_image_from_packet);
}

MediaPipeImage::MediaPipeImage() = default;

MediaPipeImage::MediaPipeImage(Ref<godot::Image> image) {
	set_godot_image(image);
}

MediaPipeImage::MediaPipeImage(mediapipe::Image image) {
	this->image = image;
}

MediaPipeImage::MediaPipeImage(mediapipe::ImageFrameSharedPtr image_frame) {
	this->image = mediapipe::Image(image_frame);
}

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeImage::MediaPipeImage(mediapipe::GpuBuffer gpu_buffer) {
	this->image = mediapipe::Image(gpu_buffer);
}
#endif

MediaPipeImage::~MediaPipeImage() = default;

bool MediaPipeImage::is_gpu_image() {
	return image.UsesGpu();
}

Ref<Image> MediaPipeImage::get_godot_image() {
	Ref<Image> godot_image;
	if (is_gpu_image())
		ERR_FAIL_COND_V(!image.ConvertToCpu(), godot_image);

	mediapipe::ImageFrameSharedPtr image_frame = image.GetImageFrameSharedPtr();
	ERR_FAIL_COND_V(image_frame == nullptr, godot_image);
	Image::Format image_format;
	switch (image.image_format()) {
		case mediapipe::ImageFormat::SRGB:
			image_format = Image::FORMAT_RGB8;
			break;
		case mediapipe::ImageFormat::SRGBA:
			image_format = Image::FORMAT_RGBA8;
			break;
		default:
			ERR_PRINT("Unsupported image format.");
			return godot_image;
	}
	PackedByteArray data;
	data.resize(image_frame->PixelDataSize());
	image_frame->CopyToBuffer(data.ptrw(), data.size());
	godot_image = godot::Image::create_from_data(image_frame->Width(), image_frame->Height(), false, image_format, data);
	return godot_image;
}

void MediaPipeImage::set_godot_image(Ref<godot::Image> image) {
	ERR_FAIL_COND(image.is_null());
	mediapipe::ImageFormat::Format image_format;
	switch (image->get_format()) {
		case Image::FORMAT_RGB8:
			image_format = mediapipe::ImageFormat::SRGB;
			break;
		case Image::FORMAT_RGBA8:
			image_format = mediapipe::ImageFormat::SRGBA;
			break;
		default:
			ERR_PRINT("Unsupported image format.");
			return;
	}
	auto image_frame = std::make_shared<mediapipe::ImageFrame>();
	image_frame->CopyPixelData(
			image_format, image->get_width(), image->get_height(),
			image->get_data().ptr(), mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	this->image = mediapipe::Image(image_frame);
}

Ref<MediaPipePacket> MediaPipeImage::make_packet() {
	mediapipe::Packet packet = mediapipe::MakePacket<mediapipe::Image>(image);
	return memnew(MediaPipePacket(packet));
}

Ref<MediaPipePacket> MediaPipeImage::make_image_frame_packet() {
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
