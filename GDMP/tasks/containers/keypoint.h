#ifndef GDMP_TASK_KEYPOINT
#define GDMP_TASK_KEYPOINT

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"

#include "mediapipe/tasks/cc/components/containers/keypoint.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeNormalizedKeypoint : public RefCounted {
		GDCLASS(MediaPipeNormalizedKeypoint, RefCounted)

	private:
		NormalizedKeypoint keypoint;

	protected:
		static void _bind_methods();

	public:
		MediaPipeNormalizedKeypoint();
		MediaPipeNormalizedKeypoint(const NormalizedKeypoint &keypoint);

		Vector2 get_point();
		String get_label();
		float get_score();

		bool has_label();
		bool has_score();
};

#endif
