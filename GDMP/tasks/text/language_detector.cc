#include "language_detector.h"

void MediaPipeLanguageDetectorPrediction::_register_methods() {
	register_method("get_language_code", &MediaPipeLanguageDetectorPrediction::get_language_code);
	register_method("get_probability", &MediaPipeLanguageDetectorPrediction::get_probability);
}

void MediaPipeLanguageDetectorPrediction::_init() {}

MediaPipeLanguageDetectorPrediction *MediaPipeLanguageDetectorPrediction::_new(const LanguageDetectorPrediction &prediction) {
	MediaPipeLanguageDetectorPrediction *r = MediaPipeLanguageDetectorPrediction::_new();
	r->prediction = prediction;
	return r;
}

String MediaPipeLanguageDetectorPrediction::get_language_code() {
	return prediction.language_code.c_str();
}

float MediaPipeLanguageDetectorPrediction::get_probability() {
	return prediction.probability;
}

void MediaPipeLanguageDetector::_register_methods() {
	register_method("initialize", &MediaPipeLanguageDetector::initialize);
	register_method("detect", &MediaPipeLanguageDetector::detect);
}

void MediaPipeLanguageDetector::_init() {}

void MediaPipeLanguageDetector::_register_task() {
	register_class<MediaPipeLanguageDetectorPrediction>();
	register_class<MediaPipeLanguageDetector>();
}

bool MediaPipeLanguageDetector::initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeClassifierOptions> classifier_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(classifier_options.is_null(), false);
	auto options = std::make_unique<LanguageDetectorOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->classifier_options = classifier_options->get_options();
	auto create_task = LanguageDetector::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Array MediaPipeLanguageDetector::detect(const String &text) {
	Array detect_result;
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	auto result = task->Detect(text.utf8().get_data());
	if (result.ok()) {
		const LanguageDetectorResult &results = result.value();
		detect_result.resize(results.size());
		for (int i = 0; i < detect_result.size(); i++) {
			const LanguageDetectorPrediction &result = results[i];
			detect_result[i] = MediaPipeLanguageDetectorPrediction::_new(result);
		}
	} else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

GDMP_REGISTER_TASK(MediaPipeLanguageDetector, MediaPipeTextTask)
