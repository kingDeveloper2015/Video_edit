// Copyright (c) 2014, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <jni.h>
#include "JavaStringAdapter.h"
#include "ProgressListenerCallback.h"

/* Header for class com_intel_inde_mp_android_JNIProgressListener */

namespace AMPLoader {
    ProgressListenerStorage progressListenerMap;
}

using namespace AMPLoader;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onMediaStartJNI
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onMediaStartJNI
  (JNIEnv * , jobject , jlong thiz) {
      
      log.D("JNIProgressListener","onMediaStart this=%lld", thiz);
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onMediaStart);
}

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onMediaProgressJNI
 * Signature: (Ljava/lang/Object;F)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onMediaProgressJNI
  (JNIEnv *, jobject, jlong thiz, jfloat progress){
      log.D("JNIProgressListener","onMediaProgress, this=%lld, progress=%f", thiz, progress);
      
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onMediaProgress, progress);
}

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onMediaDoneJNI
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onMediaDoneJNI
  (JNIEnv *, jobject, jlong thiz){
      log.D("JNIProgressListener","onMediaDoneJNI, this=%lld", thiz);
      
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onMediaDone);
}

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onMediaPauseJNI
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onMediaPauseJNI
  (JNIEnv *, jobject, jlong thiz){
      log.D("JNIProgressListener","onMediaPauseJNI, this=%lld", thiz);
      
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onMediaPause);
}

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onMediaStopJNI
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onMediaStopJNI
  (JNIEnv *, jobject, jlong thiz) {
      log.D("JNIProgressListener","onMediaStopJNI, this=%lld", thiz);
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onMediaStop);
}

/*
 * Class:     com_intel_inde_mp_android_JNIProgressListener
 * Method:    onErrorJNI
 * Signature: (Ljava/lang/Object;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIProgressListener_onErrorJNI
  (JNIEnv * env, jobject, jlong thiz, jstring errorString) {
    
      NativeJNI_t jni(*s_javaVM, log);
      std::string str = JavaStringAdapter<NativeJNI_t>(jni).convert(errorString);

      log.D("JNIProgressListener","onErrorJNI, this=%lld, error=%s", thiz, str.c_str());
      callProgressListener(progressListenerMap, log, thiz, &ProgressListener<NativeJNI_t>::onError, str);
}

#ifdef __cplusplus
}
#endif
