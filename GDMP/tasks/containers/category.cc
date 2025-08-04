#include "category.h"

#include "Engine.hpp"

void MediaPipeCategory::_register_methods() {
	register_method("get_index", &MediaPipeCategory::get_index);
	register_method("get_score", &MediaPipeCategory::get_score);
	register_method("get_category_name", &MediaPipeCategory::get_category_name);
	register_method("get_display_name", &MediaPipeCategory::get_display_name);
	register_method("has_category_name", &MediaPipeCategory::has_category_name);
	register_method("has_display_name", &MediaPipeCategory::has_display_name);
	register_method("get_proto", &MediaPipeCategory::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeCategory::make_vector_proto_packet);
}

MediaPipeCategory *MediaPipeCategory::_new(const Category &category) {
	MediaPipeCategory *c = MediaPipeCategory::_new();
	c->category = category;
	return c;
}

mediapipe::Classification MediaPipeCategory::to_proto(const Category &category) {
	ProtoType proto;
	proto.set_index(category.index);
	proto.set_score(category.score);
	if (category.category_name.has_value()) {
		proto.set_label(category.category_name.value());
	}
	if (category.display_name.has_value()) {
		proto.set_display_name(category.display_name.value());
	}
	return proto;
}

void MediaPipeCategory::_init() {
	category.index = 0;
	category.score = 0;
}

int MediaPipeCategory::get_index() const {
	return category.index;
}

float MediaPipeCategory::get_score() const {
	return category.score;
}

String MediaPipeCategory::get_category_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return category.category_name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_category_name(), String());
	return category.category_name->c_str();
}

String MediaPipeCategory::get_display_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return category.display_name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_display_name(), String());
	return category.display_name->c_str();
}

bool MediaPipeCategory::has_category_name() const {
	return category.category_name.has_value();
}

bool MediaPipeCategory::has_display_name() const {
	return category.display_name.has_value();
}

Ref<MediaPipeProto> MediaPipeCategory::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(category)));
}

Ref<MediaPipePacket> MediaPipeCategory::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeCategory> category = array[i];
		ERR_BREAK(category.is_null());
		vector.push_back(to_proto(category->category));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}