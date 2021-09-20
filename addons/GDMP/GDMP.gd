extends Node

enum {
	CAMERA_FACING_FRONT,
	CAMERA_FACING_BACK
}

const Classification = preload("Classification.gd")
const Detection = preload("Detection.gd")
const Landmark = preload("Landmark.gd")

var plugin : Object
var image : Image = Image.new()
var packet_data : Dictionary = {}

func _enter_tree():
	match OS.get_name():
		"Android":
			if Engine.has_singleton("GDMP"):
				plugin = Engine.get_singleton("GDMP")
				plugin.init(get_instance_id())
		_:
			plugin = Node.new()
			plugin.set_script(load("res://addons/GDMP/GDMP.gdns"))
			plugin.connect("new_frame", self, "_on_new_frame")
			plugin.connect("new_packet", self, "__on_new_packet")
			add_child(plugin)
	plugin.connect("on_new_proto", self, "_on_new_packet")
	plugin.connect("on_new_proto_vector", self, "_on_new_packet")

func __on_new_packet(stream_name, is_vector, data):
	data = Array(data)
	if is_vector:
		var count : int = data[0]
		var n : int = (data.size()-1)/count
		assert((data.size()-1)/n == count)
		var landmarks_list : Array = []
		for i in range(count):
			var landmarks_data = data.slice(1+n*i, 1+n*i+n)
			landmarks_list.append(parse_landmarks(landmarks_data))
		if packet_data.has(stream_name):
			for obj in packet_data[stream_name]:
				if is_instance_valid(obj):
					obj.call(packet_data[stream_name][obj], landmarks_list.duplicate())
	else:
		var landmarks = parse_landmarks(data)
		if packet_data.has(stream_name):
			for obj in packet_data[stream_name]:
				if is_instance_valid(obj):
					obj.call(packet_data[stream_name][obj], landmarks.duplicate())

func _on_new_packet(stream_name : String, data) -> void:
	if packet_data.has(stream_name):
		for obj in packet_data[stream_name]:
			if is_instance_valid(obj):
				obj.call(packet_data[stream_name][obj], data)

func _on_new_frame(data, width, height) -> void:
	var channel : int = int(data.size()/width/height)
	data.resize(width*height*channel)
	if channel == 4:
		image.create_from_data(width, height, false, Image.FORMAT_RGBA8, data)
	else:
		image.create_from_data(width, height, false, Image.FORMAT_RGB8, data)

func init_graph(graph_path : String, input_side_packets : Dictionary = {}) -> void:
	packet_data.clear()
	plugin.initGraph(graph_path, input_side_packets)

func add_packet_callback(stream_name, is_vector, target, method) -> void:
	if not packet_data.has(stream_name):
		plugin.addPacketCallback(stream_name, is_vector)
		packet_data[stream_name] = {}
	packet_data[stream_name][target] = method

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

func start_camera(index : int = 0) -> void:
	plugin.startCamera(index)

func close_camera() -> void:
	plugin.closeCamera()

func load_video(path : String):
	plugin.loadVideo(path)

func parse_landmarks(data):
	var count : int = data[0]
	var landmarks_data = Array(data).slice(1, data.size()-1)
	assert(landmarks_data.size()/3 == count)
	var landmark_list : Array = []
	for i in range(count):
		landmark_list.append({
			"x": landmarks_data[i*3],
			"y": landmarks_data[i*3+1],
			"z": landmarks_data[i*3+2]
		})
	return landmark_list
