#include "category.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeCategory::_bind_methods() {
	ClassDB::bind_static_method(MediaPipeCategory::get_class_static(), D_METHOD("make_vector_proto_packet", "array"), &MediaPipeCategory::make_vector_proto_packet);
	ClassDB::bind_method(D_METHOD("get_index"), &MediaPipeCategory::get_index);
	ClassDB::bind_method(D_METHOD("get_score"), &MediaPipeCategory::get_score);
	ClassDB::bind_method(D_METHOD("get_category_name"), &MediaPipeCategory::get_category_name);
	ClassDB::bind_method(D_METHOD("get_display_name"), &MediaPipeCategory::get_display_name);
	ClassDB::bind_method(D_METHOD("has_category_name"), &MediaPipeCategory::has_category_name);
	ClassDB::bind_method(D_METHOD("has_display_name"), &MediaPipeCategory::has_display_name);
	ClassDB::bind_method(D_METHOD("get_proto"), &MediaPipeCategory::get_proto);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "index"), "", "get_index");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "category_name"), "", "get_category_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "", "get_display_name");
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

Ref<MediaPipePacket> MediaPipeCategory::make_vector_proto_packet(TypedArray<MediaPipeCategory> array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeCategory> category = array[i];
		ERR_BREAK(category.is_null());
		vector.push_back(to_proto(category->category));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return memnew(MediaPipePacket(packet));
}

MediaPipeCategory::MediaPipeCategory() = default;

MediaPipeCategory::MediaPipeCategory(const Category &category) {
	this->category = category;
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
	return memnew(MediaPipeProto(to_proto(category)));
}
