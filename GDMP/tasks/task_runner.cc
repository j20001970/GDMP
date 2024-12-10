#include "task_runner.h"

#include "Array.hpp"
#include "Variant.hpp"

#include "mediapipe/tasks/cc/core/mediapipe_builtin_op_resolver.h"

#include "GDMP/framework/packet.h"
#include "GDMP/util/packet_map.h"

void MediaPipeTaskRunner::_register_methods() {
	register_method("initialize", &MediaPipeTaskRunner::initialize);
	register_method("process", &MediaPipeTaskRunner::process);
	register_method("send", &MediaPipeTaskRunner::send);
	register_method("close", &MediaPipeTaskRunner::close);
	register_method("restart", &MediaPipeTaskRunner::restart);
	register_method("get_graph_config", &MediaPipeTaskRunner::get_graph_config);
}

void MediaPipeTaskRunner::_init() {}

bool MediaPipeTaskRunner::initialize(Ref<MediaPipeGraphConfig> config, Object *object, String method) {
	PacketsCallback packets_callback = nullptr;
	if (godot_is_instance_valid(object))
		packets_callback = [this, object, method](auto result) -> void {
			Dictionary outputs;
			if (result.ok())
				outputs = packet_map_to_dict(result.value());
			else
				ERR_PRINT(result.status().message().data());
			object->call_deferred(method, outputs);
		};
	auto op_resolver = std::make_unique<MediaPipeBuiltinOpResolver>();
	auto create_task = TaskRunner::Create(config->get_config(), std::move(op_resolver), packets_callback);
	if (create_task.ok())
		task_runner = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return true;
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
	return Ref(MediaPipeGraphConfig::_new(task_runner->GetGraphConfig()));
}
