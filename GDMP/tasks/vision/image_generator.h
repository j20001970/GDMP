#ifndef GDMP_TASK_VISION_IMAGE_GENERATOR
#define GDMP_TASK_VISION_IMAGE_GENERATOR

#include "mediapipe/tasks/cc/vision/image_generator/image_generator.h"

#include "GDMP/framework/image.h"
#include "GDMP/tasks/vision/vision_task.h"

using namespace godot;
using namespace mediapipe::tasks::vision::image_generator;

class MediaPipeImageGeneratorConditionOptions : public RefCounted {
};

class MediaPipeImageGeneratorResult : public RefCounted {
		GDCLASS(MediaPipeImageGeneratorResult, RefCounted)

	private:
		ImageGeneratorResult result;

	protected:
		static void _bind_methods();

	public:
		MediaPipeImageGeneratorResult();
		MediaPipeImageGeneratorResult(const ImageGeneratorResult &result);

		Ref<MediaPipeImage> get_generated_image();
		Ref<MediaPipeImage> get_condition_image();
};

class MediaPipeImageGenerator : public MediaPipeVisionTask {
		GDMP_VISION_TASK_CLASS(MediaPipeImageGenerator, ImageGenerator)

	protected:
		static void _bind_methods();

	public:
		enum ConditionType {
			CONDITION_TYPE_FACE = ConditionOptions::ConditionType::FACE,
			CONDITION_TYPE_EDGE = ConditionOptions::ConditionType::EDGE,
			CONDITION_TYPE_DEPTH = ConditionOptions::ConditionType::DEPTH,
		};

		bool initialize(const String &text2image_model_directory, const String &lora_weights_file_path);
		Ref<MediaPipeImageGeneratorResult> generate(const String &prompt, int iteration, int seed);
};

VARIANT_ENUM_CAST(MediaPipeImageGenerator::ConditionType)

#endif
