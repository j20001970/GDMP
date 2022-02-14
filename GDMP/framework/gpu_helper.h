#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

#include "packet.h"

namespace godot {

class GPUHelper : public Reference {
		GODOT_CLASS(GPUHelper, Reference)

	public:
		static void _register_methods();
		static GPUHelper *_new(mediapipe::GpuResources *gpu_resource);

		void _init();

		Ref<Image> get_gpu_frame(Ref<Packet> packet);
		Ref<Packet> make_packet_from_image(Ref<Image> image);
		Ref<Packet> make_packet_from_image_frame(const mediapipe::ImageFrame &image_frame);

	private:
		mediapipe::GlCalculatorHelper gpu_helper;
};

} //namespace godot

#endif
