#ifndef GDMP_TASK_CLASSIFICATION
#define GDMP_TASK_CLASSIFICATION

#include "Array.hpp"
#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/containers/classification_result.h"

#include "GDMP/tasks/containers/category.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeClassifications : public Reference {
		GODOT_CLASS(MediaPipeClassifications, Reference)

	private:
		Classifications classifications;

	public:
		static void _register_methods();
		static MediaPipeClassifications *_new(const Classifications &classifications);

		void _init();

		Array get_categories();
		int get_head_index();
		String get_head_name();

		bool has_head_name();
};

class MediaPipeClassificationResult : public Reference {
		GODOT_CLASS(MediaPipeClassificationResult, Reference)

	private:
		ClassificationResult result;

	public:
		static void _register_methods();
		static MediaPipeClassificationResult *_new(const ClassificationResult &result);

		void _init();

		Array get_classifications();
		uint64_t get_timestamp_ms();

		bool has_timestamp_ms();
};

#endif
