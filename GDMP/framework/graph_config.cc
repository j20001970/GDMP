#include "graph_config.h"

void MediaPipeGraphConfig::_register_methods() {}

MediaPipeGraphConfig *MediaPipeGraphConfig::_new(const mediapipe::CalculatorGraphConfig &config) {
	MediaPipeGraphConfig *graph_config = MediaPipeGraphConfig::_new();
	graph_config->message->CopyFrom(config);
	return graph_config;
}

void MediaPipeGraphConfig::_init() {
	config = mediapipe::CalculatorGraphConfig().New();
	message = std::unique_ptr<mediapipe::CalculatorGraphConfig>(config);
}

const mediapipe::CalculatorGraphConfig &MediaPipeGraphConfig::get_config() {
	return *config;
}
