#ifndef GDMP_GRAPH_CONFIG
#define GDMP_GRAPH_CONFIG

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/resource_format_loader.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/calculator.pb.h"

using namespace godot;

class MediaPipeGraphConfig : public Resource {
		GDCLASS(MediaPipeGraphConfig, Resource)

	private:
		mediapipe::CalculatorGraphConfig config;

	protected:
		static void _bind_methods();

	public:
		MediaPipeGraphConfig();
		MediaPipeGraphConfig(mediapipe::CalculatorGraphConfig config);
		~MediaPipeGraphConfig();

		bool has_input_stream(String stream_name);
		bool has_output_stream(String stream_name);

		// Load graph config.
		Error load(String path, bool as_binary);
		// Parse graph config in binary format.
		bool parse_bytes(PackedByteArray data);
		// Parse graph config in text format.
		bool parse_text(String data);

		// Get underlying mediapipe::CalculatorGraphConfig
		mediapipe::CalculatorGraphConfig get_config();
};

class ResourceFormatLoaderMediaPipeGraphConfig : public ResourceFormatLoader {
		GDCLASS(ResourceFormatLoaderMediaPipeGraphConfig, ResourceFormatLoader)

	private:
		static Ref<ResourceFormatLoaderMediaPipeGraphConfig> singleton;

	protected:
		static void _bind_methods();

	public:
		static void register_loader();
		static void unregister_loader();

		virtual Variant _load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode);
		virtual PackedStringArray _get_recognized_extensions();
		virtual bool _handles_type(const StringName &type);
		virtual String _get_resource_type(const String &path);
};

#endif
