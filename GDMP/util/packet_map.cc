#include "packet_map.h"

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "GDMP/framework/packet.h"

PacketMap dict_to_packet_map(Dictionary dict) {
	PacketMap packet_map;
	Array keys = dict.keys();
	for (int i = 0; i < keys.size(); i++) {
		if (keys[i].get_type() != Variant::STRING)
			continue;
		const String &key = keys[i];
		Ref<MediaPipePacket> packet = dict[key];
		if (packet.is_null()) {
			WARN_PRINT(String("key {0}: not a MediaPipePacket").format(Array::make(key)));
			continue;
		}
		packet_map[key.utf8().get_data()] = packet->get_packet();
	}
	return packet_map;
}

Dictionary packet_map_to_dict(PacketMap &packet_map) {
	Dictionary dict;
	for (auto &pair : packet_map)
		dict[pair.first.data()] = memnew(MediaPipePacket(pair.second));
	return dict;
}
