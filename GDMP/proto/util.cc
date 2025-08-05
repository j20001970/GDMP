#include "util.h"

#include "godot_cpp/variant/typed_array.hpp"

#include "GDMP/proto/proto.h"

using namespace util;

const protobuf::Message *util::get_prototype(const std::string &name) {
	const protobuf::DescriptorPool *pool = protobuf::DescriptorPool::generated_pool();
	const protobuf::Descriptor *descriptor = pool->FindMessageTypeByName(name);
	return get_prototype(descriptor);
}

const protobuf::Message *util::get_prototype(const protobuf::Descriptor *descriptor) {
	ERR_FAIL_COND_V(descriptor == nullptr, nullptr);
	protobuf::MessageFactory *factory = protobuf::MessageFactory::generated_factory();
	return factory->GetPrototype(descriptor);
}

const protobuf::FieldDescriptor *util::get_field_descriptor(const protobuf::Message &message, const String &field_name) {
	const std::string name = field_name.utf8().get_data();
	const protobuf::Descriptor *descriptor = message.GetDescriptor();
	const protobuf::FieldDescriptor *field = descriptor->FindFieldByName(name);
	return field;
}

Variant util::get_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
	ERR_FAIL_COND_V(field->is_repeated(), Variant());
	auto refl = message.GetReflection();
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return refl->GetBool(message, field);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return refl->GetDouble(message, field);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return refl->GetFloat(message, field);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return refl->GetEnum(message, field)->index();
		case protobuf::FieldDescriptor::TYPE_INT32:
			return refl->GetInt32(message, field);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return refl->GetInt64(message, field);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return refl->GetString(message, field).c_str();
		case protobuf::FieldDescriptor::TYPE_MESSAGE:
			return memnew(MediaPipeProto(refl->GetMessage(message, field)));
		case protobuf::FieldDescriptor::TYPE_BYTES:
			auto string = refl->GetString(message, field);
			PackedByteArray bytes;
			bytes.resize(string.size());
			string.copy((char *)bytes.ptrw(), string.size());
			return bytes;
	}
	ERR_PRINT("Unsupported type.");
	return Variant();
}

Variant util::get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, int index) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	ERR_FAIL_INDEX_V(index, size, Variant());
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return refl->GetRepeatedBool(message, field, index);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return refl->GetRepeatedDouble(message, field, index);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return refl->GetRepeatedFloat(message, field, index);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return refl->GetRepeatedEnumValue(message, field, index);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return refl->GetRepeatedInt32(message, field, index);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return refl->GetRepeatedInt64(message, field, index);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return refl->GetRepeatedString(message, field, index).c_str();
		case protobuf::FieldDescriptor::TYPE_MESSAGE:
			return memnew(MediaPipeProto(refl->GetRepeatedMessage(message, field, index)));
	}
	ERR_PRINT("Unsupported type.");
	return Variant();
}

#define GET_METHOD(T) T (protobuf::Reflection::*get_method)(const protobuf::Message &, const protobuf::FieldDescriptor *, int) const

template <typename T>
Variant _get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, GET_METHOD(T)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<T> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = (refl->*get_method)(message, field, i);
	return array;
}

template <>
Variant _get_repeated_field<std::string>(const protobuf::Message &message, const protobuf::FieldDescriptor *field, GET_METHOD(std::string)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<String> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = (refl->*get_method)(message, field, i).c_str();
	return array;
}

template <>
Variant _get_repeated_field<const protobuf::Message &>(const protobuf::Message &message, const protobuf::FieldDescriptor *field, GET_METHOD(const protobuf::Message &)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<MediaPipeProto> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = memnew(MediaPipeProto((refl->*get_method)(message, field, i)));
	return array;
}

Variant util::get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return _get_repeated_field<bool>(message, field, &protobuf::Reflection::GetRepeatedBool);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return _get_repeated_field<double>(message, field, &protobuf::Reflection::GetRepeatedDouble);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return _get_repeated_field<float>(message, field, &protobuf::Reflection::GetRepeatedFloat);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return _get_repeated_field<int>(message, field, &protobuf::Reflection::GetRepeatedEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return _get_repeated_field<int32_t>(message, field, &protobuf::Reflection::GetRepeatedInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return _get_repeated_field<int64_t>(message, field, &protobuf::Reflection::GetRepeatedInt64);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return _get_repeated_field<std::string>(message, field, &protobuf::Reflection::GetRepeatedString);
		case protobuf::FieldDescriptor::TYPE_MESSAGE:
			return _get_repeated_field<const protobuf::Message &>(message, field, &protobuf::Reflection::GetRepeatedMessage);
	}
	ERR_PRINT("Unsupported type.");
	return Variant();
}

