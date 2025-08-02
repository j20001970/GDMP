#ifndef GDMP_UTIL_IMAGE
#define GDMP_UTIL_IMAGE

#include <memory>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"

#include "mediapipe/framework/formats/image.h"

using namespace godot;

namespace util {

std::unique_ptr<mediapipe::ImageFrame> get_image_frame(Ref<godot::Image> image);
mediapipe::Image get_image(Ref<godot::Image> image);

Ref<godot::Image> get_image(const mediapipe::ImageFrameSharedPtr image);
Ref<godot::Image> get_image(const mediapipe::Image &image);

} //namespace util

#endif
