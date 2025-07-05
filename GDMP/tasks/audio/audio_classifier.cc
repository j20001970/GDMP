#include "audio_classifier.h"

#include "GDMP/tasks/containers/classification_result.h"

void MediaPipeAudioClassifier::_register_methods() {
	register_method("initialize", &MediaPipeAudioClassifier::initialize);
	register_method("classify", &MediaPipeAudioClassifier::classify);
	register_method("classify_async", &MediaPipeAudioClassifier::classify_async);
	register_signal<MediaPipeAudioClassifier>("result_callback");
}

void MediaPipeAudioClassifier::_register_task() {
	register_class<MediaPipeAudioClassifier>();
}

void MediaPipeAudioClassifier::_init() {}

bool MediaPipeAudioClassifier::initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeClassifierOptions> classifier_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(classifier_options.is_null(), false);
	auto options = std::make_unique<AudioClassifierOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->classifier_options = classifier_options->get_options();
	if (running_mode == RUNNING_MODE_AUDIO_STREAM)
		options->result_callback = [this](absl::StatusOr<AudioClassifierResult> result) {
			Ref<MediaPipeClassificationResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeClassificationResult::_new(result.value()));
			emit_signal("result_callback", callback_result);
		};
	auto create_task = AudioClassifier::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Array MediaPipeAudioClassifier::classify(PoolRealArray audio_data, int num_channels, double audio_sample_rate) {
	Array classify_result;
	ERR_FAIL_COND_V(task == nullptr, classify_result);
	mediapipe::Matrix audio_clip = make_audio_matrix(audio_data, num_channels);
	auto result = task->Classify(audio_clip, audio_sample_rate);
	if (result.ok()) {
		const std::vector<AudioClassifierResult> &results = result.value();
		classify_result.resize(results.size());
		for (int i = 0; i < classify_result.size(); i++) {
			const ClassificationResult &result = results[i];
			classify_result[i] = MediaPipeClassificationResult::_new(result);
		}
	} else
		ERR_PRINT(result.status().message().data());
	return classify_result;
}

bool MediaPipeAudioClassifier::classify_async(PoolRealArray audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms) {
	ERR_FAIL_COND_V(task == nullptr, false);
	mediapipe::Matrix audio_block = make_audio_matrix(audio_data, num_channels);
	auto result = task->ClassifyAsync(audio_block, audio_sample_rate, timestamp_ms);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeAudioClassifier, MediaPipeAudioTask);
