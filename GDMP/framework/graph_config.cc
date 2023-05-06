#include "graph_config.h"

#include <string>

#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "mediapipe/framework/port/parse_text_proto.h"

void MediaPipeGraphConfig::_bind_methods() {
	ClassDB::bind_method(D_METHOD("has_input_stream"), &MediaPipeGraphConfig::has_input_stream);
	ClassDB::bind_method(D_METHOD("has_output_stream"), &MediaPipeGraphConfig::has_output_stream);
	ClassDB::bind_method(D_METHOD("load"), &MediaPipeGraphConfig::load);
	ClassDB::bind_method(D_METHOD("parse_bytes"), &MediaPipeGraphConfig::parse_bytes);
	ClassDB::bind_method(D_METHOD("parse_text"), &MediaPipeGraphConfig::parse_text);
}

MediaPipeGraphConfig::MediaPipeGraphConfig() = default;

MediaPipeGraphConfig::MediaPipeGraphConfig(mediapipe::CalculatorGraphConfig config) {
	this->config = config;
}

MediaPipeGraphConfig::~MediaPipeGraphConfig() = default;

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

Error MediaPipeGraphConfig::load(String path, bool as_binary) {
	Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
	ERR_FAIL_COND_V(f.is_null(), ERR_INVALID_PARAMETER);
	bool res;
	if (as_binary) {
		uint64_t len = f->get_length();
		PackedByteArray data = f->get_buffer(len);
		res = parse_bytes(data);
	} else {
		String data = f->get_as_text();
		res = parse_text(data);
	}
	if (res)
		return OK;
	return ERR_FILE_UNRECOGNIZED;
}

bool MediaPipeGraphConfig::parse_bytes(PackedByteArray data) {
	return config.ParseFromArray(data.ptr(), data.size());
}

bool MediaPipeGraphConfig::parse_text(String data) {
	return mediapipe::ParseTextProto(data.utf8().get_data(), &config);
}

mediapipe::CalculatorGraphConfig MediaPipeGraphConfig::get_config() {
	return config;
}

void ResourceFormatLoaderMediaPipeGraphConfig::_bind_methods() {}

void ResourceFormatLoaderMediaPipeGraphConfig::register_loader() {
	if (singleton.is_null()) {
		singleton.instantiate();
		ResourceLoader::get_singleton()->add_resource_format_loader(singleton);
	}
}

void ResourceFormatLoaderMediaPipeGraphConfig::unregister_loader() {
	if (singleton.is_valid()) {
		ResourceLoader::get_singleton()->remove_resource_format_loader(singleton);
		singleton.unref();
	}
}

Variant ResourceFormatLoaderMediaPipeGraphConfig::_load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode) {
	String ext = path.get_extension().to_lower();
	Ref<MediaPipeGraphConfig> config;
	Error err = ERR_INVALID_PARAMETER;
	if (ext == "binarypb") {
		config.instantiate();
		err = config->load(path, true);
	} else if (ext == "pbtxt") {
		config.instantiate();
		err = config->load(path, false);
	}
	if (err == OK)
		return config;
	return err;
}

PackedStringArray ResourceFormatLoaderMediaPipeGraphConfig::_get_recognized_extensions() {
	return PackedStringArray(Array::make("binarypb", "pbtxt"));
}

bool ResourceFormatLoaderMediaPipeGraphConfig::_handles_type(const StringName &type) {
	return type == String("Resource");
}

String ResourceFormatLoaderMediaPipeGraphConfig::_get_resource_type(const String &path) {
	return "Resource";
}

Ref<ResourceFormatLoaderMediaPipeGraphConfig> ResourceFormatLoaderMediaPipeGraphConfig::singleton = Ref<ResourceFormatLoaderMediaPipeGraphConfig>();
