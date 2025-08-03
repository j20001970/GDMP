#include "gdextension_interface.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

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
#include "GDMP/tasks/processors/classifier_options.h"
#include "GDMP/tasks/processors/embedder_options.h"
#include "GDMP/tasks/task.h"
#include "GDMP/tasks/task_runner.h"
#include "GDMP/util/resource_provider.h"

using namespace godot;

void initialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Protobuf Message
	ClassDB::register_class<MediaPipeProto>();
	// Framework
	ClassDB::register_class<MediaPipeGraph>();
	ClassDB::register_class<MediaPipeGraphBuilder>();
	ClassDB::register_class<MediaPipeGraphConfig>();
	ClassDB::register_class<MediaPipeGraphNode>();
	ClassDB::register_class<MediaPipeImage>();
	ClassDB::register_class<MediaPipeNodeDestination>();
	ClassDB::register_class<MediaPipeNodeSource>();
	ClassDB::register_class<MediaPipePacket>();
	// GPU
	ClassDB::register_class<MediaPipeGPUHelper>();
	ClassDB::register_class<MediaPipeGPUResources>();
	// I/O
	ClassDB::register_class<MediaPipeCameraHelper>();
	// ResourceProvider
	util::set_resource_provider();
	// Tasks
	MediaPipeTask::register_tasks();
	ClassDB::register_class<MediaPipeTaskRunner>();
	// Task containers
	ClassDB::register_class<MediaPipeCategory>();
	ClassDB::register_class<MediaPipeClassifications>();
	ClassDB::register_class<MediaPipeClassificationResult>();
	ClassDB::register_class<MediaPipeDetection>();
	ClassDB::register_class<MediaPipeDetectionResult>();
	ClassDB::register_class<MediaPipeEmbedding>();
	ClassDB::register_class<MediaPipeEmbeddingResult>();
	ClassDB::register_class<MediaPipeNormalizedKeypoint>();
	ClassDB::register_class<MediaPipeLandmark>();
	ClassDB::register_class<MediaPipeLandmarks>();
	ClassDB::register_class<MediaPipeNormalizedLandmark>();
	ClassDB::register_class<MediaPipeNormalizedLandmarks>();
	// Task processors
	ClassDB::register_class<MediaPipeClassifierOptions>();
	ClassDB::register_class<MediaPipeEmbedderOptions>();
}

void uninitialize_mediapipe_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;
}

extern "C" GDExtensionBool GDE_EXPORT mediapipe_library_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_mediapipe_module);
	init_obj.register_terminator(uninitialize_mediapipe_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
	return init_obj.init();
}
