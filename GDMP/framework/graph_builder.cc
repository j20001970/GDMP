#include "graph_builder.h"

void MediaPipeGraphBuilder::_register_methods() {
	register_method("add_node", &MediaPipeGraphBuilder::add_node);
	register_method("connect_input_to", &MediaPipeGraphBuilder::connect_input_to);
	register_method("connect_to_output", &MediaPipeGraphBuilder::connect_to_output);
	register_method("get_config", &MediaPipeGraphBuilder::get_config);
}

void MediaPipeGraphBuilder::_init() {}

MediaPipeGraphBuilder::~MediaPipeGraphBuilder() {
	for (int i = 0; i < nodes.size(); i++) {
		Ref<MediaPipeGraphNode> node = nodes[i];
		node->invalidate();
	}
}

Ref<MediaPipeGraphNode> MediaPipeGraphBuilder::add_node(const String &type) {
	builder::GenericNode &node = builder.AddNode(type.utf8().get_data());
	Ref<MediaPipeGraphNode> godot_node = Ref(MediaPipeGraphNode::_new(&node));
	nodes.push_back(godot_node);
	return godot_node;
}

bool MediaPipeGraphBuilder::connect_input_to(String input_name, Ref<MediaPipeGraphNode> node, String tag_name) {
	auto src = builder.In(tag_name.utf8().get_data()).SetName(input_name.utf8().get_data());
	auto dst = node->get_node()->In(tag_name.utf8().get_data());
	src.ConnectTo(dst);
	return true;
}

bool MediaPipeGraphBuilder::connect_to_output(Ref<MediaPipeGraphNode> node, String tag_name, String output_name) {
	auto src = node->get_node()->Out(tag_name.utf8().get_data()).SetName(output_name.utf8().get_data());
	auto dst = builder.Out(tag_name.utf8().get_data());
	src.ConnectTo(dst);
	return true;
}

Ref<MediaPipeGraphConfig> MediaPipeGraphBuilder::get_config() {
	Ref<MediaPipeGraphConfig> config = Ref(MediaPipeGraphConfig::_new(builder.GetConfig()));
	return config;
}
