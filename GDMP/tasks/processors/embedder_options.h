#ifndef GDMP_TASK_EMBEDDER_OPTIONS
#define GDMP_TASK_EMBEDDER_OPTIONS

#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/tasks/cc/components/processors/embedder_options.h"

using namespace godot;
using namespace mediapipe::tasks::components::processors;

class MediaPipeEmbedderOptions : public RefCounted {
		GDCLASS(MediaPipeEmbedderOptions, RefCounted)

	private:
		EmbedderOptions options;

	protected:
		static void _bind_methods();

	public:
		void set_l2_normalize(bool p_l2_normalize);
		bool get_l2_normalize();

		void set_quantize(bool p_quantize);
		bool get_quantize();

		const EmbedderOptions &get_options();
};

#endif
