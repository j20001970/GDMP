#ifndef GDMP_PROTO_MACROS
#define GDMP_PROTO_MACROS

#include <PoolArrays.hpp>

#define GDMP_PROTO_WRAPPER(NAME, CLASS)                         \
private:                                                        \
    CLASS _data;                                                \
public:                                                         \
    void from_bytes(PoolByteArray data) {                \
         _data.ParseFromArray(data.read().ptr(), data.size());  \
    }                                                           \

#endif
