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

const protobuf::FieldDescriptor *util::get_field_descriptor(protobuf::Message **message, const String &field_name, const String &delimiter) {
	PackedStringArray names = field_name.split(delimiter);
	protobuf::Message *msg = *message;
	const protobuf::FieldDescriptor *field = nullptr;
	for (const String &name : names) {
		if (field) {
			if (field->type() == protobuf::FieldDescriptor::TYPE_MESSAGE) {
				const protobuf::Reflection *refl = msg->GetReflection();
				msg = refl->MutableMessage(msg, field);
			} else {
				ERR_PRINT(vformat("Field %s is type %s", field->name().data(), field->type_name().data()));
				return nullptr;
			}
		}
		field = get_field_descriptor(*msg, name);
		if (field == nullptr) {
			ERR_PRINT(vformat("Failed to get field descriptor %s", name));
			return nullptr;
		}
	}
	*message = msg;
	return field;
}

Variant util::get_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
	const protobuf::Reflection *refl = message.GetReflection();
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return refl->GetBool(message, field);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return refl->GetEnumValue(message, field);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return refl->GetInt32(message, field);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return refl->GetInt64(message, field);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return refl->GetFloat(message, field);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return refl->GetDouble(message, field);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return refl->GetString(message, field).c_str();
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			const protobuf::Message &proto = refl->GetMessage(message, field);
			return memnew(MediaPipeProto(proto));
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			const std::string &string = refl->GetString(message, field);
			PackedByteArray bytes;
			bytes.resize(string.size());
			string.copy((char *)bytes.ptrw(), string.size());
			return bytes;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return nullptr;
}

Variant util::get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, int index) {
	const protobuf::Reflection *refl = message.GetReflection();
	const int size = refl->FieldSize(message, field);
	ERR_FAIL_INDEX_V(index, size, nullptr);
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return refl->GetRepeatedBool(message, field, index);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return refl->GetRepeatedEnumValue(message, field, index);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return refl->GetRepeatedInt32(message, field, index);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return refl->GetRepeatedInt64(message, field, index);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return refl->GetRepeatedFloat(message, field, index);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return refl->GetRepeatedDouble(message, field, index);
		case protobuf::FieldDescriptor::TYPE_STRING:
			return refl->GetRepeatedString(message, field, index).c_str();
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			const protobuf::Message &proto = refl->GetRepeatedMessage(message, field, index);
			return memnew(MediaPipeProto(proto));
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			const std::string &string = refl->GetRepeatedString(message, field, index);
			PackedByteArray bytes;
			bytes.resize(string.size());
			string.copy((char *)bytes.ptrw(), string.size());
			return bytes;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return nullptr;
}

template <typename T>
Variant _get_repeated_field(
		const protobuf::Message &message, const protobuf::FieldDescriptor *field, const protobuf::Reflection *refl, const int size,
		T (protobuf::Reflection::*get_method)(const protobuf::Message &, const protobuf::FieldDescriptor *, int) const) {
	TypedArray<T> array;
	array.resize(size);
	for (int i = 0; i < size; i++) {
		array[i] = (refl->*get_method)(message, field, i);
	}
	return array;
}

Variant util::get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field) {
	const protobuf::Reflection *refl = message.GetReflection();
	const int size = refl->FieldSize(message, field);
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedBool);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedInt64);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedFloat);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return _get_repeated_field(message, field, refl, size, &protobuf::Reflection::GetRepeatedDouble);
		case protobuf::FieldDescriptor::TYPE_STRING: {
			TypedArray<String> array;
			array.resize(size);
			for (int i = 0; i < size; i++) {
				array[i] = refl->GetRepeatedString(message, field, i).c_str();
			}
			return array;
		}
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			TypedArray<MediaPipeProto> array;
			array.resize(size);
			for (int i = 0; i < size; i++) {
				const protobuf::Message &proto = refl->GetRepeatedMessage(message, field, i);
				array[i] = memnew(MediaPipeProto(proto));
			}
			return array;
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			TypedArray<PackedByteArray> array;
			array.resize(size);
			for (int i = 0; i < size; i++) {
				const std::string &string = refl->GetRepeatedString(message, field, i);
				PackedByteArray bytes;
				bytes.resize(string.size());
				string.copy((char *)bytes.ptrw(), string.size());
				array[i] = bytes;
			}
			return array;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return nullptr;
}

