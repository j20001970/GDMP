#include "gdextension_interface.h"

#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "GDMP/framework/graph.h"
#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_helper.h"
#include "GDMP/gpu/gpu_resources.h"
#include "GDMP/io/camera_helper.h"
#include "GDMP/proto/classification.h"
#include "GDMP/proto/detection.h"
#include "GDMP/proto/landmark.h"
#include "GDMP/proto/location_data.h"
#include "GDMP/proto/rect.h"
#include "GDMP/util/resource_util.h"

using namespace godot;

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
	ResourceFormatLoaderMediaPipeGraphConfig::register_loader();
	// ResourceProvider
	set_resource_provider();
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
