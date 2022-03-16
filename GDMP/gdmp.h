#ifndef GDMP_H
#define GDMP_H

#include "Dictionary.hpp"
#include "Godot.hpp"
#include "Node.hpp"
#include "Ref.hpp"
#include "String.hpp"

#include "framework/graph.h"
#include "io/camera_helper.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "framework/gpu_helper.h"
#endif

namespace godot {

class GDMP : public Node {
		GODOT_CLASS(GDMP, Node)

	public:
		static void _register_methods();

		void _init(); // our initializer called by Godot
		void _on_new_proto(String stream_name, Ref<Packet> packet);
		void _on_new_proto_vector(String stream_name, Ref<Packet> packet);
		void _on_new_gpu_frame(String stream_name, Ref<Packet> packet);

		void init_graph(String graph_path);
		void add_proto_callback(String stream_name);
		void add_proto_vector_callback(String stream_name);
		void add_gpu_frame_callback(String stream_name);

		void start_graph(Dictionary side_packets);

		void start_camera(int index, String stream_name);
		void close_camera();

	private:
		Graph *graph;
		Ref<CameraHelper> camera_helper;
#if !MEDIAPIPE_DISABLE_GPU
		Ref<GPUHelper> gpu_helper;
#endif
};

} // namespace godot

#endif
