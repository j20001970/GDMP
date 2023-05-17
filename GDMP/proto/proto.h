#ifndef GDMP_PROTO
#define GDMP_PROTO

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"
#include "Variant.hpp"

#include "google/protobuf/message.h"

using namespace godot;
using namespace google;

class MediaPipeProto : public Reference {
		GODOT_CLASS(MediaPipeProto, Reference);

	private:
		protobuf::Message *message;

		const protobuf::FieldDescriptor *get_field_descriptor(const String &field_name);

	public:
		static void _register_methods();
		static const protobuf::Message *get_prototype(const String &type_name);
		static MediaPipeProto *_new(protobuf::Message *message);
		static MediaPipeProto *_new(const protobuf::Message &message);

		~MediaPipeProto();

		void _init();

		// Initialize the proto with the given type name.
		bool initialize(const String &type_name);
		// Check if the proto is initialized with a valid type.
		bool is_initialized();
		// Get the name of the proto type.
		String get_type();

		// Get the list of field names for this proto.
		PoolStringArray get_fields();
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
