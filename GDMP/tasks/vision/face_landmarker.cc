#include "face_landmarker.h"

#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/landmark.h"

void MediaPipeFaceLandmarkerResult::_register_methods() {
	register_method("get_face_landmarks", &MediaPipeFaceLandmarkerResult::get_face_landmarks);
	register_method("get_face_blendshapes", &MediaPipeFaceLandmarkerResult::get_face_blendshapes);
	register_method("has_face_blendshapes", &MediaPipeFaceLandmarkerResult::has_face_blendshapes);
}

MediaPipeFaceLandmarkerResult *MediaPipeFaceLandmarkerResult::_new(const FaceLandmarkerResult &result) {
	MediaPipeFaceLandmarkerResult *r = MediaPipeFaceLandmarkerResult::_new();
	r->result = result;
	return r;
}

void MediaPipeFaceLandmarkerResult::_init() {}

Array MediaPipeFaceLandmarkerResult::get_face_landmarks() {
	Array array;
	auto face_landmarks = result.face_landmarks;
	array.resize(face_landmarks.size());
	for (int i = 0; i < face_landmarks.size(); i++)
		array[i] = MediaPipeNormalizedLandmarks::_new(face_landmarks[i]);
	return array;
}

Array MediaPipeFaceLandmarkerResult::get_face_blendshapes() {
	Array array;
	if (!has_face_blendshapes())
		return array;
	auto face_blendshapes = result.face_blendshapes.value();
	array.resize(face_blendshapes.size());
	for (int i = 0; i < face_blendshapes.size(); i++)
		array[i] = MediaPipeClassifications::_new(face_blendshapes[i]);
	return array;
}

bool MediaPipeFaceLandmarkerResult::has_face_blendshapes() {
	return result.face_blendshapes.has_value();
}

void MediaPipeFaceLandmarker::_register_methods() {
	register_method("initialize", &MediaPipeFaceLandmarker::initialize);
	register_method("detect", &MediaPipeFaceLandmarker::detect);
	register_method("detect_video", &MediaPipeFaceLandmarker::detect_video);
	register_method("detect_async", &MediaPipeFaceLandmarker::detect_async);
	register_signal<MediaPipeFaceLandmarker>("result_callback");
}

void MediaPipeFaceLandmarker::_register_task() {
	register_class<MediaPipeFaceLandmarker>();
	register_class<MediaPipeFaceLandmarkerResult>();
}

void MediaPipeFaceLandmarker::_init() {}

bool MediaPipeFaceLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
		int num_faces, float min_face_detection_confidence, float min_face_presence_confidence, float min_tracking_confidence,
		bool output_face_blendshapes) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<FaceLandmarkerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->num_faces = num_faces;
	options->min_face_detection_confidence = min_face_detection_confidence;
	options->min_face_presence_confidence = min_face_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	options->output_face_blendshapes = output_face_blendshapes;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<FaceLandmarkerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeFaceLandmarkerResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeFaceLandmarkerResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = FaceLandmarker::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeFaceLandmarkerResult> MediaPipeFaceLandmarker::detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeFaceLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Detect(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeFaceLandmarkerResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

Ref<MediaPipeFaceLandmarkerResult> MediaPipeFaceLandmarker::detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeFaceLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		detect_result = Ref(MediaPipeFaceLandmarkerResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

bool MediaPipeFaceLandmarker::detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeFaceLandmarker, MediaPipeVisionTask);
