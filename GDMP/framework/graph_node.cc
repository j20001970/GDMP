#include "graph_node.h"

#include <string>

void MediaPipeGraphNode::_register_methods() {
	register_method("get_options", &MediaPipeGraphNode::get_options);
	register_method("set_options", &MediaPipeGraphNode::set_options);
	register_method("get_input_index", &MediaPipeGraphNode::get_input_index);
	register_method("get_input_tag", &MediaPipeGraphNode::get_input_tag);
	register_method("get_output_index", &MediaPipeGraphNode::get_output_index);
	register_method("get_output_tag", &MediaPipeGraphNode::get_output_tag);
}

void MediaPipeGraphNode::_init() {
	node = nullptr;
}

MediaPipeGraphNode *MediaPipeGraphNode::_new(mediapipe::api2::builder::GenericNode *node) {
	MediaPipeGraphNode *n = MediaPipeGraphNode::_new();
	n->node = node;
	return n;
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

Ref<MediaPipeNodeDestination> MediaPipeGraphNode::get_input_index(int index) {
	ERR_FAIL_NULL_V(node, nullptr);
	return Ref(MediaPipeNodeDestination::_new(this, index));
}

Ref<MediaPipeNodeDestination> MediaPipeGraphNode::get_input_tag(String tag) {
	ERR_FAIL_NULL_V(node, nullptr);
	return Ref(MediaPipeNodeDestination::_new(this, tag));
}

Ref<MediaPipeNodeSource> MediaPipeGraphNode::get_output_index(int index) {
	ERR_FAIL_NULL_V(node, nullptr);
	return Ref(MediaPipeNodeSource::_new(this, index));
}

Ref<MediaPipeNodeSource> MediaPipeGraphNode::get_output_tag(String tag) {
	ERR_FAIL_NULL_V(node, nullptr);
	return Ref(MediaPipeNodeSource::_new(this, tag));
}

builder::GenericNode *MediaPipeGraphNode::get_node() {
	return node;
}
