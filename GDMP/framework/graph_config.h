#ifndef GDMP_GRAPH_CONFIG
#define GDMP_GRAPH_CONFIG

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Resource.hpp"
#include "String.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/calculator.pb.h"

namespace godot {

class GraphConfig : public Resource {
		GODOT_CLASS(GraphConfig, Resource)

	public:
		static void _register_methods();
		static GraphConfig *_new(mediapipe::CalculatorGraphConfig config);

		void _init();

		bool has_input_stream(String stream_name);
		bool has_output_stream(String stream_name);

		// Load graph config.
		bool load(String path, bool as_binary);
		// Parse graph config in binary format.
		bool parse_bytes(PoolByteArray data);
		// Parse graph config in text format.
		bool parse_text(String data);

		// Get underlying mediapipe::CalculatorGraphConfig
		mediapipe::CalculatorGraphConfig get_config();

	private:
		mediapipe::CalculatorGraphConfig config;
};

} //namespace godot

#endif
