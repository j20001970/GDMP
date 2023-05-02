#include "camera_helper.h"

#include <thread>

#include "godot_cpp/classes/time.hpp"

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

class MediaPipeCameraHelper::Impl : public cv::VideoCapture {
	private:
		String stream_name;
		Ref<MediaPipeGraph> graph;
		bool flip;
		bool grab_frames;
#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif
		std::thread thread;

	public:
		Impl() {}

		void set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
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
				auto cv_format = cv::COLOR_BGR2RGB;
				auto image_format = mediapipe::ImageFormat::SRGB;
#if !MEDIAPIPE_DISABLE_GPU
				Ref<MediaPipeGPUHelper> gpu_helper;
				if (gpu_resources != nullptr) {
					cv_format = cv::COLOR_BGR2RGBA;
					image_format = mediapipe::ImageFormat::SRGBA;
					gpu_helper = Ref(memnew(MediaPipeGPUHelper(gpu_resources.get())));
				}
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
					Ref<MediaPipePacket> packet = memnew(MediaPipePacket());
					int64_t frame_timestamp_us = Time::get_singleton()->get_ticks_usec();
#if !MEDIAPIPE_DISABLE_GPU
					if (gpu_helper.is_valid())
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

		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
			if (gpu_resources.is_null()) {
				this->gpu_resources = nullptr;
				return;
			}
			this->gpu_resources = gpu_resources->get_gpu_resources();
#endif
		}
};

MediaPipeCameraHelper::MediaPipeCameraHelper() {
	impl = std::make_unique<Impl>();
}

MediaPipeCameraHelper::~MediaPipeCameraHelper() = default;

bool MediaPipeCameraHelper::permission_granted() {
	return true;
}

void MediaPipeCameraHelper::request_permission() {}

void MediaPipeCameraHelper::set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
	impl->set_graph(graph, stream_name);
}

void MediaPipeCameraHelper::set_mirrored(bool value) {
	impl->set_flip(value);
}

void MediaPipeCameraHelper::start(int index, Vector2 size) {
	impl->start(index, size);
}

void MediaPipeCameraHelper::close() {
	if (impl) {
		impl->close();
	}
}

void MediaPipeCameraHelper::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
	impl->set_gpu_resources(gpu_resources);
}
