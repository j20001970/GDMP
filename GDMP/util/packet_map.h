#ifndef GDMP_UTIL_PACKET_MAP
#define GDMP_UTIL_PACKET_MAP

#include "Dictionary.hpp"

#include "mediapipe/tasks/cc/core/task_runner.h"

using namespace godot;
using namespace mediapipe::tasks::core;

namespace util {

PacketMap dict_to_packet_map(Dictionary dict);

Dictionary packet_map_to_dict(PacketMap &packet_map);

} //namespace util

#endif
