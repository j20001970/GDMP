#include "proto.h"

#include "google/protobuf/text_format.h"

#include "godot_cpp/core/class_db.hpp"

#include "GDMP/proto/util.h"

void MediaPipeProto::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "type_name"), &MediaPipeProto::initialize);
	ClassDB::bind_method(D_METHOD("is_initialized"), &MediaPipeProto::is_initialized);
	ClassDB::bind_method(D_METHOD("parse_from_buffer", "buffer"), &MediaPipeProto::parse_from_buffer);
	ClassDB::bind_method(D_METHOD("parse_from_string", "string"), &MediaPipeProto::parse_from_string);
	ClassDB::bind_method(D_METHOD("serialize_to_buffer"), &MediaPipeProto::serialize_to_buffer);
	ClassDB::bind_method(D_METHOD("serialize_to_string"), &MediaPipeProto::serialize_to_string);
	ClassDB::bind_method(D_METHOD("get_type_name"), &MediaPipeProto::get_type_name);
	ClassDB::bind_method(D_METHOD("get_fields"), &MediaPipeProto::get_fields);
	ClassDB::bind_method(D_METHOD("is_repeated_field", "field_name"), &MediaPipeProto::is_repeated_field);
	ClassDB::bind_method(D_METHOD("get_repeated_field_size", "field_name"), &MediaPipeProto::get_repeated_field_size);
	ClassDB::bind_method(D_METHOD("get_field", "field_name", "delimiter"), &MediaPipeProto::get_field, DEFVAL("/"));
	ClassDB::bind_method(D_METHOD("get_repeated_field", "field_name", "index", "delimiter"), &MediaPipeProto::get_repeated_field, DEFVAL("/"));
	ClassDB::bind_method(D_METHOD("set_field", "field_name", "value", "delimiter"), &MediaPipeProto::set_field, DEFVAL("/"));
	ClassDB::bind_method(D_METHOD("set_repeated_field", "field_name", "index", "value", "delimiter"), &MediaPipeProto::set_repeated_field, DEFVAL("/"));
	ClassDB::bind_method(D_METHOD("duplicate"), &MediaPipeProto::duplicate);
	ClassDB::bind_method(D_METHOD("get_packet"), &MediaPipeProto::get_packet);
}

bool MediaPipeProto::_register_packet_types = register_packet_types();

bool MediaPipeProto::register_packet_types() {
	std::function<bool(const mediapipe::Packet &)> validate_proto = [](const mediapipe::Packet &packet) {
		return packet.ValidateAsProtoMessageLite().ok();
	};
	std::function<Variant(const mediapipe::Packet &)> get_proto = [](const mediapipe::Packet &packet) {
		auto &proto = packet.GetProtoMessageLite();
		return memnew(MediaPipeProto(proto));
	};
	MediaPipePacket::add_packet_type({ validate_proto, get_proto });
	std::function<bool(const mediapipe::Packet &)> validate_vector_proto = [](const mediapipe::Packet &packet) {
		return packet.GetVectorOfProtoMessageLitePtrs().ok();
	};
	std::function<Variant(const mediapipe::Packet &)> get_vector_proto = [](const mediapipe::Packet &packet) {
		TypedArray<MediaPipeProto> array;
		auto vector_proto = packet.GetVectorOfProtoMessageLitePtrs().value();
		array.resize(vector_proto.size());
		for (int i = 0; i < vector_proto.size(); i++) {
			auto proto = vector_proto[i];
			array[i] = memnew(MediaPipeProto(*proto));
		}
		return array;
	};
	MediaPipePacket::add_packet_type({ validate_vector_proto, get_vector_proto });
	return true;
}

String MediaPipeProto::_to_string() const {
	String class_name = this->get_class_static();
	String type_name = "null";
	if (message != nullptr)
		type_name = message->GetTypeName().data();
	String string = "<{0}[{1}]>";
	string = string.format(Array::make(class_name, type_name));
	return string;
}

MediaPipeProto::MediaPipeProto() = default;

MediaPipeProto::MediaPipeProto(const protobuf::Message &message) {
	const protobuf::Descriptor *descriptor = message.GetDescriptor();
	const protobuf::Message *prototype = util::get_prototype(descriptor);
	protobuf::Message *copy = prototype->New();
	copy->CopyFrom(message);
	this->message = std::unique_ptr<protobuf::Message>(copy);
}

