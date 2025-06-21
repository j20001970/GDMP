#include "embedding_result.h"

#include "Engine.hpp"

void MediaPipeEmbedding::_register_methods() {
	register_method("get_float_embedding", &MediaPipeEmbedding::get_float_embedding);
	register_method("get_quantized_embedding", &MediaPipeEmbedding::get_quantized_embedding);
	register_method("get_head_index", &MediaPipeEmbedding::get_head_index);
	register_method("get_head_name", &MediaPipeEmbedding::get_head_name);
	register_method("has_head_name", &MediaPipeEmbedding::has_head_name);
}

MediaPipeEmbedding *MediaPipeEmbedding::_new(const Embedding &embedding) {
	MediaPipeEmbedding *e = MediaPipeEmbedding::_new();
	e->embedding = embedding;
	return e;
}

void MediaPipeEmbedding::_init() {
	embedding.head_index = 0;
}

PoolRealArray MediaPipeEmbedding::get_float_embedding() const {
	PoolRealArray array;
	const std::vector<float> &float_embedding = embedding.float_embedding;
	array.resize(float_embedding.size());
	memcpy(array.write().ptr(), float_embedding.data(), sizeof(float) * float_embedding.size());
	return array;
}

String MediaPipeEmbedding::get_quantized_embedding() const {
	return embedding.quantized_embedding.c_str();
}

int MediaPipeEmbedding::get_head_index() const {
	return embedding.head_index;
}

String MediaPipeEmbedding::get_head_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return embedding.head_name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_head_name(), String());
	return embedding.head_name->c_str();
}

bool MediaPipeEmbedding::has_head_name() const {
	return embedding.head_name.has_value();
}

void MediaPipeEmbeddingResult::_register_methods() {
	register_method("get_embeddings", &MediaPipeEmbeddingResult::get_embeddings);
	register_method("get_timestamp_ms", &MediaPipeEmbeddingResult::get_timestamp_ms);
	register_method("has_timestamp_ms", &MediaPipeEmbeddingResult::has_timestamp_ms);
}

MediaPipeEmbeddingResult *MediaPipeEmbeddingResult::_new(const EmbeddingResult &result) {
	MediaPipeEmbeddingResult *r = MediaPipeEmbeddingResult::_new();
	r->result = result;
	return r;
}

void MediaPipeEmbeddingResult::_init() {}

Array MediaPipeEmbeddingResult::get_embeddings() const {
	Array array;
	array.resize(result.embeddings.size());
	for (int i = 0; i < array.size(); i++) {
		const Embedding &embedding = result.embeddings[i];
		array[i] = MediaPipeEmbedding::_new(embedding);
	}
	return array;
}

uint64_t MediaPipeEmbeddingResult::get_timestamp_ms() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return result.timestamp_ms.value_or(0);
	}
	ERR_FAIL_COND_V(!has_timestamp_ms(), 0);
	return result.timestamp_ms.value();
}

bool MediaPipeEmbeddingResult::has_timestamp_ms() const {
	return result.timestamp_ms.has_value();
}
