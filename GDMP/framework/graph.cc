#include "graph.h"

#include "Array.hpp"
#include "Variant.hpp"

#include "GDMP/util/packet_map.h"

void MediaPipeGraph::_register_methods() {
	register_method("initialize", &MediaPipeGraph::initialize);
	register_method("get_config", &MediaPipeGraph::get_config);
	register_method("add_output_stream_callback", &MediaPipeGraph::add_output_stream_callback);
	register_method("get_output_side_packet", &MediaPipeGraph::get_output_side_packet);
	register_method("run", &MediaPipeGraph::run);
	register_method("start_run", &MediaPipeGraph::start_run);
	register_method("wait_until_done", &MediaPipeGraph::wait_until_done);
	register_method("wait_until_idle", &MediaPipeGraph::wait_until_idle);
	register_method("has_error", &MediaPipeGraph::has_error);
	register_method("add_packet_to_input_stream", &MediaPipeGraph::add_packet_to_input_stream);
	register_method("has_input_stream", &MediaPipeGraph::has_input_stream);
	register_method("close_input_stream", &MediaPipeGraph::close_input_stream);
	register_method("close_all_input_streams", &MediaPipeGraph::close_all_input_streams);
	register_method("close_all_packet_sources", &MediaPipeGraph::close_all_packet_sources);
	register_method("cancel", &MediaPipeGraph::cancel);
	register_method("get_gpu_resources", &MediaPipeGraph::get_gpu_resources);
	register_method("set_gpu_resources", &MediaPipeGraph::set_gpu_resources);
	register_property<MediaPipeGraph, Ref<MediaPipeGPUResources>>("gpu_resources", &MediaPipeGraph::set_gpu_resources, &MediaPipeGraph::get_gpu_resources, nullptr);
	register_signal<MediaPipeGraph>("error");
}

void MediaPipeGraph::_init() {
	graph = std::make_unique<mediapipe::CalculatorGraph>();
	std::function<void(const absl::Status &)> error_callback;
	error_callback = [this](const absl::Status status) -> void {
		call_deferred("emit_signal", "error", status.message().data());
	};
	absl::Status set_error_callback = graph->SetErrorCallback(error_callback);
	if (!set_error_callback.ok())
		ERR_PRINT(set_error_callback.message().data());
}

bool MediaPipeGraph::initialize(Ref<MediaPipeGraphConfig> config, Dictionary side_packets) {
	std::map<std::string, mediapipe::Packet> packets = util::dict_to_packet_map(side_packets);
	absl::Status result = graph->Initialize(config->get_config(), packets);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

Ref<MediaPipeGraphConfig> MediaPipeGraph::get_config() {
	return MediaPipeGraphConfig::_new(graph->Config());
}

bool MediaPipeGraph::add_output_stream_callback(const String &stream_name, Object *object, String method) {
	std::function<absl::Status(const mediapipe::Packet &)> packet_callback;
	packet_callback = [this, &object, &method](const mediapipe::Packet &packet) -> absl::Status {
		object->call_deferred(method, MediaPipePacket::_new(packet));
		return absl::OkStatus();
	};
	absl::Status result = graph->ObserveOutputStream(stream_name.utf8().get_data(), packet_callback);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

Ref<MediaPipePacket> MediaPipeGraph::get_output_side_packet(String packet_name) {
	absl::StatusOr<mediapipe::Packet> result = graph->GetOutputSidePacket(packet_name.utf8().get_data());
	if (!result.ok()) {
		ERR_PRINT(result.status().message().data());
		return nullptr;
	}
	return MediaPipePacket::_new(result.value());
}

bool MediaPipeGraph::run(Dictionary extra_side_packets) {
	std::map<std::string, mediapipe::Packet> packets = util::dict_to_packet_map(extra_side_packets);
	absl::Status result = graph->Run(packets);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::start_run(Dictionary extra_side_packets) {
	std::map<std::string, mediapipe::Packet> packets = util::dict_to_packet_map(extra_side_packets);
	absl::Status result = graph->StartRun(packets);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::wait_until_done() {
	absl::Status result = graph->WaitUntilDone();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::wait_until_idle() {
	absl::Status result = graph->WaitUntilIdle();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::has_error() {
	return graph->HasError();
}

bool MediaPipeGraph::add_packet_to_input_stream(String stream_name, Ref<MediaPipePacket> packet) {
	absl::Status result = graph->AddPacketToInputStream(stream_name.utf8().get_data(), packet->get_packet());
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::has_input_stream(String stream_name) {
	return graph->HasInputStream(stream_name.utf8().get_data());
}

bool MediaPipeGraph::close_input_stream(String stream_name) {
	absl::Status result = graph->CloseInputStream(stream_name.utf8().get_data());
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::close_all_input_streams() {
	absl::Status result = graph->CloseAllInputStreams();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::close_all_packet_sources() {
	absl::Status result = graph->CloseAllPacketSources();
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

void MediaPipeGraph::cancel() {
	graph->Cancel();
}

Ref<MediaPipeGPUResources> MediaPipeGraph::get_gpu_resources() {
#if !MEDIAPIPE_DISABLE_GPU
	std::shared_ptr<mediapipe::GpuResources> gpu_resources = graph->GetGpuResources();
	if (gpu_resources == nullptr)
		return nullptr;
	return MediaPipeGPUResources::_new(gpu_resources);
#else
	ERR_PRINT("GPU support is disabled in this build.");
	return nullptr;
#endif
}

void MediaPipeGraph::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	absl::Status result = graph->SetGpuResources(gpu_resources->get_gpu_resources());
	if (!result.ok())
		ERR_PRINT(result.message().data());
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}
