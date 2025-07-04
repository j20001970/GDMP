#include "vision_task.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeVisionTask::_bind_methods() {
	BIND_ENUM_CONSTANT(RUNNING_MODE_IMAGE);
	BIND_ENUM_CONSTANT(RUNNING_MODE_VIDEO);
	BIND_ENUM_CONSTANT(RUNNING_MODE_LIVE_STREAM);
}

void MediaPipeVisionTask::_register_task() {
	ClassDB::register_abstract_class<MediaPipeVisionTask>();
	_register_tasks();
}

RunningMode get_running_mode(MediaPipeVisionTask::RunningMode p_mode) {
	return RunningMode(p_mode);
}

ImageProcessingOptions make_image_processing_options(Rect2 roi, int rotation_degrees) {
	ImageProcessingOptions image_processing_options;
	Vector2 roi_start = roi.get_position();
	Vector2 roi_end = roi.get_end();
	image_processing_options.region_of_interest->left = roi_start.x;
	image_processing_options.region_of_interest->top = roi_start.y;
	image_processing_options.region_of_interest->right = roi_end.x;
	image_processing_options.region_of_interest->bottom = roi_end.y;
	image_processing_options.rotation_degrees = rotation_degrees;
	return image_processing_options;
}

GDMP_REGISTER_TASK(MediaPipeVisionTask, MediaPipeTask)
