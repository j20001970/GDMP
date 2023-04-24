#include "graph.h"

#include <string>
#include <thread>

#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/tool/sink.h"

using namespace godot;

void MediaPipeGraph::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize"), &MediaPipeGraph::initialize);
	ClassDB::bind_method(D_METHOD("is_initialized"), &MediaPipeGraph::is_initialized);
	ClassDB::bind_method(D_METHOD("is_running"), &MediaPipeGraph::is_running);
	ClassDB::bind_method(D_METHOD("has_input_stream"), &MediaPipeGraph::has_input_stream);
	ClassDB::bind_method(D_METHOD("has_output_stream"), &MediaPipeGraph::has_output_stream);
	ClassDB::bind_method(D_METHOD("add_packet_callback"), &MediaPipeGraph::add_packet_callback);
	ClassDB::bind_method(D_METHOD("start"), &MediaPipeGraph::start);
	ClassDB::bind_method(D_METHOD("add_packet"), &MediaPipeGraph::add_packet);
	ClassDB::bind_method(D_METHOD("stop"), &MediaPipeGraph::stop);
	ClassDB::bind_method(D_METHOD("set_gpu_resources"), &MediaPipeGraph::set_gpu_resources);
}

MediaPipeGraph::MediaPipeGraph() = default;

MediaPipeGraph::~MediaPipeGraph() {
	stop();
}

void MediaPipeGraph::initialize(Ref<MediaPipeGraphConfig> config) {
	graph_config = nullptr;
	packet_callbacks.clear();
	graph_config = std::make_unique<mediapipe::CalculatorGraphConfig>(config->get_config());
}

bool MediaPipeGraph::is_initialized() {
	return graph_config != nullptr;
}

bool MediaPipeGraph::is_running() {
	return running_graph != nullptr;
}

bool MediaPipeGraph::has_input_stream(String stream_name) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	for (std::string input : graph_config->input_stream()) {
		if (String(input.data()) == stream_name) {
			return true;
		}
	}
	return false;
}

bool MediaPipeGraph::has_output_stream(String stream_name) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	for (std::string output : graph_config->output_stream()) {
		if (String(output.data()) == stream_name) {
			return true;
		}
	}
	for (auto node : graph_config->node()) {
		for (auto output : node.output_stream()) {
			if (String(output.data()) == stream_name) {
				return true;
			}
		}
	}
	return false;
}

void MediaPipeGraph::add_packet_callback(String stream_name, Callable callback) {
	ERR_FAIL_COND(!is_initialized());
	ERR_FAIL_COND(!has_output_stream(stream_name));
	std::string side_packet_name;
	mediapipe::tool::AddCallbackCalculator(stream_name.utf8().get_data(), graph_config.get(), &side_packet_name, true);
	packet_callbacks.emplace(
			side_packet_name,
			mediapipe::MakePacket<std::function<void(const mediapipe::Packet &)>>(
					[this, stream_name, callback](const mediapipe::Packet &packet) -> void {
						Ref<MediaPipePacket> p = memnew(MediaPipePacket(packet));
						callback.callv(Array::make(stream_name, p));
					}));
}

void MediaPipeGraph::start(Dictionary side_packets) {
	ERR_FAIL_COND(!is_initialized());
	stop();
	std::map<std::string, mediapipe::Packet> packets;
	for (int i = 0; i < side_packets.keys().size(); i++) {
		if (side_packets.keys()[i].get_type() == Variant::Type::STRING) {
			String key = side_packets.keys()[i];
			Variant value = side_packets[key];
			switch (value.get_type()) {
				case Variant::Type::BOOL: {
					packets[key.utf8().get_data()] = mediapipe::MakePacket<bool>(static_cast<bool>(value));
					break;
				}
				case Variant::Type::INT: {
					packets[key.utf8().get_data()] = mediapipe::MakePacket<int>(static_cast<int>(value));
					break;
				}
				case Variant::Type::STRING: {
					String str = value;
					packets[key.utf8().get_data()] = mediapipe::MakePacket<std::string>(str.utf8().get_data());
					break;
				}
				default:
					break;
			}
		}
	}
	absl::Status start_graph = [this, &packets]() -> absl::Status {
		running_graph = std::make_unique<mediapipe::CalculatorGraph>();
		MP_RETURN_IF_ERROR(running_graph->Initialize(*graph_config, packet_callbacks));
#if !MEDIAPIPE_DISABLE_GPU
		if (!gpu_resources.is_null()) {
			MP_RETURN_IF_ERROR(running_graph->SetGpuResources(gpu_resources->get_gpu_resources()));
		}
#endif
		MP_RETURN_IF_ERROR(running_graph->StartRun(packets));
		return absl::OkStatus();
	}();
	ERR_FAIL_COND_V(!start_graph.ok(), ERR_PRINT(start_graph.ToString().data()));
}

void MediaPipeGraph::add_packet(String stream_name, Ref<MediaPipePacket> packet) {
	ERR_FAIL_COND(!is_running());
	ERR_FAIL_COND(packet.is_null());
	absl::Status add_packet = running_graph->AddPacketToInputStream(stream_name.utf8().get_data(), packet->get_packet());
	ERR_FAIL_COND_V(!add_packet.ok(), ERR_PRINT(add_packet.ToString().data()));
}

void MediaPipeGraph::stop() {
	if (!is_running()) {
		return;
	}
	// Has to stop the graph in a thread to prevent break rendering.
	std::thread([this]() -> void {
		absl::Status stop_graph = [this]() -> absl::Status {
			MP_RETURN_IF_ERROR(running_graph->CloseAllPacketSources());
			MP_RETURN_IF_ERROR(running_graph->WaitUntilDone());
			return absl::OkStatus();
		}();
		running_graph = nullptr;
		ERR_FAIL_COND_V(!stop_graph.ok(), ERR_PRINT(stop_graph.ToString().data()));
	}).join();
}

void MediaPipeGraph::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
	this->gpu_resources = gpu_resources;
}

#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GpuResources> MediaPipeGraph::get_gpu_resources() {
	return gpu_resources->get_gpu_resources();
}
#endif
