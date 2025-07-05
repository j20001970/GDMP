#ifndef GDMP_TASK_AUDIO_AUDIO_EMBEDDER
#define GDMP_TASK_AUDIO_AUDIO_EMBEDDER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_float32_array.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/audio/audio_embedder/audio_embedder.h"

#include "GDMP/tasks/audio/audio_task.h"
#include "GDMP/tasks/containers/embedding_result.h"
#include "GDMP/tasks/processors/embedder_options.h"

using namespace godot;
using namespace mediapipe::tasks::audio::audio_embedder;

class MediaPipeAudioEmbedder : public MediaPipeAudioTask {
		GDMP_AUDIO_TASK_CLASS(MediaPipeAudioEmbedder, AudioEmbedder)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode, Ref<MediaPipeEmbedderOptions> embedder_options);
		TypedArray<MediaPipeEmbeddingResult> embed(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate);
		bool embed_async(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms);
};

#endif
