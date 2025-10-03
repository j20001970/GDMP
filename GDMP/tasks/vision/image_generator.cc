#include "image_generator.h"

void MediaPipeImageGeneratorResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_generated_image"), &MediaPipeImageGeneratorResult::get_generated_image);
	ClassDB::bind_method(D_METHOD("get_condition_image"), &MediaPipeImageGeneratorResult::get_condition_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generated_image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()), "", "get_generated_image");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "condition_image", PROPERTY_HINT_RESOURCE_TYPE, MediaPipeImage::get_class_static()), "", "get_condition_image");
}

MediaPipeImageGeneratorResult::MediaPipeImageGeneratorResult() = default;

MediaPipeImageGeneratorResult::MediaPipeImageGeneratorResult(const ImageGeneratorResult &result) {
	this->result = result;
}

Ref<MediaPipeImage> MediaPipeImageGeneratorResult::get_generated_image() {
	return memnew(MediaPipeImage(result.generated_image));
}

Ref<MediaPipeImage> MediaPipeImageGeneratorResult::get_condition_image() {
	Ref<MediaPipeImage> condition_image;
	if (result.condition_image.has_value()) {
		condition_image = Ref(memnew(MediaPipeImage(result.condition_image.value())));
	}
	return condition_image;
}

void MediaPipeImageGenerator::_bind_methods() {
}

void MediaPipeImageGenerator::_register_task() {
	ClassDB::register_class<MediaPipeImageGenerator>();
}

bool MediaPipeImageGenerator::initialize(const String &text2image_model_directory, const String &lora_weights_file_path) {
	auto options = std::make_unique<ImageGeneratorOptions>();
	options->text2image_model_directory = text2image_model_directory.utf8().get_data();
	if (!lora_weights_file_path.is_empty()) {
		options->lora_weights_file_path = lora_weights_file_path.utf8().get_data();
	}
	auto create_task = ImageGenerator::Create(std::move(options));
	if (create_task.ok())
		task = std::move(create_task.value());
	else
		ERR_PRINT(create_task.status().message().data());
	return create_task.ok();
}

Ref<MediaPipeImageGeneratorResult> MediaPipeImageGenerator::generate(const String &prompt, int iteration, int seed) {
	Ref<MediaPipeImageGeneratorResult> generate_result;
	auto result = task->Generate(prompt.utf8().get_data(), iteration, seed);
	if (result.ok())
		generate_result = Ref(memnew(MediaPipeImageGeneratorResult(result.value())));
	else
		ERR_PRINT(result.status().message().data());
}

GDMP_REGISTER_TASK(MediaPipeImageGenerator, MediaPipeVisionTask);