MediaPipeProto::MediaPipeProto(const protobuf::MessageLite &message) {
	const std::string &type_name = message.GetTypeName().data();
	const protobuf::Message *prototype = util::get_prototype(type_name);
	protobuf::Message *copy = prototype->New();
	copy->ParseFromString(message.SerializeAsString());
	this->message = std::unique_ptr<protobuf::Message>(copy);
}

bool MediaPipeProto::initialize(const String &type_name) {
	ERR_FAIL_COND_V(is_initialized(), false);
	const std::string &name = type_name.utf8().get_data();
	const protobuf::Message *prototype = util::get_prototype(name);
	ERR_FAIL_COND_V(prototype == nullptr, false);
	message = std::unique_ptr<protobuf::Message>(prototype->New());
	return true;
}

bool MediaPipeProto::is_initialized() {
	if (message == nullptr)
		return false;
	return true;
}

bool MediaPipeProto::parse_from_buffer(PackedByteArray buffer) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	return message->ParseFromArray(buffer.ptr(), buffer.size());
}

bool MediaPipeProto::parse_from_string(const String &string) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	return protobuf::TextFormat::ParseFromString(string.utf8().get_data(), message.get());
}

PackedByteArray MediaPipeProto::serialize_to_buffer() {
	PackedByteArray buffer;
	ERR_FAIL_COND_V(!is_initialized(), buffer);
	buffer.resize(message->ByteSizeLong());
	ERR_FAIL_COND_V(!message->SerializeToArray(buffer.ptrw(), buffer.size()), buffer);
	return buffer;
}

String MediaPipeProto::serialize_to_string() {
	std::string string;
	ERR_FAIL_COND_V(!protobuf::TextFormat::PrintToString(*message, &string), string.data());
	return string.data();
}

String MediaPipeProto::get_type_name() {
	ERR_FAIL_COND_V(!is_initialized(), "");
	return message->GetTypeName().data();
}

PackedStringArray MediaPipeProto::get_fields() {
	PackedStringArray fields;
	ERR_FAIL_COND_V(!is_initialized(), fields);
	const protobuf::Descriptor *descriptor = message->GetDescriptor();
	int count = descriptor->field_count();
	fields.resize(count);
	for (int i = 0; i < count; i++)
		fields.set(i, descriptor->field(i)->name().data());
	return fields;
}

bool MediaPipeProto::is_repeated_field(const String &field_name) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_repeated_field_size(const String &field_name) {
	ERR_FAIL_COND_V(!is_initialized(), 0);
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	const protobuf::Reflection *reflection = message->GetReflection();
	return reflection->FieldSize(*message, field);
}

Variant MediaPipeProto::get_field(const String &field_name, const String &delimiter) {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	protobuf::Message *msg = message.get();
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(&msg, field_name, delimiter);
	ERR_FAIL_NULL_V(field, nullptr);
	if (field->is_repeated()) {
		return util::get_repeated_field(*msg, field);
	} else {
		return util::get_field(*msg, field);
	}
}

Variant MediaPipeProto::get_repeated_field(const String &field_name, int index, const String &delimiter) {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	protobuf::Message *msg = message.get();
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(&msg, field_name, delimiter);
	ERR_FAIL_COND_V(!field->is_repeated(), nullptr);
	return util::get_repeated_field(*msg, field, index);
}

bool MediaPipeProto::set_field(const String &field_name, Variant value, const String &delimiter) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	protobuf::Message *msg = message.get();
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(&msg, field_name, delimiter);
	if (field->is_repeated()) {
		return util::set_repeated_field(*msg, field, value);
	} else {
		return util::set_field(*msg, field, value);
	}
}

bool MediaPipeProto::set_repeated_field(const String &field_name, int index, Variant value, const String &delimiter) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	protobuf::Message *msg = message.get();
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(&msg, field_name, delimiter);
	ERR_FAIL_COND_V(!field->is_repeated(), false);
	return util::set_repeated_field(*msg, field, index, value);
}

Ref<MediaPipeProto> MediaPipeProto::duplicate() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	return memnew(MediaPipeProto(*message));
}

Ref<MediaPipePacket> MediaPipeProto::get_packet() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	auto create_packet = mediapipe::packet_internal::PacketFromDynamicProto(message->GetTypeName().data(), message->SerializeAsString());
	ERR_FAIL_COND_V_MSG(!create_packet.ok(), nullptr, create_packet.status().message().data());
	return memnew(MediaPipePacket(create_packet.value()));
}

protobuf::Message *MediaPipeProto::get_message() {
	return message.get();
}
