#include "audio_embedder.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeAudioEmbedder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "base_options", "running_mode", "l2_normalize", "quantize"),
			&MediaPipeAudioEmbedder::initialize, DEFVAL(RUNNING_MODE_AUDIO_CLIPS), DEFVAL(false), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("embed", "audio_data", "num_channels", "audio_sample_rate"), &MediaPipeAudioEmbedder::embed);
	ClassDB::bind_method(D_METHOD("embed_async", "audio_data", "num_channels", "audio_sample_rate", "timestamp_ms"), &MediaPipeAudioEmbedder::embed_async);
	ADD_SIGNAL(MethodInfo("result_callback", PropertyInfo(Variant::ARRAY, "result", PROPERTY_HINT_ARRAY_TYPE, MediaPipeAudioEmbedder::get_class_static())));
}

void MediaPipeAudioEmbedder::_register_task() {
	ClassDB::register_class<MediaPipeAudioEmbedder>();
}

bool MediaPipeAudioEmbedder::initialize(Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode, bool l2_normalize, bool quantize) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<AudioEmbedderOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->embedder_options.l2_normalize = l2_normalize;
	options->embedder_options.quantize = quantize;
	if (running_mode == RUNNING_MODE_AUDIO_STREAM)
		options->result_callback = [this](absl::StatusOr<AudioEmbedderResult> result) {
			Ref<MediaPipeEmbeddingResult> callback_result;
			if (result.ok())
				callback_result = Ref(memnew(MediaPipeEmbeddingResult(result.value())));
			emit_signal("result_callback", callback_result);
		};
	auto create_task = AudioEmbedder::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

TypedArray<MediaPipeEmbeddingResult> MediaPipeAudioEmbedder::embed(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate) {
	TypedArray<MediaPipeEmbeddingResult> embed_result;
	ERR_FAIL_COND_V(task == nullptr, embed_result);
	mediapipe::Matrix audio_clip = make_audio_matrix(audio_data, num_channels);
	auto result = task->Embed(audio_clip, audio_sample_rate);
	if (result.ok()) {
		const std::vector<EmbeddingResult> &results = result.value();
		embed_result.resize(results.size());
		for (int i = 0; i < embed_result.size(); i++) {
			const EmbeddingResult &result = results[i];
			embed_result[i] = memnew(MediaPipeEmbeddingResult(result));
		}
	} else
		ERR_PRINT(result.status().message().data());
	return embed_result;
}

bool MediaPipeAudioEmbedder::embed_async(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms) {
	ERR_FAIL_COND_V(task == nullptr, false);
	mediapipe::Matrix audio_block = make_audio_matrix(audio_data, num_channels);
	auto result = task->EmbedAsync(audio_block, audio_sample_rate, timestamp_ms);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeAudioEmbedder, MediaPipeAudioTask);
