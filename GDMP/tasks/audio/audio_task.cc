#include "audio_task.h"

mediapipe::Matrix make_audio_matrix(PoolRealArray audio_data, int num_channels) {
    Eigen::Map<const mediapipe::Matrix> map(audio_data.read().ptr(), num_channels, audio_data.size());
    return map.matrix();
}
