#ifndef GDMP_TASK_KEYPOINT
#define GDMP_TASK_KEYPOINT

#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"
#include "Vector2.hpp"

#include "mediapipe/tasks/cc/components/containers/keypoint.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeNormalizedKeypoint : public Reference {
		GODOT_CLASS(MediaPipeNormalizedKeypoint, Reference)

	private:
		NormalizedKeypoint keypoint;

	public:
		static void _register_methods();
		static MediaPipeNormalizedKeypoint *_new(const NormalizedKeypoint &keypoint);

		void _init();

		float get_x() const;
		float get_y() const;
		Vector2 get_point() const;
		String get_label() const;
		float get_score() const;

		bool has_label() const;
		bool has_score() const;
};

#endif
