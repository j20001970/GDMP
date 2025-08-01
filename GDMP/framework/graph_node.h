#ifndef GDMP_GRAPH_NODE
#define GDMP_GRAPH_NODE

#include "Godot.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphBuilder;
class MediaPipeGraphNode;

class MediaPipeNodeDestination : public Reference {
		GODOT_CLASS(MediaPipeNodeDestination, Reference)

	private:
		Ref<MediaPipeGraphBuilder> builder;
		Ref<MediaPipeGraphNode> node;
		std::function<builder::Destination<>()> _get_dst = nullptr;

	public:
		static void _register_methods() {}

		static MediaPipeNodeDestination *_new(Ref<MediaPipeGraphBuilder> builder, int index);
		static MediaPipeNodeDestination *_new(Ref<MediaPipeGraphBuilder> builder, String tag);
		static MediaPipeNodeDestination *_new(Ref<MediaPipeGraphNode> node, int index);
		static MediaPipeNodeDestination *_new(Ref<MediaPipeGraphNode> node, String tag);

		void _init() {}

		bool connect(builder::Source<> &src);
};

class MediaPipeNodeSource : public Reference {
		GODOT_CLASS(MediaPipeNodeSource, Reference)

	private:
		Ref<MediaPipeGraphBuilder> builder;
		Ref<MediaPipeGraphNode> node;
		std::function<builder::Source<>()> _get_src = nullptr;

	public:
		static void _register_methods() {
			register_method("connect_to", &MediaPipeNodeSource::connect_to);
		}

		static MediaPipeNodeSource *_new(Ref<MediaPipeGraphBuilder> builder, int index);
		static MediaPipeNodeSource *_new(Ref<MediaPipeGraphBuilder> builder, String tag);
		static MediaPipeNodeSource *_new(Ref<MediaPipeGraphNode> node, int index);
		static MediaPipeNodeSource *_new(Ref<MediaPipeGraphNode> node, String tag);

		void _init() {}

		bool connect_to(Ref<MediaPipeNodeDestination> dst, String stream_name);
};

class MediaPipeGraphNode : public Reference {
		GODOT_CLASS(MediaPipeGraphNode, Reference)

	private:
		builder::GenericNode *node = nullptr;

	public:
		static void _register_methods();
		static MediaPipeGraphNode *_new(mediapipe::api2::builder::GenericNode *node);

		void _init();

		// Get calculator node options.
		Ref<MediaPipeProto> get_options();
		// Set calculator node options with proto.
		bool set_options(Ref<MediaPipeProto> proto);

		Ref<MediaPipeNodeDestination> get_input_index(int index);
		Ref<MediaPipeNodeDestination> get_input_tag(String tag);
		Ref<MediaPipeNodeSource> get_output_index(int index);
		Ref<MediaPipeNodeSource> get_output_tag(String tag);

		// Get the underlying pointer to mediapipe::api2::builder::GenericNode.
		builder::GenericNode *get_node();

		friend class MediaPipeGraphBuilder;
};

#endif
