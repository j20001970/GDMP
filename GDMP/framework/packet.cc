#include "packet.h"

#include <vector>

#include "String.hpp"

void MediaPipePacket::_register_methods() {
	register_method("is_empty", &MediaPipePacket::is_empty);
	register_method("get_proto", &MediaPipePacket::get_proto);
	register_method("get_proto_vector", &MediaPipePacket::get_proto_vector);
	register_method("make", &MediaPipePacket::make);
	register_method("get_timestamp", &MediaPipePacket::get_timestamp);
	register_method("set_timestamp", &MediaPipePacket::set_timestamp);
}

MediaPipePacket *MediaPipePacket::_new(const mediapipe::Packet &packet) {
	MediaPipePacket *p = MediaPipePacket::_new();
	p->packet = packet;
	return p;
}

void MediaPipePacket::_init() {}

bool MediaPipePacket::is_empty() {
	return packet.IsEmpty();
}

PoolByteArray MediaPipePacket::get_proto() {
	PoolByteArray data;
	ERR_FAIL_COND_V(!packet.ValidateAsProtoMessageLite().ok(), data);
	data.resize(packet.GetProtoMessageLite().ByteSizeLong());
	packet.GetProtoMessageLite().SerializeToArray(data.write().ptr(), data.size());
	return data;
}

Array MediaPipePacket::get_proto_vector() {
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

void MediaPipePacket::make(Variant value) {
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

int64_t MediaPipePacket::get_timestamp() {
	return packet.Timestamp().Microseconds();
}

void MediaPipePacket::set_timestamp(int64_t timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet MediaPipePacket::get_packet() {
	return packet;
}
