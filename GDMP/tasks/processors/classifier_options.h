#ifndef GDMP_TASK_CLASSIFIER_OPTIONS
#define GDMP_TASK_CLASSIFIER_OPTIONS

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"

#include "mediapipe/tasks/cc/components/processors/classifier_options.h"

using namespace godot;
using namespace mediapipe::tasks::components::processors;

class MediaPipeClassifierOptions : public RefCounted {
		GDCLASS(MediaPipeClassifierOptions, RefCounted)

	private:
		ClassifierOptions options = {};

	protected:
		static void _bind_methods();

	public:
		void set_display_names_locale(const String &p_display_names_locale);
		String get_display_names_locale();

		void set_max_results(int p_max_results);
		int get_max_results();

		void set_score_threshold(float p_score_threshold);
		float get_score_threshold();

		void set_category_allowlist(PackedStringArray p_category_allowlist);
		PackedStringArray get_category_allowlist();

		void set_category_denylist(PackedStringArray p_category_denylist);
		PackedStringArray get_category_denylist();

		const ClassifierOptions &get_options();
};

#endif
