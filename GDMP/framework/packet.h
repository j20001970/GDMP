#ifndef GDMP_PACKET
#define GDMP_PACKET

#include "Array.hpp"
#include "Godot.hpp"
#include "Image.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/packet.h"

namespace godot {

class Packet : public Reference {
		GODOT_CLASS(Packet, Reference)

	public:
		static void _register_methods();
		static Packet *_new(mediapipe::Packet packet);

		void _init();

		// Get mediapipe::ImageFrame from packet and convert to godot::Image
		// For getting GPU frame, refer to GPUHelper::get_gpu_frame
		Ref<Image> get_image();
		// Get serialized proto byte array from packet.
		PoolByteArray get_proto();
		// Get array of serialized proto bytes from packet,
		// only valid when packet is std::vector of proto type.
		Array get_proto_vector();

		// Make a packet (for bool, int, float and std::string).
		void make(Variant value);
		// Make a mediapipe::ImageFrame packet from godot::Image.
		// For making mediapipe::GpuBuffer packet, refer to GPUHelper::make_packet_from_image
		void make_image(Ref<Image> image);
		// Make a mediapipe::ImageFrame packet.
		void make_image_frame(const mediapipe::ImageFrame &image_frame);


		// Timestamp getter and setter.
		int64 get_timestamp();
		void set_timestamp(int64_t timestamp);

		// Get actual mediapipe::Packet
		mediapipe::Packet get_packet();

	private:
		mediapipe::Packet packet;
};

} //namespace godot

#endif
