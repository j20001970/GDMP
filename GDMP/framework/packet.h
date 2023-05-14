#ifndef GDMP_PACKET
#define GDMP_PACKET

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/packet.h"

#include "GDMP/proto/proto.h"

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

		// Get proto from packet.
		Ref<MediaPipeProto> get_proto(const String &type_name);
		// Get array of proto from packet.
		TypedArray<MediaPipeProto> get_proto_vector(const String &type_name);

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
