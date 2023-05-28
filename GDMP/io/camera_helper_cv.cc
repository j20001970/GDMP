#include "camera_helper.h"

#include <thread>

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

#include "GDMP/framework/image.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "GDMP/gpu/gpu_helper.h"
#endif

class CameraHelperCV : public CameraHelperImpl, public cv::VideoCapture {
	private:
		bool flip;
		bool grab_frames;
#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif
		std::thread thread;

		void camera_thread() {
			auto cv_format = cv::COLOR_BGR2RGB;
				auto image_format = mediapipe::ImageFormat::SRGB;
#if !MEDIAPIPE_DISABLE_GPU
				Ref<MediaPipeGPUHelper> gpu_helper;
				if (gpu_resources != nullptr) {
					cv_format = cv::COLOR_BGR2RGBA;
					image_format = mediapipe::ImageFormat::SRGBA;
					gpu_helper = Ref(MediaPipeGPUHelper::_new(gpu_resources.get()));
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
					auto input_frame = std::make_shared<mediapipe::ImageFrame>(
							image_format, video_frame.cols, video_frame.rows,
							mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
					cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
					video_frame.copyTo(input_frame_mat);
					Ref<MediaPipeImage> image = MediaPipeImage::_new(input_frame);
#if !MEDIAPIPE_DISABLE_GPU
					if (gpu_helper.is_valid())
						image = gpu_helper->make_gpu_image(image);
#endif
					camera_helper->emit_signal("new_frame", image);
				}
		}

	public:
		CameraHelperCV(MediaPipeCameraHelper *camera_helper) :
				CameraHelperImpl(camera_helper) {
			flip = false;
		}

		~CameraHelperCV() = default;

		bool permission_granted() {
			return true;
		}

		void request_permission() {}

		void set_mirrored(bool value) {
			flip = value;
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

		void start(int index, Vector2 size) {
			open(index);
			set(cv::CAP_PROP_FRAME_WIDTH, size.x);
			set(cv::CAP_PROP_FRAME_HEIGHT, size.y);
			ERR_FAIL_COND(!isOpened());
			thread = std::thread(&CameraHelperCV::camera_thread, this);
		}

		void close() {
			grab_frames = false;
			if (thread.joinable()) {
				thread.join();
				release();
			}
		}
};

void MediaPipeCameraHelper::_init() {
	impl = std::make_unique<CameraHelperCV>(this);
}
