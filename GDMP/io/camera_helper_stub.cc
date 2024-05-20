#include "camera_helper.h"

MediaPipeCameraHelper::MediaPipeCameraHelper() {
	impl = std::make_unique<CameraHelperImpl>(this);
}

MediaPipeCameraHelper::~MediaPipeCameraHelper() = default;
