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
    class VideoFormatAndroidLoader:  public ConcreetClassLoader<JNI>
    {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        VideoFormatAndroidLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            :base(jni, load, adaptor, "com/intel/inde/mp/android/VideoFormatAndroid", methods){
            _jclass loadedClass = base::loadedClass;
            methods[econstructor0] = load.LoadConstructor(loadedClass, "(Ljava/lang/String;II)V");
            methods[egetNativeFormat] = load.LoadMethod(loadedClass, "getNativeFormat", "()Landroid/media/MediaFormat;");
            methods[egetByteBuffer] = load.LoadMethod(loadedClass, "getByteBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;");
            methods[esetInteger] = load.LoadMethod(loadedClass, "setInteger", "(Ljava/lang/String;I)V");
            methods[egetInteger] = load.LoadMethod(loadedClass, "getInteger", "(Ljava/lang/String;)I");
          
            methods[egetVideoCodec] = load.LoadMethod(loadedClass, "getVideoCodec", "()Ljava/lang/String;");
            methods[esetVideoFrameSize] = load.LoadMethod(loadedClass, "setVideoFrameSize", "(II)V");
            methods[egetVideoFrameSize] = load.LoadMethod(loadedClass, "getVideoFrameSize", "()Lcom/intel/inde/mp/domain/Resolution;");
            methods[egetVideoBitRateInKBytes] = load.LoadMethod(loadedClass, "getVideoBitRateInKBytes", "()I");
            methods[esetVideoBitRateInKBytes] = load.LoadMethod(loadedClass, "setVideoBitRateInKBytes", "(I)V");
            methods[egetVideoFrameRate] = load.LoadMethod(loadedClass, "getVideoFrameRate", "()I");
            methods[esetVideoFrameRate] = load.LoadMethod(loadedClass, "setVideoFrameRate", "(I)V");
            methods[esetVideoIFrameInterval] = load.LoadMethod(loadedClass, "setVideoIFrameInterval", "(I)V");
            methods[egetVideoIFrameInterval] = load.LoadMethod(loadedClass, "getVideoIFrameInterval", "()I");
            methods[esetColorFormat] = load.LoadMethod(loadedClass, "setColorFormat", "(I)V");
        }
    public:
        enum {
            econstructor0,
            egetNativeFormat,
            egetByteBuffer,
            esetInteger,
            egetInteger,

            egetVideoCodec,
            esetVideoFrameSize,
            egetVideoFrameSize,
            egetVideoBitRateInKBytes,
            esetVideoBitRateInKBytes,
            egetVideoFrameRate,
            esetVideoFrameRate,
            esetVideoIFrameInterval,
            egetVideoIFrameInterval,
            esetColorFormat,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
