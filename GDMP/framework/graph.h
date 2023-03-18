#ifndef GDMP_GRAPH
#define GDMP_GRAPH

#include <map>
#include <memory>

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/framework/calculator_graph.h"

#include "graph_config.h"
#include "packet.h"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

using namespace godot;

class MediaPipeGraph : public RefCounted {
		GDCLASS(MediaPipeGraph, RefCounted)

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraph();
		~MediaPipeGraph();

		// Initialize the calculator graph config with the file in graph_path.
		// Depending on as_text, the graph file must be in either text (pbtxt) or binary (binarypb) format.
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
#if !MEDIAPIPE_DISABLE_GPU
		// Get mediapipe GPU resources.
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();
#endif

	private:
		std::unique_ptr<mediapipe::CalculatorGraph> running_graph;
		std::unique_ptr<mediapipe::CalculatorGraphConfig> graph_config;
		std::map<std::string, mediapipe::Packet> packet_callbacks;
#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif
};

#endif
