#ifndef GDMP_GRAPH_NODE
#define GDMP_GRAPH_NODE

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphNode : public RefCounted {
		GDCLASS(MediaPipeGraphNode, RefCounted)

	private:
		builder::GenericNode *node;

		// Invalidate the node reference when its graph builder is deleted.
		void invalidate();

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraphNode();
		MediaPipeGraphNode(mediapipe::api2::builder::GenericNode *node);

		// Get calculator node options.
		Ref<MediaPipeProto> get_options();
		// Set calculator node options with proto.
		bool set_options(Ref<MediaPipeProto> proto);

		// Get the underlying pointer to mediapipe::api2::builder::GenericNode.
		builder::GenericNode *get_node();

		friend class MediaPipeGraphBuilder;
};

#endif
