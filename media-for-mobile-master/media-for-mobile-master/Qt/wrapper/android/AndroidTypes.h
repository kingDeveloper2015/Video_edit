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

#pragma once
#include <jni.h>

#include "ClassLoader.h"
#include "AndroidLogger.h"
#include "MediaComposerAndroid.h"
#include "JavaTypeAdapter.h"
#include "NativeJNI.h"
#include "AndroidMediaObjectFactoryLoader.h"
#include "AndroidMediaObjectFactory.h"
#include "JavaStringAdapter.h"
#include "StaticClassReference.h"
#include "VideoFormatAndroid.h"
#include "JavaObjectToNativeObjectMap.h"
#include "ClassLoaderFactory.h"
#include "ClassLoadersFactoryAdapter.h"
#include "AudioFormatAndroid.h"

namespace AMPLoader {

    typedef NativeJNI<AndroidLogger, JavaVM, JNIEnv, jclass, jmethodID, jfieldID, jobject, jstring, jlong, jfloat, jfloatArray, jobjectArray> NativeJNI_t;
    typedef typename NativeJNI_t::_ClassLoader _ClassLoader;
    typedef typename NativeJNI_t::_TypeAdapter _TypeAdapter;

    typedef MediaComposerLoader<NativeJNI_t> MediaComposerLoader_t;
    typedef MediaComposerAndroid<NativeJNI_t> MediaComposer_t;

    typedef AndroidMediaObjectFactoryLoader<NativeJNI_t> FactoryLoader_t;
    typedef AndroidMediaObjectFactory<NativeJNI_t> Factory_t;

    typedef VideoFormatAndroidLoader<NativeJNI_t> VideoFormatLoader_t;
    typedef VideoFormatAndroid<NativeJNI_t> VideoFormat_t;

    typedef AudioFormatAndroidLoader<NativeJNI_t> AudioFormatLoader_t;
    typedef AudioFormatAndroid<NativeJNI_t> AudioFormat_t;

    typedef JavaObjectsMap <NativeJNI_t, ProgressListener<NativeJNI_t>*> ProgressListenerStorage;
    typedef JavaObjectsMap <NativeJNI_t, VideoEffectAndroid<NativeJNI_t>*> VideoEffectsStorage;

    typedef VideoEffectAndroid<NativeJNI_t> VideoEffectAndroid_t;

    typedef JNIProgressListenerLoader<NativeJNI_t> ProgressListenerLoader_t;

    extern ProgressListenerStorage progressListenerMap;
    extern VideoEffectsStorage videoEffectMap;
    extern JavaVM *s_javaVM;
    
    typedef ClassLoadersFactoryAdapter<NativeJNI_t> ClassLoadersFactoryAdapter_t;
    extern std::shared_ptr<ClassLoadersFactoryAdapter_t> g_allAMPClasses;
}

