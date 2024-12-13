#include "task_runner.h"

#include "godot_cpp/core/class_db.hpp"

#include "mediapipe/tasks/cc/core/mediapipe_builtin_op_resolver.h"

#include "GDMP/util/packet_map.h"

void MediaPipeTaskRunner::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "config", "callback"), &MediaPipeTaskRunner::initialize, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("process", "inputs"), &MediaPipeTaskRunner::process);
	ClassDB::bind_method(D_METHOD("send", "inputs"), &MediaPipeTaskRunner::send);
	ClassDB::bind_method(D_METHOD("close"), &MediaPipeTaskRunner::close);
	ClassDB::bind_method(D_METHOD("restart"), &MediaPipeTaskRunner::restart);
	ClassDB::bind_method(D_METHOD("get_graph_config"), &MediaPipeTaskRunner::get_graph_config);
}

bool MediaPipeTaskRunner::initialize(Ref<MediaPipeGraphConfig> config, Callable callback) {
	PacketsCallback packets_callback = nullptr;
	if (callback.is_valid())
		packets_callback = [this, callback](auto result) -> void {
			Dictionary outputs;
			if (result.ok())
				outputs = packet_map_to_dict(result.value());
			else
				ERR_PRINT(result.status().message().data());
			callback.call_deferred(outputs);
		};
	auto op_resolver = std::make_unique<MediaPipeBuiltinOpResolver>();
	auto create_task = TaskRunner::Create(config->get_config(), std::move(op_resolver), packets_callback);
	if (create_task.ok())
		task_runner = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Dictionary MediaPipeTaskRunner::process(Dictionary inputs) {
	Dictionary outputs;
	ERR_FAIL_COND_V(task_runner == nullptr, outputs);
	PacketMap packet_map = dict_to_packet_map(inputs);
	absl::StatusOr<PacketMap> result = task_runner->Process(packet_map);
	if (result.ok())
		outputs = packet_map_to_dict(result.value());
	else
		ERR_PRINT(result.status().message().data());
	return outputs;
}

bool MediaPipeTaskRunner::send(Dictionary inputs) {
	ERR_FAIL_COND_V(task_runner == nullptr, false);
	PacketMap packet_map = dict_to_packet_map(inputs);
	absl::Status result = task_runner->Send(packet_map);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeTaskRunner::close() {
	ERR_FAIL_COND_V(task_runner == nullptr, false);
	absl::Status result = task_runner->Close();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeTaskRunner::restart() {
	ERR_FAIL_COND_V(task_runner == nullptr, false);
	absl::Status result = task_runner->Restart();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

Ref<MediaPipeGraphConfig> MediaPipeTaskRunner::get_graph_config() {
	ERR_FAIL_COND_V(task_runner == nullptr, nullptr);
	return memnew(MediaPipeGraphConfig(task_runner->GetGraphConfig()));
}
