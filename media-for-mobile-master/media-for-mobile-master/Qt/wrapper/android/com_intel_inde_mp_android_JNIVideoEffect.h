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
#include "ProgressListenerCallback.h"
/* Header for class com_intel_inde_mp_android_JNIVideoEffect */

using namespace AMPLoader;
VideoEffectsStorage AMPLoader::videoEffectMap;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_intel_inde_mp_android_JNIVideoEffect
 * Method:    getSegmentJNI
 * Signature: (J)Lcom/intel/inde/mp/domain/Pair;
 */
JNIEXPORT jobject JNICALL Java_com_intel_inde_mp_android_JNIVideoEffect_getSegmentJNI
  (JNIEnv *, jobject, jlong thiz) {
      log.D("JNIVideoEffect","getSegment this=%lld", thiz);
      auto segment = callVideoEffect(videoEffectMap, log, thiz, &VideoEffectAndroid<NativeJNI_t>::getSegment);
      return segment->GetNativeObject();
}

/*
 * Class:     com_intel_inde_mp_android_JNIVideoEffect
 * Method:    startJNI
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIVideoEffect_startJNI
  (JNIEnv * , jobject, jlong thiz) {
      log.D("JNIVideoEffect","start() this=%lld", thiz);
      callVideoEffect(videoEffectMap, log, thiz, &VideoEffectAndroid<NativeJNI_t>::start);
}

/*
 * Class:     com_intel_inde_mp_android_JNIVideoEffect
 * Method:    applyEffectJNI
 * Signature: (JIJ[F)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIVideoEffect_applyEffectJNI
  (JNIEnv *, jobject, jlong thiz, jint textureid, jlong timeProgress, jfloatArray transformMatrix) {
      log.D("JNIVideoEffect","applyEffect this=%lld, id=%d, pts=%lld", thiz, textureid, timeProgress);
      callVideoEffect(videoEffectMap, log, thiz, &VideoEffectAndroid<NativeJNI_t>::applyEffect, textureid, timeProgress, transformMatrix);
}

/*
 * Class:     com_intel_inde_mp_android_JNIVideoEffect
 * Method:    setInputResolutionJNI
 * Signature: (JLcom/intel/inde/mp/domain/Resolution;)V
 */
JNIEXPORT void JNICALL Java_com_intel_inde_mp_android_JNIVideoEffect_setInputResolutionJNI
  (JNIEnv * , jobject, jlong thiz, jobject resolution) {
      log.D("JNIVideoEffect","setInputResolution() this=%lld resolution=%p", thiz, resolution);
      callVideoEffect(videoEffectMap, log, thiz, &VideoEffectAndroid<NativeJNI_t>::setInputResolution, resolution);
}

/*
 * Class:     com_intel_inde_mp_android_JNIVideoEffect
 * Method:    fitToCurrentSurfaceJNI
 * Signature: (JZ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_inde_mp_android_JNIVideoEffect_fitToCurrentSurfaceJNI
  (JNIEnv *, jobject, jlong thiz, jboolean shouldFit) {
      log.D("JNIVideoEffect","fitToCurrentSurface() this=%lld", thiz);
      return callVideoEffect(videoEffectMap, log, thiz, &VideoEffectAndroid<NativeJNI_t>::fitToCurrentSurface, shouldFit!=0);
}

#ifdef __cplusplus
}
#endif
