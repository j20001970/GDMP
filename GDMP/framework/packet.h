#ifndef GDMP_PACKET
#define GDMP_PACKET

#include <memory>

#include "Array.hpp"
#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/packet.h"

using namespace godot;

class MediaPipePacket : public Reference {
		GODOT_CLASS(MediaPipePacket, Reference)

	private:
		mediapipe::Packet packet;

	public:
		static void _register_methods();
		static MediaPipePacket *_new(const mediapipe::Packet &packet);

		void _init();

		// Check if the packet is empty.
		bool is_empty();

		// Get serialized proto byte array from packet.
		PoolByteArray get_proto();
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
