#include "image.h"

#include "godot_cpp/core/error_macros.hpp"

using namespace util;

std::unique_ptr<mediapipe::ImageFrame> util::get_image_frame(Ref<godot::Image> image) {
	std::unique_ptr<mediapipe::ImageFrame> image_frame = nullptr;
	ERR_FAIL_COND_V(image.is_null(), image_frame);
	mediapipe::ImageFormat::Format image_format;
	switch (image->get_format()) {
		case godot::Image::FORMAT_L8:
			image_format = mediapipe::ImageFormat::GRAY8;
			break;
		case godot::Image::FORMAT_RGB8:
			image_format = mediapipe::ImageFormat::SRGB;
			break;
		case godot::Image::FORMAT_RGBA8:
			image_format = mediapipe::ImageFormat::SRGBA;
			break;
		case godot::Image::FORMAT_RF:
			image_format = mediapipe::ImageFormat::VEC32F1;
			break;
		default:
			ERR_PRINT("Unsupported image format.");
			return image_frame;
	}
	image_frame = std::make_unique<mediapipe::ImageFrame>();
	image_frame->CopyPixelData(
			image_format, image->get_width(), image->get_height(),
			image->get_data().ptr(), mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	return std::move(image_frame);
}

mediapipe::Image util::get_image(Ref<godot::Image> image) {
	return mediapipe::Image(get_image_frame(image));
}

Ref<godot::Image> util::get_image(const mediapipe::ImageFrameSharedPtr image_frame) {
	Ref<godot::Image> image;
	ERR_FAIL_NULL_V(image_frame, image);
	PackedByteArray data;
	data.resize(image_frame->PixelDataSizeStoredContiguously());
	godot::Image::Format image_format;
	switch (image_frame->Format()) {
		case mediapipe::ImageFormat::SRGB:
			image_format = godot::Image::FORMAT_RGB8;
			image_frame->CopyToBuffer(data.ptrw(), data.size());
			break;
		case mediapipe::ImageFormat::SRGBA:
			image_format = godot::Image::FORMAT_RGBA8;
			image_frame->CopyToBuffer(data.ptrw(), data.size());
			break;
		case mediapipe::ImageFormat::GRAY8:
			image_format = godot::Image::FORMAT_L8;
			image_frame->CopyToBuffer(data.ptrw(), data.size());
			break;
		case mediapipe::ImageFormat::VEC32F1:
			image_format = godot::Image::FORMAT_RF;
			image_frame->CopyToBuffer((float *)data.ptrw(), data.size());
			break;
		default:
			ERR_PRINT("Unsupported image format.");
			return image;
	}
	image = godot::Image::create_from_data(image_frame->Width(), image_frame->Height(), false, image_format, data);
	return image;
}

Ref<godot::Image> util::get_image(const mediapipe::Image &image) {
	return get_image(image.GetImageFrameSharedPtr());
}
