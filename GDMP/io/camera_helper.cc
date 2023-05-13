#include "camera_helper.h"

void MediaPipeCameraHelper::_register_methods() {
	register_method("permission_granted", &MediaPipeCameraHelper::permission_granted);
	register_method("request_permission", &MediaPipeCameraHelper::request_permission);
	register_method("set_mirrored", &MediaPipeCameraHelper::set_mirrored);
	register_method("start", &MediaPipeCameraHelper::start);
	register_method("close", &MediaPipeCameraHelper::close);
	register_method("set_gpu_resources", &MediaPipeCameraHelper::set_gpu_resources);
	register_signal<MediaPipeCameraHelper>("permission_result");
	register_signal<MediaPipeCameraHelper>("new_frame");
}
