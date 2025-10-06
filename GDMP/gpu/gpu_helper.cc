#include "gpu_helper.h"

#include "godot_cpp/core/class_db.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_buffer.h"
#endif

#if !MEDIAPIPE_DISABLE_GPU
mediapipe::GpuBuffer MediaPipeGPUHelper::get_gpu_buffer(mediapipe::Image image) {
	mediapipe::GpuBuffer gpu_buffer;
	ERR_FAIL_COND_V(!gpu_helper.Initialized(), gpu_buffer);
	gpu_helper.RunInGlContext([this, &image, &gpu_buffer]() -> void {
		gpu_buffer = image.GetGpuBuffer();
	});
	return gpu_buffer;
}
#endif

void MediaPipeGPUHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "gpu_resources"), &MediaPipeGPUHelper::initialize);
	ClassDB::bind_method(D_METHOD("is_initialized"), &MediaPipeGPUHelper::is_initialized);
	ClassDB::bind_method(D_METHOD("make_gpu_image", "image"), &MediaPipeGPUHelper::make_gpu_image);
	ClassDB::bind_method(D_METHOD("make_gpu_buffer_packet", "image"), &MediaPipeGPUHelper::make_gpu_buffer_packet);
}

MediaPipeGPUHelper::MediaPipeGPUHelper() = default;

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeGPUHelper::MediaPipeGPUHelper(mediapipe::GpuResources *gpu_resources) {
	gpu_helper.InitializeForTest(gpu_resources);
}
#endif

MediaPipeGPUHelper::~MediaPipeGPUHelper() = default;

bool MediaPipeGPUHelper::initialize(Ref<MediaPipeGPUResources> gpu_resources) {
#if !MEDIAPIPE_DISABLE_GPU
	ERR_FAIL_COND_V(gpu_resources.is_null(), false);
	gpu_helper.InitializeForTest(gpu_resources->get_gpu_resources().get());
	return gpu_helper.Initialized();
#else
	ERR_PRINT("GPU support is disabled in this build.");
	return false;
#endif
}

bool MediaPipeGPUHelper::is_initialized() {
#if !MEDIAPIPE_DISABLE_GPU
	return gpu_helper.Initialized();
#else
	ERR_PRINT("GPU support is disabled in this build.");
	return false;
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
