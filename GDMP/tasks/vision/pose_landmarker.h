#ifndef GDMP_TASK_VISION_POSE_LANDMARKER
#define GDMP_TASK_VISION_POSE_LANDMARKER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/vision/pose_landmarker/pose_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/landmark.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::pose_landmarker;

class MediaPipePoseLandmarkerResult : public RefCounted {
		GDCLASS(MediaPipePoseLandmarkerResult, RefCounted)

	private:
		PoseLandmarkerResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipePoseLandmarkerResult();
		MediaPipePoseLandmarkerResult(const PoseLandmarkerResult &result);

		TypedArray<MediaPipeImage> get_segmentation_masks();
		TypedArray<MediaPipeNormalizedLandmarks> get_pose_landmarks();
		TypedArray<MediaPipeLandmarks> get_pose_world_landmarks();

		bool has_segmentation_masks();
};

class MediaPipePoseLandmarker : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipePoseLandmarker, PoseLandmarker)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode,
				int num_poses, float min_pose_detection_confidence, float min_pose_presence_confidence, float min_tracking_confidence, bool output_segmentation_masks);
		Ref<MediaPipePoseLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipePoseLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
