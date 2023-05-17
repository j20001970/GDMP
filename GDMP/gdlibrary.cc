#include "gdnative/gdnative.h"

#include "Godot.hpp"
#include "GodotGlobal.hpp"

#include "GDMP/framework/graph.h"
#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/image.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_helper.h"
#include "GDMP/gpu/gpu_resources.h"
#include "GDMP/io/camera_helper.h"
#include "GDMP/proto/classification.h"
#include "GDMP/proto/detection.h"
#include "GDMP/proto/landmark.h"
#include "GDMP/proto/location_data.h"
#include "GDMP/proto/rect.h"
#include "GDMP/proto/proto.h"
#include "GDMP/util/resource_util.h"

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
	register_class<MediaPipeGraph>();
	register_class<MediaPipeGraphConfig>();
	register_class<MediaPipeImage>();
	register_class<MediaPipePacket>();
	register_class<MediaPipeProto>();
	// GPU
	register_class<MediaPipeGPUHelper>();
	register_class<MediaPipeGPUResources>();
	// I/O
	register_class<MediaPipeCameraHelper>();
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
	// ResourceProvider
	set_resource_provider();
}
