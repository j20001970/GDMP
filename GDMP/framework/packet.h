#ifndef GDMP_PACKET
#define GDMP_PACKET

#include "Array.hpp"
#include "Godot.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/packet.h"

#include "GDMP/proto/proto.h"

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

		// Get value from the packet.
		Variant get();
		// Set value of the packet, return true on success.
		// If given value is null, clear the packet.
		bool set(Variant value);

		// Get packet timestamp in microseconds.
		int64_t get_timestamp();
		// Set packet timestamp in microseconds.
		void set_timestamp(int64_t timestamp);

		// Get underlying mediapipe::Packet.
		mediapipe::Packet get_packet();
};

#endif
