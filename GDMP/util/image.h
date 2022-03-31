#ifndef GDMP_IMAGE_UTIL
#define GDMP_IMAGE_UTIL

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"

#include "mediapipe/framework/formats/image_frame.h"

namespace godot {

// Convert mediapipe::ImageFrame to godot::Image
inline Ref<Image> to_image(const mediapipe::ImageFrame &image_frame) {
	Ref<Image> image = Ref<Image>(Image::_new());
	PoolByteArray data;
	int width = image_frame.Width();
	int height = image_frame.Height();
	int channel = image_frame.NumberOfChannels();
	data.resize(width * height * channel);
	image_frame.CopyToBuffer(data.write().ptr(), width * height * channel);
	image->create_from_data(width, height, false, Image::FORMAT_RGBA8, data);
	return image;
}

// Convert godot::Image to mediapipe::ImageFrame
inline mediapipe::ImageFrame to_image_frame(Ref<Image> image) {
	mediapipe::ImageFrame image_frame;
	image_frame.CopyPixelData(mediapipe::ImageFormat::SRGBA, image->get_width(), image->get_height(),
			image->get_data().read().ptr(), mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
	return image_frame;
};

} //namespace godot

#endif
