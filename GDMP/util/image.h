#ifndef GDMP_IMAGE_UTIL
#define GDMP_IMAGE_UTIL

#include <memory>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"

#include "mediapipe/framework/formats/image.h"

using namespace godot;

std::unique_ptr<mediapipe::ImageFrame> get_image_frame(Ref<godot::Image> image);
mediapipe::Image get_image(Ref<godot::Image> image);

Ref<godot::Image> get_image(const mediapipe::ImageFrameSharedPtr image);
Ref<godot::Image> get_image(const mediapipe::Image &image);

#endif
