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

#include "packet.h"

#if !MEDIAPIPE_DISABLE_GPU
#include "gpu_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

namespace godot {

class Graph : public Object {
		GODOT_CLASS(Graph, Object)

	public:
		static void _register_methods();

		void _init();

		// Initialize the calculator graph with the file in graph_path.
		// Depending on as_text, the graph file must be in either text (pbtxt) or binary (binarypb) format.
		// If initialization failed in the process, the underlying mediapipe::CalculatorGraph must remain null.
		void initialize(String graph_path, bool as_text);
		// Add a callback for given stream_name.
		void add_packet_callback(String stream_name, Object *object, String method);
		// Start the calculator graph for sending packets to input stream,
		// with side_packets for setting input side packet value.
		void start(Dictionary side_packets);
		// Add a packet to graph input stream.
		void add_packet(String stream_name, Ref<Packet> packet);
		// Stop the graph.
		void stop();
#if !MEDIAPIPE_DISABLE_GPU
		// Get mediapipe GPU resources, only valid after graph is initialized.
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();
		// Get GPU helper for performing GPU-related operations, only valid after graph is initialized.
		Ref<GPUHelper> get_gpu_helper();
#ifdef __ANDROID__
		jobject egl_manager;
#endif
#endif

	private:
		bool started;
		std::unique_ptr<mediapipe::CalculatorGraph> graph;
};

} //namespace godot

#endif
