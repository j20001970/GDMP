#include "text_task.h"

void MediaPipeTextTask::_register_methods() {}

void MediaPipeTextTask::_register_task() {
	_register_tasks();
}

GDMP_REGISTER_TASK(MediaPipeTextTask, MediaPipeTask)
