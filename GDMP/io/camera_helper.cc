#include "camera_helper.h"

#include "godot_cpp/core/class_db.hpp"

void MediaPipeCameraHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("permission_granted"), &MediaPipeCameraHelper::permission_granted);
	ClassDB::bind_method(D_METHOD("request_permission"), &MediaPipeCameraHelper::request_permission);
	ClassDB::bind_method(D_METHOD("set_graph"), &MediaPipeCameraHelper::set_graph);
	ClassDB::bind_method(D_METHOD("set_mirrored"), &MediaPipeCameraHelper::set_mirrored);
	ClassDB::bind_method(D_METHOD("start"), &MediaPipeCameraHelper::start);
	ClassDB::bind_method(D_METHOD("close"), &MediaPipeCameraHelper::close);
	ClassDB::bind_method(D_METHOD("set_gpu_resources"), &MediaPipeCameraHelper::set_gpu_resources);
	ADD_SIGNAL(MethodInfo("permission_result", PropertyInfo(Variant::BOOL, "granted")));
	BIND_ENUM_CONSTANT(FACING_FRONT);
	BIND_ENUM_CONSTANT(FACING_BACK);
}
