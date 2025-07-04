#include "gesture_recognizer.h"

void MediaPipeGestureRecognizerResult::_register_methods() {
	register_method("get_gestures", &MediaPipeGestureRecognizerResult::get_gestures);
	register_method("get_handedness", &MediaPipeGestureRecognizerResult::get_handedness);
	register_method("get_hand_landmarks", &MediaPipeGestureRecognizerResult::get_hand_landmarks);
	register_method("get_hand_world_landmarks", &MediaPipeGestureRecognizerResult::get_hand_world_landmarks);
}

MediaPipeGestureRecognizerResult *MediaPipeGestureRecognizerResult::_new(const GestureRecognizerResult &result) {
	MediaPipeGestureRecognizerResult *r = MediaPipeGestureRecognizerResult::_new();
	r->result = result;
	return r;
}

void MediaPipeGestureRecognizerResult::_init() {}

Array MediaPipeGestureRecognizerResult::get_gestures() {
	Array array;
	auto gestures = result.gestures;
	array.resize(gestures.size());
	for (int i = 0; i < gestures.size(); i++)
		array[i] = MediaPipeProto::_new(gestures[i]);
	return array;
}

Array MediaPipeGestureRecognizerResult::get_handedness() {
	Array array;
	auto handedness = result.handedness;
	array.resize(handedness.size());
	for (int i = 0; i < handedness.size(); i++)
		array[i] = MediaPipeProto::_new(handedness[i]);
	return array;
}

Array MediaPipeGestureRecognizerResult::get_hand_landmarks() {
	Array array;
	auto hand_landmarks = result.hand_landmarks;
	array.resize(hand_landmarks.size());
	for (int i = 0; i < hand_landmarks.size(); i++)
		array[i] = MediaPipeProto::_new(hand_landmarks[i]);
	return array;
}

Array MediaPipeGestureRecognizerResult::get_hand_world_landmarks() {
	Array array;
	auto hand_world_landmarks = result.hand_world_landmarks;
	array.resize(hand_world_landmarks.size());
	for (int i = 0; i < hand_world_landmarks.size(); i++)
		array[i] = MediaPipeProto::_new(hand_world_landmarks[i]);
	return array;
}

void MediaPipeGestureRecognizer::_register_methods() {
	register_method("initialize", &MediaPipeGestureRecognizer::initialize);
	register_method("recognize", &MediaPipeGestureRecognizer::recognize);
	register_method("recognize_video", &MediaPipeGestureRecognizer::recognize_video);
	register_method(("recognize_async"), &MediaPipeGestureRecognizer::recognize_async);
	register_signal<MediaPipeGestureRecognizer>("result_callback");
}

void MediaPipeGestureRecognizer::_register_task() {
	register_class<MediaPipeGestureRecognizer>();
	register_class<MediaPipeGestureRecognizerResult>();
}

void MediaPipeGestureRecognizer::_init() {}

bool MediaPipeGestureRecognizer::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
		int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<GestureRecognizerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->num_hands = num_hands;
	options->min_hand_detection_confidence = min_hand_detection_confidence;
	options->min_hand_presence_confidence = min_hand_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<GestureRecognizerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeGestureRecognizerResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeGestureRecognizerResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
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
		recognize_result = Ref(MediaPipeGestureRecognizerResult::_new(result.value()));
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
		recognize_result = Ref(MediaPipeGestureRecognizerResult::_new(result.value()));
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

GDMP_REGISTER_TASK(MediaPipeGestureRecognizer, MediaPipeVisionTask);
