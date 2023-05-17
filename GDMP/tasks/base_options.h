#ifndef GDMP_TASK_BASE_OPTIONS
#define GDMP_TASK_BASE_OPTIONS

#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/core/base_options.h"

using namespace godot;
using namespace mediapipe::tasks::core;

class MediaPipeTaskBaseOptions : public RefCounted {
		GDCLASS(MediaPipeTaskBaseOptions, RefCounted)

	private:
		PackedByteArray model_asset_buffer;
		String model_asset_path;
		int delegate;

	protected:
		static void _bind_methods();

	public:
		enum Delegate {
			DELEGATE_CPU = BaseOptions::Delegate::CPU,
			DELEGATE_GPU = BaseOptions::Delegate::GPU,
			DELEGATE_EDGETPU_NNAPI = BaseOptions::Delegate::EDGETPU_NNAPI,
		};

		PackedByteArray get_model_asset_buffer();
		void set_model_asset_buffer(PackedByteArray buffer);

		String get_model_asset_path();
		void set_model_asset_path(const String &path);

		Delegate get_delegate();
		void set_delegate(Delegate delegate);

		std::unique_ptr<BaseOptions> get_base_options();
};

VARIANT_ENUM_CAST(MediaPipeTaskBaseOptions::Delegate)

#endif
