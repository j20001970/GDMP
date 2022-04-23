#include "camera_helper.h"

using namespace godot;

void CameraHelper::_register_methods() {
	register_method("_on_permission_result", &CameraHelper::_on_permission_result);
	register_method("permission_granted", &CameraHelper::permission_granted);
	register_method("request_permission", &CameraHelper::request_permission);
	register_method("set_graph", &CameraHelper::set_graph);
	register_method("start", &CameraHelper::start);
	register_method("close", &CameraHelper::close);
#if !MEDIAPIPE_DISABLE_GPU
	register_method("set_use_gpu", &CameraHelper::set_use_gpu);
#endif
	register_signal<CameraHelper>("permission_granted");
	register_signal<CameraHelper>("permission_denied");
}
