#include "camera_helper.h"

#include <thread>

#include "OS.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

#include "GDMP/framework/packet.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "GDMP/gpu/gpu_helper.h"
#endif

class CameraHelper::CameraHelperImpl : public cv::VideoCapture {
	private:
		bool grab_frames;
		bool flip;
		std::thread thread;
		String stream_name;
		Ref<Graph> graph;
#if !MEDIAPIPE_DISABLE_GPU
		bool use_gpu;
#endif

	public:
		CameraHelperImpl() {
#if !MEDIAPIPE_DISABLE_GPU
			use_gpu = true;
#endif
		}

		void set_graph(Ref<Graph> graph, String stream_name) {
			this->graph = graph;
			this->stream_name = stream_name;
		}

		void set_flip(bool value) {
			this->flip = value;
		}

		void start(int index, Vector2 size) {
			ERR_FAIL_COND(graph.is_null());
			close();
			open(index);
			ERR_FAIL_COND(!isOpened());
			set(cv::CAP_PROP_FRAME_WIDTH, size.x);
			set(cv::CAP_PROP_FRAME_HEIGHT, size.y);
			ERR_FAIL_COND(!isOpened());
			thread = std::thread([this, size]() -> void {
#if !MEDIAPIPE_DISABLE_GPU
				Ref<GPUHelper> gpu_helper = GPUHelper::_new(graph->get_gpu_resources().get());
				auto cv_format = use_gpu ? cv::COLOR_BGR2RGBA : cv::COLOR_BGR2RGB;
				auto image_format = use_gpu ? mediapipe::ImageFormat::SRGBA : mediapipe::ImageFormat::SRGB;
#else
				auto cv_format = cv::COLOR_BGR2RGB;
				auto image_format = mediapipe::ImageFormat::SRGB;
#endif
				grab_frames = true;
				while (grab_frames) {
					cv::Mat video_frame;
					*this >> video_frame;
					if (flip) {
						cv::flip(video_frame, video_frame, 1);
					}
					cv::cvtColor(video_frame, video_frame, cv_format);
					auto input_frame = std::make_unique<mediapipe::ImageFrame>(
							image_format, video_frame.cols, video_frame.rows,
							mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
					cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
					video_frame.copyTo(input_frame_mat);
					Ref<Packet> packet = Packet::_new();
					int64_t frame_timestamp_us = OS::get_singleton()->get_ticks_usec();
#if !MEDIAPIPE_DISABLE_GPU
					if (use_gpu)
						packet = gpu_helper->make_packet_from_image_frame(std::move(input_frame));
					else
#endif
						packet->make_image_frame(std::move(input_frame));
					packet->set_timestamp(frame_timestamp_us);
					graph->add_packet(stream_name, packet);
				}
			});
		}

		void close() {
			grab_frames = false;
			if (thread.joinable()) {
				thread.join();
				release();
			}
		}

#if !MEDIAPIPE_DISABLE_GPU
		void set_use_gpu(bool use_gpu) {
			this->use_gpu = use_gpu;
		}
#endif
};

CameraHelper::CameraHelper() = default;

CameraHelper::~CameraHelper() = default;

void CameraHelper::_init() {
	impl = std::make_unique<CameraHelperImpl>();
}

bool CameraHelper::permission_granted() {
	return true;
}

void CameraHelper::request_permission() {}

void CameraHelper::set_graph(Ref<Graph> graph, String stream_name) {
	impl->set_graph(graph, stream_name);
}

void CameraHelper::set_mirrored(bool value) {
	impl->set_flip(value);
}

void CameraHelper::start(int index, Vector2 size) {
	impl->start(index, size);
}

void CameraHelper::close() {
	if (impl) {
		impl->close();
	}
}

#if !MEDIAPIPE_DISABLE_GPU
void CameraHelper::set_use_gpu(bool use_gpu) {
	impl->set_use_gpu(use_gpu);
}
#endif
