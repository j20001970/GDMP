#include "proto.h"

#include "godot_cpp/core/class_db.hpp"

#include "GDMP/proto/util.h"

void MediaPipeProto::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "type_name"), &MediaPipeProto::initialize);
	ClassDB::bind_method(D_METHOD("is_initialized"), &MediaPipeProto::is_initialized);
	ClassDB::bind_method(D_METHOD("get_type_name"), &MediaPipeProto::get_type_name);
	ClassDB::bind_method(D_METHOD("get_fields"), &MediaPipeProto::get_fields);
	ClassDB::bind_method(D_METHOD("is_repeated_field", "field_name"), &MediaPipeProto::is_repeated_field);
	ClassDB::bind_method(D_METHOD("get_repeated_field_size", "field_name"), &MediaPipeProto::get_repeated_field_size);
	ClassDB::bind_method(D_METHOD("get", "field_name"), &MediaPipeProto::get);
	ClassDB::bind_method(D_METHOD("get_repeated", "field_name", "index"), &MediaPipeProto::get_repeated);
	ClassDB::bind_method(D_METHOD("set", "field_name", "value"), &MediaPipeProto::set);
	ClassDB::bind_method(D_METHOD("duplicate"), &MediaPipeProto::duplicate);
}

const protobuf::FieldDescriptor *MediaPipeProto::get_field_descriptor(const String &field_name) {
	const std::string name = field_name.utf8().get_data();
	const protobuf::Descriptor *descriptor = message->GetDescriptor();
	const protobuf::FieldDescriptor *field = descriptor->FindFieldByName(name);
	return field;
}

MediaPipeProto::MediaPipeProto() {
	message = nullptr;
}

MediaPipeProto::MediaPipeProto(const protobuf::Message &message) {
	const protobuf::Descriptor *descriptor = message.GetDescriptor();
	const protobuf::Message *prototype = get_prototype(descriptor);
	this->message = prototype->New();
	this->message->CopyFrom(message);
}

MediaPipeProto::MediaPipeProto(const protobuf::MessageLite &message) {
	const std::string &type_name = message.GetTypeName();
	const protobuf::Message *prototype = get_prototype(type_name);
	this->message = prototype->New();
	this->message->ParseFromString(message.SerializeAsString());
}

MediaPipeProto::~MediaPipeProto() {
	if (message)
		::free(message);
}

bool MediaPipeProto::initialize(const String &type_name) {
	if (message)
		::free(message);
	const std::string &name = type_name.utf8().get_data();
	const protobuf::Message *prototype = get_prototype(name);
	ERR_FAIL_COND_V(prototype == nullptr, false);
	message = prototype->New();
	return true;
}

bool MediaPipeProto::is_initialized() {
	if (message == nullptr)
		return false;
	return true;
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
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_repeated_field_size(const String &field_name) {
	ERR_FAIL_COND_V(!is_initialized(), 0);
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	const protobuf::Reflection *reflection = message->GetReflection();
	return reflection->FieldSize(*message, field);
}

Variant MediaPipeProto::get(const String &field_name) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated())
		return get_repeated_field_all(*message, field);
	else
		return get_field(*message, field);
}

Variant MediaPipeProto::get_repeated(const String &field_name, int index) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	ERR_FAIL_COND_V(!field->is_repeated(), Variant());
	return get_repeated_field(*message, field, index);
}

bool MediaPipeProto::set(const String &field_name, Variant value) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated())
		ERR_FAIL_V_MSG(false, "Setting repeated field is unimplemented.");
	else
		return set_field(*message, field, value);
}

Ref<MediaPipeProto> MediaPipeProto::duplicate() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	return memnew(MediaPipeProto(*message));
}

protobuf::Message *MediaPipeProto::get_message() {
	return message;
}
