#ifndef GDMP_VISION_TASK
#define GDMP_VISION_TASK

#include "godot_cpp/variant/rect2.hpp"

#include "mediapipe/tasks/cc/vision/core/image_processing_options.h"

using namespace godot;
using namespace mediapipe::tasks::vision::core;

ImageProcessingOptions make_image_processing_options(Rect2 roi, int rotation_degrees);

#endif
