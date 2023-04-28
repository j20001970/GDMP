#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include <memory>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "mediapipe/GDMP/framework/packet.h"
#include "mediapipe/GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeGPUHelper : public RefCounted {
		GDCLASS(MediaPipeGPUHelper, RefCounted)

	protected:
		static void _bind_methods();

	public:
		MediaPipeGPUHelper();
#if !MEDIAPIPE_DISABLE_GPU
		MediaPipeGPUHelper(mediapipe::GpuResources *gpu_resource);
#endif
		~MediaPipeGPUHelper();

		// Initialize GPU helper from GPU resources.
		void initialize(Ref<MediaPipeGPUResources> gpu_resources);
		// Get GPU frame from GpuBuffer packet and convert to godot::Image
		Ref<Image> get_gpu_frame(Ref<MediaPipePacket> packet);
		// Make a mediapipe::GpuBuffer packet from godot::Image
		Ref<MediaPipePacket> make_packet_from_image(Ref<Image> image);
		// Make a mediapipe::GpuBuffer packet from mediapipe::ImageFrame
		Ref<MediaPipePacket> make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame);

#if !MEDIAPIPE_DISABLE_GPU
	private:
		mediapipe::GlCalculatorHelper gpu_helper;
#endif
};

#endif
