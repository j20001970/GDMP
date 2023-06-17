#include "pose_landmarker.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

#include "GDMP/tasks/vision/vision_task.h"

void MediaPipePoseLandmarkerResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_segmentation_masks"), &MediaPipePoseLandmarkerResult::get_segmentation_masks);
	ClassDB::bind_method(D_METHOD("get_pose_landmarks"), &MediaPipePoseLandmarkerResult::get_pose_landmarks);
	ClassDB::bind_method(D_METHOD("get_pose_world_landmarks"), &MediaPipePoseLandmarkerResult::get_pose_world_landmarks);
	ClassDB::bind_method(D_METHOD("has_segmentation_masks"), &MediaPipePoseLandmarkerResult::has_segmentation_masks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "segmentation_masks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeImage::get_class_static()), "", "get_segmentation_masks");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "pose_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedLandmarks::get_class_static()), "", "get_pose_landmarks");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "pose_world_landmarks", PROPERTY_HINT_ARRAY_TYPE, MediaPipeLandmarks::get_class_static()), "", "get_pose_world_landmarks");
}

MediaPipePoseLandmarkerResult::MediaPipePoseLandmarkerResult() = default;

MediaPipePoseLandmarkerResult::MediaPipePoseLandmarkerResult(const PoseLandmarkerResult &result) {
	this->result = result;
}

TypedArray<MediaPipeImage> MediaPipePoseLandmarkerResult::get_segmentation_masks() {
	TypedArray<MediaPipeImage> array;
	if (!has_segmentation_masks())
		return array;
	auto segmentation_masks = result.segmentation_masks.value();
	array.resize(segmentation_masks.size());
	for (int i = 0; i < segmentation_masks.size(); i++)
		array[i] = memnew(MediaPipeImage(segmentation_masks[i]));
	return array;
}

TypedArray<MediaPipeNormalizedLandmarks> MediaPipePoseLandmarkerResult::get_pose_landmarks() {
	TypedArray<MediaPipeNormalizedLandmarks> array;
	auto pose_landmarks = result.pose_landmarks;
	array.resize(pose_landmarks.size());
	for (int i = 0; i < pose_landmarks.size(); i++)
		array[i] = memnew(MediaPipeNormalizedLandmarks(pose_landmarks[i]));
	return array;
}

TypedArray<MediaPipeLandmarks> MediaPipePoseLandmarkerResult::get_pose_world_landmarks() {
	TypedArray<MediaPipeLandmarks> array;
	auto pose_world_landmarks = result.pose_world_landmarks;
	array.resize(pose_world_landmarks.size());
	for (int i = 0; i < pose_world_landmarks.size(); i++)
		array[i] = memnew(MediaPipeLandmarks(pose_world_landmarks[i]));
	return array;
}

bool MediaPipePoseLandmarkerResult::has_segmentation_masks() {
	return result.segmentation_masks.has_value();
}

void MediaPipePoseLandmarker::_bind_methods() {
	ClassDB::bind_method(D_METHOD(
								 "initialize", "base_options", "running_mode",
								 "num_poses", "min_pose_detection_confidence", "min_pose_presence_confidence", "min_tracking_confidence", "output_segmentation_masks"),
			&MediaPipePoseLandmarker::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(1), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(0.5), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("detect", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipePoseLandmarker::detect, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipePoseLandmarker::detect_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipePoseLandmarker::detect_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipePoseLandmarkerResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipePoseLandmarker::_register_task() {
	ClassDB::register_class<MediaPipePoseLandmarker>();
	ClassDB::register_class<MediaPipePoseLandmarkerResult>();
}

bool MediaPipePoseLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode,
		int num_poses, float min_pose_detection_confidence, float min_pose_presence_confidence, float min_tracking_confidence, bool output_segmentation_masks) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<PoseLandmarkerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = RunningMode(running_mode);
	options->num_poses = num_poses;
	options->min_pose_detection_confidence = min_pose_detection_confidence;
	options->min_pose_presence_confidence = min_pose_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	options->output_segmentation_masks = output_segmentation_masks;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<PoseLandmarkerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipePoseLandmarkerResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipePoseLandmarkerResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = PoseLandmarker::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipePoseLandmarkerResult> MediaPipePoseLandmarker::detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipePoseLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Detect(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		detect_result = Ref(memnew(MediaPipePoseLandmarkerResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

Ref<MediaPipePoseLandmarkerResult> MediaPipePoseLandmarker::detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipePoseLandmarkerResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		detect_result = Ref(memnew(MediaPipePoseLandmarkerResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

bool MediaPipePoseLandmarker::detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipePoseLandmarker);
