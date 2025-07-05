#ifndef GDMP_TASK_VISION_OBJECT_DETECTOR
#define GDMP_TASK_VISION_OBJECT_DETECTOR

#include "Rect2.hpp"
#include "Ref.hpp"

#include "mediapipe/tasks/cc/vision/object_detector/object_detector.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/detection_result.h"
#include "GDMP/tasks/processors/classifier_options.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision;

class MediaPipeObjectDetector : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeObjectDetector, ObjectDetector)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeClassifierOptions> classifier_options);
		Ref<MediaPipeDetectionResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeDetectionResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
