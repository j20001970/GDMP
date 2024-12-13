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

#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeGraph : public Reference {
		GODOT_CLASS(MediaPipeGraph, Reference)

	private:
		std::unique_ptr<mediapipe::CalculatorGraph> graph;

	public:
		static void _register_methods();

		void _init();

		// Initialize the graph from config.
		bool initialize(Ref<MediaPipeGraphConfig> config, Dictionary side_packets);
		// Get graph config.
		Ref<MediaPipeGraphConfig> get_config();

		// Add a packet callback for the graph output stream.
		bool add_output_stream_callback(const String &stream_name, Object *object, String method);
		// Get output side packet by name.
		Ref<MediaPipePacket> get_output_side_packet(String packet_name);

		// Run the graph, blocking the thread until all calculators are done.
		bool run(Dictionary extra_side_packets);
		// Start running the graph.
		bool start_run(Dictionary extra_side_packets);

		// Wait for current run to finish, all input streams have been closed, and all calculators are stopped.
		bool wait_until_done();
		// Wait until the running graph is in idle mode.
		bool wait_until_idle();

		// Check if the graph has encountered an error.
		bool has_error();

		// Add a packet to the graph input stream.
		bool add_packet_to_input_stream(String stream_name, Ref<MediaPipePacket> packet);
		// Check if the graph input stream exists.
		bool has_input_stream(String stream_name);

		// Close a graph input stream.
		bool close_input_stream(String stream_name);
		// Close all graph input streams.
		bool close_all_input_streams();
		// Close all graph input streams and source calculator nodes.
		bool close_all_packet_sources();

		// Abort the graph scheduler.
		void cancel();

		// Return GPU resources.
		Ref<MediaPipeGPUResources> get_gpu_resources();
		// Set GPU resources.
		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources);
};

#endif
