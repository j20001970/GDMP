#ifndef GDMP_TASK_RUNNER
#define GDMP_TASK_RUNNER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/dictionary.hpp"

#include "mediapipe/tasks/cc/core/task_runner.h"

#include "GDMP/framework/graph_config.h"

using namespace godot;
using namespace mediapipe::tasks::core;

class MediaPipeTaskRunner : public RefCounted {
		GDCLASS(MediaPipeTaskRunner, RefCounted)

	private:
		std::unique_ptr<TaskRunner> task_runner;

	protected:
		static void _bind_methods();

	public:
		// Initialize the task runner with graph config, optionally with callback for asynchronous result.
		bool initialize(Ref<MediaPipeGraphConfig> config, Callable callback);

		// Synchronous method for processing input packets.
		// Current thread is blocked until a result is returned.
		Dictionary process(Dictionary inputs);
		// Asynchronous method for handling live streaming data.
		// Packets callback must be provided for receiving output packets.
		bool send(Dictionary inputs);

		// Shut down the task runner.
		bool close();
		// Restart the task runner.
		bool restart();

		Ref<MediaPipeGraphConfig> get_graph_config();
};

#endif
