#include "Godot.hpp"

#include "framework/graph.h"
#include "framework/graph_config.h"
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

extern "C" void GDN_EXPORT mediapipe_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT mediapipe_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT mediapipe_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);
	// Framework
	godot::register_class<Graph>();
	godot::register_class<GraphConfig>();
	godot::register_class<Packet>();
#if !MEDIAPIPE_DISABLE_GPU
	godot::register_class<GPUHelper>();
#endif
	// I/O
	godot::register_class<CameraHelper>();
	// Classification
	godot::register_class<Classification>();
	godot::register_class<ClassificationList>();
	godot::register_class<ClassificationListCollection>();
	// Deteciton
	godot::register_class<AssociatedDetection>();
	godot::register_class<Detection>();
	godot::register_class<DetectionList>();
	// Landmark
	godot::register_class<Landmark>();
	godot::register_class<LandmarkList>();
	godot::register_class<LandmarkListCollection>();
	godot::register_class<NormalizedLandmark>();
	godot::register_class<NormalizedLandmarkList>();
	godot::register_class<NormalizedLandmarkListCollection>();
	// LocationData
	godot::register_class<BoundingBox>();
	godot::register_class<RelativeBoundingBox>();
	godot::register_class<BinaryMask>();
	godot::register_class<RelativeKeypoint>();
	godot::register_class<LocationData>();
	// Rasterization
	godot::register_class<Interval>();
	godot::register_class<Rasterization>();
	// Rect
	godot::register_class<Rect>();
	godot::register_class<NormalizedRect>();
}
