#include "audio_task.h"

void MediaPipeAudioTask::_bind_methods() {
	BIND_ENUM_CONSTANT(RUNNING_MODE_AUDIO_CLIPS);
	BIND_ENUM_CONSTANT(RUNNING_MODE_AUDIO_STREAM);
}

void MediaPipeAudioTask::_register_task() {
	ClassDB::register_abstract_class<MediaPipeAudioTask>();
	_register_tasks();
}

RunningMode get_running_mode(MediaPipeAudioTask::RunningMode p_mode) {
	return RunningMode(p_mode);
}

mediapipe::Matrix make_audio_matrix(PackedFloat32Array audio_data, int num_channels) {
	Eigen::Map<const mediapipe::Matrix> map(audio_data.ptr(), num_channels, audio_data.size());
	return map.matrix();
}

GDMP_REGISTER_TASK(MediaPipeAudioTask, MediaPipeTask)
