#ifndef GDMP_TASK_EMBEDDER_OPTIONS
#define GDMP_TASK_EMBEDDER_OPTIONS

#include "Godot.hpp"
#include "Reference.hpp"

#include "mediapipe/tasks/cc/components/processors/embedder_options.h"

using namespace godot;
using namespace mediapipe::tasks::components::processors;

class MediaPipeEmbedderOptions : public Reference {
		GODOT_CLASS(MediaPipeEmbedderOptions, Reference)

	private:
		EmbedderOptions options = {};

	public:
		static void _register_methods();

		void _init();

		void set_l2_normalize(bool p_l2_normalize);
		bool get_l2_normalize();

		void set_quantize(bool p_quantize);
		bool get_quantize();

		const EmbedderOptions &get_options();
};

#endif
