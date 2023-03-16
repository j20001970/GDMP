#ifndef GDMP_PROTO_MACROS
#define GDMP_PROTO_MACROS

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "../framework/packet.h"

// Define a Godot wrapper class that contains a mediapipe proto type.
#define GDMP_PROTO_WRAPPER(NAME, CLASS)                          \
	GDCLASS(NAME, RefCounted)                                    \
                                                                 \
public:                                                          \
	NAME() = default;                                            \
	~NAME() = default;                                           \
	NAME(CLASS data) {                                           \
		NAME *p = new NAME();                                    \
		p->_data = data;                                         \
	}                                                            \
	/* Parse the serialized proto data from given byte array. */ \
	void from_bytes(PackedByteArray data) {                      \
		_data.ParseFromArray(data.ptr(), data.size());           \
	}                                                            \
	/* Create a packet object that contains the proto data. */   \
	Ref<MediaPipePacket> make_packet() {                                  \
		return new MediaPipePacket(mediapipe::MakePacket<CLASS>(_data));  \
	}                                                            \
	/* Get the serialized proto data byte array. */              \
	PackedByteArray to_bytes() {                                 \
		size_t size = _data.ByteSizeLong();                      \
		PackedByteArray bytes;                                   \
		bytes.resize(size);                                      \
		_data.SerializeToArray(bytes.ptrw(), size);              \
		return bytes;                                            \
	}                                                            \
	/* Get the actual proto object. */                           \
	CLASS data() {                                               \
		return _data;                                            \
	}                                                            \
                                                                 \
private:                                                         \
	CLASS _data;

// Register proto wrapper common methods.
#define GDMP_REGISTER_PROTO(CLASS)                                    \
	ClassDB::bind_method(D_METHOD("from_bytes"), &CLASS::from_bytes); \
	ClassDB::bind_method(D_METHOD("to_bytes"), &CLASS::to_bytes);     \
	ClassDB::bind_method(D_METHOD("make_packet"), &CLASS::make_packet);

// Macros for method string.
#define _GET_METHOD(x) "get_" #x
#define _SET_METHOD(x) "set_" #x
#define _SIZE_METHOD(x) #x "_size"

// Register setter and getter method for proto field.
#define GDMP_REGISTER_FIELD(NAME, CLASS)                                   \
	ClassDB::bind_method(D_METHOD(_SET_METHOD(NAME)), &CLASS::set_##NAME); \
	ClassDB::bind_method(D_METHOD(_GET_METHOD(NAME)), &CLASS::get_##NAME);

// Register getter and size method for repeated proto field.
#define GDMP_REGISTER_REPEATED_FIELD(NAME, CLASS)                            \
	ClassDB::bind_method(D_METHOD(_SIZE_METHOD(NAME)), &CLASS::NAME##_size); \
	ClassDB::bind_method(D_METHOD(_GET_METHOD(NAME)), &CLASS::get_##NAME);

// Define setter and getter method for proto field that is a raw type like
// scalar, bool or byte (that godot::Variant can convert automatically).
#define GDMP_RAW_FIELD(NAME)              \
	Variant get_##NAME() {                \
		if (_data.has_##NAME()) {         \
			return Variant(_data.NAME()); \
		} else {                          \
			return Variant();             \
		}                                 \
	}                                     \
	void set_##NAME(Variant value) {      \
		_data.set_##NAME(value);          \
	}

// Define setter and getter method for proto field that is a std::string.
#define GDMP_STRING_FIELD(NAME)                    \
	Variant get_##NAME() {                         \
		if (_data.has_##NAME()) {                  \
			return Variant(_data.NAME().c_str());  \
		} else {                                   \
			return Variant();                      \
		}                                          \
	}                                              \
	void set_##NAME(String value) {                \
		_data.set_##NAME(value.utf8().get_data()); \
	}

// Define setter and getter method for proto field that is another proto type.
#define GDMP_PROTO_FIELD(NAME, CLASS)             \
	Variant get_##NAME() {                        \
		if (_data.has_##NAME()) {                 \
			CLASS *ref = new CLASS(_data.NAME()); \
			return Variant(Ref<CLASS>(ref));      \
		} else {                                  \
			return Variant();                     \
		}                                         \
	}                                             \
	void set_##NAME(Ref<CLASS> value) {           \
		auto data = value->data();                \
		_data.set_allocated_##NAME(&data);        \
	}

// Define size method for any repeated proto field.
#define _REPEATED_FIELD(NAME)       \
	int NAME##_size() {             \
		return _data.NAME##_size(); \
	}

// Define getter and size method for repeated raw field.
#define GDMP_REPEATED_RAW_FIELD(NAME)      \
	_REPEATED_FIELD(NAME)                  \
	Variant get_##NAME(int index) {        \
		return Variant(_data.NAME(index)); \
	}

// Define getter and size method for repeated std::string field.
#define GDMP_REPEATED_STRING_FIELD(NAME)  \
	_REPEATED_FIELD(NAME)                 \
	String get_##NAME(int index) {        \
		return _data.NAME(index).c_str(); \
	}

// Define getter and size method for repeated proto field.
#define GDMP_REPEATED_PROTO_FIELD(NAME, CLASS)     \
	_REPEATED_FIELD(NAME)                          \
	Ref<CLASS> get_##NAME(int index) {             \
		CLASS *ref = new CLASS(_data.NAME(index)); \
		return Ref<CLASS>(ref);                    \
	}

#endif
