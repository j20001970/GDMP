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

using namespace godot;

extern "C" void GDN_EXPORT mediapipe_gdnative_init(godot_gdnative_init_options *o) {
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT mediapipe_gdnative_terminate(godot_gdnative_terminate_options *o) {
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT mediapipe_nativescript_init(void *handle) {
	Godot::nativescript_init(handle);
	// Framework
	register_class<Graph>();
	register_class<GraphConfig>();
	register_class<Packet>();
#if !MEDIAPIPE_DISABLE_GPU
	register_class<GPUHelper>();
#endif
	// I/O
	register_class<CameraHelper>();
	// Classification
	register_class<Classification>();
	register_class<ClassificationList>();
	register_class<ClassificationListCollection>();
	// Deteciton
	register_class<AssociatedDetection>();
	register_class<Detection>();
	register_class<DetectionList>();
	// Landmark
	register_class<Landmark>();
	register_class<LandmarkList>();
	register_class<LandmarkListCollection>();
	register_class<NormalizedLandmark>();
	register_class<NormalizedLandmarkList>();
	register_class<NormalizedLandmarkListCollection>();
	// LocationData
	register_class<BoundingBox>();
	register_class<RelativeBoundingBox>();
	register_class<BinaryMask>();
	register_class<RelativeKeypoint>();
	register_class<LocationData>();
	// Rasterization
	register_class<Interval>();
	register_class<Rasterization>();
	// Rect
	register_class<Rect>();
	register_class<NormalizedRect>();
}
