#include "resource_provider.h"

#include <string>

#include "File.hpp"
#include "Ref.hpp"
#include "Defs.hpp"
#include "Array.hpp"
#include "PoolArrays.hpp"
#include "String.hpp"

#include "mediapipe/util/resource_util_custom.h"
#include "mediapipe/util/resource_util_internal.h"

using namespace godot;

// GDMP implementation of mediapipe::GetResourceContents
// The function will attempt to access path as Godot resource path.
// If godot_path is not found, fallback to platform-dependent mediapipe::internal::DefaultGetResourceContents
absl::Status GetResourceContents(const std::string &path, std::string *output) {
	String godot_path = String(path.c_str());
	if (godot_path.is_rel_path())
		godot_path = String("res://").plus_file(godot_path);

	Ref<File> file = File::_new();
	file->open(godot_path, File::READ);
	if (file->is_open()) {
		PoolByteArray data = file->get_buffer(file->get_len());
		output->resize(data.size());
		memcpy((void *)output->data(), data.read().ptr(), data.size());
		return absl::OkStatus();
	}
	WARN_PRINT(String("{0} not found, fallback to DefaultGetResourceContents").format(Array::make(godot_path)));
	return mediapipe::internal::DefaultGetResourceContents(path, output, true);
}

void util::set_resource_provider() {
	mediapipe::SetCustomGlobalResourceProvider(GetResourceContents);
}
