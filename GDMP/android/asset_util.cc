#include "mediapipe/java/com/google/mediapipe/framework/jni/jni_util.h"
#include "mediapipe/util/android/asset_manager_util.h"

#define ANDROID_ASSET_UTIL_METHOD(METHOD_NAME) \
	Java_com_google_mediapipe_framework_AndroidAssetUtil_##METHOD_NAME

extern "C" JNIEXPORT jboolean JNICALL ANDROID_ASSET_UTIL_METHOD(nativeInitializeAssetManager)(
		JNIEnv *env, jclass clz,
		jobject android_context,
		jstring cache_dir_path) {
	mediapipe::AssetManager *asset_manager = Singleton<mediapipe::AssetManager>::get();
	return asset_manager->InitializeFromActivity(
			env, android_context,
			mediapipe::android::JStringToStdString(env, cache_dir_path));
}
