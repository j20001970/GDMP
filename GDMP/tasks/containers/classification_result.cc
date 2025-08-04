#include "classification_result.h"

#include "Engine.hpp"

#include "GDMP/tasks/containers/category.h"

void MediaPipeClassifications::_register_methods() {
	register_method("get_categories", &MediaPipeClassifications::get_categories);
	register_method("get_head_index", &MediaPipeClassifications::get_head_index);
	register_method("get_head_name", &MediaPipeClassifications::get_head_name);
	register_method("has_head_name", &MediaPipeClassifications::has_head_name);
	register_method("get_proto", &MediaPipeClassifications::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeClassifications::make_vector_proto_packet);
}

MediaPipeClassifications *MediaPipeClassifications::_new(const Classifications &classifications) {
	MediaPipeClassifications *c = MediaPipeClassifications::_new();
	c->classifications = classifications;
	return c;
}

mediapipe::ClassificationList MediaPipeClassifications::to_proto(const Classifications &classifications) {
	ProtoType proto;
	proto.mutable_classification()->Reserve(classifications.categories.size());
	for (const Category &category : classifications.categories) {
		proto.mutable_classification()->Add(MediaPipeCategory::to_proto(category));
	}
	return proto;
}

void MediaPipeClassifications::_init() {
	classifications.head_index = 0;
}

Array MediaPipeClassifications::get_categories() const {
	Array array;
	array.resize(classifications.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = classifications.categories[i];
		array[i] = MediaPipeCategory::_new(category);
	}
	return array;
}

int MediaPipeClassifications::get_head_index() const {
	return classifications.head_index;
}

String MediaPipeClassifications::get_head_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return classifications.head_name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_head_name(), String());
	return classifications.head_name->c_str();
}

bool MediaPipeClassifications::has_head_name() const {
	return classifications.head_name.has_value();
}

Ref<MediaPipeProto> MediaPipeClassifications::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(classifications)));
}

Ref<MediaPipePacket> MediaPipeClassifications::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeClassifications> classifications = array[i];
		ERR_BREAK(classifications.is_null());
		vector.push_back(to_proto(classifications->classifications));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}

void MediaPipeClassificationResult::_register_methods() {
	register_method("get_classifications", &MediaPipeClassificationResult::get_classifications);
	register_method("get_timestamp_ms", &MediaPipeClassificationResult::get_timestamp_ms);
	register_method("has_timestamp_ms", &MediaPipeClassificationResult::has_timestamp_ms);
}

MediaPipeClassificationResult *MediaPipeClassificationResult::_new(const ClassificationResult &result) {
	MediaPipeClassificationResult *r = MediaPipeClassificationResult::_new();
	r->result = result;
	return r;
}

void MediaPipeClassificationResult::_init() {}

Array MediaPipeClassificationResult::get_classifications() const {
	Array array;
	array.resize(result.classifications.size());
	for (int i = 0; i < array.size(); i++) {
		const Classifications &classifications = result.classifications[i];
		array[i] = MediaPipeClassifications::_new(classifications);
	}
	return array;
}

uint64_t MediaPipeClassificationResult::get_timestamp_ms() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return result.timestamp_ms.value_or(0);
	}
	ERR_FAIL_COND_V(!has_timestamp_ms(), 0);
	return result.timestamp_ms.value();
}

bool MediaPipeClassificationResult::has_timestamp_ms() const {
	return result.timestamp_ms.has_value();
}
