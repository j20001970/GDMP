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

		int get_index() const;
		float get_score() const;
		String get_category_name() const;
		String get_display_name() const;

		bool has_category_name() const;
		bool has_display_name() const;
};

#endif
