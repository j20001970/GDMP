#ifndef GDMP_TASK_TEXT_TEXT_CLASSIFIER
#define GDMP_TASK_TEXT_TEXT_CLASSIFIER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/text/text_classifier/text_classifier.h"

#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::text_classifier;

class MediaPipeTextClassifier : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeTextClassifier, TextClassifier)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options,
				const String &display_names_locale, int max_results, float score_threshold,
				PackedStringArray category_allowlist, PackedStringArray category_denylist);
		Ref<MediaPipeClassificationResult> classify(const String &text);
};

#endif
