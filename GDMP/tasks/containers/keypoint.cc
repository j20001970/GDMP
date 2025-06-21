#include "keypoint.h"

#include "Engine.hpp"

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

void MediaPipeNormalizedKeypoint::_init() {
	keypoint.x = 0;
	keypoint.y = 0;
}

float MediaPipeNormalizedKeypoint::get_x() const {
	return keypoint.x;
}

float MediaPipeNormalizedKeypoint::get_y() const {
	return keypoint.y;
}

Vector2 MediaPipeNormalizedKeypoint::get_point() const {
	return Vector2(keypoint.x, keypoint.y);
}

String MediaPipeNormalizedKeypoint::get_label() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return keypoint.label.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_label(), String());
	return keypoint.label->c_str();
}

float MediaPipeNormalizedKeypoint::get_score() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return keypoint.score.value_or(0);
	}
	ERR_FAIL_COND_V(!has_score(), 0);
	return keypoint.score.value();
}

bool MediaPipeNormalizedKeypoint::has_label() const {
	return keypoint.label.has_value();
}

bool MediaPipeNormalizedKeypoint::has_score() const {
	return keypoint.score.has_value();
}
