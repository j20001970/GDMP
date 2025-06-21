#include "embedding_result.h"

#include "godot_cpp/classes/engine.hpp"
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

MediaPipeEmbedding::MediaPipeEmbedding() {
	embedding.head_index = 0;
}

MediaPipeEmbedding::MediaPipeEmbedding(const Embedding &embedding) {
	this->embedding = embedding;
}

PackedFloat32Array MediaPipeEmbedding::get_float_embedding() const {
	PackedFloat32Array array;
	const std::vector<float> &float_embedding = embedding.float_embedding;
	array.resize(float_embedding.size());
	memcpy(array.ptrw(), float_embedding.data(), sizeof(float) * float_embedding.size());
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

TypedArray<MediaPipeEmbedding> MediaPipeEmbeddingResult::get_embeddings() const {
	TypedArray<MediaPipeEmbedding> array;
	array.resize(result.embeddings.size());
	for (int i = 0; i < array.size(); i++) {
		const Embedding &embedding = result.embeddings[i];
		array[i] = memnew(MediaPipeEmbedding(embedding));
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
