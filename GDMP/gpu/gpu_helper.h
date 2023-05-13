#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "GDMP/framework/image.h"
#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeGPUHelper : public RefCounted {
		GDCLASS(MediaPipeGPUHelper, RefCounted)

#if !MEDIAPIPE_DISABLE_GPU
	private:
		mediapipe::GlCalculatorHelper gpu_helper;

		mediapipe::GpuBuffer get_gpu_buffer(mediapipe::Image image);
#endif

	protected:
		static void _bind_methods();

	public:
		MediaPipeGPUHelper();
#if !MEDIAPIPE_DISABLE_GPU
		MediaPipeGPUHelper(mediapipe::GpuResources *gpu_resources);
#endif
		~MediaPipeGPUHelper();

		// Initialize GPU helper from GPU resources.
		void initialize(Ref<MediaPipeGPUResources> gpu_resources);

		// Make a GPU image.
		// Return the image as-is if it is already a GPU image.
		Ref<MediaPipeImage> make_gpu_image(Ref<MediaPipeImage> image);
		// Make a mediapipe::GpuBuffer packet.
		Ref<MediaPipePacket> make_gpu_buffer_packet(Ref<MediaPipeImage> image);
};

#endif
