#ifndef GDMP_TASK
#define GDMP_TASK

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/tasks/cc/audio/core/running_mode.h"
#include "mediapipe/tasks/cc/vision/core/running_mode.h"

#include "GDMP/tasks/base_options.h"

using namespace godot;
using namespace mediapipe::tasks;

class MediaPipeTask : public RefCounted {
		GDCLASS(MediaPipeTask, RefCounted)

	private:
		static std::map<std::string, std::function<void()>> &get_register_map();

	protected:
		static void _bind_methods();
		static bool register_task(const std::string &name, std::function<void()> func);

	public:
		enum AudioRunningMode {
			RUNNING_MODE_AUDIO_CLIPS = audio::core::RunningMode::AUDIO_CLIPS,
			RUNNING_MODE_AUDIO_STREAM = audio::core::RunningMode::AUDIO_STREAM,
		};

		enum VisionRunningMode {
			RUNNING_MODE_IMAGE = vision::core::RunningMode::IMAGE,
			RUNNING_MODE_VIDEO = vision::core::RunningMode::VIDEO,
			RUNNING_MODE_LIVE_STREAM = vision::core::RunningMode::LIVE_STREAM
		};

		static void register_tasks();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options);
};

VARIANT_ENUM_CAST(MediaPipeTask::AudioRunningMode)
VARIANT_ENUM_CAST(MediaPipeTask::VisionRunningMode)

#define GDMP_TASK_CLASS(NAME, TASK) \
	GDCLASS(NAME, MediaPipeTask)    \
private:                            \
	static bool _registered;        \
	static void _register_task();   \
                                    \
	std::unique_ptr<TASK> task;

#define GDMP_REGISTER_TASK(CLASS_NAME) \
	bool CLASS_NAME::_registered = MediaPipeTask::register_task(#CLASS_NAME, CLASS_NAME::_register_task);

#endif
