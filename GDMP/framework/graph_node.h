#ifndef GDMP_GRAPH_NODE
#define GDMP_GRAPH_NODE

#include "Godot.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphNode : public Reference {
		GODOT_CLASS(MediaPipeGraphNode, Reference)

	private:
		builder::GenericNode *node;

		// Invalidate the node reference when its graph builder is deleted.
		void invalidate();

	public:
		static void _register_methods();
		static MediaPipeGraphNode *_new(mediapipe::api2::builder::GenericNode *node);

		void _init();

		// Get calculator node options.
		Ref<MediaPipeProto> get_options();
		// Set calculator node options with proto.
		bool set_options(Ref<MediaPipeProto> proto);

		// Get the underlying pointer to mediapipe::api2::builder::GenericNode.
		builder::GenericNode *get_node();

		friend class MediaPipeGraphBuilder;
};

#endif
