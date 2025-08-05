#include "proto.h"

#include "google/protobuf/text_format.h"

#include "GDMP/proto/util.h"

void MediaPipeProto::_register_methods() {
	register_method("initialize", &MediaPipeProto::initialize);
	register_method("is_initialized", &MediaPipeProto::is_initialized);
	register_method("parse_from_buffer", &MediaPipeProto::parse_from_buffer);
	register_method("parse_from_string", &MediaPipeProto::parse_from_string);
	register_method("serialize_to_buffer", &MediaPipeProto::serialize_to_buffer);
	register_method("serialize_to_string", &MediaPipeProto::serialize_to_string);
	register_method("get_type_name", &MediaPipeProto::get_type_name);
	register_method("get_fields", &MediaPipeProto::get_fields);
	register_method("is_repeated_field", &MediaPipeProto::is_repeated_field);
	register_method("get_repeated_field_size", &MediaPipeProto::get_repeated_field_size);
	register_method("get_field", &MediaPipeProto::get_field);
	register_method("get_repeated_field", &MediaPipeProto::get_repeated_field);
	register_method("set_field", &MediaPipeProto::set_field);
	register_method("duplicate", &MediaPipeProto::duplicate);
	register_method("get_packet", &MediaPipeProto::get_packet);
}

MediaPipeProto *MediaPipeProto::_new(const protobuf::Message &message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	const protobuf::Descriptor *descriptor = message.GetDescriptor();
	const protobuf::Message *prototype = util::get_prototype(descriptor);
	proto->message = prototype->New();
	proto->message->CopyFrom(message);
	return proto;
}

MediaPipeProto *MediaPipeProto::_new(const protobuf::MessageLite &message) {
	MediaPipeProto *proto = MediaPipeProto::_new();
	const std::string &type_name = message.GetTypeName();
	const protobuf::Message *prototype = util::get_prototype(type_name);
	proto->message = prototype->New();
	proto->message->ParseFromString(message.SerializeAsString());
	return proto;
}

bool MediaPipeProto::_register_packet_types = register_packet_types();

bool MediaPipeProto::register_packet_types() {
	std::function<bool(const mediapipe::Packet &)> validate_proto = [](const mediapipe::Packet &packet) {
		return packet.ValidateAsProtoMessageLite().ok();
	};
	std::function<Variant(const mediapipe::Packet &)> get_proto = [](const mediapipe::Packet &packet) {
		auto &proto = packet.GetProtoMessageLite();
		return Ref(MediaPipeProto::_new(proto));
	};
	MediaPipePacket::add_packet_type({ validate_proto, get_proto });
	std::function<bool(const mediapipe::Packet &)> validate_vector_proto = [](const mediapipe::Packet &packet) {
		return packet.GetVectorOfProtoMessageLitePtrs().ok();
	};
	std::function<Variant(const mediapipe::Packet &)> get_vector_proto = [](const mediapipe::Packet &packet) {
		Array array;
		auto vector_proto = packet.GetVectorOfProtoMessageLitePtrs().value();
		array.resize(vector_proto.size());
		for (int i = 0; i < vector_proto.size(); i++) {
			auto proto = vector_proto[i];
			array[i] = Ref(MediaPipeProto::_new(*proto));
		}
		return array;
	};
	MediaPipePacket::add_packet_type({ validate_vector_proto, get_vector_proto });
	return true;
}

String MediaPipeProto::_to_string() const {
	String class_name = this->get_class();
	String type_name = "null";
	if (message != nullptr)
		type_name = message->GetTypeName().data();
	String string = "<{0}[{1}]>";
	string = string.format(Array::make(class_name, type_name));
	return string;
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
	const protobuf::Message *prototype = util::get_prototype(name);
	ERR_FAIL_COND_V(prototype == nullptr, false);
	message = prototype->New();
	return true;
}

bool MediaPipeProto::is_initialized() {
	if (message == nullptr)
		return false;
	return true;
}

bool MediaPipeProto::parse_from_buffer(PoolByteArray buffer) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	return message->ParseFromArray(buffer.read().ptr(), buffer.size());
}

bool MediaPipeProto::parse_from_string(String string) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	return protobuf::TextFormat::ParseFromString(string.utf8().get_data(), message);
}

PoolByteArray MediaPipeProto::serialize_to_buffer() {
	PoolByteArray buffer;
	ERR_FAIL_COND_V(!is_initialized(), buffer);
	buffer.resize(message->ByteSizeLong());
	ERR_FAIL_COND_V(!message->SerializeToArray(buffer.write().ptr(), buffer.size()), buffer);
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
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, field_name);
	ERR_FAIL_COND_V(field == nullptr, false);
	return field->is_repeated();
}

int MediaPipeProto::get_repeated_field_size(String field_name) {
	ERR_FAIL_COND_V(!is_initialized(), 0);
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, field_name);
	ERR_FAIL_COND_V(field == nullptr, 0);
	ERR_FAIL_COND_V(!field->is_repeated(), 0);
	const protobuf::Reflection *reflection = message->GetReflection();
	return reflection->FieldSize(*message, field);
}

Variant MediaPipeProto::get_field(String field_name) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, names[0]);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = util::get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, proto);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		return proto->get_field(next_names);
	}
	if (field->is_repeated())
		return util::get_repeated_field(*message, field);
	else
		return util::get_field(*message, field);
}

Variant MediaPipeProto::get_repeated_field(String field_name, int index) {
	ERR_FAIL_COND_V(!is_initialized(), Variant());
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, names[0]);
	ERR_FAIL_COND_V(field == nullptr, Variant());
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = util::get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, proto);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		return proto->get_repeated_field(next_names, index);
	}
	ERR_FAIL_COND_V(!field->is_repeated(), Variant());
	return util::get_repeated_field(*message, field, index);
}

bool MediaPipeProto::set_field(String field_name, Variant value) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	PoolStringArray names = field_name.split("/");
	const protobuf::FieldDescriptor *field = util::get_field_descriptor(*message, names[0]);
	ERR_FAIL_COND_V(field == nullptr, false);
	if (names.size() > 1) {
		Ref<MediaPipeProto> proto = util::get_field(*message, field);
		ERR_FAIL_COND_V(proto == nullptr, false);
		names.remove(0);
		String next_names = names[0];
		for (int i = 1; i < names.size(); i++) {
			next_names = next_names.plus_file(names[i]);
		}
		ERR_FAIL_COND_V(!proto->set_field(next_names, value), false);
		return util::set_field(*message, field, proto);
	}
	if (field->is_repeated()) {
		ERR_PRINT("Setting repeated field is unimplemented.");
		return false;
	} else
		return util::set_field(*message, field, value);
}

Ref<MediaPipeProto> MediaPipeProto::duplicate() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	return MediaPipeProto::_new(*message);
}

Ref<MediaPipePacket> MediaPipeProto::get_packet() {
	ERR_FAIL_COND_V(!is_initialized(), nullptr);
	auto create_packet = mediapipe::packet_internal::PacketFromDynamicProto(message->GetTypeName(), message->SerializeAsString());
	ERR_FAIL_COND_V(!create_packet.ok(), nullptr);
	return Ref(MediaPipePacket::_new(create_packet.value()));
}

protobuf::Message *MediaPipeProto::get_message() {
	return message;
}
