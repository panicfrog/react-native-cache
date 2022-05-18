#include <jni.h>
#include "react-native-cache.h"
#include <jsi/jsi.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_reactnativecache_CacheModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
    return cache::multiply(a, b);
}

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_reactnativecache_CacheModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi) {
//}

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativecache_CacheModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi,
                                                    jstring path) {
    auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi);
    if (runtime) {
        const char* p = env->GetStringUTFChars(path, 0);
        cache::install(*runtime, p);
        env->ReleaseStringUTFChars(path, p);
    }
    // TODO: implement nativeInstall()
}
