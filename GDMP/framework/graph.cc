#include "graph.h"

#include <string>
#include <thread>

#include "Array.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/tool/sink.h"

void Graph::_register_methods() {
	register_method("initialize", &Graph::initialize);
	register_method("is_initialized", &Graph::is_initialized);
	register_method("is_running", &Graph::is_running);
	register_method("has_input_stream", &Graph::has_input_stream);
	register_method("has_output_stream", &Graph::has_output_stream);
	register_method("add_packet_callback", &Graph::add_packet_callback);
	register_method("start", &Graph::start);
	register_method("add_packet", &Graph::add_packet);
	register_method("stop", &Graph::stop);
	register_method("set_gpu_resources", &Graph::set_gpu_resources);
}

void Graph::_init() {}

void Graph::initialize(Ref<GraphConfig> config) {
	graph_config = nullptr;
	packet_callbacks.clear();
	graph_config = std::make_unique<mediapipe::CalculatorGraphConfig>(config->get_config());
}

bool Graph::is_initialized() {
	return graph_config != nullptr;
}

bool Graph::is_running() {
	return running_graph != nullptr;
}

bool Graph::has_input_stream(String stream_name) {
	ERR_FAIL_COND_V(!is_initialized(), false);
	for (std::string input : graph_config->input_stream()) {
		if (String(input.data()) == stream_name) {
			return true;
		}
	}
	return false;
}

bool Graph::has_output_stream(String stream_name) {
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

void Graph::add_packet_callback(String stream_name, Object *object, String method) {
	ERR_FAIL_COND(!is_initialized());
	ERR_FAIL_COND(!has_output_stream(stream_name));
	std::string side_packet_name;
	mediapipe::tool::AddCallbackCalculator(stream_name.alloc_c_string(), graph_config.get(), &side_packet_name, true);
	packet_callbacks.emplace(
			side_packet_name,
			mediapipe::MakePacket<std::function<void(const mediapipe::Packet &)>>(
					[this, stream_name, object, method](const mediapipe::Packet &packet) -> void {
						if (object == nullptr) {
							return;
						}
						Ref<Packet> p = Packet::_new(packet);
						object->call_deferred(method, Array::make(stream_name, p));
					}));
}

void Graph::start(Dictionary side_packets) {
	ERR_FAIL_COND(!is_initialized());
	stop();
	std::map<std::string, mediapipe::Packet> packets;
	for (int i = 0; i < side_packets.keys().size(); i++) {
		if (side_packets.keys()[i].get_type() == Variant::Type::STRING) {
			String key = side_packets.keys()[i];
			Variant value = side_packets[key];
			switch (value.get_type()) {
				case Variant::Type::BOOL: {
					packets[key.alloc_c_string()] = mediapipe::MakePacket<bool>(static_cast<bool>(value));
					break;
				}
				case Variant::Type::INT: {
					packets[key.alloc_c_string()] = mediapipe::MakePacket<int>(static_cast<int>(value));
					break;
				}
				case Variant::Type::STRING: {
					String str = value;
					packets[key.alloc_c_string()] = mediapipe::MakePacket<std::string>(str.alloc_c_string());
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
		if (gpu_resources != nullptr)
			MP_RETURN_IF_ERROR(running_graph->SetGpuResources(gpu_resources));
#endif
		MP_RETURN_IF_ERROR(running_graph->StartRun(packets));
		return absl::OkStatus();
	}();
	ERR_FAIL_COND_V(!start_graph.ok(), ERR_PRINT(start_graph.ToString().data()));
}

void Graph::add_packet(String stream_name, Ref<Packet> packet) {
	ERR_FAIL_COND(!is_running());
	ERR_FAIL_COND(packet.is_null());
	absl::Status add_packet = running_graph->AddPacketToInputStream(stream_name.alloc_c_string(), packet->get_packet());
	ERR_FAIL_COND_V(!add_packet.ok(), ERR_PRINT(add_packet.ToString().data()));
}

void Graph::stop() {
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

void Graph::set_gpu_resources(Ref<GPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	if (gpu_resources.is_null()) {
		this->gpu_resources = nullptr;
		return;
	}
	this->gpu_resources = gpu_resources->get_gpu_resources();
#endif
}
