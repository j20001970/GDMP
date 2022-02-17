extends Node

enum {
	CAMERA_FACING_FRONT,
	CAMERA_FACING_BACK
}

# Classification
const Classification : NativeScript = preload("proto/Classification.gdns")
const ClassificationList : NativeScript = preload("proto/ClassificationList.gdns")
const ClassificationListCollection : NativeScript = preload("proto/ClassificationListCollection.gdns")

# Detection
const Detection : NativeScript = preload("proto/Detection.gdns")
const DetectionList : NativeScript = preload("proto/DetectionList.gdns")

# Landmark
const Landmark : NativeScript = preload("proto/Landmark.gdns")
const LandmarkList : NativeScript = preload("proto/LandmarkList.gdns")
const LandmarkListCollection : NativeScript = preload("proto/LandmarkListCollection.gdns")
const NormalizedLandmark : NativeScript = preload("proto/NormalizedLandmark.gdns")
const NormalizedLandmarkList : NativeScript = preload("proto/NormalizedLandmarkList.gdns")
const NormalizedLandmarkListCollection : NativeScript = preload("proto/NormalizedLandmarkListCollection.gdns")

# LocationData
const BoundingBox : NativeScript = preload("proto/BoundingBox.gdns")
const RelativeBoundingBox : NativeScript = preload("proto/RelativeBoundingBox.gdns")
const BinaryMask : NativeScript = preload("proto/BinaryMask.gdns")
const RelativeKeypoint : NativeScript = preload("proto/RelativeKeypoint.gdns")
const LocationData : NativeScript = preload("proto/LocationData.gdns")

# Rasterization
const Interval : NativeScript = preload("proto/Interval.gdns")
const Rasterization : NativeScript = preload("proto/Rasterization.gdns")

# Rect
const Rect : NativeScript = preload("proto/Rect.gdns")
const NormalizedRect : NativeScript = preload("proto/NormalizedRect.gdns")

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

func init_graph(graph_path : String) -> void:
	packet_data.clear()
	plugin.initGraph(graph_path)

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

func start_graph(side_packets : Dictionary = {}) -> void:
	plugin.startGraph(side_packets)

func start_camera(index : int = 0, stream_name : String = "input_video") -> void:
	plugin.startCamera(index, stream_name)

func close_camera() -> void:
	plugin.closeCamera()
