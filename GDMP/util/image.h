#ifndef GDMP_IMAGE_UTIL
#define GDMP_IMAGE_UTIL

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"

#include "mediapipe/framework/formats/image_frame.h"

using namespace godot;

// Convert mediapipe::ImageFrame to godot::Image.
// The image format must be either ImageFormat::SRGB or ImageFormat::SRGBA.
inline Ref<Image> to_image(const mediapipe::ImageFrame &image_frame) {
	Ref<Image> image = Ref<Image>(Image::_new());
	Image::Format image_format;
	switch (image_frame.Format()) {
		case mediapipe::ImageFormat::SRGB:
			image_format = Image::FORMAT_RGB8;
			break;
		case mediapipe::ImageFormat::SRGBA:
			image_format = Image::FORMAT_RGBA8;
			break;
		default:
			ERR_PRINT("Unsupported type to convert image.");
			return image;
	}
	PoolByteArray data;
	data.resize(image_frame.PixelDataSize());
	image_frame.CopyToBuffer(data.write().ptr(), data.size());
	image->create_from_data(image_frame.Width(), image_frame.Height(), false, image_format, data);
	return image;
}

// Convert godot::Image to mediapipe::ImageFrame.
// The image format must be either Image::FORMAT_RGB8 or Image::FORMAT_RGBA8.
inline std::unique_ptr<mediapipe::ImageFrame> to_image_frame(Ref<Image> image) {
	std::unique_ptr<mediapipe::ImageFrame> image_frame = std::make_unique<mediapipe::ImageFrame>();
	mediapipe::ImageFormat::Format image_format;
	switch (image->get_format()) {
		case Image::FORMAT_RGB8:
			image_format = mediapipe::ImageFormat::SRGB;
			break;
		case Image::FORMAT_RGBA8:
			image_format = mediapipe::ImageFormat::SRGBA;
			break;
		default:
			ERR_PRINT("Unsupported type to convert image.");
			return image_frame;
	}
	image_frame->CopyPixelData(image_format, image->get_width(), image->get_height(),
			image->get_data().read().ptr(), mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	return image_frame;
};

#endif
