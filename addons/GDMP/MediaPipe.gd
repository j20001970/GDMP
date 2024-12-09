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
const GraphBuilder: NativeScript = preload("framework/MediaPipeGraphBuilder.gdns")
const GraphConfig: NativeScript = preload("framework/MediaPipeGraphConfig.gdns")
const GraphNode: NativeScript = preload("framework/MediaPipeGraphNode.gdns")
const Image: NativeScript = preload("framework/MediaPipeImage.gdns")
const Packet: NativeScript = preload("framework/MediaPipePacket.gdns")
const Proto: NativeScript = preload("res://addons/GDMP/framework/MediaPipeProto.gdns")

# GPU
const GPUHelper: NativeScript = preload("gpu/MediaPipeGPUHelper.gdns")
const GPUResources: NativeScript = preload("gpu/MediaPipeGPUResources.gdns")

# I/O
const CameraHelper: NativeScript = preload("io/MediaPipeCameraHelper.gdns")

# Tasks
const MediaPipeTaskRunner: NativeScript = preload("tasks/MediaPipeTaskRunner.gdns")
const TaskBaseOptions: NativeScript = preload("tasks/MediaPipeTaskBaseOptions.gdns")

# Vision Tasks
const FaceDetector: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeFaceDetector.gdns")
const FaceLandmarker: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeFaceLandmarker.gdns")
const GestureRecognizer: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeGestureRecognizer.gdns")
const HandLandmarker: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeHandLandmarker.gdns")
const ImageClassifier: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeImageClassifier.gdns")
const ImageEmbedder: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeImageEmbedder.gdns")
const ImageSegmenter: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeImageSegmenter.gdns")
const ObjectDetector: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipeObjectDetector.gdns")
const PoseLandmarker: NativeScript = preload("res://addons/GDMP/tasks/vision/MediaPipePoseLandmarker.gdns")
