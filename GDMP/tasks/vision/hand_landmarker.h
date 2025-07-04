#ifndef GDMP_TASK_VISION_HAND_LANDMARKER
#define GDMP_TASK_VISION_HAND_LANDMARKER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/vision/hand_landmarker/hand_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/landmark.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::hand_landmarker;

class MediaPipeHandLandmarkerResult : public RefCounted {
		GDCLASS(MediaPipeHandLandmarkerResult, RefCounted)

	private:
		HandLandmarkerResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeHandLandmarkerResult();
		MediaPipeHandLandmarkerResult(const HandLandmarkerResult &result);

		TypedArray<MediaPipeClassifications> get_handedness();
		TypedArray<MediaPipeNormalizedLandmarks> get_hand_landmarks();
		TypedArray<MediaPipeLandmarks> get_hand_world_landmarks();
};

class MediaPipeHandLandmarker : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeHandLandmarker, HandLandmarker)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
				int num_hands, float min_hand_detection_confidence, float min_hand_presence_confidence, float min_tracking_confidence);
		Ref<MediaPipeHandLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeHandLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
