#include "classification_result.h"

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

MediaPipeClassifications::MediaPipeClassifications() = default;

MediaPipeClassifications::MediaPipeClassifications(const Classifications &classifications) {
	this->classifications = classifications;
}

TypedArray<MediaPipeCategory> MediaPipeClassifications::get_categories() {
	TypedArray<MediaPipeCategory> array;
	auto categories = classifications.categories;
	array.resize(categories.size());
	for (int i = 0; i < categories.size(); i++)
		array[i] = memnew(MediaPipeCategory(categories[i]));
	return array;
}

int MediaPipeClassifications::get_head_index() {
	return classifications.head_index;
}

String MediaPipeClassifications::get_head_name() {
	if (!has_head_name())
		return String();
	return classifications.head_name->c_str();
}

bool MediaPipeClassifications::has_head_name() {
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

TypedArray<MediaPipeClassifications> MediaPipeClassificationResult::get_classifications() {
	TypedArray<MediaPipeClassifications> array;
	auto classifications = result.classifications;
	array.resize(classifications.size());
	for (int i = 0; i < classifications.size(); i++)
		array[i] = memnew(MediaPipeClassifications(classifications[i]));
	return array;
}

uint64_t MediaPipeClassificationResult::get_timestamp_ms() {
	if (!has_timestamp_ms())
		return 0;
	return result.timestamp_ms.value();
}

bool MediaPipeClassificationResult::has_timestamp_ms() {
	return result.timestamp_ms.has_value();
}
