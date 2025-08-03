#include "resource_provider.h"

#include <string>

#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/util/resource_util_custom.h"
#include "mediapipe/util/resource_util_internal.h"

using namespace godot;

// GDMP implementation of mediapipe::GetResourceContents
// The function will attempt to access path as Godot resource path.
// If godot_path is not found, fallback to platform-dependent mediapipe::internal::DefaultGetResourceContents
absl::Status GetResourceContents(const std::string &path, std::string *output) {
	String godot_path = String(path.c_str());
	if (godot_path.is_relative_path())
		godot_path = String("res://").path_join(godot_path);

	Ref<FileAccess> file = FileAccess::open(godot_path, FileAccess::READ);
	if (file.is_valid() && file->is_open()) {
		PackedByteArray data = file->get_buffer(file->get_length());
		output->resize(data.size());
		memcpy((void *)output->data(), data.ptr(), data.size());
		return absl::OkStatus();
	}
	WARN_PRINT_ED(String("{0} not found, fallback to DefaultGetResourceContents").format(Array::make(godot_path)));
	return mediapipe::internal::DefaultGetResourceContents(path, output, true);
}

void util::set_resource_provider() {
	mediapipe::SetCustomGlobalResourceProvider(GetResourceContents);
}
