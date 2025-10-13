#include "landmark.h"

#include "Engine.hpp"

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
	register_method("get_proto", &MediaPipeLandmark::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeLandmark::make_vector_proto_packet);
}

MediaPipeLandmark *MediaPipeLandmark::_new(const Landmark &landmark) {
	MediaPipeLandmark *l = MediaPipeLandmark::_new();
	l->landmark = landmark;
	return l;
}

mediapipe::Landmark MediaPipeLandmark::to_proto(const Landmark &landmark) {
	ProtoType proto;
	proto.set_x(landmark.x);
	proto.set_y(landmark.y);
	proto.set_z(landmark.z);
	if (landmark.visibility.has_value()) {
		proto.set_visibility(landmark.visibility.value());
	}
	if (landmark.presence.has_value()) {
		proto.set_presence(landmark.presence.value());
	}
	return proto;
}

void MediaPipeLandmark::_init() {}

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

Ref<MediaPipeProto> MediaPipeLandmark::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(landmark)));
}

Ref<MediaPipePacket> MediaPipeLandmark::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeLandmark> landmark = array[i];
		ERR_BREAK(landmark.is_null());
		vector.push_back(to_proto(landmark->landmark));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
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
	register_method("get_proto", &MediaPipeNormalizedLandmark::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeNormalizedLandmark::make_vector_proto_packet);
}

MediaPipeNormalizedLandmark *MediaPipeNormalizedLandmark::_new(const NormalizedLandmark &landmark) {
	MediaPipeNormalizedLandmark *l = MediaPipeNormalizedLandmark::_new();
	l->landmark = landmark;
	return l;
}

mediapipe::NormalizedLandmark MediaPipeNormalizedLandmark::to_proto(const NormalizedLandmark &landmark) {
	ProtoType proto;
	proto.set_x(landmark.x);
	proto.set_y(landmark.y);
	proto.set_z(landmark.z);
	if (landmark.visibility.has_value()) {
		proto.set_visibility(landmark.visibility.value());
	}
	if (landmark.presence.has_value()) {
		proto.set_presence(landmark.presence.value());
	}
	return proto;
}

void MediaPipeNormalizedLandmark::_init() {}

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

Ref<MediaPipeProto> MediaPipeNormalizedLandmark::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(landmark)));
}

Ref<MediaPipePacket> MediaPipeNormalizedLandmark::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeNormalizedLandmark> landmark = array[i];
		ERR_BREAK(landmark.is_null());
		vector.push_back(to_proto(landmark->landmark));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}

void MediaPipeLandmarks::_register_methods() {
	register_method("get_landmarks", &MediaPipeLandmarks::get_landmarks);
	register_method("get_proto", &MediaPipeLandmarks::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeLandmarks::make_vector_proto_packet);
}

MediaPipeLandmarks *MediaPipeLandmarks::_new(const Landmarks &landmarks) {
	MediaPipeLandmarks *l = MediaPipeLandmarks::_new();
	l->landmarks = landmarks;
	return l;
}

mediapipe::LandmarkList MediaPipeLandmarks::to_proto(const Landmarks &landmarks) {
	ProtoType proto;
	proto.mutable_landmark()->Reserve(landmarks.landmarks.size());
	for (const Landmark &landmark : landmarks.landmarks) {
		proto.mutable_landmark()->Add(MediaPipeLandmark::to_proto(landmark));
	}
	return proto;
}

void MediaPipeLandmarks::_init() {}

Array MediaPipeLandmarks::get_landmarks() const {
	Array array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < array.size(); i++)
		array[i] = MediaPipeLandmark::_new(landmarks.landmarks[i]);
	return array;
}

Ref<MediaPipeProto> MediaPipeLandmarks::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(landmarks)));
}

Ref<MediaPipePacket> MediaPipeLandmarks::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeLandmarks> landmarks = array[i];
		ERR_BREAK(landmarks.is_null());
		vector.push_back(to_proto(landmarks->landmarks));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}

void MediaPipeNormalizedLandmarks::_register_methods() {
	register_method("get_landmarks", &MediaPipeNormalizedLandmarks::get_landmarks);
	register_method("get_proto", &MediaPipeNormalizedLandmarks::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeNormalizedLandmarks::make_vector_proto_packet);
}

MediaPipeNormalizedLandmarks *MediaPipeNormalizedLandmarks::_new(const NormalizedLandmarks &landmarks) {
	MediaPipeNormalizedLandmarks *l = MediaPipeNormalizedLandmarks::_new();
	l->landmarks = landmarks;
	return l;
}

mediapipe::NormalizedLandmarkList MediaPipeNormalizedLandmarks::to_proto(const NormalizedLandmarks &landmarks) {
	ProtoType proto;
	proto.mutable_landmark()->Reserve(landmarks.landmarks.size());
	for (const NormalizedLandmark &landmark : landmarks.landmarks) {
		proto.mutable_landmark()->Add(MediaPipeNormalizedLandmark::to_proto(landmark));
	}
	return proto;
}

void MediaPipeNormalizedLandmarks::_init() {}

Array MediaPipeNormalizedLandmarks::get_landmarks() const {
	Array array;
	array.resize(landmarks.landmarks.size());
	for (int i = 0; i < array.size(); i++)
		array[i] = MediaPipeNormalizedLandmark::_new(landmarks.landmarks[i]);
	return array;
}

Ref<MediaPipeProto> MediaPipeNormalizedLandmarks::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(landmarks)));
}

Ref<MediaPipePacket> MediaPipeNormalizedLandmarks::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeNormalizedLandmarks> landmarks = array[i];
		ERR_BREAK(landmarks.is_null());
		vector.push_back(to_proto(landmarks->landmarks));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}
