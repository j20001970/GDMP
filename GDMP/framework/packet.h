#ifndef GDMP_PACKET
#define GDMP_PACKET

#include <memory>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/packet.h"

using namespace godot;

class MediaPipePacket : public RefCounted {
		GDCLASS(MediaPipePacket, RefCounted)

	protected:
		static void _bind_methods();

	public:
		MediaPipePacket();
		MediaPipePacket(const mediapipe::Packet &packet);
		~MediaPipePacket();

		// Check if the packet is empty.
		bool is_empty();

		// Get mediapipe::ImageFrame from packet and convert to godot::Image.
		// For getting GPU frame, refer to GPUHelper::get_gpu_frame
		Ref<Image> get_image();
		// Get serialized proto byte array from packet.
		PackedByteArray get_proto();
		// Get array of serialized proto bytes from packet,
		Array get_proto_vector();

		// Make a packet from variant.
		void make(Variant value);
		// Make a mediapipe::ImageFrame packet from godot::Image.
		// For making mediapipe::GpuBuffer packet, refer to GPUHelper::make_packet_from_image
		void make_image(Ref<Image> image);
		// Make a mediapipe::ImageFrame packet.
		void make_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame);

		// Get packet timestamp in microseconds.
		int64 get_timestamp();
		// Set packet timestamp in microseconds.
		void set_timestamp(int64_t timestamp);

		// Get actual mediapipe::Packet
		mediapipe::Packet get_packet();

	private:
		mediapipe::Packet packet;
};

#endif
