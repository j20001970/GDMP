#ifndef GDMP_TASK_AUDIO_AUDIO_CLASSIFIER
#define GDMP_TASK_AUDIO_AUDIO_CLASSIFIER

#include "Array.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"

#include "mediapipe/tasks/cc/audio/audio_classifier/audio_classifier.h"

#include "GDMP/tasks/audio/audio_task.h"
#include "GDMP/tasks/processors/classifier_options.h"

using namespace godot;
using namespace mediapipe::tasks::audio::audio_classifier;

class MediaPipeAudioClassifier : public MediaPipeAudioTask {
		GDMP_AUDIO_TASK_CLASS(MediaPipeAudioClassifier, AudioClassifier)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeClassifierOptions> classifier_options);
		Array classify(PoolRealArray audio_data, int num_channels, double audio_sample_rate);
		bool classify_async(PoolRealArray audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms);
};

#endif
