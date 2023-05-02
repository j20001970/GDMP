#include "camera_helper.h"

void CameraHelper::_register_methods() {
	register_method("permission_granted", &CameraHelper::permission_granted);
	register_method("request_permission", &CameraHelper::request_permission);
	register_method("set_graph", &CameraHelper::set_graph);
	register_method("set_mirrored", &CameraHelper::set_mirrored);
	register_method("start", &CameraHelper::start);
	register_method("close", &CameraHelper::close);
	register_method("set_gpu_resources", &CameraHelper::set_gpu_resources);
	register_signal<CameraHelper>("permission_result");
}
