#include "embedder_options.h"

void MediaPipeEmbedderOptions::_register_methods() {
	register_method("set_l2_normalize", &MediaPipeEmbedderOptions::set_l2_normalize);
	register_method("get_l2_normalize", &MediaPipeEmbedderOptions::get_l2_normalize);
	register_method("set_quantize", &MediaPipeEmbedderOptions::set_quantize);
	register_method("get_quantize", &MediaPipeEmbedderOptions::get_quantize);
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
