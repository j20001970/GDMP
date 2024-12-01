#include "graph_node.h"

#include <string>

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string.hpp"

void MediaPipeGraphNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_options"), &MediaPipeGraphNode::get_options);
	ClassDB::bind_method(D_METHOD("set_options", "options"), &MediaPipeGraphNode::set_options);
}

MediaPipeGraphNode::MediaPipeGraphNode() {
	node = nullptr;
}

MediaPipeGraphNode::MediaPipeGraphNode(mediapipe::api2::builder::GenericNode *node) {
	this->node = node;
}

void MediaPipeGraphNode::invalidate() {
	node = nullptr;
}

Ref<MediaPipeProto> MediaPipeGraphNode::get_options() {
	ERR_FAIL_COND_V(node == nullptr, nullptr);
	protobuf::Any any = node->GetOptions<protobuf::Any>();
	String type_url = String(any.type_url().data());
	String type_name = type_url.substr(1 + type_url.find("/"));
	ERR_FAIL_COND_V(type_name.is_empty(), nullptr);
	const std::string &value = any.value();
	Ref<MediaPipeProto> proto = memnew(MediaPipeProto());
	ERR_FAIL_COND_V(!proto->initialize(type_name), nullptr);
	ERR_FAIL_COND_V(!proto->get_message()->ParseFromString(value), nullptr);
	return proto;
}

bool MediaPipeGraphNode::set_options(Ref<MediaPipeProto> proto) {
	ERR_FAIL_COND_V(node == nullptr, false);
	ERR_FAIL_COND_V(!proto->is_initialized(), false);
	std::string type_name = proto->get_type_name().utf8().get_data();
	type_name = "type.googleapis.com/" + type_name;
	auto bytes = proto->get_message()->SerializeAsString();
	node->GetOptions<protobuf::Any>().mutable_type_url()->swap(type_name);
	node->GetOptions<protobuf::Any>().mutable_value()->swap(bytes);
	return true;
}

builder::GenericNode *MediaPipeGraphNode::get_node() {
	return node;
}
