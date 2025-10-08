#include "image_classifier.h"

void MediaPipeImageClassifier::_register_methods() {
	register_method("initialize", &MediaPipeImageClassifier::initialize);
	register_method("classify", &MediaPipeImageClassifier::classify);
	register_method("classify_video", &MediaPipeImageClassifier::classify_video);
	register_method("classify_async", &MediaPipeImageClassifier::classify_async);
	register_signal<MediaPipeImageClassifier>("result_callback");
}

void MediaPipeImageClassifier::_register_task() {
	register_class<MediaPipeImageClassifier>();
}

void MediaPipeImageClassifier::_init() {}

bool MediaPipeImageClassifier::initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeClassifierOptions> classifier_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(classifier_options.is_null(), false);
	auto options = std::make_unique<ImageClassifierOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->classifier_options = classifier_options->get_options();
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<ImageClassifierResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeClassificationResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeClassificationResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
			call_deferred("emit_signal", "result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = ImageClassifier::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeClassificationResult> MediaPipeImageClassifier::classify(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeClassificationResult> classify_result;
	ERR_FAIL_COND_V(image.is_null(), classify_result);
	ERR_FAIL_COND_V(task == nullptr, classify_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Classify(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		classify_result = Ref(MediaPipeClassificationResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return classify_result;
}

Ref<MediaPipeClassificationResult> MediaPipeImageClassifier::classify_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeClassificationResult> classify_result;
	ERR_FAIL_COND_V(image.is_null(), classify_result);
	ERR_FAIL_COND_V(task == nullptr, classify_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->ClassifyForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		classify_result = Ref(MediaPipeClassificationResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return classify_result;
}

bool MediaPipeImageClassifier::classify_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->ClassifyAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeImageClassifier, MediaPipeVisionTask);
