#ifndef GDMP_TASK_RUNNER
#define GDMP_TASK_RUNNER

#include "Dictionary.hpp"
#include "Object.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/core/task_runner.h"

#include "GDMP/framework/graph_config.h"

using namespace godot;
using namespace mediapipe::tasks::core;

class MediaPipeTaskRunner : public Reference {
		GODOT_CLASS(MediaPipeTaskRunner, Reference)

	private:
		static PacketMap dict_to_packet_map(Dictionary dict);
		static Dictionary packet_map_to_dict(PacketMap &packet_map);

		std::unique_ptr<TaskRunner> task_runner;

	public:
		static void _register_methods();

		void _init();

		// Initialize the task runner with graph config, optionally with callback for asynchronous result.
		bool initialize(Ref<MediaPipeGraphConfig> config, Object *object, String method);

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
