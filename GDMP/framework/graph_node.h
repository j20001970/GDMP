#ifndef GDMP_GRAPH_NODE
#define GDMP_GRAPH_NODE

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/framework/api2/builder.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::api2;

class MediaPipeGraphBuilder;
class MediaPipeGraphNode;

class MediaPipeNodeDestination : public RefCounted {
		GDCLASS(MediaPipeNodeDestination, RefCounted)

	private:
		std::function<builder::Destination<>()> _get_dst = nullptr;

	protected:
		static void _bind_methods() {}

	public:
		MediaPipeNodeDestination() = default;
		MediaPipeNodeDestination(Ref<MediaPipeGraphBuilder> builder, int index);
		MediaPipeNodeDestination(Ref<MediaPipeGraphBuilder> builder, const String &tag);
		MediaPipeNodeDestination(Ref<MediaPipeGraphNode> node, int index);
		MediaPipeNodeDestination(Ref<MediaPipeGraphNode> node, const String &tag);

		bool connect(builder::Source<> &src);
};

class MediaPipeNodeSource : public RefCounted {
		GDCLASS(MediaPipeNodeSource, RefCounted)

	private:
		std::function<builder::Source<>()> _get_src = nullptr;

	protected:
		static void _bind_methods() {
			ClassDB::bind_method(D_METHOD("connect_to", "destination", "stream_name"), &MediaPipeNodeSource::connect_to);
		}

	public:
		MediaPipeNodeSource() = default;
		MediaPipeNodeSource(Ref<MediaPipeGraphBuilder> builder, int index);
		MediaPipeNodeSource(Ref<MediaPipeGraphBuilder> builder, const String &tag);
		MediaPipeNodeSource(Ref<MediaPipeGraphNode> node, int index);
		MediaPipeNodeSource(Ref<MediaPipeGraphNode> node, const String &tag);

		bool connect_to(Ref<MediaPipeNodeDestination> dst, const String &stream_name);
};

class MediaPipeGraphNode : public RefCounted {
		GDCLASS(MediaPipeGraphNode, RefCounted)

	private:
		builder::GenericNode *node = nullptr;

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraphNode() = default;
		MediaPipeGraphNode(mediapipe::api2::builder::GenericNode *node);

		// Get calculator node options.
		Ref<MediaPipeProto> get_options();
		// Set calculator node options with proto.
		bool set_options(Ref<MediaPipeProto> proto);

		Ref<MediaPipeNodeDestination> get_input_index(int index);
		Ref<MediaPipeNodeDestination> get_input_tag(const String &tag);
		Ref<MediaPipeNodeSource> get_output_index(int index);
		Ref<MediaPipeNodeSource> get_output_tag(const String &tag);

		// Get the underlying pointer to mediapipe::api2::builder::GenericNode.
		builder::GenericNode *get_node();

		friend class MediaPipeGraphBuilder;
};

#endif
