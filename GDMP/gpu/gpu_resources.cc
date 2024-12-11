#include "gpu_resources.h"

#include "Ref.hpp"

void MediaPipeGPUResources::_register_methods() {}

void MediaPipeGPUResources::_init() {
#if !MEDIAPIPE_DISABLE_GPU
	auto create_gpu_resources = mediapipe::GpuResources::Create();
	ERR_FAIL_COND_V(!create_gpu_resources.ok(), ERR_PRINT(create_gpu_resources.status().ToString().data()));
	gpu_resources = create_gpu_resources.value();
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

#if !MEDIAPIPE_DISABLE_GPU
MediaPipeGPUResources *MediaPipeGPUResources::_new(std::shared_ptr<mediapipe::GpuResources> gpu_resources) {
	MediaPipeGPUResources *resources = MediaPipeGPUResources::_new();
	resources->gpu_resources = gpu_resources;
	return resources;
}
#endif
#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GpuResources> MediaPipeGPUResources::get_gpu_resources() {
	return gpu_resources;
}
#endif
