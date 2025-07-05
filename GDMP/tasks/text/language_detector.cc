#include "language_detector.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeLanguageDetectorPrediction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_language_code"), &MediaPipeLanguageDetectorPrediction::get_language_code);
	ClassDB::bind_method(D_METHOD("get_probability"), &MediaPipeLanguageDetectorPrediction::get_probability);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "language_code"), "", "get_language_code");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability"), "", "get_probability");
}

MediaPipeLanguageDetectorPrediction::MediaPipeLanguageDetectorPrediction() = default;

MediaPipeLanguageDetectorPrediction::MediaPipeLanguageDetectorPrediction(const LanguageDetectorPrediction &prediction) {
	this->prediction = prediction;
}

String MediaPipeLanguageDetectorPrediction::get_language_code() {
	return prediction.language_code.c_str();
}

float MediaPipeLanguageDetectorPrediction::get_probability() {
	return prediction.probability;
}

void MediaPipeLanguageDetector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "base_options", "classifier_options"), &MediaPipeLanguageDetector::initialize);
	ClassDB::bind_method(D_METHOD("detect", "text"), &MediaPipeLanguageDetector::detect);
}

void MediaPipeLanguageDetector::_register_task() {
	ClassDB::register_class<MediaPipeLanguageDetectorPrediction>();
	ClassDB::register_class<MediaPipeLanguageDetector>();
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

TypedArray<MediaPipeLanguageDetectorPrediction> MediaPipeLanguageDetector::detect(const String &text) {
	TypedArray<MediaPipeLanguageDetectorPrediction> detect_result;
	ERR_FAIL_COND_V(task == nullptr, detect_result);
	auto result = task->Detect(text.utf8().get_data());
	if (result.ok()) {
		const LanguageDetectorResult &results = result.value();
		detect_result.resize(results.size());
		for (int i = 0; i < detect_result.size(); i++) {
			const LanguageDetectorPrediction &result = results[i];
			detect_result[i] = memnew(MediaPipeLanguageDetectorPrediction(result));
		}
	} else
		ERR_PRINT(result.status().message().data());
	return detect_result;
}

GDMP_REGISTER_TASK(MediaPipeLanguageDetector, MediaPipeTextTask)
