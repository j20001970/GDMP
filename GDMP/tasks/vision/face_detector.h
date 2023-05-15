#ifndef GDMP_TASK_VISION_FACE_DETECTOR
#define GDMP_TASK_VISION_FACE_DETECTOR

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/rect2.hpp"

#include "mediapipe/tasks/cc/vision/face_detector/face_detector.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/detection_result.h"
#include "GDMP/tasks/task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::face_detector;

class MediaPipeFaceDetector : public MediaPipeTask {
		GDMP_TASK_CLASS(MediaPipeFaceDetector, FaceDetector)

	protected:
		static void _bind_methods();

	public:
		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, VisionRunningMode running_mode, float min_detection_confidence, float min_suppression_threshold);
		Ref<MediaPipeDetectionResult> detect(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeDetectionResult> detect_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool detect_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
