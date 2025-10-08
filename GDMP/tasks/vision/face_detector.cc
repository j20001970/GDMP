#include "face_detector.h"

void MediaPipeFaceDetector::_register_methods() {
	register_method("initialize", &MediaPipeFaceDetector::initialize);
	register_method("detect", &MediaPipeFaceDetector::detect);
	register_method("detect_video", &MediaPipeFaceDetector::detect_video);
	register_method("detect_async", &MediaPipeFaceDetector::detect_async);
	register_signal<MediaPipeFaceDetector>("result_callback");
}

void MediaPipeFaceDetector::_register_task() {
	register_class<MediaPipeFaceDetector>();
}

void MediaPipeFaceDetector::_init() {}

bool MediaPipeFaceDetector::initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, float min_detection_confidence, float min_suppression_threshold) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<FaceDetectorOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->min_detection_confidence = min_detection_confidence;
	options->min_suppression_threshold = min_suppression_threshold;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<FaceDetectorResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeDetectionResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeDetectionResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
			call_deferred("emit_signal", "result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = FaceDetector::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeDetectionResult> MediaPipeFaceDetector::detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeDetectionResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Detect(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeDetectionResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

Ref<MediaPipeDetectionResult> MediaPipeFaceDetector::detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeDetectionResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeDetectionResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

bool MediaPipeFaceDetector::detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeFaceDetector, MediaPipeVisionTask);
