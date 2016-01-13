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
    class AudioFormatAndroidLoader:  public ConcreetClassLoader<JNI> {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        AudioFormatAndroidLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            : base(jni, load, adaptor, "com/intel/inde/mp/android/AudioFormatAndroid", methods){
            _jclass loadedClass = base::loadedClass;

            methods[econstructor0] = load.LoadConstructor(loadedClass, "(Ljava/lang/String;II)V");
            methods[egetNativeFormat] = load.LoadMethod(loadedClass, "getNativeFormat", "()Landroid/media/MediaFormat;");
            methods[egetByteBuffer] = load.LoadMethod(loadedClass, "getByteBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;");
            methods[esetInteger] = load.LoadMethod(loadedClass, "setInteger", "(Ljava/lang/String;I)V");
            methods[egetInteger] = load.LoadMethod(loadedClass, "getInteger", "(Ljava/lang/String;)I");

            methods[egetAudioCodec] = load.LoadMethod(loadedClass, "getAudioCodec", "()Ljava/lang/String;");
            methods[egetAudioSampleRateInHz] = load.LoadMethod(loadedClass, "getAudioSampleRateInHz", "()I");
            methods[esetAudioSampleRateInHz] = load.LoadMethod(loadedClass, "setAudioSampleRateInHz", "(I)V");
            methods[egetAudioChannelCount] = load.LoadMethod(loadedClass, "getAudioChannelCount", "()I");
            methods[esetAudioChannelCount] = load.LoadMethod(loadedClass, "setAudioChannelCount", "(I)V");
            methods[egetAudioBitrateInBytes] = load.LoadMethod(loadedClass, "getAudioBitrateInBytes", "()I");
            methods[esetAudioBitrateInBytes] = load.LoadMethod(loadedClass, "setAudioBitrateInBytes", "(I)V");
            methods[esetKeyMaxInputSize] = load.LoadMethod(loadedClass, "setKeyMaxInputSize", "(I)V");
            methods[egetAudioProfile] = load.LoadMethod(loadedClass, "getAudioProfile", "()I");
            methods[esetAudioProfile] = load.LoadMethod(loadedClass, "setAudioProfile", "(I)V");
        }
    public:
        enum {
            econstructor0,
            egetNativeFormat,
            egetByteBuffer,
            esetInteger,
            egetInteger,

            egetAudioCodec,
            egetAudioSampleRateInHz,
            esetAudioSampleRateInHz,
            egetAudioChannelCount,
            esetAudioChannelCount,
            egetAudioBitrateInBytes,
            esetAudioBitrateInBytes,
            esetKeyMaxInputSize,
            egetAudioProfile,
            esetAudioProfile,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
