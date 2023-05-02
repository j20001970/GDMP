#ifndef GDMP_CAMERA_HELPER
#define GDMP_CAMERA_HELPER

#include <memory>

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/string.hpp"

#include "GDMP/framework/graph.h"
#include "GDMP/gpu/gpu_resources.h"

using namespace godot;

class MediaPipeCameraHelper : public RefCounted {
		GDCLASS(MediaPipeCameraHelper, RefCounted)

	public:
		class Impl;

	private:
		std::unique_ptr<Impl> impl;

	protected:
		static void _bind_methods();

	public:
		enum CameraFacing {
			FACING_FRONT,
			FACING_BACK
		};

		MediaPipeCameraHelper();
		~MediaPipeCameraHelper();

		// Return true if camera permission is granted.
		// Always return true if there is no permission model.
		bool permission_granted();
		// Request camera permission if not granted.
		// No-op if there is no permission model.
		void request_permission();
		// Set the target graph to send video frames to with stream name.
		void set_graph(Ref<MediaPipeGraph> graph, String stream_name);
		// Set whether to flip video frames horizontally.
		// Only works on non-mobile platforms.
		void set_mirrored(bool value);
		// Start the camera with specified index and target resolution.
		// Previously opened camera on this camera helper will be closed first.
		// Camera will not start if:
		// 1. the graph to send video frames to is not set.
		// 2. camera permission is not granted.
		// Actual supported resolutions are platform and hardware dependent.
		void start(int index, Vector2 size);
		// Close camera.
		void close();
		// Set GPU resources to produce mediapipe::GpuBuffer packets.
		// Depending on platforms, it might be required to set GPU resources to start camera.
		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources);
};

VARIANT_ENUM_CAST(MediaPipeCameraHelper::CameraFacing);

#endif
