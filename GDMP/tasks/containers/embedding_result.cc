#include "embedding_result.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeEmbedding::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_float_embedding"), &MediaPipeEmbedding::get_float_embedding);
	ClassDB::bind_method(D_METHOD("get_quantized_embedding"), &MediaPipeEmbedding::get_quantized_embedding);
	ClassDB::bind_method(D_METHOD("get_head_index"), &MediaPipeEmbedding::get_head_index);
	ClassDB::bind_method(D_METHOD("get_head_name"), &MediaPipeEmbedding::get_head_name);
	ClassDB::bind_method(D_METHOD("has_head_name"), &MediaPipeEmbedding::has_head_name);
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT32_ARRAY, "float_embedding"), "", "get_float_embedding");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "quantized_embedding"), "", "get_quantized_embedding");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "head_index"), "", "get_head_index");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "head_name"), "", "get_head_name");
}

MediaPipeEmbedding::MediaPipeEmbedding() = default;

MediaPipeEmbedding::MediaPipeEmbedding(const Embedding &embedding) {
	this->embedding = embedding;
}

PackedFloat32Array MediaPipeEmbedding::get_float_embedding() {
	PackedFloat32Array array;
	auto float_embedding = embedding.float_embedding;
	array.resize(float_embedding.size());
	for (int i = 0; i < float_embedding.size(); i++)
		array[i] = float_embedding[i];
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

void MediaPipeEmbeddingResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_embeddings"), &MediaPipeEmbeddingResult::get_embeddings);
	ClassDB::bind_method(D_METHOD("get_timestamp_ms"), &MediaPipeEmbeddingResult::get_timestamp_ms);
	ClassDB::bind_method(D_METHOD("has_timestamp_ms"), &MediaPipeEmbeddingResult::has_timestamp_ms);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "embeddings", PROPERTY_HINT_ARRAY_TYPE, MediaPipeEmbedding::get_class_static()), "", "get_embeddings");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "timestamp_ms"), "", "get_timestamp_ms");
}

MediaPipeEmbeddingResult::MediaPipeEmbeddingResult() = default;

MediaPipeEmbeddingResult::MediaPipeEmbeddingResult(const EmbeddingResult &result) {
	this->result = result;
}

TypedArray<MediaPipeEmbedding> MediaPipeEmbeddingResult::get_embeddings() {
	TypedArray<MediaPipeEmbedding> array;
	auto embeddings = result.embeddings;
	array.resize(embeddings.size());
	for (int i = 0; i < embeddings.size(); i++)
		array[i] = memnew(MediaPipeEmbedding(embeddings[i]));
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
