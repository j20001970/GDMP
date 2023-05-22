#include "camera_helper.h"

CameraHelperImpl::CameraHelperImpl(MediaPipeCameraHelper *camera_helper) {
	this->camera_helper = camera_helper;
}

CameraHelperImpl::~CameraHelperImpl() {}

bool CameraHelperImpl::permission_granted() {
	return true;
}

void CameraHelperImpl::request_permission() {}

void CameraHelperImpl::set_mirrored(bool value) {}

void CameraHelperImpl::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {}

void CameraHelperImpl::start(int index, Vector2 size) {}

void CameraHelperImpl::close() {}

void MediaPipeCameraHelper::_register_methods() {
	register_method("permission_granted", &MediaPipeCameraHelper::permission_granted);
	register_method("request_permission", &MediaPipeCameraHelper::request_permission);
	register_method("set_mirrored", &MediaPipeCameraHelper::set_mirrored);
	register_method("set_gpu_resources", &MediaPipeCameraHelper::set_gpu_resources);
	register_method("start", &MediaPipeCameraHelper::start);
	register_method("close", &MediaPipeCameraHelper::close);
	register_signal<MediaPipeCameraHelper>("permission_result");
	register_signal<MediaPipeCameraHelper>("new_frame");
}
