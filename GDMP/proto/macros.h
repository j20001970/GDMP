#ifndef GDMP_PROTO_MACROS
#define GDMP_PROTO_MACROS

#include <PoolArrays.hpp>
#include <Variant.hpp>

#define GDMP_PROTO_WRAPPER(NAME, CLASS)                         \
GODOT_CLASS(NAME, Reference)                                    \
private:                                                        \
    CLASS _data;                                                \
public:                                                         \
    void _init(){}                                              \
    void _init(CLASS data){                                     \
        _data = data;                                           \
    }                                                           \
    void from_bytes(PoolByteArray data) {                       \
         _data.ParseFromArray(data.read().ptr(), data.size());  \
    }                                                           \
    PoolByteArray to_bytes() {                                  \
        size_t size = _data.ByteSizeLong();                     \
        PoolByteArray bytes;                                    \
        bytes.resize(size);                                     \
        _data.SerializeToArray(bytes.write().ptr(), size);      \
        return bytes;                                           \
    }                                                           \

#define GDMP_RAW_FIELD(NAME)            \
Variant get_##NAME(){                   \
    if(_data.has_##NAME()){             \
        return Variant(_data.NAME());   \
    }                                   \
    else{                               \
        return Variant();               \
    }                                   \
}                                       \
void set_##NAME(Variant value){         \
    _data.set_##NAME(value);            \
}                                       \

#define GDMP_STRING_FIELD(NAME)                 \
Variant get_##NAME(){                           \
    if(_data.has_##NAME()){                     \
        return Variant(_data.NAME().c_str());   \
    }                                           \
    else{                                       \
        return Variant();                       \
    }                                           \
}                                               \
void set_##NAME(String value) {                 \
    _data.set_##NAME(value.alloc_c_string());   \
}                                               \

#define GDMP_PROTO_FIELD(NAME, CLASS)   \
Variant get_##NAME(){                   \
    if(_data.has_##NAME()){             \
        CLASS *ref = CLASS()._new();    \
        ref->_init(_data.NAME());       \
        return Variant(Ref<CLASS>(ref));\
    }                                   \
    else{                               \
        return Variant();               \
    }                                   \
}                                       \

#define _REPEATED_FIELD(NAME)   \
int NAME##_size() {             \
    return _data.NAME##_size(); \
}                               \

#define GDMP_REPEATED_RAW_FIELD(NAME)   \
_REPEATED_FIELD(NAME)                   \
Variant get_##NAME(int index) {         \
    return Variant(_data.NAME(index));  \
}                                       \

#define GDMP_REPEATED_STRING_FIELD(NAME)    \
_REPEATED_FIELD(NAME)                       \
String get_##NAME(int index){               \
    return _data.NAME(index).c_str();       \
}                                           \

#define GDMP_REPEATED_PROTO_FIELD(NAME, CLASS)  \
_REPEATED_FIELD(NAME)                           \
Ref<CLASS> get_##NAME(int index){               \
    CLASS *ref = CLASS()._new();                \
    ref->_init(_data.NAME(index));              \
    return Ref<CLASS>(ref);                     \
}                                               \

#endif
