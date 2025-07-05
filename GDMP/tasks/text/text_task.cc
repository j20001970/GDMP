#include "text_task.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeTextTask::_bind_methods() {}

void MediaPipeTextTask::_register_task() {
	ClassDB::register_abstract_class<MediaPipeTextTask>();
	_register_tasks();
}

GDMP_REGISTER_TASK(MediaPipeTextTask, MediaPipeTask)
