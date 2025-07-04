#ifndef GDMP_TASK_VISION_IMAGE_SEGMENTER
#define GDMP_TASK_VISION_IMAGE_SEGMENTER

#include "Array.hpp"
#include "PoolArrays.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/vision/image_segmenter/image_segmenter.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::image_segmenter;

class MediaPipeImageSegmenterResult : public Reference {
		GODOT_CLASS(MediaPipeImageSegmenterResult, Reference)

	private:
		ImageSegmenterResult result;

	public:
		static void _register_methods();
		static MediaPipeImageSegmenterResult *_new(const ImageSegmenterResult &result);

		void _init();

		Array get_confidence_masks();
		Ref<MediaPipeImage> get_category_mask();

		bool has_confidence_masks();
		bool has_category_mask();
};

class MediaPipeImageSegmenter : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeImageSegmenter, ImageSegmenter)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				String display_names_locale, bool output_confidence_masks, bool output_category_mask);
		Ref<MediaPipeImageSegmenterResult> segment(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeImageSegmenterResult> segment_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool segment_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		PoolStringArray get_labels();
};

#endif
