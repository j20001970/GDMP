#include "face_landmarker.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeFaceLandmarkerResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_face_landmarks"), &MediaPipeFaceLandmarkerResult::get_face_landmarks);
	ClassDB::bind_method(D_METHOD("get_face_blendshapes"), &MediaPipeFaceLandmarkerResult::get_face_blendshapes);
	ClassDB::bind_method(D_METHOD("has_face_blendshapes"), &MediaPipeFaceLandmarkerResult::has_face_blendshapes);
	ClassDB::bind_method(D_METHOD("get_facial_transformation_matrixes"), &MediaPipeFaceLandmarkerResult::get_facial_transformation_matrixes);
	ClassDB::bind_method(D_METHOD("has_facial_transformation_matrixes"), &MediaPipeFaceLandmarkerResult::has_facial_transformation_matrixes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "face_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedLandmarks::get_class_static()), "", "get_face_landmarks");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "face_blendshapes", PROPERTY_HINT_ARRAY_TYPE, MediaPipeClassifications::get_class_static()), "", "get_face_blendshapes");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "facial_transformation_matrixes"), "", "get_facial_transformation_matrixes");
}

MediaPipeFaceLandmarkerResult::MediaPipeFaceLandmarkerResult() = default;

MediaPipeFaceLandmarkerResult::MediaPipeFaceLandmarkerResult(const FaceLandmarkerResult &result) {
	this->result = result;
}

TypedArray<MediaPipeNormalizedLandmarks> MediaPipeFaceLandmarkerResult::get_face_landmarks() {
	TypedArray<MediaPipeNormalizedLandmarks> array;
	auto face_landmarks = result.face_landmarks;
	array.resize(face_landmarks.size());
	for (int i = 0; i < face_landmarks.size(); i++)
		array[i] = memnew(MediaPipeNormalizedLandmarks(face_landmarks[i]));
	return array;
}

TypedArray<MediaPipeClassifications> MediaPipeFaceLandmarkerResult::get_face_blendshapes() {
	TypedArray<MediaPipeClassifications> array;
	if (!has_face_blendshapes())
		return array;
	auto face_blendshapes = result.face_blendshapes.value();
	array.resize(face_blendshapes.size());
	for (int i = 0; i < face_blendshapes.size(); i++)
		array[i] = memnew(MediaPipeClassifications(face_blendshapes[i]));
	return array;
}

bool MediaPipeFaceLandmarkerResult::has_face_blendshapes() {
	return result.face_blendshapes.has_value();
}

Array MediaPipeFaceLandmarkerResult::get_facial_transformation_matrixes() {
	Array array;
	if (!has_facial_transformation_matrixes())
		return array;

	auto matrixes = result.facial_transformation_matrixes.value();
	array.resize(matrixes.size());

	for (int i = 0; i < matrixes.size(); i++) {
		Projection projection;
		auto matrix = matrixes[i];

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				projection.columns[x][y] = matrix(x, y);
			}
		}
		array[i] = projection;
	}

	return array;
}

bool MediaPipeFaceLandmarkerResult::has_facial_transformation_matrixes() {
	return result.facial_transformation_matrixes.has_value();
}

void MediaPipeFaceLandmarker::_bind_methods() {
	ClassDB::bind_method(D_METHOD(
								 "initialize", "base_options", "running_mode",
								 "num_faces", "min_face_detection_confidence", "min_face_presence_confidence", "min_tracking_confidence",
								 "output_face_blendshapes", "output_facial_transformation_matrixes"),
			&MediaPipeFaceLandmarker::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(1), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(false), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("detect", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeFaceLandmarker::detect, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeFaceLandmarker::detect_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeFaceLandmarker::detect_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeFaceLandmarkerResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeFaceLandmarker::_register_task() {
	ClassDB::register_class<MediaPipeFaceLandmarker>();
	ClassDB::register_class<MediaPipeFaceLandmarkerResult>();
}

bool MediaPipeFaceLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
		int num_faces, float min_face_detection_confidence, float min_face_presence_confidence, float min_tracking_confidence,
		bool output_face_blendshapes, bool output_facial_transformation_matrixes) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<FaceLandmarkerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->num_faces = num_faces;
	options->min_face_detection_confidence = min_face_detection_confidence;
	options->min_face_presence_confidence = min_face_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	options->output_face_blendshapes = output_face_blendshapes;
	options->output_facial_transformation_matrixes = output_facial_transformation_matrixes;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<FaceLandmarkerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeFaceLandmarkerResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipeFaceLandmarkerResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			call_deferred("emit_signal", "result_callback", callback_result, callback_image, timestamp_ms);
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
		detect_result = Ref(memnew(MediaPipeFaceLandmarkerResult(result.value())));
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
		detect_result = Ref(memnew(MediaPipeFaceLandmarkerResult(result.value())));
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
