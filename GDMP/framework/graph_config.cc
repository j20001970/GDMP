#include "graph_config.h"

#include <string>

#include "File.hpp"

#include "mediapipe/framework/port/parse_text_proto.h"

void MediaPipeGraphConfig::_register_methods() {
	register_method("has_input_stream", &MediaPipeGraphConfig::has_input_stream);
	register_method("has_output_stream", &MediaPipeGraphConfig::has_output_stream);
	register_method("load", &MediaPipeGraphConfig::load);
	register_method("parse_bytes", &MediaPipeGraphConfig::parse_bytes);
	register_method("parse_text", &MediaPipeGraphConfig::parse_text);
}

MediaPipeGraphConfig *MediaPipeGraphConfig::_new(mediapipe::CalculatorGraphConfig config) {
	MediaPipeGraphConfig *graph_config = MediaPipeGraphConfig::_new();
	graph_config->config = config;
	return graph_config;
}

void MediaPipeGraphConfig::_init() {}

bool MediaPipeGraphConfig::has_input_stream(String stream_name) {
	for (std::string input : config.input_stream()) {
		if (String(input.data()) == stream_name) {
			return true;
		}
	}
	return false;
}

bool MediaPipeGraphConfig::has_output_stream(String stream_name) {
	for (std::string output : config.output_stream()) {
		if (String(output.data()) == stream_name) {
			return true;
		}
	}
	for (auto node : config.node()) {
		for (auto output : node.output_stream()) {
			if (String(output.data()) == stream_name) {
				return true;
			}
		}
	}
	return false;
}

bool MediaPipeGraphConfig::load(String path, bool as_binary) {
	Ref<File> f = File::_new();
	Error err = f->open(path, File::READ);
	ERR_FAIL_COND_V(err != Error::OK, false);
	if (as_binary) {
		uint64_t len = f->get_len();
		PoolByteArray data = f->get_buffer(len);
		return parse_bytes(data);
	} else {
		String data = f->get_as_text();
		return parse_text(data);
	}
}

bool MediaPipeGraphConfig::parse_bytes(PoolByteArray data) {
	return config.ParseFromArray(data.read().ptr(), data.size());
}

bool MediaPipeGraphConfig::parse_text(String data) {
	return mediapipe::ParseTextProto(data.utf8().get_data(), &config);
}

mediapipe::CalculatorGraphConfig MediaPipeGraphConfig::get_config() {
	return config;
}
