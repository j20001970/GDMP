#ifndef GDMP_ANDROID_JNI
#define GDMP_ANDROID_JNI

#include <jni.h>

extern JavaVM *jvm;

bool get_env(JNIEnv **env);

#endif
