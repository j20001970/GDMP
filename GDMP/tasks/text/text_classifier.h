#ifndef GDMP_TASK_TEXT_TEXT_CLASSIFIER
#define GDMP_TASK_TEXT_TEXT_CLASSIFIER

#include "Ref.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/text/text_classifier/text_classifier.h"

#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/processors/classifier_options.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::text_classifier;

class MediaPipeTextClassifier : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeTextClassifier, TextClassifier)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeClassifierOptions> classifier_options);
		Ref<MediaPipeClassificationResult> classify(const String &text);
};

#endif
