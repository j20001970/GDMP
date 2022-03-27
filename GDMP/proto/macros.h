#ifndef GDMP_PROTO_MACROS
#define GDMP_PROTO_MACROS

#include "PoolArrays.hpp"
#include "Ref.hpp"
#include "Variant.hpp"

#include "../framework/packet.h"

#define GDMP_PROTO_WRAPPER(NAME, CLASS)                           \
	GODOT_CLASS(NAME, Reference)                                  \
public:                                                           \
	static NAME *_new(CLASS data) {                               \
		NAME *p = NAME::_new();                                   \
		p->_data = data;                                          \
		return p;                                                 \
	}                                                             \
	void _init() {}                                               \
	void from_bytes(PoolByteArray data) {                         \
		_data.ParseFromArray(data.read().ptr(), data.size());     \
	}                                                             \
	Ref<Packet> make_packet() {                                   \
		return Packet::_new(mediapipe::MakePacket<CLASS>(_data)); \
	}                                                             \
	PoolByteArray to_bytes() {                                    \
		size_t size = _data.ByteSizeLong();                       \
		PoolByteArray bytes;                                      \
		bytes.resize(size);                                       \
		_data.SerializeToArray(bytes.write().ptr(), size);        \
		return bytes;                                             \
	}                                                             \
	CLASS data() {                                                \
		return _data;                                             \
	}                                                             \
                                                                  \
private:                                                          \
	CLASS _data;

#define GDMP_REGISTER_PROTO(CLASS)                       \
	register_method("from_bytes", &CLASS::from_bytes);   \
	register_method("make_packet", &CLASS::make_packet); \
	register_method("to_bytes", &CLASS::to_bytes);

#define _GET_METHOD(x) "get_" #x
#define _SET_METHOD(x) "set_" #x
#define _SIZE_METHOD(x) #x "_size"

#define GDMP_REGISTER_FIELD(NAME, CLASS)                    \
	register_method(_SET_METHOD(NAME), &CLASS::set_##NAME); \
	register_method(_GET_METHOD(NAME), &CLASS::get_##NAME);

#define GDMP_REGISTER_REPEATED_FIELD(NAME, CLASS)             \
	register_method(_SIZE_METHOD(NAME), &CLASS::NAME##_size); \
	register_method(_GET_METHOD(NAME), &CLASS::get_##NAME);

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

#define GDMP_STRING_FIELD(NAME)                   \
	Variant get_##NAME() {                        \
		if (_data.has_##NAME()) {                 \
			return Variant(_data.NAME().c_str()); \
		} else {                                  \
			return Variant();                     \
		}                                         \
	}                                             \
	void set_##NAME(String value) {               \
		_data.set_##NAME(value.alloc_c_string()); \
	}

#define GDMP_PROTO_FIELD(NAME, CLASS)               \
	Variant get_##NAME() {                          \
		if (_data.has_##NAME()) {                   \
			CLASS *ref = CLASS::_new(_data.NAME()); \
			return Variant(Ref<CLASS>(ref));        \
		} else {                                    \
			return Variant();                       \
		}                                           \
	}                                               \
	void set_##NAME(Ref<CLASS> value) {             \
		auto data = value->data();                  \
		_data.set_allocated_##NAME(&data);          \
	}

#define _REPEATED_FIELD(NAME)       \
	int NAME##_size() {             \
		return _data.NAME##_size(); \
	}

#define GDMP_REPEATED_RAW_FIELD(NAME)      \
	_REPEATED_FIELD(NAME)                  \
	Variant get_##NAME(int index) {        \
		return Variant(_data.NAME(index)); \
	}

#define GDMP_REPEATED_STRING_FIELD(NAME)  \
	_REPEATED_FIELD(NAME)                 \
	String get_##NAME(int index) {        \
		return _data.NAME(index).c_str(); \
	}

#define GDMP_REPEATED_PROTO_FIELD(NAME, CLASS)       \
	_REPEATED_FIELD(NAME)                            \
	Ref<CLASS> get_##NAME(int index) {               \
		CLASS *ref = CLASS::_new(_data.NAME(index)); \
		return Ref<CLASS>(ref);                      \
	}

#endif
