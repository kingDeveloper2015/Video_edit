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
#include "ObjectReference.h"
#include "ConcreetClassLoader.h"
#include "TypeRemap.h"
namespace AMPLoader {

    template<class JNI>
    class AndroidMediaObjectFactoryLoader:  public ConcreetClassLoader<JNI>
    {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        AndroidMediaObjectFactoryLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            :base(jni, load, adaptor, "com/intel/inde/mp/android/AndroidMediaObjectFactory", methods){
            _jclass loadedClass = base::loadedClass;
            methods[econstructor0] = load.LoadConstructor(loadedClass, "(Landroid/content/Context;)V");
            methods[ecreateMediaSource] = load.LoadMethod(loadedClass, "createMediaSource", "(Ljava/lang/String;)Lcom/intel/inde/mp/domain/MediaSource;");
            methods[ecreateMediaSource1] = load.LoadMethod(loadedClass, "createMediaSource", "(Ljava/io/FileDescriptor;)Lcom/intel/inde/mp/domain/MediaSource;");
            methods[ecreateMediaSource2] = load.LoadMethod(loadedClass, "createMediaSource", "(Lcom/intel/inde/mp/Uri;)Lcom/intel/inde/mp/domain/MediaSource;");
            methods[ecreateVideoDecoder] = load.LoadMethod(loadedClass, "createVideoDecoder", "(Lcom/intel/inde/mp/domain/MediaFormat;)Lcom/intel/inde/mp/domain/VideoDecoder;");
            methods[ecreateVideoEncoder] = load.LoadMethod(loadedClass, "createVideoEncoder", "()Lcom/intel/inde/mp/domain/VideoEncoder;");
            methods[ecreateAudioDecoder] = load.LoadMethod(loadedClass, "createAudioDecoder", "()Lcom/intel/inde/mp/domain/AudioDecoder;");
            methods[ecreateAudioEncoder] = load.LoadMethod(loadedClass, "createAudioEncoder", "(Ljava/lang/String;)Lcom/intel/inde/mp/domain/AudioEncoder;");
            methods[ecreateSink] = load.LoadMethod(loadedClass, "createSink", "(Ljava/lang/String;Lcom/intel/inde/mp/IProgressListener;Lcom/intel/inde/mp/domain/ProgressTracker;)Lcom/intel/inde/mp/domain/Render;");
            methods[ecreateSink1] = load.LoadMethod(loadedClass, "createSink", "(Lcom/intel/inde/mp/StreamingParameters;Lcom/intel/inde/mp/IProgressListener;Lcom/intel/inde/mp/domain/ProgressTracker;)Lcom/intel/inde/mp/domain/Render;");
            methods[ecreateCaptureSource] = load.LoadMethod(loadedClass, "createCaptureSource", "()Lcom/intel/inde/mp/domain/ICaptureSource;");
            methods[ecreateVideoFormat] = load.LoadMethod(loadedClass, "createVideoFormat", "(Ljava/lang/String;II)Lcom/intel/inde/mp/domain/MediaFormat;");
            methods[ecreateAudioFormat] = load.LoadMethod(loadedClass, "createAudioFormat", "(Ljava/lang/String;II)Lcom/intel/inde/mp/domain/MediaFormat;");
            methods[ecreateVideoEffector] = load.LoadMethod(loadedClass, "createVideoEffector", "()Lcom/intel/inde/mp/domain/VideoEffector;");
            methods[ecreateEffectorSurface] = load.LoadMethod(loadedClass, "createEffectorSurface", "()Lcom/intel/inde/mp/domain/IEffectorSurface;");
            methods[ecreatePreviewRender] = load.LoadMethod(loadedClass, "createPreviewRender", "(Ljava/lang/Object;Ljava/lang/Object;)Lcom/intel/inde/mp/domain/IPreview;");
            methods[ecreateAudioEffects] = load.LoadMethod(loadedClass, "createAudioEffects", "()Lcom/intel/inde/mp/domain/AudioEffector;");
            methods[ecreateCameraSource] = load.LoadMethod(loadedClass, "createCameraSource", "()Lcom/intel/inde/mp/domain/ICameraSource;");
            methods[ecreateMicrophoneSource] = load.LoadMethod(loadedClass, "createMicrophoneSource", "()Lcom/intel/inde/mp/domain/IMicrophoneSource;");
            methods[ecreateAudioContentRecognition] = load.LoadMethod(loadedClass, "createAudioContentRecognition", "()Lcom/intel/inde/mp/domain/IAudioContentRecognition;");
            methods[egetCurrentEglContext] = load.LoadMethod(loadedClass, "getCurrentEglContext", "()Lcom/intel/inde/mp/domain/IEglContext;");
            methods[egetEglUtil] = load.LoadMethod(loadedClass, "getEglUtil", "()Lcom/intel/inde/mp/domain/graphics/IEglUtil;");
            methods[ecreateFrameBuffer] = load.LoadMethod(loadedClass, "createFrameBuffer", "()Lcom/intel/inde/mp/domain/IFrameBuffer;");
            methods[ecreateAudioDecoder1] = load.LoadMethod(loadedClass, "createAudioDecoder", "()Lcom/intel/inde/mp/domain/Plugin;");
        }
    public:
        enum {
            econstructor0,
            ecreateMediaSource,
            ecreateMediaSource1,
            ecreateMediaSource2,
            ecreateVideoDecoder,
            ecreateVideoEncoder,
            ecreateAudioDecoder,
            ecreateAudioEncoder,
            ecreateSink,
            ecreateSink1,
            ecreateCaptureSource,
            ecreateVideoFormat,
            ecreateAudioFormat,
            ecreateVideoEffector,
            ecreateEffectorSurface,
            ecreatePreviewRender,
            ecreateAudioEffects,
            ecreateCameraSource,
            ecreateMicrophoneSource,
            ecreateAudioContentRecognition,
            egetCurrentEglContext,
            egetEglUtil,
            ecreateFrameBuffer,
            ecreateAudioDecoder1,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
