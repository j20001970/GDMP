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
	if (packet.ValidateAsType<bool>().ok())
		return packet.Get<bool>();
	if (packet.ValidateAsType<int32_t>().ok())
		return packet.Get<int32_t>();
	if (packet.ValidateAsType<int64_t>().ok())
		return packet.Get<int64_t>();
	if (packet.ValidateAsType<float>().ok())
		return packet.Get<float>();
	if (packet.ValidateAsType<std::string>().ok())
		return packet.Get<std::string>().c_str();
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
