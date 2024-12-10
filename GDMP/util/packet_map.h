#ifndef GDMP_PACKET_MAP
#define GDMP_PACKET_MAP

#include "godot_cpp/variant/dictionary.hpp"

#include "mediapipe/tasks/cc/core/task_runner.h"

using namespace godot;
using namespace mediapipe::tasks::core;

PacketMap dict_to_packet_map(Dictionary dict);

Dictionary packet_map_to_dict(PacketMap &packet_map);

#endif
