#ifndef GDMP_GRAPH_CONFIG
#define GDMP_GRAPH_CONFIG

#include "mediapipe/framework/calculator.pb.h"

#include "GDMP/proto/proto.h"

using namespace godot;

class MediaPipeGraphConfig : public MediaPipeProto {
		GDCLASS(MediaPipeGraphConfig, MediaPipeProto)

	private:
		const mediapipe::CalculatorGraphConfig *config;

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraphConfig();
		MediaPipeGraphConfig(const mediapipe::CalculatorGraphConfig &config);
		~MediaPipeGraphConfig();

		// Get underlying mediapipe::CalculatorGraphConfig
		const mediapipe::CalculatorGraphConfig &get_config();
};

#endif
