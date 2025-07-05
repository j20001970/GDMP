#include "classifier_options.h"

void MediaPipeClassifierOptions::_register_methods() {
	register_method("set_display_names_locale", &MediaPipeClassifierOptions::set_display_names_locale);
	register_method("get_display_names_locale", &MediaPipeClassifierOptions::get_display_names_locale);
	register_method("set_max_results", &MediaPipeClassifierOptions::set_max_results);
	register_method("get_max_results", &MediaPipeClassifierOptions::get_max_results);
	register_method("set_score_threshold", &MediaPipeClassifierOptions::set_score_threshold);
	register_method("get_score_threshold", &MediaPipeClassifierOptions::get_score_threshold);
	register_method("set_category_allowlist", &MediaPipeClassifierOptions::set_category_allowlist);
	register_method("get_category_allowlist", &MediaPipeClassifierOptions::get_category_allowlist);
	register_method("set_category_denylist", &MediaPipeClassifierOptions::set_category_denylist);
	register_method("get_category_denylist", &MediaPipeClassifierOptions::get_category_denylist);
}

void MediaPipeClassifierOptions::_init() {}

void MediaPipeClassifierOptions::set_display_names_locale(String p_display_names_locale) {
	options.display_names_locale = p_display_names_locale.utf8().get_data();
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

void MediaPipeClassifierOptions::set_category_allowlist(PoolStringArray p_category_allowlist) {
	options.category_allowlist.resize(p_category_allowlist.size());
	for (int i = 0; i < p_category_allowlist.size(); i++) {
		options.category_allowlist[i] = p_category_allowlist[i].utf8().get_data();
	}
}

PoolStringArray MediaPipeClassifierOptions::get_category_allowlist() {
	PoolStringArray category_allowlist;
	category_allowlist.resize(options.category_allowlist.size());
	for (int i = 0; i < category_allowlist.size(); i++) {
		category_allowlist.set(i, options.category_allowlist[i].c_str());
	}
	return category_allowlist;
}

void MediaPipeClassifierOptions::set_category_denylist(PoolStringArray p_category_denylist) {
	options.category_denylist.resize(p_category_denylist.size());
	for (int i = 0; i < p_category_denylist.size(); i++) {
		options.category_denylist[i] = p_category_denylist[i].utf8().get_data();
	}
}

PoolStringArray MediaPipeClassifierOptions::get_category_denylist() {
	PoolStringArray category_denylist;
	category_denylist.resize(options.category_denylist.size());
	for (int i = 0; i < category_denylist.size(); i++) {
		category_denylist.set(i, options.category_denylist[i].c_str());
	}
	return category_denylist;
}

const ClassifierOptions &MediaPipeClassifierOptions::get_options() {
	return options;
}
