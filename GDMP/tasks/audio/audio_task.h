#ifndef GDMP_AUDIO_TASK
#define GDMP_AUDIO_TASK

#include "Godot.hpp"
#include "PoolArrays.hpp"

#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/tasks/cc/audio/core/running_mode.h"

#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::audio::core;

class MediaPipeAudioTask : public MediaPipeTask {
		GDMP_TASK_CLASS

	public:
		static void _register_methods();

		enum RunningMode {
			RUNNING_MODE_AUDIO_CLIPS = AUDIO_CLIPS,
			RUNNING_MODE_AUDIO_STREAM = AUDIO_STREAM,
		};
};

#define GDMP_AUDIO_TASK_CLASS(NAME, TASK) \
	GODOT_CLASS(NAME, Reference)          \
	GDMP_TASK_CLASS                       \
                                          \
private:                                  \
	std::unique_ptr<TASK> task;

RunningMode get_running_mode(int p_mode);

mediapipe::Matrix make_audio_matrix(PoolVector2Array audio_data, bool is_stereo);

#endif
