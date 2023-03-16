#include "camera_helper.h"

using namespace godot;

void MediaPipeCameraHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("permission_granted"), &MediaPipeCameraHelper::permission_granted);
	ClassDB::bind_method(D_METHOD("request_permission"), &MediaPipeCameraHelper::request_permission);
	ClassDB::bind_method(D_METHOD("set_graph"), &MediaPipeCameraHelper::set_graph);
	ClassDB::bind_method(D_METHOD("set_mirrored"), &MediaPipeCameraHelper::set_mirrored);
	ClassDB::bind_method(D_METHOD("start"), &MediaPipeCameraHelper::start);
	ClassDB::bind_method(D_METHOD("close"), &MediaPipeCameraHelper::close);
#if !MEDIAPIPE_DISABLE_GPU
	ClassDB::bind_method(D_METHOD("set_use_gpu"), &MediaPipeCameraHelper::set_use_gpu);
#endif
	ADD_SIGNAL(MethodInfo("permission_result", PropertyInfo(Variant::BOOL, "granted")));
}
