#ifndef GDMP_TASK_CATEGORY
#define GDMP_TASK_CATEGORY

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/components/containers/category.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeCategory : public RefCounted {
		GDCLASS(MediaPipeCategory, RefCounted)

	private:
		Category category;

	protected:
		static void _bind_methods();

	public:
		MediaPipeCategory();
		MediaPipeCategory(const Category &category);

		int get_index() const;
		float get_score() const;
		String get_category_name() const;
		String get_display_name() const;

		bool has_category_name() const;
		bool has_display_name() const;
};

#endif
