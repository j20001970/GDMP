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

	protected:
		static void _bind_methods();

		protobuf::Message *message;

		String _to_string() const;

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

		// Parse proto from binary format.
		bool parse_from_buffer(PackedByteArray buffer);
		// Parse proto from text format.
		bool parse_from_string(const String &string);
		// Serialize proto to binary format.
		PackedByteArray serialize_to_buffer();
		// Serialize proto to text format.
		String serialize_to_string();

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
