#ifndef GDMP_PACKET
#define GDMP_PACKET

#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/packet.h"

using namespace godot;

class MediaPipePacket : public RefCounted {
		GDCLASS(MediaPipePacket, RefCounted)

	private:
		mediapipe::Packet packet;

	protected:
		static void _bind_methods();

	public:
		MediaPipePacket();
		MediaPipePacket(const mediapipe::Packet &packet);
		~MediaPipePacket();

		// Check if the packet is empty.
		bool is_empty();

		// Get serialized proto byte array from packet.
		PackedByteArray get_proto();
		// Get array of serialized proto bytes from packet.
		Array get_proto_vector();

		// Make a packet from variant.
		void make(Variant value);

		// Get packet timestamp in microseconds.
		int64 get_timestamp();
		// Set packet timestamp in microseconds.
		void set_timestamp(int64_t timestamp);

		// Get underlying mediapipe::Packet.
		mediapipe::Packet get_packet();
};

#endif
