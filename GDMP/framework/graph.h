#ifndef GDMP_GRAPH
#define GDMP_GRAPH

#include <map>
#include <memory>

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/framework/calculator_graph.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "mediapipe/GDMP/gpu/gpu_resources.h"
#include "mediapipe/GDMP/framework/graph_config.h"
#include "mediapipe/GDMP/framework/packet.h"

using namespace godot;

class MediaPipeGraph : public RefCounted {
		GDCLASS(MediaPipeGraph, RefCounted)

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraph();
		~MediaPipeGraph();

		// Initialize the graph from graph config.
		void initialize(Ref<MediaPipeGraphConfig> config);
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
		void add_packet_callback(String stream_name, Callable callback);
		// Start the calculator graph for sending packets to input stream,
		// with side_packets for setting input side packet value.
		void start(Dictionary side_packets);
		// Add a packet to graph input stream.
		void add_packet(String stream_name, Ref<MediaPipePacket> packet);
		// Stop the graph.
		void stop();
		// Set GPU resources.
		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources);
#if !MEDIAPIPE_DISABLE_GPU
		// Get mediapipe GPU resources.
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();
#endif

	private:
		std::unique_ptr<mediapipe::CalculatorGraph> running_graph;
		std::unique_ptr<mediapipe::CalculatorGraphConfig> graph_config;
		std::map<std::string, mediapipe::Packet> packet_callbacks;
		Ref<MediaPipeGPUResources> gpu_resources;
};

#endif
