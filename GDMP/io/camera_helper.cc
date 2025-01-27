#include "camera_helper.h"

#include "godot_cpp/core/class_db.hpp"

CameraHelperImpl::CameraHelperImpl(MediaPipeCameraHelper *camera_helper) {
	this->camera_helper = camera_helper;
	WARN_PRINT_ONCE("MediaPipeCameraHelper has been deprecated and will be removed in the future. Please consider CameraServer-based methods for accessing camera frames.");
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

void MediaPipeCameraHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("permission_granted"), &MediaPipeCameraHelper::permission_granted);
	ClassDB::bind_method(D_METHOD("request_permission"), &MediaPipeCameraHelper::request_permission);
	ClassDB::bind_method(D_METHOD("set_mirrored", "value"), &MediaPipeCameraHelper::set_mirrored);
	ClassDB::bind_method(D_METHOD("set_gpu_resources", "gpu_resources"), &MediaPipeCameraHelper::set_gpu_resources);
	ClassDB::bind_method(D_METHOD("start", "index", "size"), &MediaPipeCameraHelper::start);
	ClassDB::bind_method(D_METHOD("close"), &MediaPipeCameraHelper::close);
	ADD_SIGNAL(MethodInfo("permission_result", PropertyInfo(Variant::BOOL, "granted")));
	ADD_SIGNAL(MethodInfo("new_frame", PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MediaPipeImage")));
	BIND_ENUM_CONSTANT(FACING_FRONT);
	BIND_ENUM_CONSTANT(FACING_BACK);
}

bool MediaPipeCameraHelper::permission_granted() {
	return impl->permission_granted();
}

void MediaPipeCameraHelper::request_permission() {
	impl->request_permission();
}

void MediaPipeCameraHelper::set_mirrored(bool value) {
	impl->set_mirrored(value);
}

void MediaPipeCameraHelper::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
	impl->set_gpu_resources(gpu_resources);
}

void MediaPipeCameraHelper::start(int index, Vector2 size) {
	impl->close();
	ERR_FAIL_COND(!permission_granted());
	impl->start(index, size);
}

void MediaPipeCameraHelper::close() {
	impl->close();
}
