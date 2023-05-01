#ifndef GDMP_GPU_RESOURCES
#define GDMP_GPU_RESOURCES

#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

using namespace godot;

class MediaPipeGPUResources : public RefCounted {
		GDCLASS(MediaPipeGPUResources, RefCounted)

#if !MEDIAPIPE_DISABLE_GPU
	private:
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif

	protected:
		static void _bind_methods();

	public:
		MediaPipeGPUResources();
		~MediaPipeGPUResources();

#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();
#endif
};

#endif
