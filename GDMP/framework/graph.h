#ifndef GDMP_GRAPH
#define GDMP_GRAPH

#include <map>
#include <memory>

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/framework/calculator_graph.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeGraph : public RefCounted {
		GDCLASS(MediaPipeGraph, RefCounted)

	private:
		std::map<std::string, mediapipe::Packet> packet_callbacks;
		std::map<std::string, mediapipe::Packet> side_packets;
		std::unique_ptr<mediapipe::CalculatorGraph> running_graph;
		std::unique_ptr<mediapipe::CalculatorGraphConfig> graph_config;
#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif

	protected:
		static void _bind_methods();

	public:
		~MediaPipeGraph();

		// Initialize the graph from graph config.
		void initialize(Ref<MediaPipeGraphConfig> config);
		// Check if the graph config has initialized.
		bool is_initialized();

		// Check if the initialized graph config has specified input stream.
		// Always return false if graph config is not initialized.
		bool has_input_stream(const String &stream_name);
		// Check if the initialized graph config has specified output stream.
		// Always return false if graph config is not initialized.
		bool has_output_stream(const String &stream_name);

		// Add a packet callback for given stream_name.
		void add_packet_callback(const String &stream_name, Callable callback);
		// Set side packet.
		void set_side_packet(const String &stream_name, Ref<MediaPipePacket> packet);
		// Set GPU resources.
		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources);

		// Start the calculator graph for sending packets to input stream.
		void start();
		// Stop the graph.
		void stop();
		// Check if the graph is running.
		bool is_running();

		// Add a packet to graph input stream.
		void add_packet(const String &stream_name, Ref<MediaPipePacket> packet);
};

#endif
