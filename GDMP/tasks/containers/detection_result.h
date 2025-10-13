#ifndef GDMP_TASK_DETECTION
#define GDMP_TASK_DETECTION

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/rect2i.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/components/containers/detection_result.h"

#include "GDMP/tasks/containers/category.h"
#include "GDMP/tasks/containers/keypoint.h"

#include "GDMP/framework/packet.h"
#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeDetection : public RefCounted {
		GDCLASS(MediaPipeDetection, RefCounted)

	private:
		typedef mediapipe::Detection ProtoType;

		Detection detection = {};

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const Detection &detection);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeDetection> array);

		MediaPipeDetection();
		MediaPipeDetection(const Detection &detection);

		TypedArray<MediaPipeCategory> get_categories() const;
		Rect2i get_bounding_box() const;
		TypedArray<MediaPipeNormalizedKeypoint> get_keypoints() const;

		bool has_keypoints() const;

		Ref<MediaPipeProto> get_proto();
};

class MediaPipeDetectionResult : public RefCounted {
		GDCLASS(MediaPipeDetectionResult, RefCounted)

	private:
		typedef mediapipe::DetectionList ProtoType;

		DetectionResult result = {};

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const DetectionResult &result);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeDetectionResult> array);

		MediaPipeDetectionResult();
		MediaPipeDetectionResult(const DetectionResult &result);

		TypedArray<MediaPipeDetection> get_detections() const;

		Ref<MediaPipeProto> get_proto();
};

#endif
