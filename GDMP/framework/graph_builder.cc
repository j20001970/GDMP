#include "graph_builder.h"

void MediaPipeGraphBuilder::_register_methods() {
	register_method("add_node", &MediaPipeGraphBuilder::add_node);
	register_method("get_input_index", &MediaPipeGraphBuilder::get_input_index);
	register_method("get_input_tag", &MediaPipeGraphBuilder::get_input_tag);
	register_method("get_output_index", &MediaPipeGraphBuilder::get_output_index);
	register_method("get_output_tag", &MediaPipeGraphBuilder::get_output_tag);
	register_method("get_config", &MediaPipeGraphBuilder::get_config);
}

void MediaPipeGraphBuilder::_init() {}

MediaPipeGraphBuilder::~MediaPipeGraphBuilder() {
	for (int i = 0; i < nodes.size(); i++) {
		Ref<MediaPipeGraphNode> node = nodes[i];
		node->node = nullptr;
	}
}

Ref<MediaPipeGraphNode> MediaPipeGraphBuilder::add_node(String type) {
	builder::GenericNode &node = builder.AddNode(type.utf8().get_data());
	Ref<MediaPipeGraphNode> godot_node = Ref(MediaPipeGraphNode::_new(&node));
	nodes.push_back(godot_node);
	return godot_node;
}

Ref<MediaPipeNodeSource> MediaPipeGraphBuilder::get_input_index(int index) {
	return Ref(MediaPipeNodeSource::_new(this, index));
}

Ref<MediaPipeNodeSource> MediaPipeGraphBuilder::get_input_tag(String tag) {
	return Ref(MediaPipeNodeSource::_new(this, tag));
}

Ref<MediaPipeNodeDestination> MediaPipeGraphBuilder::get_output_index(int index) {
	return Ref(MediaPipeNodeDestination::_new(this, index));
}

Ref<MediaPipeNodeDestination> MediaPipeGraphBuilder::get_output_tag(String tag) {
	return Ref(MediaPipeNodeDestination::_new(this, tag));
}

Ref<MediaPipeGraphConfig> MediaPipeGraphBuilder::get_config() {
	Ref<MediaPipeGraphConfig> config = Ref(MediaPipeGraphConfig::_new(builder.GetConfig()));
	return config;
}

builder::Graph &MediaPipeGraphBuilder::get_builder() {
	return builder;
}
