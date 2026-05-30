#include "graph_config.h"

void MediaPipeGraphConfig::_bind_methods() {}

MediaPipeGraphConfig::MediaPipeGraphConfig() {
	config = mediapipe::CalculatorGraphConfig().New();
	message = std::unique_ptr<mediapipe::CalculatorGraphConfig>(config);
}

MediaPipeGraphConfig::MediaPipeGraphConfig(const mediapipe::CalculatorGraphConfig &config) :
		MediaPipeGraphConfig() {
	message->CopyFrom(config);
}

const mediapipe::CalculatorGraphConfig &MediaPipeGraphConfig::get_config() {
	return *config;
}
