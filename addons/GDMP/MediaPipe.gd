extends Node

enum {
	CAMERA_FACING_FRONT,
	CAMERA_FACING_BACK
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
