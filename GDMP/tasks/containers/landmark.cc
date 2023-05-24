#include "landmark.h"

void MediaPipeLandmark::_register_methods() {
	register_method("get_x", &MediaPipeLandmark::get_x);
	register_method("get_y", &MediaPipeLandmark::get_y);
	register_method("get_z", &MediaPipeLandmark::get_z);
	register_method("get_visibility", &MediaPipeLandmark::get_visibility);
	register_method("get_presence", &MediaPipeLandmark::get_presence);
	register_method("get_name", &MediaPipeLandmark::get_name);
	register_method("has_visibility", &MediaPipeLandmark::has_visibility);
	register_method("has_presence", &MediaPipeLandmark::has_presence);
	register_method("has_name", &MediaPipeLandmark::has_name);
}

MediaPipeLandmark *MediaPipeLandmark::_new(const Landmark &landmark) {
	MediaPipeLandmark *l = MediaPipeLandmark::_new();
	l->landmark = landmark;
	return l;
}

void MediaPipeLandmark::_init() {}

float MediaPipeLandmark::get_x() {
	return landmark.x;
}

float MediaPipeLandmark::get_y() {
	return landmark.y;
}

float MediaPipeLandmark::get_z() {
	return landmark.z;
}

float MediaPipeLandmark::get_visibility() {
	if (!has_visibility())
		return 0;
	return landmark.visibility.value();
}

float MediaPipeLandmark::get_presence() {
	if (!has_presence())
		return 0;
	return landmark.presence.value();
}

String MediaPipeLandmark::get_name() {
	if (!has_name())
		return String();
	return landmark.name.value().c_str();
}

bool MediaPipeLandmark::has_visibility() {
	return landmark.visibility.has_value();
}

bool MediaPipeLandmark::has_presence() {
	return landmark.presence.has_value();
}

bool MediaPipeLandmark::has_name() {
	return landmark.name.has_value();
}

void MediaPipeNormalizedLandmark::_register_methods() {
	register_method("get_x", &MediaPipeNormalizedLandmark::get_x);
	register_method("get_y", &MediaPipeNormalizedLandmark::get_y);
	register_method("get_z", &MediaPipeNormalizedLandmark::get_z);
	register_method("get_visibility", &MediaPipeNormalizedLandmark::get_visibility);
	register_method("get_presence", &MediaPipeNormalizedLandmark::get_presence);
	register_method("get_name", &MediaPipeNormalizedLandmark::get_name);
	register_method("has_visibility", &MediaPipeNormalizedLandmark::has_visibility);
	register_method("has_presence", &MediaPipeNormalizedLandmark::has_presence);
	register_method("has_name", &MediaPipeNormalizedLandmark::has_name);
}

MediaPipeNormalizedLandmark *MediaPipeNormalizedLandmark::_new(const NormalizedLandmark &landmark) {
	MediaPipeNormalizedLandmark *l = MediaPipeNormalizedLandmark::_new();
	l->landmark = landmark;
	return l;
}

void MediaPipeNormalizedLandmark::_init() {}

float MediaPipeNormalizedLandmark::get_x() {
	return landmark.x;
}

float MediaPipeNormalizedLandmark::get_y() {
	return landmark.y;
}

float MediaPipeNormalizedLandmark::get_z() {
	return landmark.z;
}

float MediaPipeNormalizedLandmark::get_visibility() {
	if (!has_visibility())
		return 0;
	return landmark.visibility.value();
}

float MediaPipeNormalizedLandmark::get_presence() {
	if (!has_presence())
		return 0;
	return landmark.presence.value();
}

String MediaPipeNormalizedLandmark::get_name() {
	if (!has_name())
		return String();
	return landmark.name.value().c_str();
}

bool MediaPipeNormalizedLandmark::has_visibility() {
	return landmark.visibility.has_value();
}

bool MediaPipeNormalizedLandmark::has_presence() {
	return landmark.presence.has_value();
}

bool MediaPipeNormalizedLandmark::has_name() {
	return landmark.name.has_value();
}

void MediaPipeLandmarks::_register_methods() {
	register_method("get_landmarks", &MediaPipeLandmarks::get_landmarks);
}

MediaPipeLandmarks *MediaPipeLandmarks::_new(const Landmarks &landmarks) {
	MediaPipeLandmarks *l = MediaPipeLandmarks::_new();
	l->landmarks = landmarks;
	return l;
}

void MediaPipeLandmarks::_init() {}

Array MediaPipeLandmarks::get_landmarks() {
	Array array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < landmarks.landmarks.size(); i++)
		array[i] = MediaPipeLandmark::_new(landmarks.landmarks[i]);
	return array;
}

void MediaPipeNormalizedLandmarks::_register_methods() {
	register_method("get_landmarks", &MediaPipeNormalizedLandmarks::get_landmarks);
}

MediaPipeNormalizedLandmarks *MediaPipeNormalizedLandmarks::_new(const NormalizedLandmarks &landmarks) {
	MediaPipeNormalizedLandmarks *l = MediaPipeNormalizedLandmarks::_new();
	l->landmarks = landmarks;
	return l;
}

void MediaPipeNormalizedLandmarks::_init() {}

Array MediaPipeNormalizedLandmarks::get_landmarks() {
	Array array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < landmarks.landmarks.size(); i++)
		array[i] = MediaPipeNormalizedLandmark::_new(landmarks.landmarks[i]);
	return array;
}
