#include "keypoint.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeNormalizedKeypoint::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_point"), &MediaPipeNormalizedKeypoint::get_point);
	ClassDB::bind_method(D_METHOD("get_label"), &MediaPipeNormalizedKeypoint::get_label);
	ClassDB::bind_method(D_METHOD("get_score"), &MediaPipeNormalizedKeypoint::get_score);
	ClassDB::bind_method(D_METHOD("has_label"), &MediaPipeNormalizedKeypoint::has_label);
	ClassDB::bind_method(D_METHOD("has_score"), &MediaPipeNormalizedKeypoint::has_score);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "point"), "", "get_point");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "label"), "", "get_label");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "", "get_score");
}

MediaPipeNormalizedKeypoint::MediaPipeNormalizedKeypoint() = default;

MediaPipeNormalizedKeypoint::MediaPipeNormalizedKeypoint(const NormalizedKeypoint &keypoint) {
	this->keypoint = keypoint;
}

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
