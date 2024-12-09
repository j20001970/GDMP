#ifndef GDMP_GRAPH_CONFIG
#define GDMP_GRAPH_CONFIG

#include "Godot.hpp"

#include "mediapipe/framework/calculator.pb.h"

#include "GDMP/proto/proto.h"

using namespace godot;

class MediaPipeGraphConfig : public MediaPipeProto {
		GODOT_CLASS(MediaPipeGraphConfig, MediaPipeProto)

	private:
		const mediapipe::CalculatorGraphConfig *config;

	public:
		static void _register_methods();
		static MediaPipeGraphConfig *_new(const mediapipe::CalculatorGraphConfig &config);

		void _init();

		// Get underlying mediapipe::CalculatorGraphConfig
		const mediapipe::CalculatorGraphConfig &get_config();
};

#endif
