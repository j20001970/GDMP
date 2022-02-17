#include "gdmp.h"

#include "Array.hpp"

using namespace godot;

void GDMP::_register_methods() {
	register_method("_on_new_proto", &GDMP::_on_new_proto);
	register_method("_on_new_proto_vector", &GDMP::_on_new_proto_vector);
#if !MEDIAPIPE_DISABLE_GPU
	register_method("_on_new_gpu_frame", &GDMP::_on_new_gpu_frame);
#endif
	register_method("initGraph", &GDMP::init_graph);
	register_method("addProtoCallback", &GDMP::add_proto_callback);
	register_method("addProtoVectorCallback", &GDMP::add_proto_vector_callback);
	register_method("addGpuFrameCallback", &GDMP::add_gpu_frame_callback);
	register_method("startGraph", &GDMP::start_graph);
	register_method("startCamera", &GDMP::start_camera);
	register_method("closeCamera", &GDMP::close_camera);
	register_signal<GDMP>("on_new_proto", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_bytes", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
	register_signal<GDMP>("on_new_proto_vector", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_vector", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<GDMP>("on_new_frame", "stream_name", GODOT_VARIANT_TYPE_STRING, "width", GODOT_VARIANT_TYPE_INT, "height", GODOT_VARIANT_TYPE_INT, "data", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
}

void GDMP::_init() {
	// initialize any variables here
	camera_helper = Ref<CameraHelper>(CameraHelper::_new());
}

void GDMP::_on_new_proto(String stream_name, Ref<Packet> packet) {
	emit_signal("on_new_proto", Array::make(stream_name, packet->get_proto()));
}

void GDMP::_on_new_proto_vector(String stream_name, Ref<Packet> packet) {
	emit_signal("on_new_proto_vector", Array::make(stream_name, packet->get_proto_vector()));
}

#if !MEDIAPIPE_DISABLE_GPU
void GDMP::_on_new_gpu_frame(String stream_name, Ref<Packet> packet) {
	Ref<Image> image = gpu_helper->get_gpu_frame(packet);
	emit_signal("on_new_frame", Array::make(stream_name, image->get_width(), image->get_height(), image->get_data()));
}
#endif

void GDMP::init_graph(String graph_path) {
	close_camera();
	graph = Ref<Graph>(Graph::_new());
	graph->initialize(graph_path, true);
#if !MEDIAPIPE_DISABLE_GPU
	gpu_helper = graph->get_gpu_helper();
#endif
}

void GDMP::add_proto_callback(String stream_name) {
	graph->add_packet_callback(stream_name, this, "_on_new_proto");
}

void GDMP::add_proto_vector_callback(String stream_name) {
	graph->add_packet_callback(stream_name, this, "_on_new_proto_vector");
}

#if !MEDIAPIPE_DISABLE_GPU
void GDMP::add_gpu_frame_callback(String stream_name) {
	graph->add_packet_callback(stream_name, this, "_on_new_gpu_frame");
}
#endif

void GDMP::start_graph(Dictionary side_packets) {
	graph->start(side_packets);
}

void GDMP::start_camera(int index, String stream_name) {
	camera_helper->set_graph(graph, stream_name);
	camera_helper->set_flip_y(true);
	camera_helper->start(index);
}

void GDMP::close_camera() {
	if (camera_helper.is_null()) {
		return;
	}
	camera_helper->close();
}
