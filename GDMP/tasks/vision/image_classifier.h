#ifndef GDMP_TASK_VISION_IMAGE_CLASSIFIER
#define GDMP_TASK_VISION_IMAGE_CLASSIFIER

#include "PoolArrays.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/vision/image_classifier/image_classifier.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::image_classifier;

class MediaPipeImageClassifier : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeImageClassifier, ImageClassifier)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				String display_names_locale, int max_results, float score_threshold,
				PoolStringArray category_allowlist, PoolStringArray category_denylist);
		Ref<MediaPipeClassificationResult> classify(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeClassificationResult> classify_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool classify_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
