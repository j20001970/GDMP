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
		// Instantiate uninitialized proto message.
		MediaPipeProto();
		// Create from copying an existing protobuf::Message.
		MediaPipeProto(const protobuf::Message &message);
		// Create from copying an existing protobuf::MessageLite.
		MediaPipeProto(const protobuf::MessageLite &message);
		// Destructor, free message if initialized.
		~MediaPipeProto();

		// Initialize the proto with the given type name, return true on success.
		bool initialize(const String &type_name);
		// Check if the proto is initialized with a valid type.
		bool is_initialized();
		// Get the proto type name, return empty string if uninitialized.
		String get_type_name();

		// Get the list of field names for this proto.
		PackedStringArray get_fields();
		// Check if a field is repeated.
		bool is_repeated_field(const String &field_name);
		// Get the size of repeated field.
		int get_repeated_field_size(const String &field_name);

		// Get the value from the field of the proto.
		Variant get(const String &field_name);
		// Get the value of repeated field on index.
		Variant get_repeated(const String &field_name, int index);

		// Set the value of the field for the proto.
		// Return true on success.
		bool set(const String &field_name, Variant value);

		// Returns a new copy of the proto message.
		Ref<MediaPipeProto> duplicate();

		// Get underlying protobuf::Message.
		protobuf::Message *get_message();
};

#endif
