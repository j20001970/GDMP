#include <string>

#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/string.hpp"

#include "absl/strings/match.h"

#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"

using namespace godot;

namespace mediapipe {

namespace internal {

absl::Status DefaultGetResourceContents(const std::string &path, std::string *output, bool read_as_binary) {
	Ref<FileAccess> file = FileAccess::open(path.c_str(), FileAccess::READ);
	if (!file->is_open()) {
		return absl::InvalidArgumentError("Failed to get resource contents: " + std::to_string((int)file->get_open_error()));
	}
	if (read_as_binary) {
		PackedByteArray data = file->get_buffer(file->get_length());
		output->resize(data.size());
		memcpy((void *)output->data(), data.ptr(), data.size());
	} else {
		*output = file->get_as_text().utf8().get_data();
	}
	return absl::OkStatus();
}

} //namespace internal

absl::StatusOr<std::string> PathToResourceAsFile(const std::string &path) {
	if (absl::StartsWith("res://", path) || absl::StartsWith("user://", path)) {
		return path;
	}
	return "res://" + path;
}

} //namespace mediapipe
