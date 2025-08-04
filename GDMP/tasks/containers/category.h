#ifndef GDMP_TASK_CATEGORY
#define GDMP_TASK_CATEGORY

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"

#include "mediapipe/tasks/cc/components/containers/category.h"

#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeCategory : public RefCounted {
		GDCLASS(MediaPipeCategory, RefCounted)

	private:
		typedef mediapipe::Classification ProtoType;

		Category category;

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const Category &category);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeCategory> array);

		MediaPipeCategory();
		MediaPipeCategory(const Category &category);

		int get_index() const;
		float get_score() const;
		String get_category_name() const;
		String get_display_name() const;

		bool has_category_name() const;
		bool has_display_name() const;

		Ref<MediaPipeProto> get_proto();
};

#endif
