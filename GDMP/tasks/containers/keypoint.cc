#include "keypoint.h"

#include "Engine.hpp"

void MediaPipeNormalizedKeypoint::_register_methods() {
	register_method("get_point", &MediaPipeNormalizedKeypoint::get_point);
	register_method("get_label", &MediaPipeNormalizedKeypoint::get_label);
	register_method("get_score", &MediaPipeNormalizedKeypoint::get_score);
	register_method("has_label", &MediaPipeNormalizedKeypoint::has_label);
	register_method("has_score", &MediaPipeNormalizedKeypoint::has_score);
	register_method("get_proto", &MediaPipeNormalizedKeypoint::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeNormalizedKeypoint::make_vector_proto_packet);
}

MediaPipeNormalizedKeypoint *MediaPipeNormalizedKeypoint::_new(const NormalizedKeypoint &keypoint) {
	MediaPipeNormalizedKeypoint *k = MediaPipeNormalizedKeypoint::_new();
	k->keypoint = keypoint;
	return k;
}

mediapipe::LocationData::RelativeKeypoint MediaPipeNormalizedKeypoint::to_proto(const NormalizedKeypoint &keypoint) {
	ProtoType proto;
	proto.set_x(keypoint.x);
	proto.set_y(keypoint.y);
	if (keypoint.label.has_value()) {
		proto.set_keypoint_label(keypoint.label.value());
	}
	if (keypoint.score.has_value()) {
		proto.set_score(keypoint.score.value());
	}
	return proto;
}

void MediaPipeNormalizedKeypoint::_init() {}

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

Ref<MediaPipeProto> MediaPipeNormalizedKeypoint::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(keypoint)));
}

Ref<MediaPipePacket> MediaPipeNormalizedKeypoint::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeNormalizedKeypoint> keypoint = array[i];
		ERR_BREAK(keypoint.is_null());
		vector.push_back(to_proto(keypoint->keypoint));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}
