#ifndef GDMP_TASK_KEYPOINT
#define GDMP_TASK_KEYPOINT

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"

#include "mediapipe/framework/formats/location_data.pb.h"
#include "mediapipe/tasks/cc/components/containers/keypoint.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeNormalizedKeypoint : public RefCounted {
		GDCLASS(MediaPipeNormalizedKeypoint, RefCounted)

	private:
		typedef mediapipe::LocationData::RelativeKeypoint ProtoType;

		NormalizedKeypoint keypoint = {};

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const NormalizedKeypoint &keypoint);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeNormalizedKeypoint> array);

		MediaPipeNormalizedKeypoint();
		MediaPipeNormalizedKeypoint(const NormalizedKeypoint &keypoint);

		float get_x() const;
		float get_y() const;
		Vector2 get_point() const;
		String get_label() const;
		float get_score() const;

		bool has_label() const;
		bool has_score() const;

		Ref<MediaPipeProto> get_proto();
};

#endif
