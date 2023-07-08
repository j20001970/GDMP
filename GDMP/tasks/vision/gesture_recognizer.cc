#include "gesture_recognizer.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

#include "GDMP/tasks/vision/vision_task.h"

void MediaPipeGestureRecognizerResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_gestures"), &MediaPipeGestureRecognizerResult::get_gestures);
	ClassDB::bind_method(D_METHOD("get_handedness"), &MediaPipeGestureRecognizerResult::get_handedness);
	ClassDB::bind_method(D_METHOD("get_hand_landmarks"), &MediaPipeGestureRecognizerResult::get_hand_landmarks);
	ClassDB::bind_method(D_METHOD("get_hand_world_landmarks"), &MediaPipeGestureRecognizerResult::get_hand_world_landmarks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "gestures", PROPERTY_HINT_ARRAY_TYPE, MediaPipeProto::get_class_static()), "", "get_gestures");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "handedness", PROPERTY_HINT_ARRAY_TYPE, MediaPipeProto::get_class_static()), "", "get_handedness");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "hand_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeProto::get_class_static()), "", "get_hand_landmarks");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "hand_world_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeProto::get_class_static()), "", "get_hand_world_landmarks");
}

MediaPipeGestureRecognizerResult::MediaPipeGestureRecognizerResult() = default;

MediaPipeGestureRecognizerResult::MediaPipeGestureRecognizerResult(const GestureRecognizerResult &result) {
	this->result = result;
}

TypedArray<MediaPipeProto> MediaPipeGestureRecognizerResult::get_gestures() {
	TypedArray<MediaPipeProto> array;
	auto gestures = result.gestures;
	array.resize(gestures.size());
	for (int i = 0; i < gestures.size(); i++)
		array[i] = memnew(MediaPipeProto(gestures[i]));
	return array;
}

TypedArray<MediaPipeProto> MediaPipeGestureRecognizerResult::get_handedness() {
	TypedArray<MediaPipeProto> array;
	auto handedness = result.handedness;
	array.resize(handedness.size());
	for (int i = 0; i < handedness.size(); i++)
		array[i] = memnew(MediaPipeProto(handedness[i]));
	return array;
}

TypedArray<MediaPipeProto> MediaPipeGestureRecognizerResult::get_hand_landmarks() {
	TypedArray<MediaPipeProto> array;
	auto hand_landmarks = result.hand_landmarks;
	array.resize(hand_landmarks.size());
	for (int i = 0; i < hand_landmarks.size(); i++)
		array[i] = memnew(MediaPipeProto(hand_landmarks[i]));
	return array;
}

TypedArray<MediaPipeProto> MediaPipeGestureRecognizerResult::get_hand_world_landmarks() {
	TypedArray<MediaPipeProto> array;
	auto hand_world_landmarks = result.hand_world_landmarks;
	array.resize(hand_world_landmarks.size());
	for (int i = 0; i < hand_world_landmarks.size(); i++)
		array[i] = memnew(MediaPipeProto(hand_world_landmarks[i]));
	return array;
}

void MediaPipeGestureRecognizer::_bind_methods() {
	ClassDB::bind_method(D_METHOD(
								 "initialize", "base_options", "running_mode",
								 "num_hands", "min_hand_detection_confidence", "min_hand_presence_confidence", "min_tracking_confidence"),
			&MediaPipeGestureRecognizer::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(1), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(0.5));
	ClassDB::bind_method(D_METHOD("recognize", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeGestureRecognizer::recognize, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("recognize_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeGestureRecognizer::recognize_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("recognize_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeGestureRecognizer::recognize_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeGestureRecognizerResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeGestureRecognizer::_register_task() {
	ClassDB::register_class<MediaPipeGestureRecognizer>();
	ClassDB::register_class<MediaPipeGestureRecognizerResult>();
}

bool MediaPipeGestureRecognizer::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode,
		int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<GestureRecognizerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = RunningMode(running_mode);
	options->num_hands = num_hands;
	options->min_hand_detection_confidence = min_hand_detection_confidence;
	options->min_hand_presence_confidence = min_hand_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<GestureRecognizerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeGestureRecognizerResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipeGestureRecognizerResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = GestureRecognizer::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeGestureRecognizerResult> MediaPipeGestureRecognizer::recognize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeGestureRecognizerResult> recognize_result;
	ERR_FAIL_COND_V(image.is_null(), recognize_result);
	ERR_FAIL_COND_V(task == nullptr, recognize_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Recognize(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		recognize_result = Ref(memnew(MediaPipeGestureRecognizerResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return recognize_result;
}

Ref<MediaPipeGestureRecognizerResult> MediaPipeGestureRecognizer::recognize_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeGestureRecognizerResult> recognize_result;
	ERR_FAIL_COND_V(image.is_null(), recognize_result);
	ERR_FAIL_COND_V(task == nullptr, recognize_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->RecognizeForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		recognize_result = Ref(memnew(MediaPipeGestureRecognizerResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return recognize_result;
}

bool MediaPipeGestureRecognizer::recognize_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->RecognizeAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeGestureRecognizer);
