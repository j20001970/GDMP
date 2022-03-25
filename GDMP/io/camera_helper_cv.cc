#include "camera_helper.h"

#include <thread>

#include "OS.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

#include "../framework/packet.h"

using namespace godot;

void CameraHelper::_register_methods() {
	register_method("set_flip_y", &CameraHelper::set_flip_y);
	register_method("set_graph", &CameraHelper::set_graph);
	register_method("start", &CameraHelper::start);
	register_method("close", &CameraHelper::close);
#if !MEDIAPIPE_DISABLE_GPU
	register_method("set_use_gpu", &CameraHelper::set_use_gpu);
#endif
}

class CameraHelper::CameraHelperImpl : public cv::VideoCapture {
	public:
		std::thread thread;
};

CameraHelper::CameraHelper() = default;

CameraHelper::~CameraHelper() = default;

void CameraHelper::_init() {
	impl = std::make_unique<CameraHelperImpl>();
	flip_y = false;
	started = false;
#if !MEDIAPIPE_DISABLE_GPU
	use_gpu = true;
#endif
}

void CameraHelper::_on_permission_result(PoolStringArray permissions, PoolIntArray results) {
}

void CameraHelper::set_flip_y(bool flip) {
	flip_y = flip;
}

void CameraHelper::set_graph(Graph *graph, String stream_name) {
	this->stream_name = stream_name;
	this->graph = graph;
}

void CameraHelper::start(int index) {
	if (graph == nullptr) {
		Godot::print("Graph is not set.");
		return;
	}
	impl->open(index);
	if (!impl->isOpened()) {
		Godot::print("Failed to open camera.");
		return;
	}
	started = true;
	impl->thread = std::thread([this]() -> void {
		cv::VideoCapture capture = *impl;
		capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
		capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
		capture.set(cv::CAP_PROP_FPS, 30);
#if !MEDIAPIPE_DISABLE_GPU
		Ref<GPUHelper> gpu_helper = graph->get_gpu_helper();
#endif
		while (started) {
			cv::Mat video_frame;
			capture >> video_frame;
			if (flip_y) {
				cv::flip(video_frame, video_frame, 1);
			}
			cv::cvtColor(video_frame, video_frame, cv::COLOR_BGR2RGBA);
			auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
					mediapipe::ImageFormat::SRGBA, video_frame.cols, video_frame.rows,
					mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
			cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
			video_frame.copyTo(input_frame_mat);
			Ref<Packet> packet;
			int64_t frame_timestamp_us = OS::get_singleton()->get_ticks_usec();
#if !MEDIAPIPE_DISABLE_GPU
			if (use_gpu)
				packet = gpu_helper->make_packet_from_image_frame(*input_frame);
			else
#endif
				packet = Packet::make_image_frame(*input_frame);
			packet->set_timestamp(frame_timestamp_us);
			graph->add_packet(stream_name, packet);
		}
	});
}

void CameraHelper::close() {
	started = false;
	if (impl && impl->thread.joinable()) {
		impl->thread.join();
		impl->release();
	}
}

#if !MEDIAPIPE_DISABLE_GPU
void CameraHelper::set_use_gpu(bool use_gpu) {
	this->use_gpu = use_gpu;
}
#endif
