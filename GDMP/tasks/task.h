#ifndef GDMP_TASK
#define GDMP_TASK

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/audio/core/running_mode.h"
#include "mediapipe/tasks/cc/vision/core/running_mode.h"

#include "mediapipe/tasks/cc/core/base_options.h"

using namespace godot;
using namespace mediapipe::tasks;
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

class MediaPipeTask : public Reference {
	private:
		static std::map<std::string, std::function<void()>> &get_register_map();

	protected:
		static bool register_task(const std::string &name, std::function<void()> func);

	public:
		enum AudioRunningMode {
			RUNNING_MODE_AUDIO_CLIPS = audio::core::RunningMode::AUDIO_CLIPS,
			RUNNING_MODE_AUDIO_STREAM = audio::core::RunningMode::AUDIO_STREAM,
		};

		enum VisionRunningMode {
			RUNNING_MODE_IMAGE = vision::core::RunningMode::IMAGE,
			RUNNINE_MODE_VIDEO = vision::core::RunningMode::VIDEO,
			RUNNING_MODE_LIVE_STREAM = vision::core::RunningMode::LIVE_STREAM
		};

		static void register_tasks();
};

#define GDMP_TASK_CLASS(NAME, TASK) \
	GODOT_CLASS(NAME, Reference)    \
private:                            \
	static bool _registered;        \
	static void _register_task();   \
                                    \
	std::unique_ptr<TASK> task;

#define GDMP_REGISTER_TASK(CLASS_NAME) \
	bool CLASS_NAME::_registered = MediaPipeTask::register_task(#CLASS_NAME, CLASS_NAME::_register_task);

#endif
