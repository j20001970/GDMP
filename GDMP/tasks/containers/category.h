#ifndef GDMP_TASK_CATEGORY
#define GDMP_TASK_CATEGORY

#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/containers/category.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeCategory : public Reference {
		GODOT_CLASS(MediaPipeCategory, Reference)

	private:
		Category category;

	public:
		static void _register_methods();
		static MediaPipeCategory *_new(const Category &category);

		void _init();

		int get_index();
		float get_score();
		String get_category_name();
		String get_display_name();

		bool has_category_name();
		bool has_display_name();
};

#endif
