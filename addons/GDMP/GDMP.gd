extends Node

enum {
	CAMERA_FACING_FRONT,
	CAMERA_FACING_BACK
}

# Classification
const Classification : NativeScript = preload("Classification.gdns")
const ClassificationList : NativeScript = preload("ClassificationList.gdns")
const ClassificationListCollection : NativeScript = preload("ClassificationListCollection.gdns")

# Detection
const Detection : NativeScript = preload("Detection.gdns")
const DetectionList : NativeScript = preload("DetectionList.gdns")

# Landmark
const Landmark : NativeScript = preload("Landmark.gdns")
const LandmarkList : NativeScript = preload("LandmarkList.gdns")
const LandmarkListCollection : NativeScript = preload("LandmarkListCollection.gdns")
const NormalizedLandmark : NativeScript = preload("NormalizedLandmark.gdns")
const NormalizedLandmarkList : NativeScript = preload("NormalizedLandmarkList.gdns")
const NormalizedLandmarkListCollection : NativeScript = preload("NormalizedLandmarkListCollection.gdns")

# LocationData
const BoundingBox : NativeScript = preload("BoundingBox.gdns")
const RelativeBoundingBox : NativeScript = preload("RelativeBoundingBox.gdns")
const BinaryMask : NativeScript = preload("BinaryMask.gdns")
const RelativeKeypoint : NativeScript = preload("RelativeKeypoint.gdns")
const LocationData : NativeScript = preload("LocationData.gdns")

# Rasterization
const Interval : NativeScript = preload("Interval.gdns")
const Rasterization : NativeScript = preload("Rasterization.gdns")

# Rect
const Rect : NativeScript = preload("Rect.gdns")
const NormalizedRect : NativeScript = preload("NormalizedRect.gdns")

var plugin : Object
var packet_data : Dictionary = {}

func _enter_tree():
	match OS.get_name():
		"Android":
			if Engine.has_singleton("GDMP"):
				plugin = Engine.get_singleton("GDMP")
		_:
			plugin = Node.new()
			plugin.set_script(load("res://addons/GDMP/GDMP.gdns"))
			add_child(plugin)
	plugin.connect("on_new_proto", self, "_on_new_packet")
	plugin.connect("on_new_proto_vector", self, "_on_new_packet")
	plugin.connect("on_new_frame", self, "_on_new_frame")

func _on_new_packet(stream_name : String, data) -> void:
	if packet_data.has(stream_name):
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], data)

func _on_new_frame(stream_name : String, width : int, height : int, data : PoolByteArray) -> void:
	if packet_data.has(stream_name):
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], width, height, data)

func init_graph(graph_path : String, input_side_packets : Dictionary = {}) -> void:
	packet_data.clear()
	plugin.initGraph(graph_path, input_side_packets)

func add_proto_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		plugin.addProtoCallback(stream_name)
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func add_proto_vector_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		plugin.addProtoVectorCallback(stream_name)
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func add_gpu_frame_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		plugin.addGpuFrameCallback(stream_name)
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func start_camera(index : int = 0, stream_name : String = "input_video") -> void:
	plugin.startCamera(index, stream_name)

func close_camera() -> void:
	plugin.closeCamera()

func load_video(path : String):
	plugin.loadVideo(path)
