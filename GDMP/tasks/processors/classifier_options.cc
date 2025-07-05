#include "classifier_options.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeClassifierOptions::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_display_names_locale", "display_names_locale"), &MediaPipeClassifierOptions::set_display_names_locale);
	ClassDB::bind_method(D_METHOD("get_display_names_locale"), &MediaPipeClassifierOptions::get_display_names_locale);
	ClassDB::bind_method(D_METHOD("set_max_results", "max_results"), &MediaPipeClassifierOptions::set_max_results);
	ClassDB::bind_method(D_METHOD("get_max_results"), &MediaPipeClassifierOptions::get_max_results);
	ClassDB::bind_method(D_METHOD("set_score_threshold", "score_threshold"), &MediaPipeClassifierOptions::set_score_threshold);
	ClassDB::bind_method(D_METHOD("get_score_threshold"), &MediaPipeClassifierOptions::get_score_threshold);
	ClassDB::bind_method(D_METHOD("set_category_allowlist", "category_allowlist"), &MediaPipeClassifierOptions::set_category_allowlist);
	ClassDB::bind_method(D_METHOD("get_category_allowlist"), &MediaPipeClassifierOptions::get_category_allowlist);
	ClassDB::bind_method(D_METHOD("set_category_denylist", "category_denylist"), &MediaPipeClassifierOptions::set_category_denylist);
	ClassDB::bind_method(D_METHOD("get_category_denylist"), &MediaPipeClassifierOptions::get_category_denylist);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_names_locale"), "set_display_names_locale", "get_display_names_locale");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_results"), "set_max_results", "get_max_results");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score_threshold"), "set_score_threshold", "get_score_threshold");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "category_allowlist"), "set_category_allowlist", "get_category_allowlist");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "category_denylist"), "set_category_denylist", "get_category_denylist");
}

void MediaPipeClassifierOptions::set_display_names_locale(const String &p_display_names_locale) {
	options.display_names_locale = p_display_names_locale.utf8();
}

String MediaPipeClassifierOptions::get_display_names_locale() {
	return options.display_names_locale.c_str();
}

void MediaPipeClassifierOptions::set_max_results(int p_max_results) {
	options.max_results = p_max_results;
}

int MediaPipeClassifierOptions::get_max_results() {
	return options.max_results;
}

void MediaPipeClassifierOptions::set_score_threshold(float p_score_threshold) {
	options.score_threshold = p_score_threshold;
}

float MediaPipeClassifierOptions::get_score_threshold() {
	return options.score_threshold;
}

void MediaPipeClassifierOptions::set_category_allowlist(PackedStringArray p_category_allowlist) {
	options.category_allowlist.resize(p_category_allowlist.size());
	for (int i = 0; i < p_category_allowlist.size(); i++) {
		options.category_allowlist[i] = p_category_allowlist[i].utf8();
	}
}

PackedStringArray MediaPipeClassifierOptions::get_category_allowlist() {
	PackedStringArray category_allowlist;
	category_allowlist.resize(options.category_allowlist.size());
	for (int i = 0; i < category_allowlist.size(); i++) {
		category_allowlist[i] = options.category_allowlist[i].c_str();
	}
	return category_allowlist;
}

void MediaPipeClassifierOptions::set_category_denylist(PackedStringArray p_category_denylist) {
	options.category_denylist.resize(p_category_denylist.size());
	for (int i = 0; i < p_category_denylist.size(); i++) {
		options.category_denylist[i] = p_category_denylist[i].utf8();
	}
}

PackedStringArray MediaPipeClassifierOptions::get_category_denylist() {
	PackedStringArray category_denylist;
	category_denylist.resize(options.category_denylist.size());
	for (int i = 0; i < category_denylist.size(); i++) {
		category_denylist[i] = options.category_denylist[i].c_str();
	}
	return category_denylist;
}

const ClassifierOptions &MediaPipeClassifierOptions::get_options() {
	return options;
}
