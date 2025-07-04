#include "audio_task.h"

void MediaPipeAudioTask::_register_task() {
	_register_tasks();
}

RunningMode get_running_mode(MediaPipeAudioTask::RunningMode p_mode) {
	return RunningMode(p_mode);
}

mediapipe::Matrix make_audio_matrix(PoolRealArray audio_data, int num_channels) {
    Eigen::Map<const mediapipe::Matrix> map(audio_data.read().ptr(), num_channels, audio_data.size());
    return map.matrix();
}

GDMP_REGISTER_TASK(MediaPipeAudioTask, MediaPipeTask)
