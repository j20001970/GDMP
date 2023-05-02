#ifndef GDMP_GRAPH
#define GDMP_GRAPH

#include <map>
#include <memory>

#include "Dictionary.hpp"
#include "Godot.hpp"
#include "Object.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/framework/calculator_graph.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class Graph : public Reference {
		GODOT_CLASS(Graph, Reference)

	private:
		std::map<std::string, mediapipe::Packet> packet_callbacks;
		std::unique_ptr<mediapipe::CalculatorGraph> running_graph;
		std::unique_ptr<mediapipe::CalculatorGraphConfig> graph_config;
#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif

	public:
		static void _register_methods();

		void _init();

		// Initialize the graph from graph config.
		void initialize(Ref<GraphConfig> config);
		// Check if the graph config has initialized.
		bool is_initialized();
		// Check if the graph is running.
		bool is_running();
		// Check if the initialized graph config has specified input stream.
		// Always return false if graph config is not initialized.
		bool has_input_stream(String stream_name);
		// Check if the initialized graph config has specified output stream.
		// Always return false if graph config is not initialized.
		bool has_output_stream(String stream_name);
		// Add a packet callback for given stream_name.
		void add_packet_callback(String stream_name, Object *object, String method);
		// Start the calculator graph for sending packets to input stream,
		// with side_packets for setting input side packet value.
		void start(Dictionary side_packets);
		// Add a packet to graph input stream.
		void add_packet(String stream_name, Ref<Packet> packet);
		// Stop the graph.
		void stop();
		// Set GPU resources.
		void set_gpu_resources(Ref<GPUResources> gpu_resources);
};

#endif
