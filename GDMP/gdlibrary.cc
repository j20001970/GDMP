#include "Godot.hpp"

#include "framework/graph.h"
#include "framework/packet.h"
#include "gdmp.h"
#include "io/camera_helper.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "framework/gpu_helper.h"
#endif

#include "proto/classification.h"
#include "proto/detection.h"
#include "proto/landmark.h"
#include "proto/location_data.h"
#include "proto/rect.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_singleton() {
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);
	godot::register_class<godot::GDMP>();
	godot::register_class<godot::Packet>();
	godot::register_class<godot::Graph>();
	godot::register_class<godot::CameraHelper>();
#if !MEDIAPIPE_DISABLE_GPU
	godot::register_class<godot::GPUHelper>();
#endif
	// Classification
	godot::register_class<godot::Classification>();
	godot::register_class<godot::ClassificationList>();
	godot::register_class<godot::ClassificationListCollection>();
	// Deteciton
	godot::register_class<godot::AssociatedDetection>();
	godot::register_class<godot::Detection>();
	godot::register_class<godot::DetectionList>();
	// Landmark
	godot::register_class<godot::Landmark>();
	godot::register_class<godot::LandmarkList>();
	godot::register_class<godot::LandmarkListCollection>();
	godot::register_class<godot::NormalizedLandmark>();
	godot::register_class<godot::NormalizedLandmarkList>();
	godot::register_class<godot::NormalizedLandmarkListCollection>();
	// LocationData
	godot::register_class<godot::BoundingBox>();
	godot::register_class<godot::RelativeBoundingBox>();
	godot::register_class<godot::BinaryMask>();
	godot::register_class<godot::RelativeKeypoint>();
	godot::register_class<godot::LocationData>();
	// Rasterization
	godot::register_class<godot::Interval>();
	godot::register_class<godot::Rasterization>();
	// Rect
	godot::register_class<godot::Rect>();
	godot::register_class<godot::NormalizedRect>();
}
