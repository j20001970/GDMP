#include "classification_result.h"

void MediaPipeClassifications::_register_methods() {
	register_method("get_categories", &MediaPipeClassifications::get_categories);
	register_method("get_head_index", &MediaPipeClassifications::get_head_index);
	register_method("get_head_name", &MediaPipeClassifications::get_head_name);
	register_method("has_head_name", &MediaPipeClassifications::has_head_name);
}

MediaPipeClassifications *MediaPipeClassifications::_new(const Classifications &classifications) {
	MediaPipeClassifications *c = MediaPipeClassifications::_new();
	c->classifications = classifications;
	return c;
}

void MediaPipeClassifications::_init() {}

Array MediaPipeClassifications::get_categories() {
	Array array;
	auto categories = classifications.categories;
	array.resize(categories.size());
	for (int i = 0; i < categories.size(); i++)
		array[i] = MediaPipeCategory::_new(categories[i]);
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

void MediaPipeClassificationResult::_register_methods() {
	register_method("get_classifications", &MediaPipeClassificationResult::get_classifications);
	register_method("get_timestamp_ms", &MediaPipeClassificationResult::get_timestamp_ms);
	register_method("has_timestamp_ms", &MediaPipeClassificationResult::has_timestamp_ms);
}

MediaPipeClassificationResult *MediaPipeClassificationResult::_new(const ClassificationResult &result) {
	MediaPipeClassificationResult *r = MediaPipeClassificationResult::_new();
	r->result = result;
	return r;
}

void MediaPipeClassificationResult::_init() {}

Array MediaPipeClassificationResult::get_classifications() {
	Array array;
	auto classifications = result.classifications;
	array.resize(classifications.size());
	for (int i = 0; i < classifications.size(); i++)
		array[i] = MediaPipeClassifications::_new(classifications[i]);
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
