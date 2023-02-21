#include "gdextension_interface.h"

#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/godot.hpp"

#include "framework/graph.h"
#include "framework/packet.h"
#include "io/camera_helper.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "framework/gpu_helper.h"
#endif

#include "proto/classification.h"
#include "proto/detection.h"
#include "proto/landmark.h"
#include "proto/location_data.h"
#include "proto/rect.h"

using namespace godot;

void initialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Framework
	ClassDB::register_class<Graph>();
	ClassDB::register_class<Packet>();
#if !MEDIAPIPE_DISABLE_GPU
	ClassDB::register_class<GPUHelper>();
#endif
	// I/O
	ClassDB::register_class<CameraHelper>();
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
}

void uninitialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
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
