#include "hand_landmarker.h"

#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/landmark.h"

void MediaPipeHandLandmarkerResult::_register_methods() {
	register_method("get_handedness", &MediaPipeHandLandmarkerResult::get_handedness);
	register_method("get_hand_landmarks", &MediaPipeHandLandmarkerResult::get_hand_landmarks);
	register_method("get_hand_world_landmarks", &MediaPipeHandLandmarkerResult::get_hand_world_landmarks);
}

MediaPipeHandLandmarkerResult *MediaPipeHandLandmarkerResult::_new(const HandLandmarkerResult &result) {
	MediaPipeHandLandmarkerResult *r = MediaPipeHandLandmarkerResult::_new();
	r->result = result;
	return r;
}

void MediaPipeHandLandmarkerResult::_init() {}

Array MediaPipeHandLandmarkerResult::get_handedness() {
	Array array;
	auto handedness = result.handedness;
	array.resize(handedness.size());
	for (int i = 0; i < handedness.size(); i++)
		array[i] = MediaPipeClassifications::_new(handedness[i]);
	return array;
}

Array MediaPipeHandLandmarkerResult::get_hand_landmarks() {
	Array array;
	auto hand_landmarks = result.hand_landmarks;
	array.resize(hand_landmarks.size());
	for (int i = 0; i < hand_landmarks.size(); i++)
		array[i] = MediaPipeNormalizedLandmarks::_new(hand_landmarks[i]);
	return array;
}

Array MediaPipeHandLandmarkerResult::get_hand_world_landmarks() {
	Array array;
	auto hand_world_landmarks = result.hand_world_landmarks;
	array.resize(hand_world_landmarks.size());
	for (int i = 0; i < hand_world_landmarks.size(); i++)
		array[i] = MediaPipeLandmarks::_new(hand_world_landmarks[i]);
	return array;
}

void MediaPipeHandLandmarker::_register_methods() {
	register_method("initialize", &MediaPipeHandLandmarker::initialize);
	register_method("detect", &MediaPipeHandLandmarker::detect);
	register_method("detect_video", &MediaPipeHandLandmarker::detect_video);
	register_method("detect_async", &MediaPipeHandLandmarker::detect_async);
	register_signal<MediaPipeHandLandmarker>("result_callback");
}

void MediaPipeHandLandmarker::_register_task() {
	register_class<MediaPipeHandLandmarker>();
	register_class<MediaPipeHandLandmarkerResult>();
}

void MediaPipeHandLandmarker::_init() {}

bool MediaPipeHandLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
		int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<HandLandmarkerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->num_hands = num_hands;
	options->min_hand_detection_confidence = min_hand_detection_confidence;
	options->min_hand_presence_confidence = min_hand_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<HandLandmarkerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeHandLandmarkerResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeHandLandmarkerResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = HandLandmarker::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeHandLandmarkerResult> MediaPipeHandLandmarker::detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeHandLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Detect(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeHandLandmarkerResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

Ref<MediaPipeHandLandmarkerResult> MediaPipeHandLandmarker::detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeHandLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeHandLandmarkerResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

bool MediaPipeHandLandmarker::detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeHandLandmarker, MediaPipeVisionTask);
