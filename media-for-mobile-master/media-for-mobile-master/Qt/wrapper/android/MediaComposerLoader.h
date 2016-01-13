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
    class MediaComposerLoader:  public ConcreetClassLoader<JNI> {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        MediaComposerLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
                : base(jni, load, adaptor, "com/intel/inde/mp/MediaComposer", methods){

            _jclass loadedClass = base::loadedClass;

            methods[econstructor0] = load.LoadConstructor(loadedClass, "(Lcom/intel/inde/mp/domain/IAndroidMediaObjectFactory;Lcom/intel/inde/mp/IProgressListener;)V");
            methods[eaddSourceFile] = load.LoadMethod(loadedClass, "addSourceFile", "(Ljava/lang/String;)V");
            methods[eaddSourceFile1] = load.LoadMethod(loadedClass, "addSourceFile", "(Ljava/io/FileDescriptor;)V");
            methods[eaddSourceFile2] = load.LoadMethod(loadedClass, "addSourceFile", "(Lcom/intel/inde/mp/Uri;)V");
            methods[eremoveSourceFile] = load.LoadMethod(loadedClass, "removeSourceFile", "(Lcom/intel/inde/mp/MediaFile;)V");
            methods[einsertSourceFile] = load.LoadMethod(loadedClass, "insertSourceFile", "(ILjava/lang/String;)V");
            methods[egetSourceFiles] = load.LoadMethod(loadedClass, "getSourceFiles", "()Ljava/util/List;");
            methods[esetTargetFile] = load.LoadMethod(loadedClass, "setTargetFile", "(Ljava/lang/String;)V");
            methods[egetDurationInMicroSec] = load.LoadMethod(loadedClass, "getDurationInMicroSec", "()J");
            methods[esetTargetVideoFormat] = load.LoadMethod(loadedClass, "setTargetVideoFormat", "(Lcom/intel/inde/mp/VideoFormat;)V");
            methods[egetTargetVideoFormat] = load.LoadMethod(loadedClass, "getTargetVideoFormat", "()Lcom/intel/inde/mp/VideoFormat;");
            methods[esetTargetAudioFormat] = load.LoadMethod(loadedClass, "setTargetAudioFormat", "(Lcom/intel/inde/mp/AudioFormat;)V");
            methods[egetTargetAudioFormat] = load.LoadMethod(loadedClass, "getTargetAudioFormat", "()Lcom/intel/inde/mp/AudioFormat;");
            methods[eaddVideoEffect] = load.LoadMethod(loadedClass, "addVideoEffect", "(Lcom/intel/inde/mp/IVideoEffect;)V");
            methods[eremoveVideoEffect] = load.LoadMethod(loadedClass, "removeVideoEffect", "(Lcom/intel/inde/mp/IVideoEffect;)V");
            methods[egetVideoEffector] = load.LoadMethod(loadedClass, "getVideoEffects", "()Ljava/util/Collection;");
            methods[eaddAudioEffect] = load.LoadMethod(loadedClass, "addAudioEffect", "(Lcom/intel/inde/mp/IAudioEffect;)V");
            methods[eremoveAudioEffect] = load.LoadMethod(loadedClass, "removeAudioEffect", "(Lcom/intel/inde/mp/IAudioEffect;)V");
            methods[egetAudioEffects] = load.LoadMethod(loadedClass, "getAudioEffects", "()Ljava/util/Collection;");
            methods[estart] = load.LoadMethod(loadedClass, "start", "()V");
            methods[epause] = load.LoadMethod(loadedClass, "pause", "()V");
            methods[eresume] = load.LoadMethod(loadedClass, "resume", "()V");
            methods[estop] = load.LoadMethod(loadedClass, "stop", "()V");
        }
    public:
        enum {
            econstructor0,
            eaddSourceFile,
            eaddSourceFile1,
            eaddSourceFile2,
            eremoveSourceFile,
            einsertSourceFile,
            egetSourceFiles,
            esetTargetFile,
            egetDurationInMicroSec,
            esetTargetVideoFormat,
            egetTargetVideoFormat,
            esetTargetAudioFormat,
            egetTargetAudioFormat,
            eaddVideoEffect,
            eremoveVideoEffect,
            egetVideoEffector,
            eaddAudioEffect,
            eremoveAudioEffect,
            egetAudioEffects,
            estart,
            epause,
            eresume,
            estop,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
