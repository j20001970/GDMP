#include "graph_config.h"

void MediaPipeGraphConfig::_bind_methods() {}

MediaPipeGraphConfig::MediaPipeGraphConfig() {
	message = mediapipe::CalculatorGraphConfig().New();
	config = static_cast<mediapipe::CalculatorGraphConfig *>(message);
}

MediaPipeGraphConfig::MediaPipeGraphConfig(const mediapipe::CalculatorGraphConfig &config) {
	message = config.New();
	message->CopyFrom(config);
	this->config = static_cast<mediapipe::CalculatorGraphConfig *>(message);
}

MediaPipeGraphConfig::~MediaPipeGraphConfig() = default;

const mediapipe::CalculatorGraphConfig &MediaPipeGraphConfig::get_config() {
	return *config;
}
