#include "gpu_helper.h"

#include "godot_cpp/core/class_db.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_buffer.h"
#endif
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

mediapipe::GpuBuffer MediaPipeGPUHelper::get_gpu_buffer(mediapipe::Image image) {
	mediapipe::GpuBuffer gpu_buffer;
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
	gpu_buffer = image.GetGpuBuffer();
#else
	ERR_FAIL_COND_V(!gpu_helper.Initialized(), gpu_buffer);
	gpu_helper.RunInGlContext([this, &image, &gpu_buffer]() -> void {
		gpu_buffer = image.GetGpuBuffer();
	});
#endif
	return gpu_buffer;
}

void MediaPipeGPUHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize"), &MediaPipeGPUHelper::initialize);
	ClassDB::bind_method(D_METHOD("make_gpu_image"), &MediaPipeGPUHelper::make_gpu_image);
	ClassDB::bind_method(D_METHOD("make_gpu_buffer_packet"), &MediaPipeGPUHelper::make_gpu_buffer_packet);
}

MediaPipeGPUHelper::MediaPipeGPUHelper() = default;

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeGPUHelper::MediaPipeGPUHelper(mediapipe::GpuResources *gpu_resources) {
	gpu_helper.InitializeForTest(gpu_resources);
}
#endif

MediaPipeGPUHelper::~MediaPipeGPUHelper() = default;

void MediaPipeGPUHelper::initialize(Ref<MediaPipeGPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND(gpu_resources.is_null());
	gpu_helper.InitializeForTest(gpu_resources->get_gpu_resources().get());
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

Ref<MediaPipeImage> MediaPipeGPUHelper::make_gpu_image(Ref<MediaPipeImage> image) {
	Ref<MediaPipeImage> gpu_image;
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND_V(image.is_null(), gpu_image);
	if (image->is_gpu_image())
		return image;

	mediapipe::GpuBuffer gpu_buffer = get_gpu_buffer(image->get_mediapipe_image());
	gpu_image = Ref(memnew(MediaPipeImage(gpu_buffer)));
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return gpu_image;
}

Ref<MediaPipePacket> MediaPipeGPUHelper::make_gpu_buffer_packet(Ref<MediaPipeImage> image) {
	Ref<MediaPipePacket> packet;
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND_V(image.is_null(), packet);
	mediapipe::GpuBuffer gpu_buffer = get_gpu_buffer(image->get_mediapipe_image());
	packet = Ref(memnew(MediaPipePacket(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_buffer))));
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return packet;
}
