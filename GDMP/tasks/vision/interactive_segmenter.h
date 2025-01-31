#ifndef GDMP_TASK_VISION_INTERACTIVE_SEGMENTER
#define GDMP_TASK_VISION_INTERACTIVE_SEGMENTER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/vector2.hpp"

#include "mediapipe/tasks/cc/vision/interactive_segmenter/interactive_segmenter.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/task.h"
#include "GDMP/tasks/vision/image_segmenter.h"

using namespace godot;
using namespace mediapipe::tasks::vision::interactive_segmenter;

class MediaPipeInteractiveSegmenter : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeInteractiveSegmenter, InteractiveSegmenter)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options,
				bool output_confidence_masks, bool output_category_mask);
		Ref<MediaPipeImageSegmenterResult> segment(
				Ref<MediaPipeImage> image, Vector2 keypoint, PackedVector2Array scribble,
				Rect2 region_of_interest, int rotation_degrees);
};

#endif
