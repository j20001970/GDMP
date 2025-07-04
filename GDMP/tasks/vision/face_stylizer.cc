#include "face_stylizer.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"

void MediaPipeFaceStylizer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "base_options"), &MediaPipeFaceStylizer::initialize);
	ClassDB::bind_method(D_METHOD("stylize", "image", "region_of_interest", "rotation_degrees"),
			&MediaPipeFaceStylizer::stylize, DEFVAL(Rect2()), DEFVAL(0));
}

void MediaPipeFaceStylizer::_register_task() {
	ClassDB::register_class<MediaPipeFaceStylizer>();
}
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
			stylize_result = Ref(memnew(MediaPipeImage(result->value())));
	} else
		ERR_PRINT(result.status().message().data());
	return stylize_result;
}

GDMP_REGISTER_TASK(MediaPipeFaceStylizer, MediaPipeVisionTask);
