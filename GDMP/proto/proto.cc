#include "proto.h"

#include "godot_cpp/core/class_db.hpp"

#include "GDMP/proto/util.h"

void MediaPipeProto::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "type_name"), &MediaPipeProto::initialize);
	ClassDB::bind_method(D_METHOD("is_initialized"), &MediaPipeProto::is_initialized);
	ClassDB::bind_method(D_METHOD("get_type"), &MediaPipeProto::get_type);
	ClassDB::bind_method(D_METHOD("get_fields"), &MediaPipeProto::get_fields);
	ClassDB::bind_method(D_METHOD("is_repeated_field", "field_name"), &MediaPipeProto::is_repeated_field);
	ClassDB::bind_method(D_METHOD("get_field_size", "field_name"), &MediaPipeProto::get_field_size);
	ClassDB::bind_method(D_METHOD("get", "field_name"), &MediaPipeProto::get);
	ClassDB::bind_method(D_METHOD("get_repeated", "field_name"), &MediaPipeProto::get_repeated);
	ClassDB::bind_method(D_METHOD("set", "field_name", "value"), &MediaPipeProto::set);
}

const protobuf::Message *MediaPipeProto::get_prototype(const String &type_name) {
	auto pool = protobuf::DescriptorPool::generated_pool();
	auto desc = pool->FindMessageTypeByName(type_name.utf8().get_data());
	ERR_FAIL_COND_V(desc == nullptr, nullptr);
	return protobuf::MessageFactory::generated_factory()->GetPrototype(desc);
}

MediaPipeProto::MediaPipeProto() {
	message = nullptr;
}

MediaPipeProto::MediaPipeProto(const protobuf::Message &message) {
	auto prototype = protobuf::MessageFactory::generated_factory()->GetPrototype(message.GetDescriptor());
	this->message = prototype->New();
	this->message->CopyFrom(message);
}

MediaPipeProto::MediaPipeProto(protobuf::Message *message) {
	this->message = message;
}

MediaPipeProto::~MediaPipeProto() {
	if (message)
		::free(message);
}

const protobuf::FieldDescriptor *MediaPipeProto::get_field_descriptor(const String &field_name) {
	ERR_FAIL_COND_V(message == nullptr, nullptr);
	auto desc = message->GetDescriptor();
	auto field = desc->FindFieldByName(field_name.utf8().get_data());
	return field;
}

bool MediaPipeProto::initialize(const String &type_name) {
	ERR_FAIL_COND_V(message != nullptr, false);
	auto prototype = get_prototype(type_name);
	ERR_FAIL_COND_V(prototype == nullptr, false);
	message = prototype->New();
	return true;
}

bool MediaPipeProto::is_initialized() {
	if (message == nullptr)
		return false;
	return true;
}

String MediaPipeProto::get_type() {
	ERR_FAIL_COND_V(message == nullptr, "");
	return message->GetTypeName().c_str();
}

PackedStringArray MediaPipeProto::get_fields() {
	PackedStringArray fields;
	ERR_FAIL_COND_V(message == nullptr, fields);
	auto desc = message->GetDescriptor();
	int count = desc->field_count();
	fields.resize(count);
	for (int i = 0; i < count; i++)
		fields.set(i, desc->field(i)->name().c_str());
	return fields;
}

bool MediaPipeProto::is_repeated_field(const String &field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_field_size(const String &field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	auto refl = message->GetReflection();
	return refl->FieldSize(*message, field);
}

Variant MediaPipeProto::get(const String &field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated())
		return get_repeated_field_all(*message, field);
	else
		return get_field(*message, field);
}

Variant MediaPipeProto::get_repeated(const String &field_name, int index) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	ERR_FAIL_COND_V(!field->is_repeated(), Variant());
	return get_repeated_field(*message, field, index);
}

bool MediaPipeProto::set(const String &field_name, Variant value) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated())
		ERR_FAIL_V_MSG(false, "Setting repeated field is unimplemented.");
	else
		return set_field(message, field, value);
}

protobuf::Message *MediaPipeProto::get_proto() {
	ERR_FAIL_COND_V(message == nullptr, nullptr);
	auto desc = message->GetDescriptor();
	protobuf::Message *proto = protobuf::MessageFactory::generated_factory()->GetPrototype(desc)->New();
	proto->CopyFrom(*message);
	return proto;
}
