#include "packet.h"

#include <vector>

#include "godot_cpp/core/class_db.hpp"

void MediaPipePacket::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_empty"), &MediaPipePacket::is_empty);
	ClassDB::bind_method(D_METHOD("get_proto"), &MediaPipePacket::get_proto);
	ClassDB::bind_method(D_METHOD("get_proto_vector"), &MediaPipePacket::get_proto_vector);
	ClassDB::bind_method(D_METHOD("make"), &MediaPipePacket::make);
	ClassDB::bind_method(D_METHOD("get_timestamp"), &MediaPipePacket::get_timestamp);
	ClassDB::bind_method(D_METHOD("set_timestamp"), &MediaPipePacket::set_timestamp);
}

MediaPipePacket::MediaPipePacket() = default;

MediaPipePacket::MediaPipePacket(const mediapipe::Packet &packet) {
	this->packet = packet;
}

MediaPipePacket::~MediaPipePacket() = default;

bool MediaPipePacket::is_empty() {
	return packet.IsEmpty();
}

Ref<MediaPipeProto> MediaPipePacket::get_proto(const String &type_name) {
	Ref<MediaPipeProto> proto;
	auto prototype = MediaPipeProto::get_prototype(type_name);
	ERR_FAIL_COND_V(prototype == nullptr, proto);
	ERR_FAIL_COND_V(!packet.ValidateAsProtoMessageLite().ok(), proto);
	auto message = prototype->New();
	message->ParseFromString(packet.GetProtoMessageLite().SerializeAsString());
	proto = Ref(memnew(MediaPipeProto(message)));
	return proto;
}

TypedArray<MediaPipeProto> MediaPipePacket::get_proto_vector(const String &type_name) {
	TypedArray<MediaPipeProto> array;
	auto prototype = MediaPipeProto::get_prototype(type_name);
	ERR_FAIL_COND_V(prototype == nullptr, array);
	auto get_proto_vector = packet.GetVectorOfProtoMessageLitePtrs();
	ERR_FAIL_COND_V(!get_proto_vector.ok(), array);
	auto proto_vector = get_proto_vector.value();
	array.resize(proto_vector.size());
	for (int i = 0; i < proto_vector.size(); i++) {
		auto message = proto_vector[i];
		auto msg = prototype->New();
		msg->ParseFromString(message->SerializeAsString());
		Ref<MediaPipeProto> proto = memnew(MediaPipeProto(msg));
		array[i] = proto;
	}
	return array;
}

void MediaPipePacket::make(Variant value) {
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

int64_t MediaPipePacket::get_timestamp() {
	return packet.Timestamp().Microseconds();
}

void MediaPipePacket::set_timestamp(int64_t timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet MediaPipePacket::get_packet() {
	return packet;
}
