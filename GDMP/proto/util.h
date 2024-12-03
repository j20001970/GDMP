#ifndef GDMP_PROTO_UTIL
#define GDMP_PROTO_UTIL

#include "godot_cpp/variant/variant.hpp"

#include "google/protobuf/message.h"

using namespace godot;
using namespace google;

const protobuf::Message *get_prototype(const std::string &name);

const protobuf::Message *get_prototype(const protobuf::Descriptor *descriptor);

Variant get_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field);

Variant get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, int index);

Variant get_repeated_field_all(const protobuf::Message &message, const protobuf::FieldDescriptor *field);

bool set_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value);

#endif
