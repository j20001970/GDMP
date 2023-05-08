#include "jni.h"

JavaVM *jvm = NULL;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env;
	if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
		return JNI_ERR;
	jvm = vm;
	return JNI_VERSION_1_6;
}
