#include "packet.h"

#include <vector>

#include "../util/image.h"

using namespace godot;

void Packet::_register_methods() {
	register_method("get_proto", &Packet::get_proto);
	register_method("get_proto_vector", &Packet::get_proto_vector);
	register_method("get_timestamp", &Packet::get_timestamp);
	register_method("set_timestamp", &Packet::set_timestamp);
}

Packet *Packet::_new(mediapipe::Packet packet) {
	Packet *p = Packet::_new();
	p->packet = packet;
	return p;
}

Ref<Packet> Packet::make_image(Ref<Image> image) {
	mediapipe::ImageFrame image_frame = to_image_frame(image);
	return make_image_frame(image_frame);
}

Ref<Packet> Packet::make_image_frame(const mediapipe::ImageFrame &image_frame) {
	return Ref<Packet>(Packet::_new(mediapipe::Adopt(&image_frame)));
}

void Packet::_init(){};

PoolByteArray Packet::get_proto() {
	PoolByteArray data;
	data.resize(packet.GetProtoMessageLite().ByteSizeLong());
	packet.GetProtoMessageLite().SerializeToArray(data.write().ptr(), data.size());
	return data;
}

Array Packet::get_proto_vector() {
	auto proto_vector = packet.GetVectorOfProtoMessageLitePtrs().value();
	Array data;
	for (int i = 0; i < proto_vector.size(); i++) {
		auto message = proto_vector[i];
		PoolByteArray proto_bytes;
		proto_bytes.resize(message->ByteSizeLong());
		message->SerializeToArray(proto_bytes.write().ptr(), proto_bytes.size());
		data.push_back(proto_bytes);
	}
	return data;
}

Ref<Image> Packet::get_frame() {
	auto &image_frame = get_packet().Get<mediapipe::ImageFrame>();
	return to_image(image_frame);
}

int64_t Packet::get_timestamp() {
	return packet.Timestamp().Microseconds();
}

void Packet::set_timestamp(int64_t timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet Packet::get_packet() {
	return packet;
}
