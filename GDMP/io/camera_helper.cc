#include "camera_helper.h"

using namespace godot;

void CameraHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("permission_granted"), &CameraHelper::permission_granted);
	ClassDB::bind_method(D_METHOD("request_permission"), &CameraHelper::request_permission);
	ClassDB::bind_method(D_METHOD("set_graph"), &CameraHelper::set_graph);
	ClassDB::bind_method(D_METHOD("set_mirrored"), &CameraHelper::set_mirrored);
	ClassDB::bind_method(D_METHOD("start"), &CameraHelper::start);
	ClassDB::bind_method(D_METHOD("close"), &CameraHelper::close);
#if !MEDIAPIPE_DISABLE_GPU
	ClassDB::bind_method(D_METHOD("set_use_gpu"), &CameraHelper::set_use_gpu);
#endif
	ADD_SIGNAL(MethodInfo("permission_granted"));
	ADD_SIGNAL(MethodInfo("permission_denied"));
}