template <typename T>
bool _set_field(
		protobuf::Message &message, const protobuf::FieldDescriptor *field, const protobuf::Reflection *refl,
		Variant value, Variant::Type type,
		void (protobuf::Reflection::*set_method)(protobuf::Message *, const protobuf::FieldDescriptor *, T) const) {
	ERR_FAIL_COND_V(value.get_type() != type, false);
	(refl->*set_method)(&message, field, value);
	return true;
}

bool util::set_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value) {
	const protobuf::Reflection *refl = message.GetReflection();
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return _set_field(message, field, refl, value, Variant::BOOL, &protobuf::Reflection::SetBool);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return _set_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::SetEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return _set_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::SetInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return _set_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::SetInt64);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return _set_field(message, field, refl, value, Variant::FLOAT, &protobuf::Reflection::SetFloat);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return _set_field(message, field, refl, value, Variant::FLOAT, &protobuf::Reflection::SetDouble);
		case protobuf::FieldDescriptor::TYPE_STRING: {
			ERR_FAIL_COND_V(value.get_type() != Variant::STRING, false);
			String string = value;
			refl->SetString(&message, field, string.utf8().get_data());
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			protobuf::Message *sub_message = refl->MutableMessage(&message, field);
			if (value.get_type() == Variant::NIL) {
				sub_message->Clear();
				return true;
			}
			ERR_FAIL_COND_V(value.get_type() != Variant::OBJECT, false);
			Ref<MediaPipeProto> proto = value;
			ERR_FAIL_COND_V(proto.is_null(), false);
			ERR_FAIL_COND_V(!proto->is_initialized(), false);
			ERR_FAIL_COND_V(sub_message->GetTypeName() != proto->get_message()->GetTypeName(), false);
			sub_message->CopyFrom(*proto->get_message());
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			ERR_FAIL_COND_V(value.get_type() != Variant::PACKED_BYTE_ARRAY, false);
			PackedByteArray bytes = value;
			std::string string = std::string((char *)bytes.ptr(), bytes.size());
			refl->SetString(&message, field, string);
			return true;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return false;
}

template <typename T>
bool _set_repeated_field(
		protobuf::Message &message, const protobuf::FieldDescriptor *field, const protobuf::Reflection *refl,
		Variant value, Variant::Type type, const int index,
		void (protobuf::Reflection::*set_method)(protobuf::Message *, const protobuf::FieldDescriptor *, int, T) const) {
	ERR_FAIL_COND_V(value.get_type() != type, false);
	(refl->*set_method)(&message, field, index, value);
	return true;
}

