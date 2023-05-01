#include "gpu_resources.h"

void MediaPipeGPUResources::_bind_methods() {}

MediaPipeGPUResources::MediaPipeGPUResources() {
#if !MEDIAPIPE_DISABLE_GPU
	auto create_gpu_resources = mediapipe::GpuResources::Create();
	ERR_FAIL_COND_MSG(!create_gpu_resources.ok(), create_gpu_resources.status().ToString().data());
	gpu_resources = create_gpu_resources.value();
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

MediaPipeGPUResources::~MediaPipeGPUResources() = default;

#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GpuResources> MediaPipeGPUResources::get_gpu_resources() {
	return gpu_resources;
}
#endif
