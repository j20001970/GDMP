#include "image_embedder.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeImageEmbedder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "base_options", "running_mode", "embedder_options"), &MediaPipeImageEmbedder::initialize);
	ClassDB::bind_method(D_METHOD("embed", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeImageEmbedder::embed, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("embed_video", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeImageEmbedder::embed_video, DEFVAL(Rect2()), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("embed_async", "image", "timestamp_ms", "region_of_interest", "rotation_degrees"),
			&MediaPipeImageEmbedder::embed_async, DEFVAL(Rect2()), DEFVAL(0));
	ADD_SIGNAL(MethodInfo("result_callback",
			PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeEmbeddingResult::get_class_static()),
			PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()),
			PropertyInfo(Variant::INT, "timestamp_ms")));
}

void MediaPipeImageEmbedder::_register_task() {
	ClassDB::register_class<MediaPipeImageEmbedder>();
}

bool MediaPipeImageEmbedder::initialize(Ref<MediaPipeTaskBaseOptions> base_options, RunningMode running_mode, Ref<MediaPipeEmbedderOptions> embedder_options) {
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
				callback_result = Ref(memnew(MediaPipeEmbeddingResult(result.value())));
			Ref<MediaPipeImage> callback_image = memnew(MediaPipeImage(image));
			call_deferred("emit_signal", "result_callback", callback_result, callback_image, timestamp_ms);
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
		embed_result = Ref(memnew(MediaPipeEmbeddingResult(result.value())));
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
		embed_result = Ref(memnew(MediaPipeEmbeddingResult(result.value())));
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
