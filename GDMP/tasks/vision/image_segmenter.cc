#include "image_segmenter.h"

void MediaPipeImageSegmenterResult::_register_methods() {
	register_method("get_confidence_masks", &MediaPipeImageSegmenterResult::get_confidence_masks);
	register_method("get_category_mask", &MediaPipeImageSegmenterResult::get_category_mask);
	register_method("has_confidence_masks", &MediaPipeImageSegmenterResult::has_confidence_masks);
	register_method("has_category_mask", &MediaPipeImageSegmenterResult::has_category_mask);
}

MediaPipeImageSegmenterResult *MediaPipeImageSegmenterResult::_new(const ImageSegmenterResult &result) {
	MediaPipeImageSegmenterResult *r = MediaPipeImageSegmenterResult::_new();
	r->result = result;
	return r;
}

void MediaPipeImageSegmenterResult::_init() {}

Array MediaPipeImageSegmenterResult::get_confidence_masks() {
	Array array;
	if (!has_confidence_masks())
		return array;
	auto confidence_masks = result.confidence_masks.value();
	array.resize(confidence_masks.size());
	for (int i = 0; i < confidence_masks.size(); i++)
		array[i] = MediaPipeImage::_new(confidence_masks[i]);
	return array;
}

Ref<MediaPipeImage> MediaPipeImageSegmenterResult::get_category_mask() {
	Ref<MediaPipeImage> category_mask;
	if (!has_category_mask())
		return category_mask;
	category_mask = Ref(MediaPipeImage::_new(result.category_mask.value()));
	return category_mask;
}

bool MediaPipeImageSegmenterResult::has_confidence_masks() {
	return result.confidence_masks.has_value();
}

bool MediaPipeImageSegmenterResult::has_category_mask() {
	return result.category_mask.has_value();
}

void MediaPipeImageSegmenter::_register_methods() {
	register_method("initialize", &MediaPipeImageSegmenter::initialize);
	register_method("segment", &MediaPipeImageSegmenter::segment);
	register_method("segment_video", &MediaPipeImageSegmenter::segment_video);
	register_method("segment_async", &MediaPipeImageSegmenter::segment_async);
	register_method("get_labels", &MediaPipeImageSegmenter::get_labels);
	register_signal<MediaPipeImageSegmenter>("result_callback");
}

void MediaPipeImageSegmenter::_register_task() {
	register_class<MediaPipeImageSegmenter>();
	register_class<MediaPipeImageSegmenterResult>();
}

void MediaPipeImageSegmenter::_init() {}

bool MediaPipeImageSegmenter::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
		String display_names_locale, bool output_confidence_masks, bool output_category_mask) {
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
				callback_result = Ref(MediaPipeImageSegmenterResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
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
		segment_result = Ref(MediaPipeImageSegmenterResult::_new(result.value()));
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
		segment_result = Ref(MediaPipeImageSegmenterResult::_new(result.value()));
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

PoolStringArray MediaPipeImageSegmenter::get_labels() {
	PoolStringArray labels;
	ERR_FAIL_COND_V(task == nullptr, labels);
	for (auto label : task->GetLabels())
		labels.push_back(label.c_str());
	return labels;
}

GDMP_REGISTER_TASK(MediaPipeImageSegmenter, MediaPipeVisionTask);
