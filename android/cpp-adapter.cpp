#include <jni.h>
#include "react-native-cache.h"
#include <jsi/jsi.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_reactnativecache_CacheModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
    return cache::multiply(a, b);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativecache_CacheModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi) {
    auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi);
    if (runtime)
        cache::install(*runtime);
}
