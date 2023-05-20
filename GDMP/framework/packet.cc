#include "packet.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipePacket::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_empty"), &MediaPipePacket::is_empty);
	ClassDB::bind_method(D_METHOD("get"), &MediaPipePacket::get);
	ClassDB::bind_method(D_METHOD("set", "value"), &MediaPipePacket::set);
	ClassDB::bind_method(D_METHOD("get_proto", "type_name"), &MediaPipePacket::get_proto);
	ClassDB::bind_method(D_METHOD("get_proto_vector", "type_name"), &MediaPipePacket::get_proto_vector);
	ClassDB::bind_method(D_METHOD("get_timestamp"), &MediaPipePacket::get_timestamp);
	ClassDB::bind_method(D_METHOD("set_timestamp", "timestamp"), &MediaPipePacket::set_timestamp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "timestamp"), "set_timestamp", "get_timestamp");
}

MediaPipePacket::MediaPipePacket() = default;

MediaPipePacket::MediaPipePacket(const mediapipe::Packet &packet) {
	this->packet = packet;
}

bool MediaPipePacket::is_empty() {
	return packet.IsEmpty();
}

Variant MediaPipePacket::get() {
	ERR_FAIL_COND_V(is_empty(), Variant());
	if (packet.ValidateAsType<bool>().ok())
		return packet.Get<bool>();
	if (packet.ValidateAsType<int>().ok())
		return packet.Get<int>();
	if (packet.ValidateAsType<float>().ok())
		return packet.Get<float>();
	if (packet.ValidateAsType<std::string>().ok())
		return packet.Get<std::string>().c_str();
	ERR_FAIL_V_MSG(Variant(), "Unsupported type.");
}

bool MediaPipePacket::set(Variant value) {
	mediapipe::Timestamp timestamp = packet.Timestamp();
	switch (value.get_type()) {
		case Variant::NIL:
			packet = mediapipe::Packet();
			return true;
		case Variant::BOOL:
			packet = mediapipe::MakePacket<bool>(value).At(timestamp);
			return true;
		case Variant::INT:
			packet = mediapipe::MakePacket<int>(value).At(timestamp);
			return true;
		case Variant::FLOAT:
			packet = mediapipe::MakePacket<float>(value).At(timestamp);
			return true;
		case Variant::STRING:
			String string = value;
			packet = mediapipe::MakePacket<std::string>(string.utf8().get_data()).At(timestamp);
			return true;
	}
	ERR_FAIL_V_MSG(false, "Unsupported type.");
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

int64_t MediaPipePacket::get_timestamp() {
	return packet.Timestamp().Value();
}

void MediaPipePacket::set_timestamp(int64_t timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet MediaPipePacket::get_packet() {
	return packet;
}
