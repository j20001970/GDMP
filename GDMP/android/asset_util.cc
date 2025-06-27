#include "asset_util.h"

#include "mediapipe/util/android/asset_manager_util.h"

bool initialize_asset_manager(JNIEnv *env) {
	jclass cls = env->FindClass("android/app/ActivityThread");
	jmethodID _current_activity_thread = env->GetStaticMethodID(cls, "currentActivityThread", "()Landroid/app/ActivityThread;");
	jobject activity_thread = env->CallStaticObjectMethod(cls, _current_activity_thread);
	jmethodID _get_application = env->GetMethodID(cls, "getApplication", "()Landroid/app/Application;");
	jobject context = env->CallObjectMethod(activity_thread, _get_application);
	mediapipe::AssetManager *asset_manager = Singleton<mediapipe::AssetManager>::get();
	return asset_manager->InitializeFromContext(env, context, "");
}
