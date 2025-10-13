#ifndef GDMP_TASK_CLASSIFICATION
#define GDMP_TASK_CLASSIFICATION

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/components/containers/classification_result.h"

#include "GDMP/framework/packet.h"
#include "GDMP/proto/proto.h"
#include "GDMP/tasks/containers/category.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeClassifications : public RefCounted {
		GDCLASS(MediaPipeClassifications, RefCounted)

	private:
		typedef mediapipe::ClassificationList ProtoType;

		Classifications classifications = {};

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const Classifications &classifications);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeClassifications> array);

		MediaPipeClassifications();
		MediaPipeClassifications(const Classifications &classifications);

		TypedArray<MediaPipeCategory> get_categories() const;
		int get_head_index() const;
		String get_head_name() const;

		bool has_head_name() const;

		Ref<MediaPipeProto> get_proto();
};

class MediaPipeClassificationResult : public RefCounted {
		GDCLASS(MediaPipeClassificationResult, RefCounted)

	private:
		ClassificationResult result = {};

	protected:
		static void _bind_methods();

	public:
		MediaPipeClassificationResult();
		MediaPipeClassificationResult(const ClassificationResult &result);

		TypedArray<MediaPipeClassifications> get_classifications() const;
		uint64_t get_timestamp_ms() const;

		bool has_timestamp_ms() const;
};

#endif
