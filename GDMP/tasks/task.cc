#include "task.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeTaskBaseOptions::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_model_asset_buffer"), &MediaPipeTaskBaseOptions::get_model_asset_buffer);
	ClassDB::bind_method(D_METHOD("set_model_asset_buffer"), &MediaPipeTaskBaseOptions::set_model_asset_buffer);
	ClassDB::bind_method(D_METHOD("get_model_asset_path"), &MediaPipeTaskBaseOptions::get_model_asset_path);
	ClassDB::bind_method(D_METHOD("set_model_asset_path"), &MediaPipeTaskBaseOptions::set_model_asset_path);
	ClassDB::bind_method(D_METHOD("get_delegate"), &MediaPipeTaskBaseOptions::get_delegate);
	ClassDB::bind_method(D_METHOD("set_delegate"), &MediaPipeTaskBaseOptions::set_delegate);
	ADD_PROPERTY(PropertyInfo(Variant::Type::PACKED_BYTE_ARRAY, "model_asset_buffer"), "set_model_asset_buffer", "get_model_asset_buffer");
	ADD_PROPERTY(PropertyInfo(Variant::Type::STRING, "model_asset_path"), "set_model_asset_path", "get_model_asset_path");
	ADD_PROPERTY(PropertyInfo(Variant::Type::INT, "delegate"), "set_delegate", "get_delegate");
	BIND_ENUM_CONSTANT(DELEGATE_CPU);
	BIND_ENUM_CONSTANT(DELEGATE_GPU);
	BIND_ENUM_CONSTANT(DELEGATE_EDGETPU_NNAPI);
}

PackedByteArray MediaPipeTaskBaseOptions::get_model_asset_buffer() {
	return model_asset_buffer;
}

void MediaPipeTaskBaseOptions::set_model_asset_buffer(PackedByteArray buffer) {
	model_asset_buffer = buffer;
}

String MediaPipeTaskBaseOptions::get_model_asset_path() {
	return model_asset_path;
}

void MediaPipeTaskBaseOptions::set_model_asset_path(const String &path) {
	model_asset_path = path;
}

MediaPipeTaskBaseOptions::Delegate MediaPipeTaskBaseOptions::get_delegate() {
	return Delegate(delegate);
}

void MediaPipeTaskBaseOptions::set_delegate(Delegate delegate) {
	this->delegate = delegate;
}

std::unique_ptr<BaseOptions> MediaPipeTaskBaseOptions::get_base_options() {
	std::unique_ptr<BaseOptions> base_options = std::make_unique<BaseOptions>();
	if (model_asset_buffer.size())
		base_options->model_asset_buffer = std::make_unique<std::string>((char *)model_asset_buffer.ptr(), model_asset_buffer.size());
	base_options->model_asset_path = model_asset_path.utf8().get_data();
	base_options->delegate = BaseOptions::Delegate(delegate);
	return std::move(base_options);
}

void MediaPipeTask::_bind_methods() {
	BIND_ENUM_CONSTANT(RUNNING_MODE_AUDIO_CLIPS)
	BIND_ENUM_CONSTANT(RUNNING_MODE_AUDIO_STREAM)
	BIND_ENUM_CONSTANT(RUNNING_MODE_IMAGE)
	BIND_ENUM_CONSTANT(RUNNING_MODE_VIDEO)
	BIND_ENUM_CONSTANT(RUNNING_MODE_LIVE_STREAM)
}

std::map<std::string, std::function<void()>> &MediaPipeTask::get_register_map() {
	static std::map<std::string, std::function<void()>> register_map;
	return register_map;
}

bool MediaPipeTask::register_task(const std::string &name, std::function<void()> func) {
	auto &register_map = get_register_map();
	if (auto it = register_map.find(name); it == register_map.end())
		register_map.insert(std::make_pair(name, func));
	return true;
}

void MediaPipeTask::register_tasks() {
	for (auto func : get_register_map())
		func.second();
}
