#include "graph_builder.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeGraphBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_node", "type"), &MediaPipeGraphBuilder::add_node);
	ClassDB::bind_method(D_METHOD("connect_input_to", "input_name", "node", "tag_name"), &MediaPipeGraphBuilder::connect_input_to);
	ClassDB::bind_method(D_METHOD("connect_to_output", "node", "tag_name", "output_name"), &MediaPipeGraphBuilder::connect_to_output);
	ClassDB::bind_method(D_METHOD("get_config"), &MediaPipeGraphBuilder::get_config);
}

MediaPipeGraphBuilder::~MediaPipeGraphBuilder() {
	for (int i = 0; i < nodes.size(); i++) {
		Ref<MediaPipeGraphNode> node = nodes[i];
		node->invalidate();
	}
}

Ref<MediaPipeGraphNode> MediaPipeGraphBuilder::add_node(const String &type) {
	builder::GenericNode &node = builder.AddNode(type.utf8().get_data());
	Ref<MediaPipeGraphNode> godot_node = memnew(MediaPipeGraphNode(&node));
	nodes.push_back(godot_node);
	return godot_node;
}

bool MediaPipeGraphBuilder::connect_input_to(const String &input_name, Ref<MediaPipeGraphNode> node, const String &tag_name) {
	auto src = builder.In(tag_name.utf8().get_data()).SetName(input_name.utf8().get_data());
	auto dst = node->get_node()->In(tag_name.utf8().get_data());
	src.ConnectTo(dst);
	return true;
}

bool MediaPipeGraphBuilder::connect_to_output(Ref<MediaPipeGraphNode> node, const String &tag_name, const String &output_name) {
	auto src = node->get_node()->Out(tag_name.utf8().get_data()).SetName(output_name.utf8().get_data());
	auto dst = builder.Out(tag_name.utf8().get_data());
	src.ConnectTo(dst);
	return true;
}

Ref<MediaPipeGraphConfig> MediaPipeGraphBuilder::get_config() {
	Ref<MediaPipeGraphConfig> config = memnew(MediaPipeGraphConfig(builder.GetConfig()));
	return config;
}
