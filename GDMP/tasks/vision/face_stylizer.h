#ifndef GDMP_TASK_VISION_FACE_STYLIZER
#define GDMP_TASK_VISION_FACE_STYLIZER

#include "godot_cpp/classes/ref.hpp"

#include "mediapipe/tasks/cc/vision/face_stylizer/face_stylizer.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::face_stylizer;

class MediaPipeFaceStylizer : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeFaceStylizer, FaceStylizer)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options);
		Ref<MediaPipeImage> stylize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
};

#endif
