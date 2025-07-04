#ifndef GDMP_TASK_VISION_INTERACTIVE_SEGMENTER
#define GDMP_TASK_VISION_INTERACTIVE_SEGMENTER

#include "PoolArrays.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "Vector2.hpp"

#include "mediapipe/tasks/cc/vision/interactive_segmenter/interactive_segmenter.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/image_segmenter.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::interactive_segmenter;

class MediaPipeInteractiveSegmenter : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeInteractiveSegmenter, InteractiveSegmenter)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options,
				bool output_confidence_masks, bool output_category_mask);
		Ref<MediaPipeImageSegmenterResult> segment(
				Ref<MediaPipeImage> image, Vector2 keypoint, PoolVector2Array scribble,
				Rect2 region_of_interest, int rotation_degrees);
};

#endif
