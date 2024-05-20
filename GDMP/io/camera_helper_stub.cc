#include "camera_helper.h"

void MediaPipeCameraHelper::_init() {
	impl = std::make_unique<CameraHelperImpl>(this);
}
