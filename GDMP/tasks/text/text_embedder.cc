#include "text_embedder.h"

void MediaPipeTextEmbedder::_register_methods() {
	register_method("initialize", &MediaPipeTextEmbedder::initialize);
	register_method("embed", &MediaPipeTextEmbedder::embed);
}

void MediaPipeTextEmbedder::_register_task() {
	register_class<MediaPipeTextEmbedder>();
}

void MediaPipeTextEmbedder::_init() {}

bool MediaPipeTextEmbedder::initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeEmbedderOptions> embedder_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(embedder_options.is_null(), false);
	auto options = std::make_unique<TextEmbedderOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->embedder_options = embedder_options->get_options();
	auto create_task = TextEmbedder::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeEmbeddingResult> MediaPipeTextEmbedder::embed(const String &text) {
	Ref<MediaPipeEmbeddingResult> embed_result;
	ERR_FAIL_COND_V(task == nullptr, embed_result);
	auto result = task->Embed(text.utf8().get_data());
	if (result.ok())
		embed_result = Ref(MediaPipeEmbeddingResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return embed_result;
}
