#ifndef GDMP_TASK_LANDMARK
#define GDMP_TASK_LANDMARK

#include "Array.hpp"
#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/containers/landmark.h"

#include "GDMP/framework/packet.h"
#include "GDMP/proto/proto.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeLandmark : public Reference {
		GODOT_CLASS(MediaPipeLandmark, Reference)

	private:
		typedef mediapipe::Landmark ProtoType;

		Landmark landmark;

	public:
		static void _register_methods();
		static MediaPipeLandmark *_new(const Landmark &landmark);

		static ProtoType to_proto(const Landmark &landmark);

		void _init();

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
		Ref<MediaPipePacket> make_vector_proto_packet(Array array);
};

class MediaPipeNormalizedLandmark : public Reference {
		GODOT_CLASS(MediaPipeNormalizedLandmark, Reference)

	private:
		typedef mediapipe::NormalizedLandmark ProtoType;

		NormalizedLandmark landmark;

	public:
		static void _register_methods();
		static MediaPipeNormalizedLandmark *_new(const NormalizedLandmark &landmark);

		static ProtoType to_proto(const NormalizedLandmark &landmark);

		void _init();

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
		Ref<MediaPipePacket> make_vector_proto_packet(Array array);
};

class MediaPipeLandmarks : public Reference {
		GODOT_CLASS(MediaPipeLandmarks, Reference)

	private:
		typedef mediapipe::LandmarkList ProtoType;

		Landmarks landmarks;

	public:
		static void _register_methods();
		static MediaPipeLandmarks *_new(const Landmarks &landmarks);

		static ProtoType to_proto(const Landmarks &landmarks);

		void _init();

		Array get_landmarks() const;

		Ref<MediaPipeProto> get_proto();
		Ref<MediaPipePacket> make_vector_proto_packet(Array array);
};

class MediaPipeNormalizedLandmarks : public Reference {
		GODOT_CLASS(MediaPipeNormalizedLandmarks, Reference)

	private:
		typedef mediapipe::NormalizedLandmarkList ProtoType;

		NormalizedLandmarks landmarks;

	public:
		static void _register_methods();
		static MediaPipeNormalizedLandmarks *_new(const NormalizedLandmarks &landmarks);

		static ProtoType to_proto(const NormalizedLandmarks &landmarks);

		void _init();

		Array get_landmarks() const;

		Ref<MediaPipeProto> get_proto();
		Ref<MediaPipePacket> make_vector_proto_packet(Array array);
};

#endif
