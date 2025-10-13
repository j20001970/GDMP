#ifndef GDMP_TASK_CLASSIFIER_OPTIONS
#define GDMP_TASK_CLASSIFIER_OPTIONS

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/processors/classifier_options.h"

using namespace godot;
using namespace mediapipe::tasks::components::processors;

class MediaPipeClassifierOptions : public Reference {
		GODOT_CLASS(MediaPipeClassifierOptions, Reference)

	private:
		ClassifierOptions options = {};

	public:
		static void _register_methods();

		void _init();

		void set_display_names_locale(String p_display_names_locale);
		String get_display_names_locale();

		void set_max_results(int p_max_results);
		int get_max_results();

		void set_score_threshold(float p_score_threshold);
		float get_score_threshold();

		void set_category_allowlist(PoolStringArray p_category_allowlist);
		PoolStringArray get_category_allowlist();

		void set_category_denylist(PoolStringArray p_category_denylist);
		PoolStringArray get_category_denylist();

		const ClassifierOptions &get_options();
};

#endif
