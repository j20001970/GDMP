#ifndef GDMP_TASK_VISION_FACE_LANDMARKER
#define GDMP_TASK_VISION_FACE_LANDMARKER

#include "Array.hpp"
#include "Godot.hpp"
#include "Rect2.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/tasks/cc/vision/face_landmarker/face_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::face_landmarker;

class MediaPipeFaceLandmarkerResult : public Reference {
		GODOT_CLASS(MediaPipeFaceLandmarkerResult, Reference)

	private:
		FaceLandmarkerResult result;

	public:
		static void _register_methods();
		static MediaPipeFaceLandmarkerResult *_new(const FaceLandmarkerResult &result);

		void _init();

		Array get_face_landmarks();
		Array get_face_blendshapes();

		bool has_face_blendshapes();
};

class MediaPipeFaceLandmarker : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeFaceLandmarker, FaceLandmarker)

	public:
		static void _register_methods();

		void _init();

		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, int running_mode,
				int num_faces, float min_face_detection_confidence, float min_face_presence_confidence, float min_tracking_confidence,
				bool output_face_blendshapes);
		Ref<MediaPipeFaceLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeFaceLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
