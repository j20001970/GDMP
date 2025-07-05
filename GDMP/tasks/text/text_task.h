#ifndef GDMP_TEXT_TASK
#define GDMP_TEXT_TASK

#include "Godot.hpp"

#include "GDMP/tasks/task.h"

using namespace godot;

class MediaPipeTextTask : public MediaPipeTask {
		GDMP_TASK_CLASS

	public:
		static void _register_methods();
};

#define GDMP_TEXT_TASK_CLASS(NAME, TASK) \
	GODOT_CLASS(NAME, MediaPipeTextTask) \
	GDMP_TASK_CLASS                      \
                                         \
private:                                 \
	std::unique_ptr<TASK> task;

#endif
