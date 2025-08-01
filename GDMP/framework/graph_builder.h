#ifndef GDMP_GRAPH_BUILDER
#define GDMP_GRAPH_BUILDER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/graph_node.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphBuilder : public RefCounted {
		GDCLASS(MediaPipeGraphBuilder, RefCounted)

	private:
		builder::Graph builder;
		TypedArray<MediaPipeGraphNode> nodes;

	protected:
		static void _bind_methods();

	public:
		~MediaPipeGraphBuilder();

		// Add a node into the graph, returning reference to the node.
		// Returned MediaPipeGraphNode are valid during the lifetime of their corresponding graph builder.
		// MediaPipeGraphBuilder is responsible for invalidating its nodes when deleted.
		Ref<MediaPipeGraphNode> add_node(const String &type);

		Ref<MediaPipeNodeSource> get_input_index(int index);
		Ref<MediaPipeNodeSource> get_input_tag(const String &tag);
		Ref<MediaPipeNodeDestination> get_output_index(int index);
		Ref<MediaPipeNodeDestination> get_output_tag(const String &tag);

		// Get a copy of graph config of this graph builder.
		Ref<MediaPipeGraphConfig> get_config();

		builder::Graph &get_builder();
};

#endif
