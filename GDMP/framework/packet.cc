#include "packet.h"

void MediaPipePacket::_register_methods() {
	register_method("is_empty", &MediaPipePacket::is_empty);
	register_method("get", &MediaPipePacket::get);
	register_method("set", &MediaPipePacket::set);
	register_method("get_timestamp", &MediaPipePacket::get_timestamp);
	register_method("set_timestamp", &MediaPipePacket::set_timestamp);
	register_property<MediaPipePacket, int64_t>("timestamp", &MediaPipePacket::set_timestamp, &MediaPipePacket::get_timestamp, mediapipe::Timestamp::Unset().Value());
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
	if (packet.ValidateAsProtoMessageLite().ok())
		return Ref(MediaPipeProto::_new(packet.GetProtoMessageLite()));
	auto get_proto_vector = packet.GetVectorOfProtoMessageLitePtrs();
	if (get_proto_vector.ok()) {
		Array array;
		auto proto_vector = get_proto_vector.value();
		array.resize(proto_vector.size());
		for (int i = 0; i < proto_vector.size(); i++) {
			auto message = proto_vector[i];
			Ref<MediaPipeProto> proto = Ref(MediaPipeProto::_new(*message));
			array[i] = proto;
		}
		return array;
	}
	ERR_PRINT("Unsupported type");
	return Variant();
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
		case Variant::REAL:
			packet = mediapipe::MakePacket<float>(value).At(timestamp);
			return true;
		case Variant::STRING:
			String string = value;
			packet = mediapipe::MakePacket<std::string>(string.utf8().get_data()).At(timestamp);
			return true;
	}
	ERR_PRINT("Unsupported type");
	return false;
}

int64_t MediaPipePacket::get_timestamp() {
	return packet.Timestamp().Value();
}

void MediaPipePacket::set_timestamp(int64 timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet MediaPipePacket::get_packet() {
	return packet;
}
