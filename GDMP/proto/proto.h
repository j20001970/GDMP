#ifndef GDMP_PROTO
#define GDMP_PROTO

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "google/protobuf/message.h"

using namespace godot;
using namespace google;

class MediaPipeProto : public RefCounted {
		GDCLASS(MediaPipeProto, RefCounted);

	private:
		protobuf::Message *message;

		const protobuf::FieldDescriptor *get_field_descriptor(const String &field_name);

	protected:
		static void _bind_methods();

	public:
		static const protobuf::Message *get_prototype(const String &type_name);

		MediaPipeProto();
		MediaPipeProto(protobuf::Message *message);
		MediaPipeProto(const protobuf::Message &message);
		~MediaPipeProto();

		// Initialize the proto with the given type name.
		bool initialize(const String &type_name);
		// Check if the proto is initialized with a valid type.
		bool is_initialized();
		// Get the name of the proto type.
		String get_type();

		// Get the list of field names for this proto.
		PackedStringArray get_fields();
		// Check if a field is repeated.
		bool is_repeated_field(const String &field_name);
		// Get the size of repeated field.
		int get_field_size(const String &field_name);

		// Get the value from the field of the proto.
		Variant get(const String &field_name);
		// Get the value of repeated field on index.
		Variant get_repeated(const String &field_name, int index);

		// Set the value of the field for the proto.
		// Return true on success.
		bool set(const String &field_name, Variant value);

		// Get a copy of the proto, passing ownership to caller.
		protobuf::Message *get_proto();
};

#endif
