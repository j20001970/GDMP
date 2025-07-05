#ifndef GDMP_TASK_TEXT_TEXT_EMBEDDER
#define GDMP_TASK_TEXT_TEXT_EMBEDDER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/text/text_embedder/text_embedder.h"

#include "GDMP/tasks/containers/embedding_result.h"
#include "GDMP/tasks/processors/embedder_options.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::text_embedder;

class MediaPipeTextEmbedder : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeTextEmbedder, TextEmbedder)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeEmbedderOptions> embedder_options);
		Ref<MediaPipeEmbeddingResult> embed(const String &text);
};

#endif
