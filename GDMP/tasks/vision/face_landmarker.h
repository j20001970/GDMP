#ifndef GDMP_TASK_VISION_FACE_LANDMARKER
#define GDMP_TASK_VISION_FACE_LANDMARKER

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/projection.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "mediapipe/tasks/cc/vision/face_landmarker/face_landmarker.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/classification_result.h"
#include "GDMP/tasks/containers/landmark.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::face_landmarker;

class MediaPipeFaceLandmarkerResult : public RefCounted {
		GDCLASS(MediaPipeFaceLandmarkerResult, RefCounted)

	private:
		FaceLandmarkerResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeFaceLandmarkerResult();
		MediaPipeFaceLandmarkerResult(const FaceLandmarkerResult &result);

		TypedArray<MediaPipeNormalizedLandmarks> get_face_landmarks();
		TypedArray<MediaPipeClassifications> get_face_blendshapes();
		// FIXME TypedArray<Projection> fails with a compilation error
		Array get_facial_transformation_matrixes();

		bool has_face_blendshapes();
		bool has_facial_transformation_matrixes();
};

class MediaPipeFaceLandmarker : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeFaceLandmarker, FaceLandmarker)

	protected:
		static void _bind_methods();

	public:
		bool initialize(
				Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode,
				int num_faces, float min_face_detection_confidence, float min_face_presence_confidence, float min_tracking_confidence,
				bool output_face_blendshapes, bool output_facial_transformation_matrixes);
		Ref<MediaPipeFaceLandmarkerResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeFaceLandmarkerResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
