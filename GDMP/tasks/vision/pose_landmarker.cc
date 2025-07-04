#include "pose_landmarker.h"

#include "GDMP/tasks/containers/landmark.h"

void MediaPipePoseLandmarkerResult::_register_methods() {
	register_method("get_segmentation_masks", &MediaPipePoseLandmarkerResult::get_segmentation_masks);
	register_method("get_pose_landmarks", &MediaPipePoseLandmarkerResult::get_pose_landmarks);
	register_method("get_pose_world_landmarks", &MediaPipePoseLandmarkerResult::get_pose_world_landmarks);
	register_method("has_segmentation_masks", &MediaPipePoseLandmarkerResult::has_segmentation_masks);
}

MediaPipePoseLandmarkerResult *MediaPipePoseLandmarkerResult::_new(const PoseLandmarkerResult &result) {
	MediaPipePoseLandmarkerResult *r = MediaPipePoseLandmarkerResult::_new();
	r->result = result;
	return r;
}

void MediaPipePoseLandmarkerResult::_init() {}

Array MediaPipePoseLandmarkerResult::get_segmentation_masks() {
	Array array;
	if (!has_segmentation_masks())
		return array;
	auto segmentation_masks = result.segmentation_masks.value();
	array.resize(segmentation_masks.size());
	for (int i = 0; i < segmentation_masks.size(); i++)
		array[i] = MediaPipeImage::_new(segmentation_masks[i]);
	return array;
}

Array MediaPipePoseLandmarkerResult::get_pose_landmarks() {
	Array array;
	auto pose_landmarks = result.pose_landmarks;
	array.resize(pose_landmarks.size());
	for (int i = 0; i < pose_landmarks.size(); i++)
		array[i] = MediaPipeNormalizedLandmarks::_new(pose_landmarks[i]);
	return array;
}

Array MediaPipePoseLandmarkerResult::get_pose_world_landmarks() {
	Array array;
	auto pose_world_landmarks = result.pose_world_landmarks;
	array.resize(pose_world_landmarks.size());
	for (int i = 0; i < pose_world_landmarks.size(); i++)
		array[i] = MediaPipeLandmarks::_new(pose_world_landmarks[i]);
	return array;
}

bool MediaPipePoseLandmarkerResult::has_segmentation_masks() {
	return result.segmentation_masks.has_value();
}

void MediaPipePoseLandmarker::_register_methods() {
	register_method("initialize", &MediaPipePoseLandmarker::initialize);
	register_method("detect", &MediaPipePoseLandmarker::detect);
	register_method("detect_video", &MediaPipePoseLandmarker::detect_video);
	register_method("detect_async", &MediaPipePoseLandmarker::detect_async);
	register_signal<MediaPipePoseLandmarker>("result_callback");
}

void MediaPipePoseLandmarker::_register_task() {
	register_class<MediaPipePoseLandmarker>();
	register_class<MediaPipePoseLandmarkerResult>();
}

void MediaPipePoseLandmarker::_init() {}

bool MediaPipePoseLandmarker::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
		int num_poses, float min_pose_detection_confidence, float min_pose_presence_confidence, float min_tracking_confidence, bool output_segmentation_masks) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<PoseLandmarkerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->num_poses = num_poses;
	options->min_pose_detection_confidence = min_pose_detection_confidence;
	options->min_pose_presence_confidence = min_pose_presence_confidence;
	options->min_tracking_confidence = min_tracking_confidence;
	options->output_segmentation_masks = output_segmentation_masks;
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<PoseLandmarkerResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipePoseLandmarkerResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipePoseLandmarkerResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
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
		detect_result = Ref(MediaPipePoseLandmarkerResult::_new(result.value()));
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
		detect_result = Ref(MediaPipePoseLandmarkerResult::_new(result.value()));
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

GDMP_REGISTER_TASK(MediaPipePoseLandmarker, MediaPipeVisionTask);
