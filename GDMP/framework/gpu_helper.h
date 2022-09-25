#ifndef GDMP_GPU_HELPER
#define GDMP_GPU_HELPER

#include <memory>

#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

#include "graph.h"
#include "packet.h"

namespace godot {

class GPUHelper : public RefCounted {
		GDCLASS(GPUHelper, RefCounted)

	protected:
		static void _bind_methods();

	public:
		GPUHelper();
		GPUHelper(mediapipe::GpuResources *gpu_resource);
		~GPUHelper();

		// Initialize GPU helper with given graph.
		void initialize(Ref<Graph> graph);
		// Get GPU frame from GpuBuffer packet and convert to godot::Image
		Ref<Image> get_gpu_frame(Ref<Packet> packet);
		// Make a mediapipe::GpuBuffer packet from godot::Image
		Ref<Packet> make_packet_from_image(Ref<Image> image);
		// Make a mediapipe::GpuBuffer packet from mediapipe::ImageFrame
		Ref<Packet> make_packet_from_image_frame(std::unique_ptr<mediapipe::ImageFrame> image_frame);

	private:
		mediapipe::GlCalculatorHelper gpu_helper;
};

} //namespace godot

#endif
