#ifndef GDMP_TASK_EMBEDDING
#define GDMP_TASK_EMBEDDING

#include "Array.hpp"
#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/containers/embedding_result.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeEmbedding : public Reference {
		GODOT_CLASS(MediaPipeEmbedding, Reference)

	private:
		Embedding embedding;

	public:
		static void _register_methods();
		static MediaPipeEmbedding *_new(const Embedding &embedding);

		void _init();

		PoolRealArray get_float_embedding();
		String get_quantized_embedding();
		int get_head_index();
		String get_head_name();

		bool has_head_name();
};

class MediaPipeEmbeddingResult : public Reference {
		GODOT_CLASS(MediaPipeEmbeddingResult, Reference)

	private:
		EmbeddingResult result;

	public:
		static void _register_methods();
		static MediaPipeEmbeddingResult *_new(const EmbeddingResult &result);

		void _init();

		Array get_embeddings();
		uint64_t get_timestamp_ms();

		bool has_timestamp_ms();
};

#endif
