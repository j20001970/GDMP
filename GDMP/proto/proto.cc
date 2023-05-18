#include "proto.h"

#include "GDMP/proto/util.h"

void MediaPipeProto::_register_methods() {
	register_method("initialize", &MediaPipeProto::initialize);
	register_method("is_initialized", &MediaPipeProto::is_initialized);
	register_method("get_type", &MediaPipeProto::get_type);
	register_method("get_fields", &MediaPipeProto::get_fields);
	register_method("is_repeated_field", &MediaPipeProto::is_repeated_field);
	register_method("get_field_size", &MediaPipeProto::get_field_size);
	register_method("get", &MediaPipeProto::get);
	register_method("get_repeated", &MediaPipeProto::get_repeated);
	register_method("set", &MediaPipeProto::set);
}

const protobuf::Message *MediaPipeProto::get_prototype(String type_name) {
	auto pool = protobuf::DescriptorPool::generated_pool();
	auto desc = pool->FindMessageTypeByName(type_name.utf8().get_data());
	ERR_FAIL_COND_V(desc == nullptr, nullptr);
	return protobuf::MessageFactory::generated_factory()->GetPrototype(desc);
}

MediaPipeProto *MediaPipeProto::_new(const protobuf::Message &message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	auto prototype = protobuf::MessageFactory::generated_factory()->GetPrototype(message.GetDescriptor());
	proto->message = prototype->New();
	proto->message->CopyFrom(message);
	return proto;
}

MediaPipeProto *MediaPipeProto::_new(protobuf::Message *message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	proto->message = message;
	return proto;
}

void MediaPipeProto::_init() {
	message = nullptr;
}

MediaPipeProto::~MediaPipeProto() {
	if (message)
		::free(message);
}

const protobuf::FieldDescriptor *MediaPipeProto::get_field_descriptor(String field_name) {
	ERR_FAIL_COND_V(message == nullptr, nullptr);
	auto desc = message->GetDescriptor();
	auto field = desc->FindFieldByName(field_name.utf8().get_data());
	return field;
}

bool MediaPipeProto::initialize(String type_name) {
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

PoolStringArray MediaPipeProto::get_fields() {
	PoolStringArray fields;
	ERR_FAIL_COND_V(message == nullptr, fields);
	auto desc = message->GetDescriptor();
	int count = desc->field_count();
	fields.resize(count);
	for (int i = 0; i < count; i++)
		fields.set(i, desc->field(i)->name().c_str());
	return fields;
}

bool MediaPipeProto::is_repeated_field(String field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_field_size(String field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	auto refl = message->GetReflection();
	return refl->FieldSize(*message, field);
}

Variant MediaPipeProto::get(String field_name) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated())
		return get_repeated_field_all(*message, field);
	else
		return get_field(*message, field);
}

Variant MediaPipeProto::get_repeated(String field_name, int index) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	ERR_FAIL_COND_V(!field->is_repeated(), Variant());
	return get_repeated_field(*message, field, index);
}

bool MediaPipeProto::set(String field_name, Variant value) {
	auto field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (field->is_repeated()) {
		ERR_PRINT("Setting repeated field is unimplemented.");
		return false;
	} else
		return set_field(message, field, value);
}

protobuf::Message *MediaPipeProto::get_proto() {
	ERR_FAIL_COND_V(message == nullptr, nullptr);
	auto desc = message->GetDescriptor();
	protobuf::Message *proto = protobuf::MessageFactory::generated_factory()->GetPrototype(desc)->New();
	proto->CopyFrom(*message);
	return proto;
}
