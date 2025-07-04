#ifndef GDMP_TASK_VISION_OBJECT_DETECTOR
#define GDMP_TASK_VISION_OBJECT_DETECTOR

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/vision/object_detector/object_detector.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/detection_result.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision;

class MediaPipeObjectDetector : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeObjectDetector, ObjectDetector)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
				const String &display_names_locale, int max_results, float score_threshold,
				PackedStringArray category_allowlist, PackedStringArray category_denylist);
		Ref<MediaPipeDetectionResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeDetectionResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
