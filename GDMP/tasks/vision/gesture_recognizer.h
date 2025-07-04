#ifndef GDMP_TASK_VISION_GESTURE_RECOGNIZER
#define GDMP_TASK_VISION_GESTURE_RECOGNIZER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/vision/gesture_recognizer/gesture_recognizer.h"

#include "GDMP/framework/image.h"
#include "GDMP/proto/proto.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::gesture_recognizer;

class MediaPipeGestureRecognizerResult : public RefCounted {
		GDCLASS(MediaPipeGestureRecognizerResult, RefCounted)

	private:
		GestureRecognizerResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeGestureRecognizerResult();
		MediaPipeGestureRecognizerResult(const GestureRecognizerResult &result);

		TypedArray<MediaPipeProto> get_gestures();
		TypedArray<MediaPipeProto> get_handedness();
		TypedArray<MediaPipeProto> get_hand_landmarks();
		TypedArray<MediaPipeProto> get_hand_world_landmarks();
};

class MediaPipeGestureRecognizer : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeGestureRecognizer, GestureRecognizer)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
				int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence);
		Ref<MediaPipeGestureRecognizerResult> recognize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeGestureRecognizerResult> recognize_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool recognize_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
