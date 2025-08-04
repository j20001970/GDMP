#ifndef GDMP_TASK_KEYPOINT
#define GDMP_TASK_KEYPOINT

#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"
#include "Vector2.hpp"

#include "mediapipe/framework/formats/location_data.pb.h"
#include "mediapipe/tasks/cc/components/containers/keypoint.h"

#include "GDMP/framework/packet.h"
#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeNormalizedKeypoint : public Reference {
		GODOT_CLASS(MediaPipeNormalizedKeypoint, Reference)

	private:
		typedef mediapipe::LocationData::RelativeKeypoint ProtoType;

		NormalizedKeypoint keypoint;

	public:
		static void _register_methods();
		static MediaPipeNormalizedKeypoint *_new(const NormalizedKeypoint &keypoint);

		static ProtoType to_proto(const NormalizedKeypoint &keypoint);

		void _init();

		float get_x() const;
		float get_y() const;
		Vector2 get_point() const;
		String get_label() const;
		float get_score() const;

		bool has_label() const;
		bool has_score() const;

		Ref<MediaPipeProto> get_proto();
		Ref<MediaPipePacket> make_vector_proto_packet(Array array);
};

#endif
