#ifndef GDMP_TASK_VISION_HAND_LANDMARKER
#define GDMP_TASK_VISION_HAND_LANDMARKER

#include "Array.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/tasks/cc/vision/hand_landmarker/hand_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/landmark.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::hand_landmarker;

class MediaPipeHandLandmarkerResult : public Reference {
		GODOT_CLASS(MediaPipeHandLandmarkerResult, Reference)

	private:
		HandLandmarkerResult result;

	public:
		static void _register_methods();
		static MediaPipeHandLandmarkerResult *_new(const HandLandmarkerResult &result);

		void _init();

		Array get_handedness();
		Array get_hand_landmarks();
		Array get_hand_world_landmarks();
};

class MediaPipeHandLandmarker : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeHandLandmarker, HandLandmarker)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence);
		Ref<MediaPipeHandLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeHandLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
