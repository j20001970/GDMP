extends Node

enum {
	CAMERA_FACING_FRONT,
	CAMERA_FACING_BACK
}

# Framework
const Graph : NativeScript = preload("framework/Graph.gdns")
const Packet : NativeScript = preload("framework/Packet.gdns")
const GPUHelper : NativeScript = preload("framework/GPUHelper.gdns")

# I/O
const CameraHelper : NativeScript = preload("io/CameraHelper.gdns")

# Classification
const Classification : NativeScript = preload("proto/Classification.gdns")
const ClassificationList : NativeScript = preload("proto/ClassificationList.gdns")
const ClassificationListCollection : NativeScript = preload("proto/ClassificationListCollection.gdns")

# Detection
const AssociatedDetection : NativeScript = preload("proto/AssociatedDetection.gdns")
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
var graph
var camera_helper
var gpu_helper
var packet_data : Dictionary = {}

func _on_new_proto(stream_name : String, packet) -> void:
	if packet_data.has(stream_name):
		var bytes : PoolByteArray = packet.get_proto()
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], bytes)

func _on_new_proto_vector(stream_name : String, packet) -> void:
	if packet_data.has(stream_name):
		var data : Array = packet.get_proto_vector()
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], data)

func _on_new_frame(stream_name : String, packet) -> void:
	if packet_data.has(stream_name):
		var image : Image = gpu_helper.get_gpu_frame(packet)
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], image.get_width(), image.get_height(), image.get_data())

func init_graph(graph_path : String) -> void:
	if camera_helper:
		camera_helper.close()
	if graph:
		graph.stop()
	graph = Graph.new()
	graph.initialize(graph_path, true)
	gpu_helper = graph.get_gpu_helper()
	packet_data.clear()

func add_proto_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		graph.add_packet_callback(stream_name, self, "_on_new_proto")
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func add_proto_vector_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		graph.add_packet_callback(stream_name, self, "_on_new_proto_vector")
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func add_gpu_frame_callback(stream_name : String, target : Object, method : String) -> void:
	if not packet_data.has(stream_name):
		graph.add_packet_callback(stream_name, self, "_on_new_frame")
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

func start_graph(side_packets : Dictionary = {}) -> void:
	graph.start(side_packets)

func start_camera(index : int = 0, stream_name : String = "input_video") -> void:
	camera_helper = CameraHelper.new()
	camera_helper.set_graph(graph, stream_name)
	camera_helper.start(index)

func close_camera() -> void:
	camera_helper.close()
	graph.stop()
