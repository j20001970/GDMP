#include "detection_result.h"

#include "Engine.hpp"

#include "GDMP/tasks/containers/category.h"
#include "GDMP/tasks/containers/keypoint.h"

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

Array MediaPipeDetection::get_categories() const {
	Array array;
	array.resize(detection.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = detection.categories[i];
		array[i] = MediaPipeCategory::_new(category);
	}
	return array;
}

Rect2 MediaPipeDetection::get_bounding_box() const {
	const Rect &rect = detection.bounding_box;
	return Rect2(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

Array MediaPipeDetection::get_keypoints() const {
	Array array;
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		if (!has_keypoints()) {
			return array;
		}
	}
	ERR_FAIL_COND_V(!has_keypoints(), array);
	const std::vector<NormalizedKeypoint> &keypoints = detection.keypoints.value();
	array.resize(keypoints.size());
	for (int i = 0; i < array.size(); i++) {
		const NormalizedKeypoint &keypoint = keypoints[i];
		array[i] = MediaPipeNormalizedKeypoint::_new(keypoint);
	}
	return array;
}

bool MediaPipeDetection::has_keypoints() const {
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

Array MediaPipeDetectionResult::get_detections() const {
	Array array;
	array.resize(result.detections.size());
	for (int i = 0; i < array.size(); i++) {
		const Detection &detection = result.detections[i];
		array[i] = MediaPipeDetection::_new(detection);
	}
	return array;
}
