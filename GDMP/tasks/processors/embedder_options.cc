#include "embedder_options.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeEmbedderOptions::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_l2_normalize", "l2_normalize"), &MediaPipeEmbedderOptions::set_l2_normalize);
	ClassDB::bind_method(D_METHOD("get_l2_normalize"), &MediaPipeEmbedderOptions::get_l2_normalize);
	ClassDB::bind_method(D_METHOD("set_quantize", "quantize"), &MediaPipeEmbedderOptions::set_quantize);
	ClassDB::bind_method(D_METHOD("get_quantize"), &MediaPipeEmbedderOptions::get_quantize);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "l2_normalize"), "set_l2_normalize", "get_l2_normalize");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "quantize"), "set_quantize", "get_quantize");
}

void MediaPipeEmbedderOptions::set_l2_normalize(bool p_l2_normalize) {
	options.l2_normalize = p_l2_normalize;
}

bool MediaPipeEmbedderOptions::get_l2_normalize() {
	return options.quantize;
}

void MediaPipeEmbedderOptions::set_quantize(bool p_quantize) {
	options.quantize = p_quantize;
}

bool MediaPipeEmbedderOptions::get_quantize() {
	return options.quantize;
}

const EmbedderOptions &MediaPipeEmbedderOptions::get_options() {
	return options;
}