bool util::set_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value) {
	Variant::Type value_type = value.get_type();
	auto refl = message.GetReflection();
	if (value_type == Variant::BOOL) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_BOOL, false);
		refl->SetBool(&message, field, value);
		return true;
	}
	if (value_type == Variant::FLOAT) {
		switch (field->type()) {
			case protobuf::FieldDescriptor::TYPE_DOUBLE: {
				refl->SetDouble(&message, field, value);
				return true;
			}
			case protobuf::FieldDescriptor::TYPE_FLOAT: {
				refl->SetFloat(&message, field, value);
				return true;
			}
		}
	}
	if (value_type == Variant::INT) {
		switch (field->type()) {
			case protobuf::FieldDescriptor::TYPE_ENUM: {
				refl->SetEnumValue(&message, field, value);
				return true;
			}
			case protobuf::FieldDescriptor::TYPE_INT32: {
				refl->SetInt32(&message, field, value);
				return true;
			}
			case protobuf::FieldDescriptor::TYPE_INT64: {
				refl->SetInt64(&message, field, value);
				return true;
			}
		}
	}
	if (value_type == Variant::STRING) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_STRING, false);
		String string = value;
		refl->SetString(&message, field, string.utf8().get_data());
		return true;
	}
	if (value_type == Variant::OBJECT) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_MESSAGE, false);
		Ref<MediaPipeProto> proto = value;
		ERR_FAIL_COND_V(proto.is_null(), false);
		const protobuf::Descriptor *descriptor = proto->get_message()->GetDescriptor();
		const protobuf::Message *prototype = get_prototype(descriptor);
		protobuf::Message *sub_message = prototype->New();
		sub_message->CopyFrom(*proto->get_message());
		refl->SetAllocatedMessage(&message, sub_message, field);
		return true;
	}
	if (value_type == Variant::PACKED_BYTE_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_BYTES, false);
		PackedByteArray bytes = value;
		std::string string = std::string((char *)bytes.ptr(), bytes.size());
		refl->SetString(&message, field, string);
		return true;
	}
	ERR_PRINT("Unsupported type.");
	return false;
}

bool util::set_repeated_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value) {
	Variant::Type value_type = value.get_type();
	auto refl = message.GetReflection();
	if (value_type == Variant::PACKED_FLOAT32_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_FLOAT, false);
		PackedFloat32Array array = value;
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			refl->AddFloat(&message, field, array[i]);
		}
		return true;
	}
	if (value_type == Variant::PACKED_FLOAT64_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_DOUBLE, false);
		PackedFloat64Array array = value;
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			refl->AddDouble(&message, field, array[i]);
		}
		return true;
	}
	if (value_type == Variant::PACKED_INT32_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_INT32, false);
		PackedInt32Array array = value;
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			refl->AddInt32(&message, field, array[i]);
		}
		return true;
	}
	if (value_type == Variant::PACKED_INT64_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_INT64, false);
		PackedInt64Array array = value;
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			refl->AddInt64(&message, field, array[i]);
		}
		return true;
	}
	if (value_type == Variant::PACKED_STRING_ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_STRING, false);
		PackedStringArray array = value;
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			refl->AddString(&message, field, array[i].utf8().get_data());
		}
		return true;
	}
	if (value_type == Variant::ARRAY) {
		ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_MESSAGE, false);
		TypedArray<MediaPipeProto> array = value;
		for (int i = 0; i < array.size(); i++) {
			Ref<MediaPipeProto> proto = array[i];
			ERR_FAIL_NULL_V(proto, false);
			ERR_FAIL_COND_V(!proto->is_initialized(), false);
		}
		refl->ClearField(&message, field);
		for (int i = 0; i < array.size(); i++) {
			Ref<MediaPipeProto> proto = array[i];
			protobuf::Message *new_message = refl->AddMessage(&message, field);
			ERR_CONTINUE(new_message->GetTypeName().compare(proto->get_message()->GetTypeName()));
			new_message->CopyFrom(*proto->get_message());
		}
		return true;
	}
	ERR_PRINT("Unsupported type.");
	return false;
}
