#include "packet.h"

#include "GDMP/util/image.h"

void MediaPipePacket::_register_methods() {
	register_method("is_empty", &MediaPipePacket::is_empty);
	register_method("get", &MediaPipePacket::get);
	register_method("set_bool", &MediaPipePacket::set_bool);
	register_method("set_int32", &MediaPipePacket::set_int32);
	register_method("set_int64", &MediaPipePacket::set_int64);
	register_method("set_float", &MediaPipePacket::set_float);
	register_method("set_string", &MediaPipePacket::set_string);
	register_method("set_image", &MediaPipePacket::set_image);
	register_method("set_image_frame", &MediaPipePacket::set_image_frame);
	register_method("get_timestamp", &MediaPipePacket::get_timestamp);
	register_method("set_timestamp", &MediaPipePacket::set_timestamp);
	register_property<MediaPipePacket, int64_t>("timestamp", &MediaPipePacket::set_timestamp, &MediaPipePacket::get_timestamp, mediapipe::Timestamp::Unset().Value());
}

PacketTypeList &MediaPipePacket::get_packet_type_list() {
	static PacketTypeList type_list;
	return type_list;
}

bool MediaPipePacket::_register_packet_types = register_packet_types();

bool MediaPipePacket::register_packet_types() {
	add_packet_type({ validate_packet_type<bool>, get_packet_type<bool> });
	add_packet_type({ validate_packet_type<int32_t>, get_packet_type<int32_t> });
	add_packet_type({ validate_packet_type<int64_t>, get_packet_type<int64_t> });
	add_packet_type({ validate_packet_type<float>, get_packet_type<float> });
	std::function<Variant(const mediapipe::Packet &)> get_string = [](const mediapipe::Packet &packet) {
		return packet.Get<std::string>().c_str();
	};
	add_packet_type({ MediaPipePacket::validate_packet_type<std::string>, get_string });
	return true;
};

bool MediaPipePacket::add_packet_type(PacketType packet_type) {
	ERR_FAIL_NULL_V(packet_type.first, false);
	ERR_FAIL_NULL_V(packet_type.second, false);
	PacketTypeList &type_list = get_packet_type_list();
	type_list.push_back(packet_type);
	return true;
}

MediaPipePacket *MediaPipePacket::_new(const mediapipe::Packet &packet) {
	MediaPipePacket *p = MediaPipePacket::_new();
	p->packet = packet;
	return p;
}

void MediaPipePacket::_init() {}

void MediaPipePacket::clear() {
	packet = mediapipe::Packet();
}

bool MediaPipePacket::is_empty() {
	return packet.IsEmpty();
}

String MediaPipePacket::get_type_name() {
	return packet.RegisteredTypeName().c_str();
}

Variant MediaPipePacket::get() {
	if (is_empty()) {
		return Variant();
	}
	const PacketTypeList &type_list = get_packet_type_list();
	for (const PacketType &type : type_list) {
		if (type.first(packet)) {
			return type.second(packet);
		}
	}
	ERR_PRINT("Unsupported type");
	return Variant();
}

template <typename T>
void MediaPipePacket::set(T value) {
	mediapipe::Timestamp timestamp = packet.Timestamp();
	packet = mediapipe::MakePacket<T>(value).At(timestamp);
}

template <typename T>
void MediaPipePacket::set(T *value) {
	mediapipe::Timestamp timestamp = packet.Timestamp();
	packet = mediapipe::Adopt(value).At(timestamp);
}

void MediaPipePacket::set_bool(bool value) {
	set(value);
}

void MediaPipePacket::set_int32(int32_t value) {
	set(value);
}

void MediaPipePacket::set_int64(int64_t value) {
	set(value);
}

void MediaPipePacket::set_float(float value) {
	set(value);
}

void MediaPipePacket::set_string(const String &value) {
	set(std::string(value.utf8().get_data()));
}

void MediaPipePacket::set_image(Ref<godot::Image> image) {
	ERR_FAIL_COND(image.is_null());
	set(util::get_image(image));
}

void MediaPipePacket::set_image_frame(Ref<godot::Image> image) {
	ERR_FAIL_COND(image.is_null());
	auto image_frame = util::get_image_frame(image);
	set(image_frame.release());
}

int64_t MediaPipePacket::get_timestamp() {
	return packet.Timestamp().Microseconds();
}

void MediaPipePacket::set_timestamp(int64_t timestamp_us) {
	packet = packet.At(mediapipe::Timestamp::FromMicroseconds(timestamp_us));
}

mediapipe::Packet MediaPipePacket::get_packet() {
	return packet;
}
