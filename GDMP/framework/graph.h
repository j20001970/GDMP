#ifndef GDMP_GRAPH
#define GDMP_GRAPH

#include <memory>

#include "Dictionary.hpp"
#include "Godot.hpp"
#include "Object.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/framework/calculator_graph.h"

#include "mediapipe/gpu/gl_context.h"
#include "packet.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "gpu_helper.h"
#endif

namespace godot {

class Graph : public Reference {
		GODOT_CLASS(Graph, Reference)

	public:
		static void _register_methods();

		void _init();

		void initialize(String graph_path, bool as_text);
		void add_packet_callback(String stream_name, Object *object, String method);
		void start(Dictionary side_packets);
		void add_packet(String stream_name, Ref<Packet> packet);
		void stop();

#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GlContext> get_gl_context();
		Ref<GPUHelper> get_gpu_helper();
#endif

	private:
		bool started;
		std::unique_ptr<mediapipe::CalculatorGraph> graph;
};

} //namespace godot

#endif