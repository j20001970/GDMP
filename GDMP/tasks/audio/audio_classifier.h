#ifndef GDMP_TASK_AUDIO_AUDIO_CLASSIFIER
#define GDMP_TASK_AUDIO_AUDIO_CLASSIFIER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_float32_array.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/audio/audio_classifier/audio_classifier.h"

#include "GDMP/tasks/audio/audio_task.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/processors/classifier_options.h"

using namespace godot;
using namespace mediapipe::tasks::audio::audio_classifier;

class MediaPipeAudioClassifier : public MediaPipeAudioTask {
		GDMP_AUDIO_TASK_CLASS(MediaPipeAudioClassifier, AudioClassifier)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode, Ref<MediaPipeClassifierOptions> classifier_options);
		TypedArray<MediaPipeClassificationResult> classify(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate);
		bool classify_async(PackedFloat32Array audio_data, int num_channels, double audio_sample_rate, uint64_t timestamp_ms);
};

#endif
