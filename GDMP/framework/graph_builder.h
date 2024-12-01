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
		mediapipe::api2::builder::Graph builder;
		Array nodes;

	public:
		static void _register_methods();

		void _init();
		~MediaPipeGraphBuilder();

		// Add a node into the graph, returning reference to the node.
		// Returned MediaPipeGraphNode are valid during the lifetime of their corresponding graph builder.
		// MediaPipeGraphBuilder is responsible for invalidating its nodes when deleted.
		Ref<MediaPipeGraphNode> add_node(const String &type);

		// Connect the graph input stream input_name to node with tag tag_name.
		bool connect_input_to(String input_name, Ref<MediaPipeGraphNode> node, String tag_name);
		// Connect the node with tag tag_name to graph output stream output_name.
		bool connect_to_output(Ref<MediaPipeGraphNode> node, String tag_name, String output_name);

		// Get a copy of graph config of this graph builder.
		Ref<MediaPipeGraphConfig> get_config();
};

#endif
