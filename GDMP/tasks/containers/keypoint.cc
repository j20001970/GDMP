#include "keypoint.h"

void MediaPipeNormalizedKeypoint::_register_methods() {
	register_method("get_point", &MediaPipeNormalizedKeypoint::get_point);
	register_method("get_label", &MediaPipeNormalizedKeypoint::get_label);
	register_method("get_score", &MediaPipeNormalizedKeypoint::get_score);
	register_method("has_label", &MediaPipeNormalizedKeypoint::has_label);
	register_method("has_score", &MediaPipeNormalizedKeypoint::has_score);
}

MediaPipeNormalizedKeypoint *MediaPipeNormalizedKeypoint::_new(const NormalizedKeypoint &keypoint) {
	MediaPipeNormalizedKeypoint *k = MediaPipeNormalizedKeypoint::_new();
	k->keypoint = keypoint;
	return k;
}

void MediaPipeNormalizedKeypoint::_init() {}

Vector2 MediaPipeNormalizedKeypoint::get_point() {
	return Vector2(keypoint.x, keypoint.y);
}

String MediaPipeNormalizedKeypoint::get_label() {
	if (!has_label())
		return String();
	return keypoint.label->c_str();
}

float MediaPipeNormalizedKeypoint::get_score() {
	if (!has_score())
		return 0;
	return keypoint.score.value();
}

bool MediaPipeNormalizedKeypoint::has_label() {
	return keypoint.label.has_value();
}

bool MediaPipeNormalizedKeypoint::has_score() {
	return keypoint.score.has_value();
}
