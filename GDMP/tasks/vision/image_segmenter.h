#ifndef GDMP_TASK_VISION_IMAGE_SEGMENTER
#define GDMP_TASK_VISION_IMAGE_SEGMENTER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/vision/image_segmenter/image_segmenter.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::image_segmenter;

class MediaPipeImageSegmenterResult : public RefCounted {
		GDCLASS(MediaPipeImageSegmenterResult, RefCounted)

	private:
		ImageSegmenterResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeImageSegmenterResult();
		MediaPipeImageSegmenterResult(const ImageSegmenterResult &result);

		TypedArray<MediaPipeImage> get_confidence_masks();
		Ref<MediaPipeImage> get_category_mask();

		bool has_confidence_masks();
		bool has_category_mask();
};

class MediaPipeImageSegmenter : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeImageSegmenter, ImageSegmenter)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode,
				const String &display_names_locale, bool output_confidence_masks, bool output_category_mask);
		Ref<MediaPipeImageSegmenterResult> segment(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeImageSegmenterResult> segment_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool segment_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		PackedStringArray get_labels();
};

#endif
