#ifndef GDMP_TASK_CLASSIFICATION
#define GDMP_TASK_CLASSIFICATION

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/components/containers/classification_result.h"

#include "GDMP/tasks/containers/category.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeClassifications : public RefCounted {
		GDCLASS(MediaPipeClassifications, RefCounted)

	private:
		Classifications classifications;

	protected:
		static void _bind_methods();

	public:
		MediaPipeClassifications();
		MediaPipeClassifications(const Classifications &classifications);

		TypedArray<MediaPipeCategory> get_categories();
		int get_head_index();
		String get_head_name();

		bool has_head_name();
};

class MediaPipeClassificationResult : public RefCounted {
		GDCLASS(MediaPipeClassificationResult, RefCounted)

	private:
		ClassificationResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeClassificationResult();
		MediaPipeClassificationResult(const ClassificationResult &result);

		TypedArray<MediaPipeClassifications> get_classifications();
		uint64_t get_timestamp_ms();

		bool has_timestamp_ms();
};

#endif
