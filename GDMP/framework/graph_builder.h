#ifndef GDMP_GRAPH_BUILDER
#define GDMP_GRAPH_BUILDER

#include "Array.hpp"
#include "Godot.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/graph_node.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphBuilder : public Reference {
		GODOT_CLASS(MediaPipeGraphBuilder, Reference)

	private:
		builder::Graph builder;
		Array nodes;

	public:
		static void _register_methods();

		void _init();
		~MediaPipeGraphBuilder();

		// Add a node into the graph, returning reference to the node.
		// Returned MediaPipeGraphNode are valid during the lifetime of their corresponding graph builder.
		// MediaPipeGraphBuilder is responsible for invalidating its nodes when deleted.
		Ref<MediaPipeGraphNode> add_node(String type);

		Ref<MediaPipeNodeSource> get_input_index(int index);
		Ref<MediaPipeNodeSource> get_input_tag(String tag);
		Ref<MediaPipeNodeDestination> get_output_index(int index);
		Ref<MediaPipeNodeDestination> get_output_tag(String tag);

		// Get a copy of graph config of this graph builder.
		Ref<MediaPipeGraphConfig> get_config();

		builder::Graph &get_builder();
};

#endif
