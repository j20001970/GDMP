#include "task.h"

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

bool MediaPipeTask::initialize(Ref<MediaPipeTaskBaseOptions> base_options) {
	return false;
}
