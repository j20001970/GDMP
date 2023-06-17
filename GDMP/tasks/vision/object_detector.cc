#include "object_detector.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

#include "GDMP/tasks/vision/vision_task.h"

void MediaPipeObjectDetector::_bind_methods() {
	ClassDB::bind_method(D_METHOD(
								 "initialize", "base_options", "running_mode",
								 "display_names_locale", "max_results", "score_threshold",
								 "category_allowlist", "category_denylist"),
			&MediaPipeObjectDetector::initialize, DEFVAL(RUNNING_MODE_IMAGE), DEFVAL(String("en")), DEFVAL(-1), DEFVAL(0.0f), DEFVAL(PackedStringArray()), DEFVAL(PackedStringArray()));
	ClassDB::bind_method(D_METHOD("detect", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeObjectDetector::detect, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeObjectDetector::detect_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("detect_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeObjectDetector::detect_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeDetectionResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeObjectDetector::_register_task() {
	ClassDB::register_class<MediaPipeObjectDetector>();
}

bool MediaPipeObjectDetector::initialize(
		Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode,
		const String &display_names_locale, int max_results, float score_threshold,
		PackedStringArray category_allowlist, PackedStringArray category_denylist) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<ObjectDetectorOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = RunningMode(running_mode);
	options->display_names_locale = display_names_locale.utf8().get_data();
	options->max_results = max_results;
	options->score_threshold = score_threshold;
	if (category_allowlist.size())
		for (auto e : category_allowlist)
			options->category_allowlist.push_back(e.utf8().get_data());
	if (category_denylist.size())
		for (auto e : category_denylist)
			options->category_denylist.push_back(e.utf8().get_data());
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<ObjectDetectorResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeDetectionResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipeDetectionResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = ObjectDetector::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeDetectionResult> MediaPipeObjectDetector::detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeDetectionResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Detect(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		detect_result = Ref(memnew(MediaPipeDetectionResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

Ref<MediaPipeDetectionResult> MediaPipeObjectDetector::detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeDetectionResult> detect_result;
	ERR_FAIL_COND_V(image.is_null(), detect_result);
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		detect_result = Ref(memnew(MediaPipeDetectionResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

bool MediaPipeObjectDetector::detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->DetectAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeObjectDetector);
