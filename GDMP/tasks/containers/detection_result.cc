#include "detection_result.h"

void MediaPipeDetection::_register_methods() {
	register_method("get_categories", &MediaPipeDetection::get_categories);
	register_method("get_bounding_box", &MediaPipeDetection::get_bounding_box);
	register_method("get_keypoints", &MediaPipeDetection::get_keypoints);
	register_method("has_keypoints", &MediaPipeDetection::has_keypoints);
}

MediaPipeDetection *MediaPipeDetection::_new(const Detection &detection) {
	MediaPipeDetection *d = MediaPipeDetection::_new();
	d->detection = detection;
	return d;
}

void MediaPipeDetection::_init() {}

Array MediaPipeDetection::get_categories() {
	Array array;
	auto &categories = detection.categories;
	array.resize(categories.size());
	for (int i = 0; i < categories.size(); i++)
		array[i] = MediaPipeCategory::_new(categories[i]);
	return array;
}

Rect2 MediaPipeDetection::get_bounding_box() {
	auto &rect = detection.bounding_box;
	return Rect2(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

Array MediaPipeDetection::get_keypoints() {
	Array array;
	if (!has_keypoints())
		return array;
	auto &keypoints = detection.keypoints.value();
	array.resize(keypoints.size());
	for (int i = 0; i < keypoints.size(); i++)
		array[i] = MediaPipeNormalizedKeypoint::_new(keypoints[i]);
	return array;
}

bool MediaPipeDetection::has_keypoints() {
	return detection.keypoints.has_value();
}

void MediaPipeDetectionResult::_register_methods() {
	register_method("get_detections", &MediaPipeDetectionResult::get_detections);
}

MediaPipeDetectionResult *MediaPipeDetectionResult::_new(const DetectionResult &result) {
	MediaPipeDetectionResult *r = MediaPipeDetectionResult::_new();
	r->result = result;
	return r;
}

void MediaPipeDetectionResult::_init() {}

Array MediaPipeDetectionResult::get_detections() {
	Array array;
	auto &detections = result.detections;
	array.resize(detections.size());
	for (int i = 0; i < detections.size(); i++)
		array[i] = MediaPipeDetection::_new(detections[i]);
	return array;
}
