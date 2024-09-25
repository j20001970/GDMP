#include <jni.h>

#include "godot_cpp/core/error_macros.hpp"

JavaVM *jvm = NULL;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env;
	if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
		return JNI_ERR;
	jvm = vm;
	return JNI_VERSION_1_6;
}

bool get_env(JNIEnv **env) {
	jint res = jvm->GetEnv((void **)env, JNI_VERSION_1_6);
	if (res == JNI_EDETACHED) {
		res = jvm->AttachCurrentThread(env, NULL);
		ERR_FAIL_COND_V(res != JNI_OK, false);
		return true;
	}
	return false;
}
