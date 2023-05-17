#include "detection_result.h"

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

TypedArray<MediaPipeCategory> MediaPipeDetection::get_categories() {
	TypedArray<MediaPipeCategory> array;
	auto &categories = detection.categories;
	array.resize(categories.size());
	for (int i = 0; i < categories.size(); i++)
		array[i] = memnew(MediaPipeCategory(categories[i]));
	return array;
}

Rect2i MediaPipeDetection::get_bounding_box() {
	auto &rect = detection.bounding_box;
	return Rect2i(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

TypedArray<MediaPipeNormalizedKeypoint> MediaPipeDetection::get_keypoints() {
	TypedArray<MediaPipeNormalizedKeypoint> array;
	if (!has_keypoints())
		return array;
	auto &keypoints = detection.keypoints.value();
	array.resize(keypoints.size());
	for (int i = 0; i < keypoints.size(); i++)
		array[i] = memnew(MediaPipeNormalizedKeypoint(keypoints[i]));
	return array;
}

bool MediaPipeDetection::has_keypoints() {
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

TypedArray<MediaPipeDetection> MediaPipeDetectionResult::get_detections() {
	TypedArray<MediaPipeDetection> array;
	auto &detections = result.detections;
	array.resize(detections.size());
	for (int i = 0; i < detections.size(); i++)
		array[i] = memnew(MediaPipeDetection(detections[i]));
	return array;
}
