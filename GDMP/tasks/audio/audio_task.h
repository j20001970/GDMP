#ifndef GDMP_AUDIO_TASK
#define GDMP_AUDIO_TASK

#include "godot_cpp/variant/packed_float32_array.hpp"

#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/tasks/cc/audio/core/running_mode.h"

using namespace godot;
using namespace mediapipe::tasks::audio::core;

typedef RunningMode RunningMode;

mediapipe::Matrix make_audio_matrix(PackedFloat32Array audio_data, int num_channels);

#endif
