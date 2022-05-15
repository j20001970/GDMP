#include "packet.h"

#include <vector>

#include "../util/image.h"

using namespace godot;

void Packet::_register_methods() {
	register_method("get_image", &Packet::get_image);
	register_method("get_proto", &Packet::get_proto);
	register_method("get_proto_vector", &Packet::get_proto_vector);
	register_method("make", &Packet::make);
	register_method("make_image", &Packet::make_image);
	register_method("get_timestamp", &Packet::get_timestamp);
	register_method("set_timestamp", &Packet::set_timestamp);
}

Packet *Packet::_new(mediapipe::Packet packet) {
	Packet *p = Packet::_new();
	p->packet = packet;
	return p;
}

void Packet::_init(){};

Ref<Image> Packet::get_image() {
	auto &image_frame = get_packet().Get<mediapipe::ImageFrame>();
	return to_image(image_frame);
}

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

void Packet::make(Variant value) {
	switch (value.get_type()) {
		case Variant::Type::BOOL:
			packet = mediapipe::MakePacket<bool>(value);
			break;
		case Variant::Type::INT:
			packet = mediapipe::MakePacket<int>(value);
			break;
		case Variant::Type::REAL:
			packet = mediapipe::MakePacket<float>(value);
			break;
		case Variant::Type::STRING:
			packet = mediapipe::MakePacket<std::string>(String(value).alloc_c_string());
			break;
		default:
			ERR_PRINT("Unsupported type to make packet.");
			break;
	}
}

void Packet::make_image(Ref<Image> image) {
	mediapipe::ImageFrame image_frame = to_image_frame(image);
	make_image_frame(image_frame);
}

void Packet::make_image_frame(const mediapipe::ImageFrame &image_frame) {
	packet = mediapipe::Adopt(&image_frame);
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
