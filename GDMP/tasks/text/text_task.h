#ifndef GDMP_TEXT_TASK
#define GDMP_TEXT_TASK

#include "GDMP/tasks/task.h"

using namespace godot;

class MediaPipeTextTask : public MediaPipeTask {
		GDCLASS(MediaPipeTextTask, MediaPipeTask)
		GDMP_TASK_CLASS

	protected:
		static void _bind_methods();
};

#define GDMP_TEXT_TASK_CLASS(NAME, TASK) \
	GDCLASS(NAME, MediaPipeTextTask)     \
	GDMP_TASK_CLASS                      \
                                         \
private:                                 \
	std::unique_ptr<TASK> task;

#endif
