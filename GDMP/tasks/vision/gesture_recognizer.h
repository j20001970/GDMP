#ifndef GDMP_TASK_VISION_GESTURE_RECOGNIZER
#define GDMP_TASK_VISION_GESTURE_RECOGNIZER

#include "Array.hpp"
#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Rect2.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/vision/gesture_recognizer/gesture_recognizer.h"

#include "GDMP/framework/image.h"
#include "GDMP/proto/proto.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::gesture_recognizer;

class MediaPipeGestureRecognizerResult : public Reference {
		GODOT_CLASS(MediaPipeGestureRecognizerResult, Reference)

	private:
		GestureRecognizerResult result;

	public:
		static void _register_methods();
		static MediaPipeGestureRecognizerResult *_new(const GestureRecognizerResult &result);

		void _init();

		Array get_gestures();
		Array get_handedness();
		Array get_hand_landmarks();
		Array get_hand_world_landmarks();
};

class MediaPipeGestureRecognizer : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeGestureRecognizer, GestureRecognizer)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence);
		Ref<MediaPipeGestureRecognizerResult> recognize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeGestureRecognizerResult> recognize_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool recognize_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
