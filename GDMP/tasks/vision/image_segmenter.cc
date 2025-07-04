#include "image_segmenter.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeImageSegmenterResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_confidence_masks"), &MediaPipeImageSegmenterResult::get_confidence_masks);
	ClassDB::bind_method(D_METHOD("get_category_mask"), &MediaPipeImageSegmenterResult::get_category_mask);
	ClassDB::bind_method(D_METHOD("has_confidence_masks"), &MediaPipeImageSegmenterResult::has_confidence_masks);
	ClassDB::bind_method(D_METHOD("has_category_mask"), &MediaPipeImageSegmenterResult::has_category_mask);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "confidence_masks", godot::PROPERTY_HINT_ARRAY_TYPE, MediaPipeImage::get_class_static()), "", "get_confidence_masks");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "category_mask", godot::PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()), "", "get_category_mask");
}

MediaPipeImageSegmenterResult::MediaPipeImageSegmenterResult() = default;

MediaPipeImageSegmenterResult::MediaPipeImageSegmenterResult(const ImageSegmenterResult &result) {
	this->result = result;
}

TypedArray<MediaPipeImage> MediaPipeImageSegmenterResult::get_confidence_masks() {
	TypedArray<MediaPipeImage> array;
	if (!has_confidence_masks())
		return array;
	auto confidence_masks = result.confidence_masks.value();
	array.resize(confidence_masks.size());
	for (int i = 0; i < confidence_masks.size(); i++)
		array[i] = memnew(MediaPipeImage(confidence_masks[i]));
	return array;
}

Ref<MediaPipeImage> MediaPipeImageSegmenterResult::get_category_mask() {
	Ref<MediaPipeImage> category_mask;
	if (!has_category_mask())
		return category_mask;
	category_mask = Ref(memnew(MediaPipeImage(result.category_mask.value())));
	return category_mask;
}

bool MediaPipeImageSegmenterResult::has_confidence_masks() {
	return result.confidence_masks.has_value();
}

bool MediaPipeImageSegmenterResult::has_category_mask() {
	return result.category_mask.has_value();
}

void MediaPipeImageSegmenter::_bind_methods() {
	ClassDB::bind_method(
			D_METHOD("initialize", "base_options", "running_mode", "display_names_locale", "output_confidence_masks", "output_category_mask"),
			&MediaPipeImageSegmenter::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(String("en")), DEFVAL(true), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("segment", "image", "region_of_interest", "rotation_degrees"), &MediaPipeImageSegmenter::segment, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(
			D_METHOD("segment_video", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeImageSegmenter::segment_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(
			D_METHOD("segment_async", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeImageSegmenter::segment_async, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_labels"), &MediaPipeImageSegmenter::get_labels);
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImageSegmenterResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeImageSegmenter::_register_task() {
	ClassDB::register_class<MediaPipeImageSegmenter>();
	ClassDB::register_class<MediaPipeImageSegmenterResult>();
}

bool MediaPipeImageSegmenter::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
		const String &display_names_locale, bool output_confidence_masks, bool output_category_mask) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<ImageSegmenterOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->display_names_locale = display_names_locale.utf8().get_data();
	options->output_confidence_masks = output_confidence_masks;
	options->output_category_mask = output_category_mask;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<ImageSegmenterResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeImageSegmenterResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipeImageSegmenterResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = ImageSegmenter::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeImageSegmenterResult> MediaPipeImageSegmenter::segment(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeImageSegmenterResult> segment_result;
	ERR_FAIL_COND_V(image.is_null(), segment_result);
	ERR_FAIL_COND_V(task == nullptr, segment_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Segment(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		segment_result = Ref(memnew(MediaPipeImageSegmenterResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return segment_result;
}

Ref<MediaPipeImageSegmenterResult> MediaPipeImageSegmenter::segment_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeImageSegmenterResult> segment_result;
	ERR_FAIL_COND_V(image.is_null(), segment_result);
	ERR_FAIL_COND_V(task == nullptr, segment_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->SegmentForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		segment_result = Ref(memnew(MediaPipeImageSegmenterResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return segment_result;
}

bool MediaPipeImageSegmenter::segment_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->SegmentAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

PackedStringArray MediaPipeImageSegmenter::get_labels() {
	PackedStringArray labels;
	ERR_FAIL_COND_V(task == nullptr, labels);
	for (auto label : task->GetLabels())
		labels.push_back(label.c_str());
	return labels;
}

GDMP_REGISTER_TASK(MediaPipeImageSegmenter, MediaPipeVisionTask);
