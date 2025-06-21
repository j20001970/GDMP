#include "landmark.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeLandmark::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_x"), &MediaPipeLandmark::get_x);
	ClassDB::bind_method(D_METHOD("get_y"), &MediaPipeLandmark::get_y);
	ClassDB::bind_method(D_METHOD("get_z"), &MediaPipeLandmark::get_z);
	ClassDB::bind_method(D_METHOD("get_visibility"), &MediaPipeLandmark::get_visibility);
	ClassDB::bind_method(D_METHOD("get_presence"), &MediaPipeLandmark::get_presence);
	ClassDB::bind_method(D_METHOD("get_name"), &MediaPipeLandmark::get_name);
	ClassDB::bind_method(D_METHOD("has_visibility"), &MediaPipeLandmark::has_visibility);
	ClassDB::bind_method(D_METHOD("has_presence"), &MediaPipeLandmark::has_presence);
	ClassDB::bind_method(D_METHOD("has_name"), &MediaPipeLandmark::has_name);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "z"), "", "get_z");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility"), "", "get_visibility");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "presence"), "", "get_presence");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
}

MediaPipeLandmark::MediaPipeLandmark() {
	landmark.x = 0;
	landmark.y = 0;
	landmark.z = 0;
}

MediaPipeLandmark::MediaPipeLandmark(const Landmark &landmark) {
	this->landmark = landmark;
}

float MediaPipeLandmark::get_x() const {
	return landmark.x;
}

float MediaPipeLandmark::get_y() const {
	return landmark.y;
}

float MediaPipeLandmark::get_z() const {
	return landmark.z;
}

float MediaPipeLandmark::get_visibility() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.visibility.value_or(0);
	}
	ERR_FAIL_COND_V(!has_visibility(), 0);
	return landmark.visibility.value();
}

float MediaPipeLandmark::get_presence() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.presence.value_or(0);
	}
	ERR_FAIL_COND_V(!has_presence(), 0);
	return landmark.presence.value();
}

String MediaPipeLandmark::get_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_name(), String());
	return landmark.name.value().c_str();
}

bool MediaPipeLandmark::has_visibility() const {
	return landmark.visibility.has_value();
}

bool MediaPipeLandmark::has_presence() const {
	return landmark.presence.has_value();
}

bool MediaPipeLandmark::has_name() const {
	return landmark.name.has_value();
}

void MediaPipeNormalizedLandmark::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_x"), &MediaPipeNormalizedLandmark::get_x);
	ClassDB::bind_method(D_METHOD("get_y"), &MediaPipeNormalizedLandmark::get_y);
	ClassDB::bind_method(D_METHOD("get_z"), &MediaPipeNormalizedLandmark::get_z);
	ClassDB::bind_method(D_METHOD("get_visibility"), &MediaPipeNormalizedLandmark::get_visibility);
	ClassDB::bind_method(D_METHOD("get_presence"), &MediaPipeNormalizedLandmark::get_presence);
	ClassDB::bind_method(D_METHOD("get_name"), &MediaPipeNormalizedLandmark::get_name);
	ClassDB::bind_method(D_METHOD("has_visibility"), &MediaPipeNormalizedLandmark::has_visibility);
	ClassDB::bind_method(D_METHOD("has_presence"), &MediaPipeNormalizedLandmark::has_presence);
	ClassDB::bind_method(D_METHOD("has_name"), &MediaPipeNormalizedLandmark::has_name);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "z"), "", "get_z");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility"), "", "get_visibility");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "presence"), "", "get_presence");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "", "get_name");
}

MediaPipeNormalizedLandmark::MediaPipeNormalizedLandmark() {
	landmark.x = 0;
	landmark.y = 0;
	landmark.z = 0;
}

MediaPipeNormalizedLandmark::MediaPipeNormalizedLandmark(const NormalizedLandmark &landmark) {
	this->landmark = landmark;
}

float MediaPipeNormalizedLandmark::get_x() const {
	return landmark.x;
}

float MediaPipeNormalizedLandmark::get_y() const {
	return landmark.y;
}

float MediaPipeNormalizedLandmark::get_z() const {
	return landmark.z;
}

float MediaPipeNormalizedLandmark::get_visibility() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.visibility.value_or(0);
	}
	ERR_FAIL_COND_V(!has_visibility(), 0);
	return landmark.visibility.value();
}

float MediaPipeNormalizedLandmark::get_presence() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.presence.value_or(0);
	}
	ERR_FAIL_COND_V(!has_presence(), 0);
	return landmark.presence.value();
}

String MediaPipeNormalizedLandmark::get_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return landmark.name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_name(), String());
	return landmark.name.value().c_str();
}

bool MediaPipeNormalizedLandmark::has_visibility() const {
	return landmark.visibility.has_value();
}

bool MediaPipeNormalizedLandmark::has_presence() const {
	return landmark.presence.has_value();
}

bool MediaPipeNormalizedLandmark::has_name() const {
	return landmark.name.has_value();
}

void MediaPipeLandmarks::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_landmarks"), &MediaPipeLandmarks::get_landmarks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "landmarks", godot::PROPERTY_HINT_ARRAY_TYPE, MediaPipeLandmark::get_class_static()), "", "get_landmarks");
}

MediaPipeLandmarks::MediaPipeLandmarks() = default;

MediaPipeLandmarks::MediaPipeLandmarks(const Landmarks &landmarks) {
	this->landmarks = landmarks;
}

TypedArray<MediaPipeLandmark> MediaPipeLandmarks::get_landmarks() const {
	TypedArray<MediaPipeLandmark> array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < array.size(); i++) {
		const Landmark &landmark = landmarks.landmarks[i];
		array[i] = memnew(MediaPipeLandmark(landmark));
	}
	return array;
}

void MediaPipeNormalizedLandmarks::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_landmarks"), &MediaPipeNormalizedLandmarks::get_landmarks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "landmarks", godot::PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedLandmark::get_class_static()), "", "get_landmarks");
}

MediaPipeNormalizedLandmarks::MediaPipeNormalizedLandmarks() = default;

MediaPipeNormalizedLandmarks::MediaPipeNormalizedLandmarks(const NormalizedLandmarks &landmarks) {
	this->landmarks = landmarks;
}

TypedArray<MediaPipeNormalizedLandmark> MediaPipeNormalizedLandmarks::get_landmarks() const {
	TypedArray<MediaPipeNormalizedLandmark> array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < array.size(); i++) {
		const NormalizedLandmark &landmark = landmarks.landmarks[i];
		array[i] = memnew(MediaPipeNormalizedLandmark(landmark));
	}
	return array;
}
