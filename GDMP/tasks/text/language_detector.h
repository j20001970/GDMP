#ifndef GDMP_TASK_TEXT_LANGUAGE_DETECTOR
#define GDMP_TASK_TEXT_LANGUAGE_DETECTOR

#include "Array.hpp"
#include "Ref.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/text/language_detector/language_detector.h"

#include "GDMP/tasks/processors/classifier_options.h"
#include "GDMP/tasks/text/text_task.h"

using namespace godot;
using namespace mediapipe::tasks::text::language_detector;

class MediaPipeLanguageDetectorPrediction : public Reference {
		GODOT_CLASS(MediaPipeLanguageDetectorPrediction, Reference)

	private:
		LanguageDetectorPrediction prediction;

	public:
		static void _register_methods();
		static MediaPipeLanguageDetectorPrediction *_new(const LanguageDetectorPrediction &result);

		void _init();

		String get_language_code();

		float get_probability();
};

class MediaPipeLanguageDetector : public MediaPipeTextTask {
		GDMP_TEXT_TASK_CLASS(MediaPipeLanguageDetector, LanguageDetector)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, Ref<MediaPipeClassifierOptions> classifier_options);
		Array detect(const String &text);
};

#endif
