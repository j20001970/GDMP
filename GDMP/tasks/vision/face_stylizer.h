#ifndef GDMP_TASK_VISION_FACE_STYLIZER
#define GDMP_TASK_VISION_FACE_STYLIZER

#include "Rect2.hpp"
#include "Ref.hpp"

#include "mediapipe/tasks/cc/vision/face_stylizer/face_stylizer.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::face_stylizer;

class MediaPipeFaceStylizer : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeFaceStylizer, FaceStylizer)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options);
		Ref<MediaPipeImage> stylize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
};

#endif
