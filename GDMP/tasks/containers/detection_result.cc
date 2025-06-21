#include "detection_result.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeDetection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_categories"), &MediaPipeDetection::get_categories);
	ClassDB::bind_method(D_METHOD("get_bounding_box"), &MediaPipeDetection::get_bounding_box);
	ClassDB::bind_method(D_METHOD("get_keypoints"), &MediaPipeDetection::get_keypoints);
	ClassDB::bind_method(D_METHOD("has_keypoints"), &MediaPipeDetection::has_keypoints);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "categories", PROPERTY_HINT_ARRAY_TYPE, MediaPipeCategory::get_class_static()), "", "get_categories");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "bounding_box"), "", "get_bounding_box");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "keypoints", PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedKeypoint::get_class_static()), "", "get_keypoints");
}

MediaPipeDetection::MediaPipeDetection() = default;

MediaPipeDetection::MediaPipeDetection(const Detection &detection) {
	this->detection = detection;
}

TypedArray<MediaPipeCategory> MediaPipeDetection::get_categories() const {
	TypedArray<MediaPipeCategory> array;
	array.resize(detection.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = detection.categories[i];
		array[i] = memnew(MediaPipeCategory(category));
	}
	return array;
}

Rect2i MediaPipeDetection::get_bounding_box() const {
	const Rect &rect = detection.bounding_box;
	return Rect2i(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

TypedArray<MediaPipeNormalizedKeypoint> MediaPipeDetection::get_keypoints() const {
	TypedArray<MediaPipeNormalizedKeypoint> array;
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
		array[i] = memnew(MediaPipeNormalizedKeypoint(keypoint));
	}
	return array;
}

bool MediaPipeDetection::has_keypoints() const {
	return detection.keypoints.has_value();
}

void MediaPipeDetectionResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_detections"), &MediaPipeDetectionResult::get_detections);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "detections", PROPERTY_HINT_ARRAY_TYPE, MediaPipeDetection::get_class_static()), "", "get_detections");
}

MediaPipeDetectionResult::MediaPipeDetectionResult() = default;

MediaPipeDetectionResult::MediaPipeDetectionResult(const DetectionResult &result) {
	this->result = result;
}

TypedArray<MediaPipeDetection> MediaPipeDetectionResult::get_detections() const {
	TypedArray<MediaPipeDetection> array;
	array.resize(result.detections.size());
	for (int i = 0; i < array.size(); i++) {
		const Detection &detection = result.detections[i];
		array[i] = memnew(MediaPipeDetection(detection));
	}
	return array;
}
