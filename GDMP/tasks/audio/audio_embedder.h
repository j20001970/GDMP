#ifndef GDMP_TASK_AUDIO_AUDIO_EMBEDDER
#define GDMP_TASK_AUDIO_AUDIO_EMBEDDER

#include "Array.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"

#include "mediapipe/tasks/cc/audio/audio_embedder/audio_embedder.h"

#include "GDMP/tasks/audio/audio_task.h"
#include "GDMP/tasks/processors/embedder_options.h"

using namespace godot;
using namespace mediapipe::tasks::audio::audio_embedder;

class MediaPipeAudioEmbedder : public MediaPipeAudioTask {
		GDMP_AUDIO_TASK_CLASS(MediaPipeAudioEmbedder, AudioEmbedder)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeEmbedderOptions> embedder_options);
		Array embed(PoolRealArray audio_data, int num_channels, double audio_sample_rate);
		bool embed_async(PoolRealArray audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms);
};

#endif
