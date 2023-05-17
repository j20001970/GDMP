#include "category.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeCategory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_index"), &MediaPipeCategory::get_index);
	ClassDB::bind_method(D_METHOD("get_score"), &MediaPipeCategory::get_score);
	ClassDB::bind_method(D_METHOD("get_category_name"), &MediaPipeCategory::get_category_name);
	ClassDB::bind_method(D_METHOD("get_display_name"), &MediaPipeCategory::get_display_name);
	ClassDB::bind_method(D_METHOD("has_category_name"), &MediaPipeCategory::has_category_name);
	ClassDB::bind_method(D_METHOD("has_display_name"), &MediaPipeCategory::has_display_name);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "index"), "", "get_index");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "category_name"), "", "get_category_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "", "get_display_name");
}

MediaPipeCategory::MediaPipeCategory() = default;

MediaPipeCategory::MediaPipeCategory(const Category &category) {
	this->category = category;
}

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
