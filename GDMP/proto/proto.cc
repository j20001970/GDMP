#include "proto.h"

#include "GDMP/proto/util.h"

void MediaPipeProto::_register_methods() {
	register_method("initialize", &MediaPipeProto::initialize);
	register_method("is_initialized", &MediaPipeProto::is_initialized);
	register_method("get_type_name", &MediaPipeProto::get_type_name);
	register_method("get_fields", &MediaPipeProto::get_fields);
	register_method("is_repeated_field", &MediaPipeProto::is_repeated_field);
	register_method("get_repeated_field_size", &MediaPipeProto::get_repeated_field_size);
	register_method("get", &MediaPipeProto::get);
	register_method("get_repeated", &MediaPipeProto::get_repeated);
	register_method("set", &MediaPipeProto::set);
	register_method("duplicate", &MediaPipeProto::duplicate);
}

MediaPipeProto *MediaPipeProto::_new(const protobuf::Message &message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	const protobuf::Descriptor *descriptor = message.GetDescriptor();
	const protobuf::Message *prototype = get_prototype(descriptor);
	proto->message = prototype->New();
	proto->message->CopyFrom(message);
	return proto;
}

MediaPipeProto *MediaPipeProto::_new(const protobuf::MessageLite &message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	const std::string &type_name = message.GetTypeName();
	const protobuf::Message *prototype = get_prototype(type_name);
	proto->message = prototype->New();
	proto->message->ParseFromString(message.SerializeAsString());
	return proto;
}

const protobuf::FieldDescriptor *MediaPipeProto::get_field_descriptor(String field_name) {
	const std::string name = field_name.utf8().get_data();
	const protobuf::Descriptor *descriptor = message->GetDescriptor();
	const protobuf::FieldDescriptor *field = descriptor->FindFieldByName(name);
	return field;
}

void MediaPipeProto::_init() {
	message = nullptr;
}

MediaPipeProto::~MediaPipeProto() {
	if (message)
		::free(message);
}

bool MediaPipeProto::initialize(String type_name) {
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

PoolStringArray MediaPipeProto::get_fields() {
	PoolStringArray fields;
	ERR_FAIL_COND_V(!is_initialized(), fields);
	const protobuf::Descriptor *descriptor = message->GetDescriptor();
	int count = descriptor->field_count();
	fields.resize(count);
	for (int i = 0; i < count; i++)
		fields.set(i, descriptor->field(i)->name().data());
	return fields;
}

bool MediaPipeProto::is_repeated_field(String field_name) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_repeated_field_size(String field_name) {
	ERR_FAIL_COND_V(!is_initialized(), 0);
	const protobuf::FieldDescriptor *field = get_field_descriptor(field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	const protobuf::Reflection *reflection = message->GetReflection();
	return reflection->FieldSize(*message, field);
}

Variant MediaPipeProto::get(String field_name) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = get_field_descriptor(names[0]);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, proto);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		return proto->get(next_names);
	}
	if (field->is_repeated())
		return get_repeated_field_all(*message, field);
	else
		return get_field(*message, field);
}

Variant MediaPipeProto::get_repeated(String field_name, int index) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = get_field_descriptor(names[0]);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, proto);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		return proto->get_repeated(next_names, index);
	}
	ERR_FAIL_COND_V(!field->is_repeated(), Variant());
	return get_repeated_field(*message, field, index);
}

bool MediaPipeProto::set(String field_name, Variant value) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = get_field_descriptor(names[0]);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, false);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		ERR_FAIL_COND_V(!proto->set(next_names, value), false);
		return set_field(*message, field, proto);
	}
	if (field->is_repeated()) {
		ERR_PRINT("Setting repeated field is unimplemented.");
		return false;
	} else
		return set_field(*message, field, value);
}

Ref<MediaPipeProto> MediaPipeProto::duplicate() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	return MediaPipeProto::_new(*message);
}

protobuf::Message *MediaPipeProto::get_message() {
	return message;
}
