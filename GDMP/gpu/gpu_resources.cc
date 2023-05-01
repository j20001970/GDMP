#include "gpu_resources.h"

void GPUResources::_register_methods() {}

void GPUResources::_init() {
#if !MEDIAPIPE_DISABLE_GPU
	auto create_gpu_resources = mediapipe::GpuResources::Create();
	ERR_FAIL_COND_V(!create_gpu_resources.ok(), ERR_PRINT(create_gpu_resources.status().ToString().data()));
	gpu_resources = create_gpu_resources.value();
#else
	ERR_PRINT("GPU support is disabled in this build.");
#endif
}

#if !MEDIAPIPE_DISABLE_GPU
std::shared_ptr<mediapipe::GpuResources> GPUResources::get_gpu_resources() {
	return gpu_resources;
}
#endif
