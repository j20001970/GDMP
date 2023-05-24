#include "category.h"

void MediaPipeCategory::_register_methods() {
	register_method("get_index", &MediaPipeCategory::get_index);
	register_method("get_score", &MediaPipeCategory::get_score);
	register_method("get_category_name", &MediaPipeCategory::get_category_name);
	register_method("get_display_name", &MediaPipeCategory::get_display_name);
	register_method("has_category_name", &MediaPipeCategory::has_category_name);
	register_method("has_display_name", &MediaPipeCategory::has_display_name);
}

MediaPipeCategory *MediaPipeCategory::_new(const Category &category) {
	MediaPipeCategory *c = MediaPipeCategory::_new();
	c->category = category;
	return c;
}

void MediaPipeCategory::_init() {}

int MediaPipeCategory::get_index() {
	return category.index;
}

float MediaPipeCategory::get_score() {
	return category.score;
}

String MediaPipeCategory::get_category_name() {
	if (!has_category_name())
		return String();
	return category.category_name->c_str();
}

String MediaPipeCategory::get_display_name() {
	if (!has_display_name())
		return String();
	return category.display_name->c_str();
}

bool MediaPipeCategory::has_category_name() {
	return category.category_name.has_value();
}

bool MediaPipeCategory::has_display_name() {
	return category.display_name.has_value();
}
