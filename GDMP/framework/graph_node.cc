#include "graph_node.h"

#include <string>

#include "String.hpp"

void MediaPipeGraphNode::_register_methods() {
	register_method("get_options", &MediaPipeGraphNode::get_options);
	register_method("set_options", &MediaPipeGraphNode::set_options);
}

void MediaPipeGraphNode::_init() {
	node = nullptr;
}

MediaPipeGraphNode *MediaPipeGraphNode::_new(mediapipe::api2::builder::GenericNode *node) {
	MediaPipeGraphNode *n = MediaPipeGraphNode::_new();
	n->node = node;
	return n;
}

void MediaPipeGraphNode::invalidate() {
	node = nullptr;
}

Ref<MediaPipeProto> MediaPipeGraphNode::get_options() {
	ERR_FAIL_COND_V(node == nullptr, nullptr);
	protobuf::Any any = node->GetOptions<protobuf::Any>();
	String type_url = String(any.type_url().data());
	String type_name = type_url.substr(1 + type_url.find("/"), -1);
	ERR_FAIL_COND_V(type_name.empty(), nullptr);
	const std::string &value = any.value();
	Ref<MediaPipeProto> proto = Ref(MediaPipeProto::_new());
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
