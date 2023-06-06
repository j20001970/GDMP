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

# Vision Tasks
const FaceDetector: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeFaceDetector.gdns")
const FaceLandmarker: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeFaceLandmarker.gdns")
const HandLandmarker: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeHandLandmarker.gdns")
