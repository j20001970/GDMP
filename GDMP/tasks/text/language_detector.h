#ifndef GDMP_TASK_TEXT_LANGUAGE_DETECTOR
#define GDMP_TASK_TEXT_LANGUAGE_DETECTOR

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/text/language_detector/language_detector.h"

#include "GDMP/tasks/processors/classifier_options.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::language_detector;

class MediaPipeLanguageDetectorPrediction : public RefCounted {
		GDCLASS(MediaPipeLanguageDetectorPrediction, RefCounted)

	private:
		LanguageDetectorPrediction prediction;

	protected:
		static void _bind_methods();

	public:
		MediaPipeLanguageDetectorPrediction();
		MediaPipeLanguageDetectorPrediction(const LanguageDetectorPrediction &prediction);

		String get_language_code();

		float get_probability();
};

class MediaPipeLanguageDetector : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeLanguageDetector, LanguageDetector)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeClassifierOptions> classifier_options);
		TypedArray<MediaPipeLanguageDetectorPrediction> detect(const String &text);
};

#endif
