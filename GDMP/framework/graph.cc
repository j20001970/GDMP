#include "graph.h"

#include "godot_cpp/core/class_db.hpp"

#include "GDMP/util/packet_map.h"

void MediaPipeGraph::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "config", "side_packets"), &MediaPipeGraph::initialize, DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("get_config"), &MediaPipeGraph::get_config);
	ClassDB::bind_method(D_METHOD("add_output_stream_callback", "stream_name", "callback"), &MediaPipeGraph::add_output_stream_callback);
	ClassDB::bind_method(D_METHOD("get_output_side_packet", "packet_name"), &MediaPipeGraph::get_output_side_packet);
	ClassDB::bind_method(D_METHOD("run", "extra_side_packets"), &MediaPipeGraph::run, DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("start_run", "extra_side_packets"), &MediaPipeGraph::start_run, DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("wait_until_done"), &MediaPipeGraph::wait_until_done);
	ClassDB::bind_method(D_METHOD("wait_until_idle"), &MediaPipeGraph::wait_until_idle);
	ClassDB::bind_method(D_METHOD("has_error"), &MediaPipeGraph::has_error);
	ClassDB::bind_method(D_METHOD("add_packet_to_input_stream", "stream_name", "packet"), &MediaPipeGraph::add_packet_to_input_stream);
	ClassDB::bind_method(D_METHOD("has_input_stream", "stream_name"), &MediaPipeGraph::has_input_stream);
	ClassDB::bind_method(D_METHOD("close_input_stream", "stream_name"), &MediaPipeGraph::close_input_stream);
	ClassDB::bind_method(D_METHOD("close_all_input_streams"), &MediaPipeGraph::close_all_input_streams);
	ClassDB::bind_method(D_METHOD("close_all_packet_sources"), &MediaPipeGraph::close_all_packet_sources);
	ClassDB::bind_method(D_METHOD("cancel"), &MediaPipeGraph::cancel);
	ClassDB::bind_method(D_METHOD("get_gpu_resources"), &MediaPipeGraph::get_gpu_resources);
	ClassDB::bind_method(D_METHOD("set_gpu_resources", "gpu_resources"), &MediaPipeGraph::set_gpu_resources);
	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "gpu_resources", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeGPUResources::get_class_static()),
			"set_gpu_resources", "get_gpu_resources");
	ADD_SIGNAL(MethodInfo("error", PropertyInfo(Variant::STRING, "message")));
}

MediaPipeGraph::MediaPipeGraph() {
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
	std::map<std::string, mediapipe::Packet> packets = dict_to_packet_map(side_packets);
	absl::Status result = graph->Initialize(config->get_config(), packets);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

Ref<MediaPipeGraphConfig> MediaPipeGraph::get_config() {
	return memnew(MediaPipeGraphConfig(graph->Config()));
}

bool MediaPipeGraph::add_output_stream_callback(const String &stream_name, Callable callback) {
	std::function<absl::Status(const mediapipe::Packet &)> packet_callback;
	packet_callback = [this, callback](const mediapipe::Packet &packet) -> absl::Status {
		callback.call_deferred(memnew(MediaPipePacket(packet)));
		return absl::OkStatus();
	};
	absl::Status result = graph->ObserveOutputStream(stream_name.utf8().get_data(), packet_callback);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

Ref<MediaPipePacket> MediaPipeGraph::get_output_side_packet(const String &packet_name) {
	absl::StatusOr<mediapipe::Packet> result = graph->GetOutputSidePacket(packet_name.utf8().get_data());
	if (!result.ok()) {
		ERR_PRINT(result.status().message().data());
		return nullptr;
	}
	return memnew(MediaPipePacket(result.value()));
}

bool MediaPipeGraph::run(Dictionary extra_side_packets) {
	std::map<std::string, mediapipe::Packet> packets = dict_to_packet_map(extra_side_packets);
	absl::Status result = graph->Run(packets);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::start_run(Dictionary extra_side_packets) {
	std::map<std::string, mediapipe::Packet> packets = dict_to_packet_map(extra_side_packets);
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

bool MediaPipeGraph::add_packet_to_input_stream(const String &stream_name, Ref<MediaPipePacket> packet) {
	absl::Status result = graph->AddPacketToInputStream(stream_name.utf8().get_data(), packet->get_packet());
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

bool MediaPipeGraph::has_input_stream(const String &stream_name) {
	return graph->HasInputStream(stream_name.utf8().get_data());
}

bool MediaPipeGraph::close_input_stream(const String &stream_name) {
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
	return memnew(MediaPipeGPUResources(gpu_resources));
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
