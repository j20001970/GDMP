#ifndef GDMP_GPU_RESOURCES
#define GDMP_GPU_RESOURCES

#include <memory>

#include "Godot.hpp"
#include "Reference.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

using namespace godot;

class GPUResources : public Reference {
		GODOT_CLASS(GPUResources, Reference);

	public:
		static void _register_methods();

		void _init();

#if !MEDIAPIPE_DISABLE_GPU
		std::shared_ptr<mediapipe::GpuResources> get_gpu_resources();

	private:
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;
#endif
};

#endif
