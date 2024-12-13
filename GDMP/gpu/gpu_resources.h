#ifndef GDMP_GPU_RESOURCES
#define GDMP_GPU_RESOURCES

#include <memory>

#include "Godot.hpp"
#include "Reference.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

using namespace godot;

class MediaPipeGPUResources : public Reference {
		GODOT_CLASS(MediaPipeGPUResources, Reference)

#if !MEDIAPIPE_DISABLE_GPU
	private:
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif

	public:
		static void _register_methods();
		#if !MEDIAPIPE_DISABLE_GPU
		static MediaPipeGPUResources *_new(std::shared_ptr<mediapipe::GpuResources> gpu_resources);
		#endif

		void _init();

#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();
#endif
};

#endif
