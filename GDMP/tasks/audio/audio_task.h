#ifndef GDMP_AUDIO_TASK
#define GDMP_AUDIO_TASK

#include "godot_cpp/variant/packed_vector2_array.hpp"

#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/tasks/cc/audio/core/running_mode.h"

#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::audio::core;

class MediaPipeAudioTask : public MediaPipeTask {
		GDCLASS(MediaPipeAudioTask, MediaPipeTask)
		GDMP_TASK_CLASS

	protected:
		static void _bind_methods();

	public:
		enum RunningMode {
			RUNNING_MODE_AUDIO_CLIPS = AUDIO_CLIPS,
			RUNNING_MODE_AUDIO_STREAM = AUDIO_STREAM,
		};
};

VARIANT_ENUM_CAST(MediaPipeAudioTask::RunningMode)

#define GDMP_AUDIO_TASK_CLASS(NAME, TASK) \
	GDCLASS(NAME, MediaPipeAudioTask)     \
	GDMP_TASK_CLASS                       \
                                          \
private:                                  \
	std::unique_ptr<TASK> task;

RunningMode get_running_mode(MediaPipeAudioTask::RunningMode p_mode);

mediapipe::Matrix make_audio_matrix(PackedVector2Array audio_data, bool is_stereo);

#endif
