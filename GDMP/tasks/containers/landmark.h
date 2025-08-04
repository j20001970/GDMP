#ifndef GDMP_TASK_LANDMARK
#define GDMP_TASK_LANDMARK

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/components/containers/landmark.h"

#include "GDMP/framework/packet.h"
#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeLandmark : public RefCounted {
		GDCLASS(MediaPipeLandmark, RefCounted)

	private:
		typedef mediapipe::Landmark ProtoType;

		Landmark landmark;

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const Landmark &landmark);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeLandmark> array);

		MediaPipeLandmark();
		MediaPipeLandmark(const Landmark &landmark);

		float get_x() const;
		float get_y() const;
		float get_z() const;
		float get_visibility() const;
		float get_presence() const;
		String get_name() const;

		bool has_visibility() const;
		bool has_presence() const;
		bool has_name() const;

		Ref<MediaPipeProto> get_proto();
};

class MediaPipeNormalizedLandmark : public RefCounted {
		GDCLASS(MediaPipeNormalizedLandmark, RefCounted)

	private:
		typedef mediapipe::NormalizedLandmark ProtoType;

		NormalizedLandmark landmark;

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const NormalizedLandmark &landmark);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeNormalizedLandmark> array);

		MediaPipeNormalizedLandmark();
		MediaPipeNormalizedLandmark(const NormalizedLandmark &landmark);

		float get_x() const;
		float get_y() const;
		float get_z() const;
		float get_visibility() const;
		float get_presence() const;
		String get_name() const;

		bool has_visibility() const;
		bool has_presence() const;
		bool has_name() const;

		Ref<MediaPipeProto> get_proto();
};

class MediaPipeLandmarks : public RefCounted {
		GDCLASS(MediaPipeLandmarks, RefCounted)

	private:
		typedef mediapipe::LandmarkList ProtoType;

		Landmarks landmarks;

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const Landmarks &landmarks);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeLandmarks> array);

		MediaPipeLandmarks();
		MediaPipeLandmarks(const Landmarks &landmarks);

		TypedArray<MediaPipeLandmark> get_landmarks() const;

		Ref<MediaPipeProto> get_proto();
};

class MediaPipeNormalizedLandmarks : public RefCounted {
		GDCLASS(MediaPipeNormalizedLandmarks, RefCounted)

	private:
		typedef mediapipe::NormalizedLandmarkList ProtoType;

		NormalizedLandmarks landmarks;

	protected:
		static void _bind_methods();

	public:
		static ProtoType to_proto(const NormalizedLandmarks &landmarks);
		static Ref<MediaPipePacket> make_vector_proto_packet(TypedArray<MediaPipeNormalizedLandmarks> array);

		MediaPipeNormalizedLandmarks();
		MediaPipeNormalizedLandmarks(const NormalizedLandmarks &landmarks);

		TypedArray<MediaPipeNormalizedLandmark> get_landmarks() const;

		Ref<MediaPipeProto> get_proto();
};

#endif
