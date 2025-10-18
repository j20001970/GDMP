#ifndef GDMP_TASK_RUNNER
#define GDMP_TASK_RUNNER

#include "Dictionary.hpp"
#include "Object.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/core/task_runner.h"

#include "GDMP/framework/graph_config.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;
using namespace mediapipe::tasks::core;

class MediaPipeTaskRunner : public Reference {
		GODOT_CLASS(MediaPipeTaskRunner, Reference)

	private:
		std::unique_ptr<TaskRunner> task_runner;

	public:
		static void _register_methods();

		void _init();

		// Initialize the task runner with graph config.
		// Use `async` to initialize with asynchronous mode.
		bool initialize(Ref<MediaPipeGraphConfig> config, bool async, Dictionary input_side_packets, Ref<MediaPipeGPUResources> gpu_resources);

		// Synchronous method for processing input packets.
		// Current thread is blocked until a result is returned.
		Dictionary process(Dictionary inputs);
		// Asynchronous method for handling live streaming data.
		// Must be initialized with asynchronous mode to use this method.
		bool send(Dictionary inputs);

		// Shut down the task runner.
		bool close();
		// Restart the task runner.
		bool restart();

		Ref<MediaPipeGraphConfig> get_graph_config();
};

#endif
