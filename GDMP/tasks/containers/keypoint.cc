#include "keypoint.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeNormalizedKeypoint::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_x"), &MediaPipeNormalizedKeypoint::get_x);
	ClassDB::bind_method(D_METHOD("get_y"), &MediaPipeNormalizedKeypoint::get_y);
	ClassDB::bind_method(D_METHOD("get_point"), &MediaPipeNormalizedKeypoint::get_point);
	ClassDB::bind_method(D_METHOD("get_label"), &MediaPipeNormalizedKeypoint::get_label);
	ClassDB::bind_method(D_METHOD("get_score"), &MediaPipeNormalizedKeypoint::get_score);
	ClassDB::bind_method(D_METHOD("has_label"), &MediaPipeNormalizedKeypoint::has_label);
	ClassDB::bind_method(D_METHOD("has_score"), &MediaPipeNormalizedKeypoint::has_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "point"), "", "get_point");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "label"), "", "get_label");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "", "get_score");
}

MediaPipeNormalizedKeypoint::MediaPipeNormalizedKeypoint() {
	keypoint.x = 0;
	keypoint.y = 0;
}

MediaPipeNormalizedKeypoint::MediaPipeNormalizedKeypoint(const NormalizedKeypoint &keypoint) {
	this->keypoint = keypoint;
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
