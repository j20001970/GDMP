#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include <memory>

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

#include "mediapipe/GDMP/framework/packet.h"
#include "mediapipe/GDMP/gpu/gpu_resources.h"

using namespace godot;

class GPUHelper : public Reference {
		GODOT_CLASS(GPUHelper, Reference)

	public:
		static void _register_methods();
		static GPUHelper *_new(mediapipe::GpuResources *gpu_resource);

		void _init();

		// Initialize GPU helper with given graph.
		void initialize(Ref<GPUResources> gpu_resources);
		// Get GPU frame from GpuBuffer packet and convert to godot::Image
		Ref<Image> get_gpu_frame(Ref<Packet> packet);
		// Make a mediapipe::GpuBuffer packet from godot::Image
		Ref<Packet> make_packet_from_image(Ref<Image> image);
		// Make a mediapipe::GpuBuffer packet from mediapipe::ImageFrame
		Ref<Packet> make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame);

	private:
		mediapipe::GlCalculatorHelper gpu_helper;
};

#endif
