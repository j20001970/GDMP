#include "gdextension_interface.h"

#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "GDMP/framework/graph.h"
#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/image.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_helper.h"
#include "GDMP/gpu/gpu_resources.h"
#include "GDMP/io/camera_helper.h"
#include "GDMP/proto/proto.h"
#include "GDMP/tasks/task.h"
#include "GDMP/util/resource_util.h"

using namespace godot;

void initialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Framework
	ClassDB::register_class<MediaPipeGraph>();
	ClassDB::register_class<MediaPipeGraphConfig>();
	ClassDB::register_class<MediaPipeImage>();
	ClassDB::register_class<MediaPipePacket>();
	ClassDB::register_class<MediaPipeProto>();
	// GPU
	ClassDB::register_class<MediaPipeGPUHelper>();
	ClassDB::register_class<MediaPipeGPUResources>();
	// I/O
	ClassDB::register_class<MediaPipeCameraHelper>();
	// ResourceFormatLoader
	ClassDB::register_class<ResourceFormatLoaderMediaPipeGraphConfig>();
	ResourceFormatLoaderMediaPipeGraphConfig::register_loader();
	// ResourceProvider
	set_resource_provider();
	// Tasks
	ClassDB::register_class<MediaPipeTask>(true);
	MediaPipeTask::register_tasks();
	ClassDB::register_class<MediaPipeTaskBaseOptions>();
}

void uninitialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	ResourceFormatLoaderMediaPipeGraphConfig::unregister_loader();
}

extern "C" GDExtensionBool GDE_EXPORT mediapipe_library_init(
		const GDExtensionInterface *p_interface,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);
	init_obj.register_initializer(initialize_mediapipe_module);
	init_obj.register_terminator(uninitialize_mediapipe_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
	return init_obj.init();
}
