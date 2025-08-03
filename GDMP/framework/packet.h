#ifndef GDMP_PACKET
#define GDMP_PACKET

#include "Array.hpp"
#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"
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

		// Clear the content of the packet.
		void clear();

		// Check if the packet is empty.
		bool is_empty();

		// Get packet type name.
		// Return empty string if packet is empty or the type is not registered.
		String get_type_name();

		// Get value from the packet.
		Variant get();

		template <typename T>
		void set(T value);
		template <typename T>
		void set(T *value);
		void set_bool(bool value);
		void set_int32(int32_t value);
		void set_int64(int64_t value);
		void set_float(float value);
		void set_string(const String &value);

		void set_image(Ref<Image> image);
		void set_image_frame(Ref<Image> image);

		// Get packet timestamp in microseconds.
		int64_t get_timestamp();
		// Set packet timestamp in microseconds.
		void set_timestamp(int64_t timestamp_us);

		// Get underlying mediapipe::Packet.
		mediapipe::Packet get_packet();
};

#endif
