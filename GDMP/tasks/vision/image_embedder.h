#ifndef GDMP_TASK_VISION_IMAGE_EMBEDDER
#define GDMP_TASK_VISION_IMAGE_EMBEDDER

#include "mediapipe/tasks/cc/vision/image_embedder/image_embedder.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/containers/embedding_result.h"
#include "GDMP/tasks/processors/embedder_options.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::image_embedder;

class MediaPipeImageEmbedder : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeImageEmbedder, ImageEmbedder)

	public:
		static void _register_methods();

		void _init();

		bool initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeEmbedderOptions> embedder_options);
		Ref<MediaPipeEmbeddingResult> embed(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees);
		Ref<MediaPipeEmbeddingResult> embed_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
		bool embed_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees);
};

#endif
