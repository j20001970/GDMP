#include "task_runner.h"

#include "godot_cpp/core/class_db.hpp"

#include "mediapipe/tasks/cc/core/mediapipe_builtin_op_resolver.h"

#include "GDMP/util/packet_map.h"

void MediaPipeTaskRunner::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "config", "async", "input_side_packets", "gpu_resources"),
			&MediaPipeTaskRunner::initialize, DEFVAL(false), DEFVAL(Dictionary()), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("process", "inputs"), &MediaPipeTaskRunner::process);
	ClassDB::bind_method(D_METHOD("send", "inputs"), &MediaPipeTaskRunner::send);
	ClassDB::bind_method(D_METHOD("close"), &MediaPipeTaskRunner::close);
	ClassDB::bind_method(D_METHOD("restart"), &MediaPipeTaskRunner::restart);
	ClassDB::bind_method(D_METHOD("get_graph_config"), &MediaPipeTaskRunner::get_graph_config);
	ADD_SIGNAL(MethodInfo("error", PropertyInfo(Variant::STRING, "message")));
	ADD_SIGNAL(MethodInfo("packets_callback", PropertyInfo(Variant::DICTIONARY, "packets")));
}

bool MediaPipeTaskRunner::initialize(Ref<MediaPipeGraphConfig> config, bool async, Dictionary input_side_packets, Ref<MediaPipeGPUResources> gpu_resources) {
	PacketsCallback packets_callback = nullptr;
	if (async)
		packets_callback = [this](absl::StatusOr<PacketMap> result) -> void {
			Dictionary outputs;
			if (result.ok())
				outputs = util::packet_map_to_dict(result.value());
			else
				ERR_PRINT(result.status().message().data());
			call_deferred("emit_signal", "packets_callback", outputs);
		};
	auto op_resolver = std::make_unique<MediaPipeBuiltinOpResolver>();
	PacketMap side_packets = {};
	if (input_side_packets.size() > 0)
		side_packets = util::dict_to_packet_map(input_side_packets);
#if !MEDIAPIPE_DISABLE_GPU
	std::shared_ptr<mediapipe::GpuResources> gpu = nullptr;
#endif
	if (gpu_resources.is_valid())
#if !MEDIAPIPE_DISABLE_GPU
		gpu = gpu_resources->get_gpu_resources();
#else
		ERR_PRINT("GPU support is disabled in this build.");
#endif
	ErrorFn error_fn = [this](absl::Status status) -> void {
		call_deferred("emit_signal", "error", status.message().data());
	};
#if !MEDIAPIPE_DISABLE_GPU
	auto create_task = TaskRunner::Create(config->get_config(), std::move(op_resolver), packets_callback, nullptr, side_packets, gpu, error_fn);
#else
	auto create_task = TaskRunner::Create(config->get_config(), std::move(op_resolver), packets_callback, nullptr, side_packets, error_fn);
#endif
	if (create_task.ok())
		task_runner = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Dictionary MediaPipeTaskRunner::process(Dictionary inputs) {
	Dictionary outputs;
	ERR_FAIL_COND_V(task_runner == nullptr, outputs);
	PacketMap packet_map = util::dict_to_packet_map(inputs);
	absl::StatusOr<PacketMap> result = task_runner->Process(packet_map);
	if (result.ok())
		outputs = util::packet_map_to_dict(result.value());
	else
		ERR_PRINT(result.status().message().data());
	return outputs;
}

bool MediaPipeTaskRunner::send(Dictionary inputs) {
	ERR_FAIL_COND_V(task_runner == nullptr, false);
	PacketMap packet_map = util::dict_to_packet_map(inputs);
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
