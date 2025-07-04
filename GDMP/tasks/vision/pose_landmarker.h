#ifndef GDMP_TASK_VISION_POSE_LANDMARKER
#define GDMP_TASK_VISION_POSE_LANDMARKER

#include "Array.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/tasks/cc/vision/pose_landmarker/pose_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::pose_landmarker;

class MediaPipePoseLandmarkerResult : public Reference {
		GODOT_CLASS(MediaPipePoseLandmarkerResult, Reference)

	private:
		PoseLandmarkerResult result;

	public:
		static void _register_methods();
		static MediaPipePoseLandmarkerResult *_new(const PoseLandmarkerResult &result);

		void _init();

		Array get_segmentation_masks();
		Array get_pose_landmarks();
		Array get_pose_world_landmarks();

		bool has_segmentation_masks();
};

class MediaPipePoseLandmarker : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipePoseLandmarker, PoseLandmarker)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				int num_poses, float min_pose_detection_confidence, float min_pose_presence_confidence, float min_tracking_confidence, bool output_segmentation_masks);
		Ref<MediaPipePoseLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipePoseLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
