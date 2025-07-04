#include "hand_landmarker.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeHandLandmarkerResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_handedness"), &MediaPipeHandLandmarkerResult::get_handedness);
	ClassDB::bind_method(D_METHOD("get_hand_landmarks"), &MediaPipeHandLandmarkerResult::get_hand_landmarks);
	ClassDB::bind_method(D_METHOD("get_hand_world_landmarks"), &MediaPipeHandLandmarkerResult::get_hand_world_landmarks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "handedness", PROPERTY_HINT_ARRAY_TYPE, MediaPipeClassifications::get_class_static()), "", "get_handedness");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "hand_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedLandmarks::get_class_static()), "", "get_hand_landmarks");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "hand_world_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeLandmarks::get_class_static()), "", "get_hand_world_landmarks");
}

MediaPipeHandLandmarkerResult::MediaPipeHandLandmarkerResult() = default;

MediaPipeHandLandmarkerResult::MediaPipeHandLandmarkerResult(const HandLandmarkerResult &result) {
	this->result = result;
}

TypedArray<MediaPipeClassifications> MediaPipeHandLandmarkerResult::get_handedness() {
	TypedArray<MediaPipeClassifications> array;
	auto handedness = result.handedness;
	array.resize(handedness.size());
	for (int i = 0; i < handedness.size(); i++)
		array[i] = memnew(MediaPipeClassifications(handedness[i]));
	return array;
}

TypedArray<MediaPipeNormalizedLandmarks> MediaPipeHandLandmarkerResult::get_hand_landmarks() {
	TypedArray<MediaPipeNormalizedLandmarks> array;
	auto hand_landmarks = result.hand_landmarks;
	array.resize(hand_landmarks.size());
	for (int i = 0; i < hand_landmarks.size(); i++)
		array[i] = memnew(MediaPipeNormalizedLandmarks(hand_landmarks[i]));
	return array;
}

TypedArray<MediaPipeLandmarks> MediaPipeHandLandmarkerResult::get_hand_world_landmarks() {
	TypedArray<MediaPipeLandmarks> array;
	auto hand_world_landmarks = result.hand_world_landmarks;
	array.resize(hand_world_landmarks.size());
	for (int i = 0; i < hand_world_landmarks.size(); i++)
		array[i] = memnew(MediaPipeLandmarks(hand_world_landmarks[i]));
	return array;
}

void MediaPipeHandLandmarker::_bind_methods() {
	ClassDB::bind_method(D_METHOD(
								 "initialize", "base_options", "running_mode",
								 "num_hands", "min_hand_detection_confidence", "min_hand_presence_confidence", "min_tracking_confidence"),
			&MediaPipeHandLandmarker::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(1), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(0.5));
	ClassDB::bind_method(D_METHOD("detect", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeHandLandmarker::detect, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeHandLandmarker::detect_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeHandLandmarker::detect_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeHandLandmarkerResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeHandLandmarker::_register_task() {
	ClassDB::register_class<MediaPipeHandLandmarker>();
	ClassDB::register_class<MediaPipeHandLandmarkerResult>();
}

bool MediaPipeHandLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
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
				callback_result = Ref(memnew(MediaPipeHandLandmarkerResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
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
		detect_result = Ref(memnew(MediaPipeHandLandmarkerResult(result.value())));
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
		detect_result = Ref(memnew(MediaPipeHandLandmarkerResult(result.value())));
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
