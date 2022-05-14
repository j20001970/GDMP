#include "graph.h"

#include <map>
#include <string>
#include <thread>

#include "Array.hpp"
#include "File.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/resource_util.h"

using namespace godot;

void Graph::_register_methods() {
	register_method("initialize", &Graph::initialize);
	register_method("is_initialized", &Graph::is_initialized);
	register_method("has_input_stream", &Graph::has_input_stream);
	register_method("add_packet_callback", &Graph::add_packet_callback);
	register_method("start", &Graph::start);
	register_method("add_packet", &Graph::add_packet);
	register_method("stop", &Graph::stop);
}

void Graph::_init() {
	graph = nullptr;
	started = false;
}

void Graph::initialize(String graph_path, bool as_text) {
	absl::Status result = [this, &graph_path, &as_text]() -> absl::Status {
		graph = std::make_unique<mediapipe::CalculatorGraph>();
		std::string graph_contents;
		MP_RETURN_IF_ERROR(mediapipe::GetResourceContents(graph_path.alloc_c_string(), &graph_contents, !as_text));
		mediapipe::CalculatorGraphConfig config;
		bool result;
		if (as_text) {
			result = mediapipe::ParseTextProto(graph_contents, &config);
		} else {
			result = config.ParseFromArray(graph_contents.data(), graph_contents.size());
		}
		if (!result) {
			return absl::InvalidArgumentError("Failed to parse graph config.");
		}
		MP_RETURN_IF_ERROR(graph->Initialize(config));
#if !MEDIAPIPE_DISABLE_GPU
		ASSIGN_OR_RETURN(auto gpu_resources, mediapipe::GpuResources::Create());
		MP_RETURN_IF_ERROR(graph->SetGpuResources(std::move(gpu_resources)));
#endif
		return absl::OkStatus();
	}();
	if (!result.ok()) {
		graph = nullptr;
		Godot::print(result.message().data());
	}
}

bool Graph::is_initialized() {
	return graph != nullptr;
}

bool Graph::has_input_stream(String stream_name) {
	if (!graph) {
		Godot::print("Graph has not initialized.");
		return false;
	}
	return graph->HasInputStream(stream_name.alloc_c_string());
}

void Graph::add_packet_callback(String stream_name, Object *object, String method) {
	absl::Status result = [this, &stream_name, &object, &method]() -> absl::Status {
		if (!graph) {
			return absl::FailedPreconditionError("Graph has not initialized.");
		}
		return graph->ObserveOutputStream(stream_name.alloc_c_string(), [this, stream_name, object, method](mediapipe::Packet packet) -> absl::Status {
			if (object == nullptr) {
				return absl::OkStatus();
			}
			Ref<Packet> p = Packet::_new(packet);
			object->call_deferred(method, Array::make(stream_name, p));
			return absl::OkStatus();
		});
	}();
	if (!result.ok()) {
		Godot::print(result.message().data());
	}
}

void Graph::start(Dictionary side_packets) {
	absl::Status result = [this, &side_packets]() -> absl::Status {
		if (!graph) {
			return absl::FailedPreconditionError("Graph has not initialized.");
		}
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
		MP_RETURN_IF_ERROR(graph->StartRun(packets));
		started = true;
		return absl::OkStatus();
	}();
	if (!result.ok()) {
		Godot::print(result.message().data());
	}
}

void Graph::add_packet(String stream_name, Ref<Packet> packet) {
	absl::Status result = [this, stream_name, &packet]() -> absl::Status {
		if (!graph) {
			return absl::FailedPreconditionError("Graph has not initialized.");
		}
		if (packet.is_null()) {
			return absl::InvalidArgumentError("Packet is null.");
		}
		return graph->AddPacketToInputStream(stream_name.alloc_c_string(), packet->get_packet());
	}();
	if (!result.ok()) {
		Godot::print(result.message().data());
	}
}

void Graph::stop() {
	// Has to stop the graph in a thread to prevent break rendering.
	std::thread([this]() -> void {
		absl::Status result = [this]() -> absl::Status {
			if (!graph) {
				return absl::OkStatus();
			}
			if (started) {
				MP_RETURN_IF_ERROR(graph->CloseAllPacketSources());
				MP_RETURN_IF_ERROR(graph->WaitUntilDone());
				graph.reset(nullptr);
			}
			started = false;
			return absl::OkStatus();
		}();
		if (!result.ok()) {
			Godot::print(result.message().data());
		}
	}).join();
}

#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GpuResources> Graph::get_gpu_resources() {
	if (!graph) {
		Godot::print("Graph has not initialized.");
		return nullptr;
	}
	return graph->GetGpuResources();
}
#endif
