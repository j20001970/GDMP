#include "gpu_helper.h"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_buffer.h"
#endif
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

#if !MEDIAPIPE_DISABLE_GPU
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
#endif

void MediaPipeGPUHelper::_register_methods() {
	register_method("initialize", &MediaPipeGPUHelper::initialize);
	register_method("make_gpu_image", &MediaPipeGPUHelper::make_gpu_image);
	register_method("make_gpu_buffer_packet", &MediaPipeGPUHelper::make_gpu_buffer_packet);
}

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeGPUHelper *MediaPipeGPUHelper::_new(mediapipe::GpuResources *gpu_resources) {
	MediaPipeGPUHelper *helper = MediaPipeGPUHelper::_new();
	helper->gpu_helper.InitializeForTest(gpu_resources);
	return helper;
}
#endif

void MediaPipeGPUHelper::_init() {}

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
	gpu_image = Ref(MediaPipeImage::_new(gpu_buffer));
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
	packet = Ref(MediaPipePacket::_new(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_buffer)));
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
	return packet;
}
