#include "util.h"

#include "godot_cpp/variant/typed_array.hpp"

#include "GDMP/proto/proto.h"

const protobuf::Message *get_prototype(const std::string &name) {
	const protobuf::DescriptorPool *pool = protobuf::DescriptorPool::generated_pool();
	const protobuf::Descriptor *descriptor = pool->FindMessageTypeByName(name);
	return get_prototype(descriptor);
}

const protobuf::Message *get_prototype(const protobuf::Descriptor *descriptor) {
	ERR_FAIL_COND_V(descriptor == nullptr, nullptr);
	protobuf::MessageFactory *factory = protobuf::MessageFactory::generated_factory();
	return factory->GetPrototype(descriptor);
}

Variant get_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
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
	}
	ERR_PRINT("Unsupported type.");
	return Variant();
}

Variant get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, int index) {
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

#define REFLECTION_METHOD(T) T (protobuf::Reflection::*get_method)(const protobuf::Message &, const protobuf::FieldDescriptor *, int) const

template <typename T>
Variant get_repeated_field_all(const protobuf::Message &message, const protobuf::FieldDescriptor *field, REFLECTION_METHOD(T)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<T> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = (refl->*get_method)(message, field, i);
	return array;
}

template <>
Variant get_repeated_field_all<std::string>(const protobuf::Message &message, const protobuf::FieldDescriptor *field, REFLECTION_METHOD(std::string)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<String> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = (refl->*get_method)(message, field, i).c_str();
	return array;
}

template <>
Variant get_repeated_field_all<const protobuf::Message &>(const protobuf::Message &message, const protobuf::FieldDescriptor *field, REFLECTION_METHOD(const protobuf::Message &)) {
	auto refl = message.GetReflection();
	int size = refl->FieldSize(message, field);
	TypedArray<MediaPipeProto> array;
	array.resize(size);
	for (int i = 0; i < size; i++)
		array[i] = memnew(MediaPipeProto((refl->*get_method)(message, field, i)));
	return array;
}

Variant get_repeated_field_all(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return get_repeated_field_all<bool>(message, field, &protobuf::Reflection::GetRepeatedBool);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return get_repeated_field_all<double>(message, field, &protobuf::Reflection::GetRepeatedDouble);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return get_repeated_field_all<float>(message, field, &protobuf::Reflection::GetRepeatedFloat);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return get_repeated_field_all<int>(message, field, &protobuf::Reflection::GetRepeatedEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return get_repeated_field_all<int32_t>(message, field, &protobuf::Reflection::GetRepeatedInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return get_repeated_field_all<int64_t>(message, field, &protobuf::Reflection::GetRepeatedInt64);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return get_repeated_field_all<std::string>(message, field, &protobuf::Reflection::GetRepeatedString);
		case protobuf::FieldDescriptor::TYPE_MESSAGE:
			return get_repeated_field_all<const protobuf::Message &>(message, field, &protobuf::Reflection::GetRepeatedMessage);
	}
	ERR_PRINT("Unsupported type.");
	return Variant();
}

bool set_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value) {
	auto refl = message.GetReflection();
	switch (value.get_type()) {
		case Variant::BOOL: {
			ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_BOOL, false);
			refl->SetBool(&message, field, value);
			return true;
		}
		case Variant::FLOAT: {
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
		case Variant::INT: {
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
		case Variant::STRING: {
			ERR_FAIL_COND_V(field->type() != protobuf::FieldDescriptor::TYPE_STRING, false);
			String string = value;
			refl->SetString(&message, field, string.utf8().get_data());
			return true;
		}
		case Variant::OBJECT: {
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
	}
	ERR_PRINT("Unsupported type.");
	return false;
}
