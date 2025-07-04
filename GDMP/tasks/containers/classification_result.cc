#include "classification_result.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeClassifications::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_categories"), &MediaPipeClassifications::get_categories);
	ClassDB::bind_method(D_METHOD("get_head_index"), &MediaPipeClassifications::get_head_index);
	ClassDB::bind_method(D_METHOD("get_head_name"), &MediaPipeClassifications::get_head_name);
	ClassDB::bind_method(D_METHOD("has_head_name"), &MediaPipeClassifications::has_head_name);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "categories", PROPERTY_HINT_ARRAY_TYPE, MediaPipeCategory::get_class_static()), "", "get_categories");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "head_index"), "", "get_head_index");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "head_name"), "", "get_head_name");
}

MediaPipeClassifications::MediaPipeClassifications() {
	classifications.head_index = 0;
}

MediaPipeClassifications::MediaPipeClassifications(const Classifications &classifications) {
	this->classifications = classifications;
}

TypedArray<MediaPipeCategory> MediaPipeClassifications::get_categories() const {
	TypedArray<MediaPipeCategory> array;
	array.resize(classifications.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = classifications.categories[i];
		array[i] = memnew(MediaPipeCategory(category));
	}
	return array;
}

int MediaPipeClassifications::get_head_index() const {
	return classifications.head_index;
}

String MediaPipeClassifications::get_head_name() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return classifications.head_name.value_or("").c_str();
	}
	ERR_FAIL_COND_V(!has_head_name(), String());
	return classifications.head_name->c_str();
}

bool MediaPipeClassifications::has_head_name() const {
	return classifications.head_name.has_value();
}

void MediaPipeClassificationResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_classifications"), &MediaPipeClassificationResult::get_classifications);
	ClassDB::bind_method(D_METHOD("get_timestamp_ms"), &MediaPipeClassificationResult::get_timestamp_ms);
	ClassDB::bind_method(D_METHOD("has_timestamp_ms"), &MediaPipeClassificationResult::has_timestamp_ms);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "classifications", PROPERTY_HINT_ARRAY_TYPE, MediaPipeClassifications::get_class_static()), "", "get_classifications");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "timestamp_ms"), "", "get_timestamp_ms");
}

MediaPipeClassificationResult::MediaPipeClassificationResult() = default;

MediaPipeClassificationResult::MediaPipeClassificationResult(const ClassificationResult &result) {
	this->result = result;
}

TypedArray<MediaPipeClassifications> MediaPipeClassificationResult::get_classifications() const {
	TypedArray<MediaPipeClassifications> array;
	array.resize(result.classifications.size());
	for (int i = 0; i < array.size(); i++) {
		const Classifications &classifications = result.classifications[i];
		array[i] = memnew(MediaPipeClassifications(classifications));
	}
	return array;
}

uint64_t MediaPipeClassificationResult::get_timestamp_ms() const {
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		return result.timestamp_ms.value_or(0);
	}
	ERR_FAIL_COND_V(!has_timestamp_ms(), 0);
	return result.timestamp_ms.value();
}

bool MediaPipeClassificationResult::has_timestamp_ms() const {
	return result.timestamp_ms.has_value();
}
