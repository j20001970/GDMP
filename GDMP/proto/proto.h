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

		const protobuf::FieldDescriptor *get_field_descriptor(String field_name);

	public:
		static void _register_methods();
		static MediaPipeProto *_new(const protobuf::Message &message);
		static MediaPipeProto *_new(const protobuf::MessageLite &message);

		~MediaPipeProto();

		void _init();

		// Initialize the proto with the given type name.
		bool initialize(String type_name);
		// Check if the proto is initialized with a valid type.
		bool is_initialized();
		// Get the proto type name, return empty string if uninitialized.
		String get_type_name();

		// Get the list of field names for this proto.
		PoolStringArray get_fields();
		// Check if a field is repeated.
		bool is_repeated_field(String field_name);
		// Get the size of repeated field.
		int get_repeated_field_size(String field_name);

		// Get the value from the field of the proto.
		Variant get(String field_name);
		// Get the value of repeated field on index.
		Variant get_repeated(String field_name, int index);

		// Set the value of the field for the proto.
		// Return true on success.
		bool set(String field_name, Variant value);

		// Get a copy of the proto, passing ownership to caller.
		protobuf::Message *get_proto();
};

#endif
