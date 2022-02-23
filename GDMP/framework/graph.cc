#include "graph.h"

#include <map>
#include <string>

#include "Array.hpp"
#include "File.hpp"
#include "Variant.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

using namespace godot;

void Graph::_register_methods() {
	register_method("initialize", &Graph::initialize);
	register_method("add_packet_callback", &Graph::add_packet_callback);
	register_method("start", &Graph::start);
	register_method("add_packet", &Graph::add_packet);
	register_method("stop", &Graph::stop);
#if !MEDIAPIPE_DISABLE_GPU
	register_method("get_gpu_helper", &Graph::get_gpu_helper);
#endif
}

void Graph::_init() {
	graph = nullptr;
	started = false;
}

void Graph::initialize(String graph_path, bool as_text) {
	absl::Status result = [this, &graph_path, &as_text]() -> absl::Status {
		graph = std::make_unique<mediapipe::CalculatorGraph>();
		Ref<File> file = Ref<File>(File::_new());
		Error err;
		err = file->open(graph_path, File::READ);
		if (err != Error::OK) {
			return absl::UnknownError(String("Failed to open graph file: {0}").format(Array::make((int)err)).alloc_c_string());
		}
		mediapipe::CalculatorGraphConfig config;
		bool result;
		if (as_text) {
			result = mediapipe::ParseTextProto<mediapipe::CalculatorGraphConfig>(file->get_as_text().alloc_c_string(), &config);
		} else {
			result = config.ParseFromArray(file->get_buffer(file->get_len()).read().ptr(), file->get_len());
		}
		file->close();
		if (!result) {
			return absl::InvalidArgumentError("Failed to parse graph config.");
		}
		MP_RETURN_IF_ERROR(graph->Initialize(config));
#if !MEDIAPIPE_DISABLE_GPU
#ifdef __ANDROID__
		JNIEnv *env = android_api->godot_android_get_env();
		jclass egl_manager_class = env->FindClass("com/google/mediapipe/glutil/EglManager");
		jmethodID egl_manager_cnstr = env->GetMethodID(egl_manager_class, "<init>", "(Ljava/lang/Object;)V");
		egl_manager = env->NewObject(egl_manager_class, egl_manager_cnstr, nullptr);
		egl_manager = env->NewGlobalRef(egl_manager);
		EGLContext context = reinterpret_cast<EGLContext>(
				env->CallLongMethod(egl_manager, env->GetMethodID(egl_manager_class, "getNativeContext", "()J")));
		env->DeleteLocalRef(egl_manager_class);
#else
		EGLContext context = mediapipe::kPlatformGlContextNone;
#endif
		ASSIGN_OR_RETURN(auto gpu_resources, mediapipe::GpuResources::Create(context));
		MP_RETURN_IF_ERROR(graph->SetGpuResources(std::move(gpu_resources)));
#endif
		return absl::OkStatus();
	}();
	if (!result.ok()) {
		graph = nullptr;
		Godot::print(result.message().data());
	}
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
		return graph->AddPacketToInputStream(stream_name.alloc_c_string(), packet->get_packet());
	}();
	if (!result.ok()) {
		Godot::print(result.message().data());
	}
}

void Graph::stop() {
	absl::Status result = [this]() -> absl::Status {
		if (!graph) {
			return absl::OkStatus();
		}
		if (started) {
			MP_RETURN_IF_ERROR(graph->CloseAllPacketSources());
			MP_RETURN_IF_ERROR(graph->WaitUntilIdle());
		}
		started = false;
		return absl::OkStatus();
	}();
	if (!result.ok()) {
		Godot::print(result.message().data());
	}
}

#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GlContext> Graph::get_gl_context() {
	return graph->GetGpuResources()->gl_context();
}

Ref<GPUHelper> Graph::get_gpu_helper() {
	if (!graph) {
		Godot::print("Graph has not initialized.");
		return Ref<GPUHelper>();
	}
	std::shared_ptr<mediapipe::GpuResources> gpu_resources = graph->GetGpuResources();
	if (gpu_resources) {
		return Ref<GPUHelper>(GPUHelper::_new(gpu_resources.get()));
	} else {
		Godot::print("Graph GPU helper not available.");
		return Ref<GPUHelper>();
	}
}
#endif
