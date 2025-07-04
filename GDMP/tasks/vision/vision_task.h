#ifndef GDMP_VISION_TASK
#define GDMP_VISION_TASK

#include "godot_cpp/variant/rect2.hpp"

#include "mediapipe/tasks/cc/vision/core/image_processing_options.h"
#include "mediapipe/tasks/cc/vision/core/running_mode.h"

#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::core;

class MediaPipeVisionTask : public MediaPipeTask {
		GDCLASS(MediaPipeVisionTask, MediaPipeTask)
		GDMP_TASK_CLASS

	protected:
		static void _bind_methods();

	public:
		enum RunningMode {
			RUNNING_MODE_IMAGE = IMAGE,
			RUNNING_MODE_VIDEO = VIDEO,
			RUNNING_MODE_LIVE_STREAM = LIVE_STREAM,
		};
};

VARIANT_ENUM_CAST(MediaPipeVisionTask::RunningMode)

#define GDMP_VISION_TASK_CLASS(NAME, TASK) \
	GDCLASS(NAME, MediaPipeVisionTask)     \
	GDMP_TASK_CLASS                        \
                                           \
private:                                   \
	std::unique_ptr<TASK> task;

RunningMode get_running_mode(MediaPipeVisionTask::RunningMode p_mode);

ImageProcessingOptions make_image_processing_options(Rect2 roi, int rotation_degrees);

#endif
