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

mediapipe::Matrix make_audio_matrix(PackedVector2Array audio_data, bool is_stereo) {
	std::vector<float> data;
	int num_channels = 1;
	if (is_stereo) {
		num_channels = 2;
		data.resize(audio_data.size() * 2);
		for (int i = 0; i < audio_data.size(); i++) {
			data[i * 2] = audio_data[i].x;
			data[i * 2 + 1] = audio_data[i].y;
		}
	} else {
		data.resize(audio_data.size());
		for (int i = 0; i < audio_data.size(); i++) {
			data[i] = audio_data[i].x;
		}
	}
	Eigen::Map<const mediapipe::Matrix> map(data.data(), num_channels, audio_data.size());
	return std::move(map.matrix());
}

GDMP_REGISTER_TASK(MediaPipeAudioTask, MediaPipeTask)
