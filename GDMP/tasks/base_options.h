#ifndef GDMP_TASK_BASE_OPTIONS
#define GDMP_TASK_BASE_OPTIONS

#include <memory>

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/core/base_options.h"

using namespace godot;
using namespace mediapipe::tasks::core;

class MediaPipeTaskBaseOptions : public Reference {
		GODOT_CLASS(MediaPipeTaskBaseOptions, Reference)

	private:
		PoolByteArray model_asset_buffer;
		String model_asset_path;
		int delegate;

	public:
		static void _register_methods();

		enum Delegate {
			DELEGATE_CPU = BaseOptions::Delegate::CPU,
			DELEGATE_GPU = BaseOptions::Delegate::GPU,
			DELEGATE_EDGETPU_NNAPI = BaseOptions::Delegate::EDGETPU_NNAPI,
		};

		void _init();

		PoolByteArray get_model_asset_buffer();
		void set_model_asset_buffer(PoolByteArray buffer);

		String get_model_asset_path();
		void set_model_asset_path(String path);

		int get_delegate();
		void set_delegate(int delegate);

		std::unique_ptr<BaseOptions> get_base_options();
};

#endif
