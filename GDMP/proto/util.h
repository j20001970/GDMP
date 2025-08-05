#ifndef GDMP_PROTO_UTIL
#define GDMP_PROTO_UTIL

#include "Variant.hpp"

#include "google/protobuf/message.h"

using namespace godot;
using namespace google;

namespace util {

const protobuf::Message *get_prototype(const std::string &name);

const protobuf::Message *get_prototype(const protobuf::Descriptor *descriptor);

const protobuf::FieldDescriptor *get_field_descriptor(const protobuf::Message &message, const String &field_name);

Variant get_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field);

Variant get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field);

Variant get_repeated_field(const protobuf::Message &message, const protobuf::FieldDescriptor *field, int index);

bool set_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value);

bool set_repeated_field(protobuf::Message &message, const protobuf::FieldDescriptor *field, Variant value);

} //namespace util

#endif
