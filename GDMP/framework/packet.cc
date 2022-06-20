#include "packet.h"

#include <vector>

#include "../util/image.h"

using namespace godot;

void Packet::_register_methods() {
	register_method("is_empty", &Packet::is_empty);
	register_method("get_image", &Packet::get_image);
	register_method("get_proto", &Packet::get_proto);
	register_method("get_proto_vector", &Packet::get_proto_vector);
	register_method("make", &Packet::make);
	register_method("make_image", &Packet::make_image);
	register_method("get_timestamp", &Packet::get_timestamp);
	register_method("set_timestamp", &Packet::set_timestamp);
}

Packet *Packet::_new(const mediapipe::Packet &packet) {
	Packet *p = Packet::_new();
	p->packet = packet;
	return p;
}

void Packet::_init(){};

bool Packet::is_empty() {
	return packet.IsEmpty();
}

Ref<Image> Packet::get_image() {
	Ref<Image> image;
	ERR_FAIL_COND_V(!packet.ValidateAsType<mediapipe::ImageFrame>().ok(), image);
	auto &image_frame = get_packet().Get<mediapipe::ImageFrame>();
	return to_image(image_frame);
}

PoolByteArray Packet::get_proto() {
	PoolByteArray data;
	ERR_FAIL_COND_V(!packet.ValidateAsProtoMessageLite().ok(), data);
	data.resize(packet.GetProtoMessageLite().ByteSizeLong());
	packet.GetProtoMessageLite().SerializeToArray(data.write().ptr(), data.size());
	return data;
}

Array Packet::get_proto_vector() {
	Array data;
	auto get_proto_vector = packet.GetVectorOfProtoMessageLitePtrs();
	ERR_FAIL_COND_V(!get_proto_vector.ok(), data);
	auto proto_vector = get_proto_vector.value();
	for (auto message : proto_vector) {
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
		case Variant::Type::STRING: {
			String string = value;
			packet = mediapipe::MakePacket<std::string>(string.alloc_c_string());
			break;
		}
		default:
			ERR_PRINT("Unsupported type to make packet.");
			break;
	}
}

void Packet::make_image(Ref<Image> image) {
	make_image_frame(std::move(to_image_frame(image)));
}

void Packet::make_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame) {
	packet = mediapipe::Adopt(image_frame.release());
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
