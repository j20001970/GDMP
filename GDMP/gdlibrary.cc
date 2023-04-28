#include "gdextension_interface.h"

#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "mediapipe/GDMP/framework/graph.h"
#include "mediapipe/GDMP/framework/graph_config.h"
#include "mediapipe/GDMP/framework/packet.h"
#include "mediapipe/GDMP/gpu/gpu_helper.h"
#include "mediapipe/GDMP/gpu/gpu_resources.h"
#include "mediapipe/GDMP/io/camera_helper.h"
#include "mediapipe/GDMP/proto/classification.h"
#include "mediapipe/GDMP/proto/detection.h"
#include "mediapipe/GDMP/proto/landmark.h"
#include "mediapipe/GDMP/proto/location_data.h"
#include "mediapipe/GDMP/proto/rect.h"

using namespace godot;

static Ref<ResourceFormatLoaderMediaPipeGraphConfig> graph_config_loader;

void initialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Framework
	ClassDB::register_class<MediaPipeGraph>();
	ClassDB::register_class<MediaPipeGraphConfig>();
	ClassDB::register_class<MediaPipePacket>();
	// GPU
	ClassDB::register_class<MediaPipeGPUHelper>();
	ClassDB::register_class<MediaPipeGPUResources>();
	// I/O
	ClassDB::register_class<MediaPipeCameraHelper>();
	// Classification
	ClassDB::register_class<Classification>();
	ClassDB::register_class<ClassificationList>();
	ClassDB::register_class<ClassificationListCollection>();
	// Deteciton
	ClassDB::register_class<AssociatedDetection>();
	ClassDB::register_class<Detection>();
	ClassDB::register_class<DetectionList>();
	// Landmark
	ClassDB::register_class<Landmark>();
	ClassDB::register_class<LandmarkList>();
	ClassDB::register_class<LandmarkListCollection>();
	ClassDB::register_class<NormalizedLandmark>();
	ClassDB::register_class<NormalizedLandmarkList>();
	ClassDB::register_class<NormalizedLandmarkListCollection>();
	// LocationData
	ClassDB::register_class<BoundingBox>();
	ClassDB::register_class<RelativeBoundingBox>();
	ClassDB::register_class<BinaryMask>();
	ClassDB::register_class<RelativeKeypoint>();
	ClassDB::register_class<LocationData>();
	// Rasterization
	ClassDB::register_class<Interval>();
	ClassDB::register_class<Rasterization>();
	// Rect
	ClassDB::register_class<Rect>();
	ClassDB::register_class<NormalizedRect>();
	// ResourceFormatLoader
	ClassDB::register_class<ResourceFormatLoaderMediaPipeGraphConfig>();
	graph_config_loader.instantiate();
	ResourceLoader::get_singleton()->add_resource_format_loader(graph_config_loader);
}

void uninitialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	ResourceLoader::get_singleton()->remove_resource_format_loader(graph_config_loader);
	graph_config_loader.unref();
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
