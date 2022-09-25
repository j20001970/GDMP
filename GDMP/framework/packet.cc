#include "packet.h"

#include <vector>

#include "../util/image.h"

using namespace godot;

void Packet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_empty"), &Packet::is_empty);
	ClassDB::bind_method(D_METHOD("get_image"), &Packet::get_image);
	ClassDB::bind_method(D_METHOD("get_proto"), &Packet::get_proto);
	ClassDB::bind_method(D_METHOD("get_proto_vector"), &Packet::get_proto_vector);
	ClassDB::bind_method(D_METHOD("make"), &Packet::make);
	ClassDB::bind_method(D_METHOD("make_image"), &Packet::make_image);
	ClassDB::bind_method(D_METHOD("get_timestamp"), &Packet::get_timestamp);
	ClassDB::bind_method(D_METHOD("set_timestamp"), &Packet::set_timestamp);
}

Packet::Packet() = default;

Packet::Packet(const mediapipe::Packet &packet) {
	this->packet = packet;
}

Packet::~Packet() = default;

bool Packet::is_empty() {
	return packet.IsEmpty();
}

Ref<Image> Packet::get_image() {
	Ref<Image> image;
	ERR_FAIL_COND_V(!packet.ValidateAsType<mediapipe::ImageFrame>().ok(), image);
	auto &image_frame = get_packet().Get<mediapipe::ImageFrame>();
	return to_image(image_frame);
}

PackedByteArray Packet::get_proto() {
	PackedByteArray data;
	ERR_FAIL_COND_V(!packet.ValidateAsProtoMessageLite().ok(), data);
	data.resize(packet.GetProtoMessageLite().ByteSizeLong());
	packet.GetProtoMessageLite().SerializeToArray(data.ptrw(), data.size());
	return data;
}

Array Packet::get_proto_vector() {
	Array data;
	auto get_proto_vector = packet.GetVectorOfProtoMessageLitePtrs();
	ERR_FAIL_COND_V(!get_proto_vector.ok(), data);
	auto proto_vector = get_proto_vector.value();
	for (auto message : proto_vector) {
		PackedByteArray proto_bytes;
		proto_bytes.resize(message->ByteSizeLong());
		message->SerializeToArray(proto_bytes.ptrw(), proto_bytes.size());
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
		case Variant::Type::FLOAT:
			packet = mediapipe::MakePacket<float>(value);
			break;
		case Variant::Type::STRING: {
			String string = value;
			packet = mediapipe::MakePacket<std::string>(string.utf8().get_data());
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
