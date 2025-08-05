#ifndef GDMP_PROTO
#define GDMP_PROTO

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"
#include "Variant.hpp"

#include "google/protobuf/message.h"

#include "GDMP/framework/packet.h"

using namespace godot;
using namespace google;

class MediaPipeProto : public Reference {
		GODOT_CLASS(MediaPipeProto, Reference);

	private:
		static bool _register_packet_types;
		static bool register_packet_types();

	protected:
		protobuf::Message *message;

		String _to_string() const;

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

		// Parse proto from binary format.
		bool parse_from_buffer(PoolByteArray buffer);
		// Parse proto from text format.
		bool parse_from_string(String string);
		// Serialize proto to binary format.
		PoolByteArray serialize_to_buffer();
		// Serialize proto to text format.
		String serialize_to_string();

		// Get the proto type name, return empty string if uninitialized.
		String get_type_name();
		// Get the list of field names for this proto.
		PoolStringArray get_fields();
		// Check if a field is repeated.
		bool is_repeated_field(String field_name);
		// Get the size of repeated field.
		int get_repeated_field_size(String field_name);

		// Get the value from the field of the proto.
		Variant get_field(String field_name);
		// Get the value of repeated field on index.
		Variant get_repeated_field(String field_name, int index);

		// Set the value of the field for the proto.
		// Return true on success.
		bool set_field(String field_name, Variant value);

		// Returns a new copy of the proto message.
		Ref<MediaPipeProto> duplicate();

		Ref<MediaPipePacket> get_packet();

		// Get underlying protobuf::Message.
		protobuf::Message *get_message();
};

#endif
