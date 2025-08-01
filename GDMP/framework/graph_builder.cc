#include "graph_builder.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeGraphBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_node", "type"), &MediaPipeGraphBuilder::add_node);
	ClassDB::bind_method(D_METHOD("get_input_index", "index"), &MediaPipeGraphBuilder::get_input_index);
	ClassDB::bind_method(D_METHOD("get_input_tag", "tag"), &MediaPipeGraphBuilder::get_input_tag);
	ClassDB::bind_method(D_METHOD("get_output_index", "index"), &MediaPipeGraphBuilder::get_output_index);
	ClassDB::bind_method(D_METHOD("get_output_tag", "tag"), &MediaPipeGraphBuilder::get_output_tag);
	ClassDB::bind_method(D_METHOD("get_config"), &MediaPipeGraphBuilder::get_config);
}

MediaPipeGraphBuilder::~MediaPipeGraphBuilder() {
	for (int i = 0; i < nodes.size(); i++) {
		Ref<MediaPipeGraphNode> node = nodes[i];
		node->node = nullptr;
	}
}

Ref<MediaPipeGraphNode> MediaPipeGraphBuilder::add_node(const String &type) {
	builder::GenericNode &node = builder.AddNode(type.utf8().get_data());
	Ref<MediaPipeGraphNode> godot_node = memnew(MediaPipeGraphNode(&node));
	nodes.push_back(godot_node);
	return godot_node;
}

Ref<MediaPipeNodeSource> MediaPipeGraphBuilder::get_input_index(int index) {
	return memnew(MediaPipeNodeSource(this, index));
}

Ref<MediaPipeNodeSource> MediaPipeGraphBuilder::get_input_tag(const String &tag) {
	return memnew(MediaPipeNodeSource(this, tag));
}

Ref<MediaPipeNodeDestination> MediaPipeGraphBuilder::get_output_index(int index) {
	return memnew(MediaPipeNodeDestination(this, index));
}

Ref<MediaPipeNodeDestination> MediaPipeGraphBuilder::get_output_tag(const String &tag) {
	return memnew(MediaPipeNodeDestination(this, tag));
}

Ref<MediaPipeGraphConfig> MediaPipeGraphBuilder::get_config() {
	Ref<MediaPipeGraphConfig> config = memnew(MediaPipeGraphConfig(builder.GetConfig()));
	return config;
}

builder::Graph &MediaPipeGraphBuilder::get_builder() {
	return builder;
}