bool util::set_repeated_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, int index, Variant value) {
	const protobuf::Reflection *refl = message.GetReflection();
	const int size = refl->FieldSize(message, field);
	ERR_FAIL_INDEX_V(index, size, false);
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return _set_repeated_field(message, field, refl, value, Variant::Type::BOOL, index, &protobuf::Reflection::SetRepeatedBool);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			return _set_repeated_field(message, field, refl, value, Variant::Type::INT, index, &protobuf::Reflection::SetRepeatedEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			return _set_repeated_field(message, field, refl, value, Variant::Type::INT, index, &protobuf::Reflection::SetRepeatedInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			return _set_repeated_field(message, field, refl, value, Variant::Type::INT, index, &protobuf::Reflection::SetRepeatedInt64);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			return _set_repeated_field(message, field, refl, value, Variant::Type::FLOAT, index, &protobuf::Reflection::SetRepeatedFloat);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			return _set_repeated_field(message, field, refl, value, Variant::Type::FLOAT, index, &protobuf::Reflection::SetRepeatedDouble);
		case protobuf::FieldDescriptor::TYPE_STRING: {
			ERR_FAIL_COND_V(value.get_type() != Variant::STRING, false);
			String string = value;
			refl->SetRepeatedString(&message, field, index, string.utf8().get_data());
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			protobuf::Message *sub_message = refl->MutableRepeatedMessage(&message, field, index);
			if (value.get_type() == Variant::NIL) {
				sub_message->Clear();
				return true;
			}
			ERR_FAIL_COND_V(value.get_type() != Variant::OBJECT, false);
			Ref<MediaPipeProto> proto = value;
			ERR_FAIL_COND_V(proto.is_null(), false);
			ERR_FAIL_COND_V(!proto->is_initialized(), false);
			sub_message->CopyFrom(*proto->get_message());
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			ERR_FAIL_COND_V(value.get_type() != Variant::PACKED_BYTE_ARRAY, false);
			PackedByteArray bytes = value;
			std::string string = std::string((char *)bytes.ptr(), bytes.size());
			refl->SetRepeatedString(&message, field, index, string);
			return true;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return false;
}

template <typename T>
bool _set_repeated_field(
		protobuf::Message &message, const protobuf::FieldDescriptor *field, const protobuf::Reflection *refl,
		Variant value, Variant::Type type,
		void (protobuf::Reflection::*add_method)(protobuf::Message *, const protobuf::FieldDescriptor *, T) const) {
	ERR_FAIL_COND_V(value.get_type() != Variant::ARRAY, false);
	Array array = value;
	ERR_FAIL_COND_V(array.get_typed_builtin() != type, false);
	refl->ClearField(&message, field);
	for (int i = 0; i < array.size(); i++) {
		(refl->*add_method)(&message, field, array[i]);
	}
	return true;
}

bool util::set_repeated_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value) {
	const protobuf::Reflection *refl = message.GetReflection();
	switch (field->type()) {
		case protobuf::FieldDescriptor::TYPE_BOOL:
			return _set_repeated_field(message, field, refl, value, Variant::BOOL, &protobuf::Reflection::AddBool);
		case protobuf::FieldDescriptor::TYPE_ENUM:
			if (value.get_type() == Variant::PACKED_INT32_ARRAY) {
				value = TypedArray<int32_t>(value);
			}
			return _set_repeated_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::AddEnumValue);
		case protobuf::FieldDescriptor::TYPE_INT32:
			if (value.get_type() == Variant::PACKED_INT32_ARRAY) {
				value = TypedArray<int32_t>(value);
			}
			return _set_repeated_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::AddInt32);
		case protobuf::FieldDescriptor::TYPE_INT64:
			if (value.get_type() == Variant::PACKED_INT64_ARRAY) {
				value = TypedArray<int64_t>(value);
			}
			return _set_repeated_field(message, field, refl, value, Variant::INT, &protobuf::Reflection::AddInt64);
		case protobuf::FieldDescriptor::TYPE_FLOAT:
			if (value.get_type() == Variant::PACKED_FLOAT32_ARRAY) {
				value = TypedArray<float>(value);
			}
			return _set_repeated_field(message, field, refl, value, Variant::FLOAT, &protobuf::Reflection::AddFloat);
		case protobuf::FieldDescriptor::TYPE_DOUBLE:
			if (value.get_type() == Variant::PACKED_FLOAT64_ARRAY) {
				value = TypedArray<double>(value);
			}
			return _set_repeated_field(message, field, refl, value, Variant::FLOAT, &protobuf::Reflection::AddDouble);
		case protobuf::FieldDescriptor::TYPE_STRING: {
			if (value.get_type() == Variant::PACKED_STRING_ARRAY) {
				value = TypedArray<String>(value);
			}
			ERR_FAIL_COND_V(value.get_type() != Variant::ARRAY, false);
			Array array = value;
			ERR_FAIL_COND_V(Variant::Type(array.get_typed_builtin()) != Variant::STRING, false);
			refl->ClearField(&message, field);
			for (int i = 0; i < array.size(); i++) {
				String string = array[i];
				refl->AddString(&message, field, string.utf8().get_data());
			}
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_MESSAGE: {
			ERR_FAIL_COND_V(value.get_type() != Variant::ARRAY, false);
			Array array = value;
			ERR_FAIL_COND_V(array.get_typed_class_name() != MediaPipeProto::get_class_static(), false);
			refl->ClearField(&message, field);
			for (int i = 0; i < array.size(); i++) {
				protobuf::Message *new_message = refl->AddMessage(&message, field);
				if (array[i].get_type() == Variant::NIL) {
					continue;
				}
				Ref<MediaPipeProto> proto = array[i];
				ERR_CONTINUE(proto.is_null());
				ERR_CONTINUE(!proto->is_initialized());
				ERR_CONTINUE(new_message->GetTypeName() != proto->get_message()->GetTypeName());
				new_message->CopyFrom(*proto->get_message());
			}
			return true;
		}
		case protobuf::FieldDescriptor::TYPE_BYTES: {
			ERR_FAIL_COND_V(value.get_type() != Variant::ARRAY, false);
			Array array = value;
			ERR_FAIL_COND_V(Variant::Type(array.get_typed_builtin()) != Variant::PACKED_BYTE_ARRAY, false);
			refl->ClearField(&message, field);
			for (int i = 0; i < array.size(); i++) {
				PackedByteArray bytes = array[i];
				std::string string = std::string((char *)bytes.ptr(), bytes.size());
				refl->AddString(&message, field, string);
			}
			return true;
		}
	}
	ERR_PRINT(vformat("Unsupported field type %s", field->type_name().data()));
	return false;
}
