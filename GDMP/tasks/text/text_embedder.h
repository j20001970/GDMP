#ifndef GDMP_TASK_TEXT_TEXT_EMBEDDER
#define GDMP_TASK_TEXT_TEXT_EMBEDDER

#include "Ref.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/text/text_embedder/text_embedder.h"

#include "GDMP/tasks/containers/embedding_result.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::text_embedder;

class MediaPipeTextEmbedder : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeTextEmbedder, TextEmbedder)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, bool l2_normalize, bool quantize);
		Ref<MediaPipeEmbeddingResult> embed(const String &text);
};

#endif
