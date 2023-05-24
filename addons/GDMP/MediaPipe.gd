extends Node

enum CameraFacing {
	FACING_FRONT,
	FACING_BACK,
}

enum TaskDelegate {
	DELEGATE_CPU,
	DELEGATE_GPU,
	DELEGATE_EDGETPU_NNAPI,
}

enum TaskRunningMode {
	RUNNING_MODE_AUDIO_CLIPS = 1,
	RUNNING_MODE_AUDIO_STREAM = 2,
	RUNNING_MODE_IMAGE = 1,
	RUNNINE_MODE_VIDEO = 2,
	RUNNING_MODE_LIVE_STREAM = 3,
}

# Framework
const Graph: NativeScript = preload("framework/MediaPipeGraph.gdns")
const GraphConfig: NativeScript = preload("framework/MediaPipeGraphConfig.gdns")
const Image: NativeScript = preload("framework/MediaPipeImage.gdns")
const Packet: NativeScript = preload("framework/MediaPipePacket.gdns")
const Proto: NativeScript = preload("res://addons/GDMP/framework/MediaPipeProto.gdns")

# GPU
const GPUHelper: NativeScript = preload("gpu/MediaPipeGPUHelper.gdns")
const GPUResources: NativeScript = preload("gpu/MediaPipeGPUResources.gdns")

# I/O
const CameraHelper: NativeScript = preload("io/MediaPipeCameraHelper.gdns")

# Tasks
const TaskBaseOptions: NativeScript = preload("tasks/MediaPipeTaskBaseOptions.gdns")

# Task Containers
const Category: NativeScript = preload("tasks/containers/MediaPipeCategory.gdns")
const Classifications: NativeScript = preload("tasks/containers/MediaPipeClassifications.gdns")
const ClassificationResult: NativeScript = preload("tasks/containers/MediaPipeClassificationResult.gdns")
const Detection: NativeScript = preload("tasks/containers/MediaPipeDetection.gdns")
const DetectionResult: NativeScript = preload("tasks/containers/MediaPipeDetectionResult.gdns")
const NormalizedKeypoint: NativeScript = preload("tasks/containers/MediaPipeNormalizedKeypoint.gdns")
const Landmark: NativeScript = preload("tasks/containers/MediaPipeLandmark.gdns")
const Landmarks: NativeScript = preload("tasks/containers/MediaPipeLandmarks.gdns")
const NormalizedLandmark: NativeScript = preload("tasks/containers/MediaPipeNormalizedLandmark.gdns")
const NormalizedLandmarks: NativeScript = preload("tasks/containers/MediaPipeNormalizedLandmarks.gdns")
