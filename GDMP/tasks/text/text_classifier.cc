#include "text_classifier.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeTextClassifier::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "base_options", "classifier_options"), &MediaPipeTextClassifier::initialize);
	ClassDB::bind_method(D_METHOD("classify", "text"), &MediaPipeTextClassifier::classify);
}

void MediaPipeTextClassifier::_register_task() {
	ClassDB::register_class<MediaPipeTextClassifier>();
}

bool MediaPipeTextClassifier::initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeClassifierOptions> classifier_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(classifier_options.is_null(), false);
	auto options = std::make_unique<TextClassifierOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->classifier_options = classifier_options->get_options();
	auto create_task = TextClassifier::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeClassificationResult> MediaPipeTextClassifier::classify(const String &text) {
	Ref<MediaPipeClassificationResult> classify_result;
	ERR_FAIL_COND_V(task == nullptr, classify_result);
	auto result = task->Classify(text.utf8().get_data());
	if (result.ok())
		classify_result = Ref(memnew(MediaPipeClassificationResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
	return classify_result;
}

GDMP_REGISTER_TASK(MediaPipeTextClassifier, MediaPipeTextTask)
