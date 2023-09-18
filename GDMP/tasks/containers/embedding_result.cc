#include "embedding_result.h"

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

void MediaPipeEmbedding::_init() {}

PoolRealArray MediaPipeEmbedding::get_float_embedding() {
	PoolRealArray array;
	auto float_embedding = embedding.float_embedding;
	array.resize(float_embedding.size());
	for (int i = 0; i < float_embedding.size(); i++)
		array.set(i, float_embedding[i]);
	return array;
}

String MediaPipeEmbedding::get_quantized_embedding() {
	return embedding.quantized_embedding.c_str();
}

int MediaPipeEmbedding::get_head_index() {
	return embedding.head_index;
}

String MediaPipeEmbedding::get_head_name() {
	if (!has_head_name())
		return String();
	return embedding.head_name->c_str();
}

bool MediaPipeEmbedding::has_head_name() {
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

Array MediaPipeEmbeddingResult::get_embeddings() {
	Array array;
	auto embeddings = result.embeddings;
	array.resize(embeddings.size());
	for (int i = 0; i < embeddings.size(); i++)
		array[i] = MediaPipeEmbedding::_new(embeddings[i]);
	return array;
}

uint64_t MediaPipeEmbeddingResult::get_timestamp_ms() {
	if (!has_timestamp_ms())
		return 0;
	return result.timestamp_ms.value();
}

bool MediaPipeEmbeddingResult::has_timestamp_ms() {
	return result.timestamp_ms.has_value();
}
