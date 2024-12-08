#include "gdnative/gdnative.h"

#include "Godot.hpp"
#include "GodotGlobal.hpp"

#include "GDMP/framework/graph.h"
#include "GDMP/framework/graph_builder.h"
#include "GDMP/framework/graph_config.h"
#include "GDMP/framework/graph_node.h"
#include "GDMP/framework/image.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_helper.h"
#include "GDMP/gpu/gpu_resources.h"
#include "GDMP/io/camera_helper.h"
#include "GDMP/proto/proto.h"
#include "GDMP/tasks/containers/category.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/detection_result.h"
#include "GDMP/tasks/containers/embedding_result.h"
#include "GDMP/tasks/containers/keypoint.h"
#include "GDMP/tasks/containers/landmark.h"
#include "GDMP/tasks/task.h"
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
	// Protobuf Message
	register_class<MediaPipeProto>();
	// Framework
	register_class<MediaPipeGraph>();
	register_class<MediaPipeGraphBuilder>();
	register_class<MediaPipeGraphConfig>();
	register_class<MediaPipeGraphNode>();
	register_class<MediaPipeImage>();
	register_class<MediaPipePacket>();
	// GPU
	register_class<MediaPipeGPUHelper>();
	register_class<MediaPipeGPUResources>();
	// I/O
	register_class<MediaPipeCameraHelper>();
	// ResourceProvider
	set_resource_provider();
	// Tasks
	MediaPipeTask::register_tasks();
	register_class<MediaPipeTaskBaseOptions>();
	register_class<MediaPipeCategory>();
	register_class<MediaPipeClassifications>();
	register_class<MediaPipeClassificationResult>();
	register_class<MediaPipeDetection>();
	register_class<MediaPipeDetectionResult>();
	register_class<MediaPipeEmbedding>();
	register_class<MediaPipeEmbeddingResult>();
	register_class<MediaPipeNormalizedKeypoint>();
	register_class<MediaPipeLandmark>();
	register_class<MediaPipeLandmarks>();
	register_class<MediaPipeNormalizedLandmark>();
	register_class<MediaPipeNormalizedLandmarks>();
}
