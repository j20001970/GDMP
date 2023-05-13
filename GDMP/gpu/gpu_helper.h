#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include <memory>

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

#include "GDMP/framework/packet.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeGPUHelper : public Reference {
		GODOT_CLASS(MediaPipeGPUHelper, Reference)

#if !MEDIAPIPE_DISABLE_GPU
	private:
		mediapipe::GlCalculatorHelper gpu_helper;
#endif

	public:
		static void _register_methods();
#if !MEDIAPIPE_DISABLE_GPU
		static MediaPipeGPUHelper *_new(mediapipe::GpuResources *gpu_resource);
#endif

		void _init();

		// Initialize GPU helper from GPU resources.
		void initialize(Ref<MediaPipeGPUResources> gpu_resources);
		// Get GPU frame from GpuBuffer packet and convert to godot::Image
		Ref<Image> get_gpu_frame(Ref<MediaPipePacket> packet);
		// Make a mediapipe::GpuBuffer packet from godot::Image
		Ref<MediaPipePacket> make_packet_from_image(Ref<Image> image);
		// Make a mediapipe::GpuBuffer packet from mediapipe::ImageFrame
		Ref<MediaPipePacket> make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame);
};

#endif
