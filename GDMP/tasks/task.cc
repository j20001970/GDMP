#include "task.h"

void MediaPipeTaskBaseOptions::_register_methods() {
	register_method("get_model_asset_buffer", &MediaPipeTaskBaseOptions::get_model_asset_buffer);
	register_method("set_model_asset_buffer", &MediaPipeTaskBaseOptions::set_model_asset_buffer);
	register_method("get_model_asset_path", &MediaPipeTaskBaseOptions::get_model_asset_path);
	register_method("set_model_asset_path", &MediaPipeTaskBaseOptions::set_model_asset_path);
	register_method("get_delegate", &MediaPipeTaskBaseOptions::get_delegate);
	register_method("set_delegate", &MediaPipeTaskBaseOptions::set_delegate);
}

void MediaPipeTaskBaseOptions::_init() {}

PoolByteArray MediaPipeTaskBaseOptions::get_model_asset_buffer() {
	return model_asset_buffer;
}

void MediaPipeTaskBaseOptions::set_model_asset_buffer(PoolByteArray buffer) {
	model_asset_buffer = buffer;
}

String MediaPipeTaskBaseOptions::get_model_asset_path() {
	return model_asset_path;
}

void MediaPipeTaskBaseOptions::set_model_asset_path(String path) {
	model_asset_path = path;
}

int MediaPipeTaskBaseOptions::get_delegate() {
	return delegate;
}

void MediaPipeTaskBaseOptions::set_delegate(int delegate) {
	this->delegate = delegate;
}

std::unique_ptr<BaseOptions> MediaPipeTaskBaseOptions::get_base_options() {
	std::unique_ptr<BaseOptions> base_options = std::make_unique<BaseOptions>();
	if (model_asset_buffer.size())
		base_options->model_asset_buffer = std::make_unique<std::string>((char *)model_asset_buffer.read().ptr(), model_asset_buffer.size());
	base_options->model_asset_path = model_asset_path.utf8().get_data();
	base_options->delegate = BaseOptions::Delegate(delegate);
	return std::move(base_options);
}

void MediaPipeTask::_register_task() {
	register_class<MediaPipeTaskBaseOptions>();
}

void MediaPipeTask::register_tasks() {
	_register_task();
	_register_tasks();
}
