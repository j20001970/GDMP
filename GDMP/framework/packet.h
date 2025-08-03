#ifndef GDMP_PACKET
#define GDMP_PACKET

#include <functional>
#include <vector>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/packet.h"

using namespace godot;

typedef std::pair<std::function<bool(const mediapipe::Packet &)>, std::function<Variant(const mediapipe::Packet &)>> PacketType;
typedef std::vector<PacketType> PacketTypeList;

class MediaPipePacket : public RefCounted {
		GDCLASS(MediaPipePacket, RefCounted)

	private:
		static PacketTypeList &get_packet_type_list();

		static bool _register_packet_types;
		static bool register_packet_types();

		mediapipe::Packet packet;

	protected:
		static void _bind_methods();

	public:
		template <typename T>
		inline static bool validate_packet_type(const mediapipe::Packet &packet) {
			return packet.ValidateAsType<T>().ok();
		}
		template <typename T>
		inline static const T &get_packet_type(const mediapipe::Packet &packet) {
			return packet.Get<T>();
		}

		static bool add_packet_type(PacketType packet_type);

		MediaPipePacket();
		MediaPipePacket(const mediapipe::Packet &packet);

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
