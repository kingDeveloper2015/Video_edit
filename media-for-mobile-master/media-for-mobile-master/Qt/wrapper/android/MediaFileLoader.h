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
    class MediaFileLoader:  public ConcreetClassLoader<JNI> {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        MediaFileLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            : base(jni, load, adaptor, "com/intel/inde/mp/MediaFile", methods){
            _jclass loadedClass = base::loadedClass;
            methods[econstructor0] = load.LoadConstructor(loadedClass, "(Lcom/intel/inde/mp/domain/MediaSource;)V");
            methods[egetMediaSource] = load.LoadMethod(loadedClass, "getMediaSource", "()Lcom/intel/inde/mp/domain/MediaSource;");
            methods[eaddSegment] = load.LoadMethod(loadedClass, "addSegment", "(Lcom/intel/inde/mp/domain/Pair;)V");
            methods[egetSegments] = load.LoadMethod(loadedClass, "getSegments", "()Ljava/util/Collection;");
            methods[einsertSegment] = load.LoadMethod(loadedClass, "insertSegment", "(ILcom/intel/inde/mp/domain/Pair;)V");
            methods[eremoveSegment] = load.LoadMethod(loadedClass, "removeSegment", "(I)V");
//            methods[egetVideoTracksCount] = load.LoadMethod(loadedClass, "getVideoTracksCount", "()I");
//            methods[egetAudioTracksCount] = load.LoadMethod(loadedClass, "getAudioTracksCount", "()I");
            methods[egetVideoFormat] = load.LoadMethod(loadedClass, "getVideoFormat", "(I)Lcom/intel/inde/mp/VideoFormat;");
            methods[egetAudioFormat] = load.LoadMethod(loadedClass, "getAudioFormat", "(I)Lcom/intel/inde/mp/AudioFormat;");
            methods[esetSelectedAudioTrack] = load.LoadMethod(loadedClass, "setSelectedAudioTrack", "(I)V");
            methods[egetDurationInMicroSec] = load.LoadMethod(loadedClass, "getDurationInMicroSec", "()J");
            methods[egetSegmentsDurationInMicroSec] = load.LoadMethod(loadedClass, "getSegmentsDurationInMicroSec", "()J");
            methods[estart] = load.LoadMethod(loadedClass, "start", "()V");
            methods[egetRotation] = load.LoadMethod(loadedClass, "getRotation", "()I");
            methods[egetFilePath] = load.LoadMethod(loadedClass, "getFilePath", "()Ljava/lang/String;");
            methods[egetFileDescriptor] = load.LoadMethod(loadedClass, "getFileDescriptor", "()Ljava/io/FileDescriptor;");
            methods[egetUri] = load.LoadMethod(loadedClass, "getUri", "()Lcom/intel/inde/mp/Uri;");
        }
    public:
        enum {
            econstructor0,
            egetMediaSource,
            eaddSegment,
            egetSegments,
            einsertSegment,
            eremoveSegment,
            egetVideoTracksCount,
            egetAudioTracksCount,
            egetVideoFormat,
            egetAudioFormat,
            esetSelectedAudioTrack,
            egetDurationInMicroSec,
            egetSegmentsDurationInMicroSec,
            estart,
            egetRotation,
            egetFilePath,
            egetFileDescriptor,
            egetUri,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
