#include "interactive_segmenter.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeInteractiveSegmenter::_bind_methods() {
	ClassDB::bind_method(
			D_METHOD("initialize", "base_options", "output_confidence_masks", "output_category_mask"),
			&MediaPipeInteractiveSegmenter::initialize, DEFVAL(true), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("segment", "image", "keypoint", "scribble", "region_of_interest", "rotation_degrees"),
			&MediaPipeInteractiveSegmenter::segment, DEFVAL(Vector2()), DEFVAL(PackedVector2Array()), DEFVAL(Rect2()), DEFVAL(0));
}

void MediaPipeInteractiveSegmenter::_register_task() {
	ClassDB::register_class<MediaPipeInteractiveSegmenter>();
}

bool MediaPipeInteractiveSegmenter::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options,
		bool output_confidence_masks, bool output_category_mask) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<InteractiveSegmenterOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->output_confidence_masks = output_confidence_masks;
	options->output_category_mask = output_category_mask;
	auto create_task = InteractiveSegmenter::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeImageSegmenterResult> MediaPipeInteractiveSegmenter::segment(
		Ref<MediaPipeImage> image, Vector2 keypoint, PackedVector2Array scribble,
		Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeImageSegmenterResult> segment_result;
	ERR_FAIL_COND_V(image.is_null(), segment_result);
	ERR_FAIL_COND_V(task == nullptr, segment_result);
	RegionOfInterest roi;
	if (!keypoint.is_zero_approx() && scribble.is_empty()) {
		roi.format = RegionOfInterest::Format::kKeyPoint;
		roi.keypoint = mediapipe::tasks::components::containers::NormalizedKeypoint();
		roi.keypoint->x = keypoint.x;
		roi.keypoint->y = keypoint.y;
	} else if (keypoint.is_zero_approx() && !scribble.is_empty()) {
		roi.format = RegionOfInterest::Format::kScribble;
		roi.scribble = std::vector<mediapipe::tasks::components::containers::NormalizedKeypoint>();
		roi.scribble->resize(scribble.size());
		for (int i = 0; i < scribble.size(); i++) {
			roi.scribble.value()[i].x = scribble[i].x;
			roi.scribble.value()[i].y = scribble[i].y;
		}
	}
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Segment(image->get_mediapipe_image(), roi, image_processing_options);
	if (result.ok())
		segment_result = Ref(memnew(MediaPipeImageSegmenterResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return segment_result;
}

GDMP_REGISTER_TASK(MediaPipeInteractiveSegmenter, MediaPipeVisionTask);
