#ifndef GDMP_CAMERA_HELPER
#define GDMP_CAMERA_HELPER

#include <memory>

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "../framework/graph.h"

namespace godot {

class CameraHelper : public Reference {
		GODOT_CLASS(CameraHelper, Reference)

	public:
		class CameraHelperImpl;

		static void _register_methods();

		CameraHelper();
		~CameraHelper();

		void _init();

		// Return true if camera permission is granted.
		// Always return true if there is no permission model.
		bool permission_granted();
		// Request camera permission if not granted.
		// No-op if there is no permission model.
		void request_permission();
		// Set the target graph to send video frames to with stream name.
		void set_graph(Graph *graph, String stream_name);
		// Start the camera with specified index and target resolution.
		// Actual supported resolutions are platform and hardware dependent.
		void start(int index, Vector2 size);
		// Close camera.
		void close();
#if !MEDIAPIPE_DISABLE_GPU
		// Set camera helper to produce either GPU (mediapipe::GpuBuffer) or CPU (mediapipe::ImageFrame) image packet.
		// No-op if platform doesn't support.
		void set_use_gpu(bool use_gpu);
#endif

	private:
		std::unique_ptr<CameraHelperImpl> impl;
		// Permission request callback.
		// TODO: see if this can be move to impl
		void _on_permission_result(PoolStringArray permissions, PoolIntArray results);
};

} //namespace godot

#endif
