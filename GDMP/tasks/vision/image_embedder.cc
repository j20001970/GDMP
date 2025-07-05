#include "image_embedder.h"

void MediaPipeImageEmbedder::_register_methods() {
	register_method("initialize", &MediaPipeImageEmbedder::initialize);
	register_method("embed", &MediaPipeImageEmbedder::embed);
	register_method("embed_video", &MediaPipeImageEmbedder::embed_video);
	register_method("embed_async", &MediaPipeImageEmbedder::embed_async);
	register_signal<MediaPipeImageEmbedder>("result_callback");
}

void MediaPipeImageEmbedder::_register_task() {
	register_class<MediaPipeImageEmbedder>();
}

void MediaPipeImageEmbedder::_init() {}

bool MediaPipeImageEmbedder::initialize(Ref<MediaPipeTaskBaseOptions> base_options, int running_mode, Ref<MediaPipeEmbedderOptions> embedder_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	ERR_FAIL_COND_V(embedder_options.is_null(), false);
	auto options = std::make_unique<ImageEmbedderOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	options->running_mode = get_running_mode(running_mode);
	options->embedder_options = embedder_options->get_options();
	if (running_mode == RUNNING_MODE_LIVE_STREAM)
		options->result_callback = [this](absl::StatusOr<ImageEmbedderResult> result, const mediapipe::Image image, uint64_t timestamp_ms) {
			Ref<MediaPipeEmbeddingResult> callback_result;
			if (result.ok())
				callback_result = Ref(MediaPipeEmbeddingResult::_new(result.value()));
			Ref<MediaPipeImage> callback_image = MediaPipeImage::_new(image);
			emit_signal("result_callback", callback_result, callback_image, timestamp_ms);
		};
	auto create_task = ImageEmbedder::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeEmbeddingResult> MediaPipeImageEmbedder::embed(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeEmbeddingResult> embed_result;
	ERR_FAIL_COND_V(image.is_null(), embed_result);
	ERR_FAIL_COND_V(task == nullptr, embed_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Embed(image->get_mediapipe_image(), image_processing_options);
	if (result.ok())
		embed_result = Ref(MediaPipeEmbeddingResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return embed_result;
}

Ref<MediaPipeEmbeddingResult> MediaPipeImageEmbedder::embed_video(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeEmbeddingResult> embed_result;
	ERR_FAIL_COND_V(image.is_null(), embed_result);
	ERR_FAIL_COND_V(task == nullptr, embed_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->EmbedForVideo(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (result.ok())
		embed_result = Ref(MediaPipeEmbeddingResult::_new(result.value()));
	else
		ERR_PRINT(result.status().message().data());
	return embed_result;
}

bool MediaPipeImageEmbedder::embed_async(Ref<MediaPipeImage> image, uint64_t timestamp_ms, Rect2 region_of_interest, int rotation_degrees) {
	ERR_FAIL_COND_V(image.is_null(), false);
	ERR_FAIL_COND_V(task == nullptr, false);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->EmbedAsync(image->get_mediapipe_image(), timestamp_ms, image_processing_options);
	if (!result.ok())
		ERR_PRINT(result.message().data());
	return result.ok();
}

GDMP_REGISTER_TASK(MediaPipeImageEmbedder, MediaPipeVisionTask);
