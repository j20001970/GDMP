#include "packet.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

#include "GDMP/util/image.h"

void MediaPipePacket::_bind_methods() {
	ClassDB::bind_method(D_METHOD("clear"), &MediaPipePacket::clear);
	ClassDB::bind_method(D_METHOD("is_empty"), &MediaPipePacket::is_empty);
	ClassDB::bind_method(D_METHOD("get_type_name"), &MediaPipePacket::get_type_name);
	ClassDB::bind_method(D_METHOD("get"), &MediaPipePacket::get);
	ClassDB::bind_method(D_METHOD("set_bool", "value"), &MediaPipePacket::set_bool);
	ClassDB::bind_method(D_METHOD("set_int32", "value"), &MediaPipePacket::set_int32);
	ClassDB::bind_method(D_METHOD("set_int64", "value"), &MediaPipePacket::set_int64);
	ClassDB::bind_method(D_METHOD("set_float", "value"), &MediaPipePacket::set_float);
	ClassDB::bind_method(D_METHOD("set_string", "value"), &MediaPipePacket::set_string);
	ClassDB::bind_method(D_METHOD("set_image", "image"), &MediaPipePacket::set_image);
	ClassDB::bind_method(D_METHOD("set_image_frame", "image"), &MediaPipePacket::set_image_frame);
	ClassDB::bind_method(D_METHOD("get_timestamp"), &MediaPipePacket::get_timestamp);
	ClassDB::bind_method(D_METHOD("set_timestamp", "timestamp_us"), &MediaPipePacket::set_timestamp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "timestamp"), "set_timestamp", "get_timestamp");
}

MediaPipePacket::MediaPipePacket() = default;

MediaPipePacket::MediaPipePacket(const mediapipe::Packet &packet) {
	this->packet = packet;
}

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
		return nullptr;
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
	return nullptr;
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
	ERR_FAIL_NULL(image);
	set(util::get_image(image));
}

void MediaPipePacket::set_image_frame(Ref<godot::Image> image) {
	ERR_FAIL_NULL(image);
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
