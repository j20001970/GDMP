#include "text_classifier.h"

void MediaPipeTextClassifier::_register_methods() {
	register_method("initialize", &MediaPipeTextClassifier::initialize);
	register_method("classify", &MediaPipeTextClassifier::classify);
}

void MediaPipeTextClassifier::_register_task() {
	register_class<MediaPipeTextClassifier>();
}

void MediaPipeTextClassifier::_init() {}

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
		classify_result = Ref(MediaPipeClassificationResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return classify_result;
}

GDMP_REGISTER_TASK(MediaPipeTextClassifier, MediaPipeTextTask)
