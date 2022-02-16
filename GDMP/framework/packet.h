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

		static Ref<Packet> make_image(Ref<Image> image);
		static Ref<Packet> make_image_frame(const mediapipe::ImageFrame &image_frame);

		void _init();

		PoolByteArray get_proto();
		Array get_proto_vector();
		Ref<Image> get_frame();

		int64 get_timestamp();
		void set_timestamp(int64_t timestamp);

		mediapipe::Packet get_packet();

	private:
		mediapipe::Packet packet;
};

} //namespace godot

#endif
