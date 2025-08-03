#ifndef GDMP_IMAGE
#define GDMP_IMAGE

#include "Godot.hpp"
#include "Image.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image.h"

#include "GDMP/framework/packet.h"

using namespace godot;

class MediaPipeImage : public Reference {
		GODOT_CLASS(MediaPipeImage, Reference)

	private:
		static bool _register_packet_types;
		static bool register_packet_types();

		mediapipe::Image image;

	public:
		static void _register_methods();
		static MediaPipeImage *_new(mediapipe::Image image);
		static MediaPipeImage *_new(mediapipe::ImageFrameSharedPtr image_frame);
#if !MEDIAPIPE_DISABLE_GPU
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
		static MediaPipeImage *_new(CVPixelBufferRef pixel_buffer);
#endif
		static MediaPipeImage *_new(mediapipe::GpuBuffer gpu_buffer);
#endif

		void _init();

		// Check if the image is a GPU image.
		bool is_gpu_image();
		// Tranfer image data to CPU if it is a GPU image.
		// For converting to GPU image, refer to MediaPipeGPUHelper::make_gpu_image.
		void convert_to_cpu();

		// Get a copy of godot::Image.
		// Convert GPU image to CPU when necessary.
		Ref<Image> get_godot_image();
		// Copy the data of godot::Image to the image.
		void set_godot_image(Ref<Image> image);

		// Get a mediapipe::Image packet from this image.
		Ref<MediaPipePacket> get_packet();
		// Get a mediapipe::ImageFrame packet from this image.
		// For making mediapipe::GpuBuffer packet, refer to MediaPipeGPUHelper::make_gpu_buffer_packet.
		Ref<MediaPipePacket> get_image_frame_packet();

		// Get underlying mediapipe::Image.
		mediapipe::Image get_mediapipe_image();
};

#endif
