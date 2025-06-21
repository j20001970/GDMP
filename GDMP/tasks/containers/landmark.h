#ifndef GDMP_TASK_LANDMARK
#define GDMP_TASK_LANDMARK

#include "Array.hpp"
#include "Godot.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/components/containers/landmark.h"

using namespace godot;
using namespace mediapipe::tasks::components::containers;

class MediaPipeLandmark : public Reference {
		GODOT_CLASS(MediaPipeLandmark, Reference)

	private:
		Landmark landmark;

	public:
		static void _register_methods();
		static MediaPipeLandmark *_new(const Landmark &landmark);

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
};

class MediaPipeNormalizedLandmark : public Reference {
		GODOT_CLASS(MediaPipeNormalizedLandmark, Reference)

	private:
		NormalizedLandmark landmark;

	public:
		static void _register_methods();
		static MediaPipeNormalizedLandmark *_new(const NormalizedLandmark &landmark);

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
};

class MediaPipeLandmarks : public Reference {
		GODOT_CLASS(MediaPipeLandmarks, Reference)

	private:
		Landmarks landmarks;

	public:
		static void _register_methods();
		static MediaPipeLandmarks *_new(const Landmarks &landmarks);

		void _init();

		Array get_landmarks() const;
};

class MediaPipeNormalizedLandmarks : public Reference {
		GODOT_CLASS(MediaPipeNormalizedLandmarks, Reference)

	private:
		NormalizedLandmarks landmarks;

	public:
		static void _register_methods();
		static MediaPipeNormalizedLandmarks *_new(const NormalizedLandmarks &landmarks);

		void _init();

		Array get_landmarks() const;
};

#endif
