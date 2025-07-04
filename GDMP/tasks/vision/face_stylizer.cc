#include "face_stylizer.h"

void MediaPipeFaceStylizer::_register_methods() {
	register_method("initialize", &MediaPipeFaceStylizer::initialize);
	register_method("stylize", &MediaPipeFaceStylizer::stylize);
}

void MediaPipeFaceStylizer::_register_task() {
	register_class<MediaPipeFaceStylizer>();
}

void MediaPipeFaceStylizer::_init() {}

bool MediaPipeFaceStylizer::initialize(Ref<MediaPipeTaskBaseOptions> base_options) {
	ERR_FAIL_COND_V(base_options.is_null(), false);
	auto options = std::make_unique<FaceStylizerOptions>();
	options->base_options = std::move(*base_options->get_base_options());
	auto create_task = FaceStylizer::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeImage> MediaPipeFaceStylizer::stylize(Ref<MediaPipeImage> image, Rect2 region_of_interest, int rotation_degrees) {
	Ref<MediaPipeImage> stylize_result;
	ERR_FAIL_COND_V(image.is_null(), stylize_result);
	ERR_FAIL_COND_V(task == nullptr, stylize_result);
	ImageProcessingOptions image_processing_options = make_image_processing_options(region_of_interest, rotation_degrees);
	auto result = task->Stylize(image->get_mediapipe_image(), image_processing_options);
	if (result.ok()) {
		if (result->has_value())
			stylize_result = Ref(MediaPipeImage::_new(result->value()));
	} else
		ERR_PRINT(result.status().message().data());
	return stylize_result;
}

GDMP_REGISTER_TASK(MediaPipeFaceStylizer, MediaPipeVisionTask);
