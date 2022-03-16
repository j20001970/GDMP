#ifndef GDMP_CAMERA_HELPER
#define GDMP_CAMERA_HELPER

#include <memory>

#include "Godot.hpp"
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

		void set_flip_y(bool flip);
		void set_graph(Graph *graph, String stream_name);
		void start(int index);
		void close();
#if !MEDIAPIPE_DISABLE_GPU
		void set_use_gpu(bool use_gpu);
#endif

	private:
		bool flip_y;
		bool started;
		String stream_name;
		Graph *graph;
		std::unique_ptr<CameraHelperImpl> impl;
#if !MEDIAPIPE_DISABLE_GPU
		bool use_gpu;
#endif
};

} //namespace godot

#endif
