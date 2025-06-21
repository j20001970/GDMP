#ifndef GDMP_TASK_LANDMARK
#define GDMP_TASK_LANDMARK

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/components/containers/landmark.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeLandmark : public RefCounted {
		GDCLASS(MediaPipeLandmark, RefCounted)

	private:
		Landmark landmark;

	protected:
		static void _bind_methods();

	public:
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
};

class MediaPipeNormalizedLandmark : public RefCounted {
		GDCLASS(MediaPipeNormalizedLandmark, RefCounted)

	private:
		NormalizedLandmark landmark;

	protected:
		static void _bind_methods();

	public:
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
};

class MediaPipeLandmarks : public RefCounted {
		GDCLASS(MediaPipeLandmarks, RefCounted)

	private:
		Landmarks landmarks;

	protected:
		static void _bind_methods();

	public:
		MediaPipeLandmarks();
		MediaPipeLandmarks(const Landmarks &landmarks);

		TypedArray<MediaPipeLandmark> get_landmarks() const;
};

class MediaPipeNormalizedLandmarks : public RefCounted {
		GDCLASS(MediaPipeNormalizedLandmarks, RefCounted)

	private:
		NormalizedLandmarks landmarks;

	protected:
		static void _bind_methods();

	public:
		MediaPipeNormalizedLandmarks();
		MediaPipeNormalizedLandmarks(const NormalizedLandmarks &landmarks);

		TypedArray<MediaPipeNormalizedLandmark> get_landmarks() const;
};

#endif
