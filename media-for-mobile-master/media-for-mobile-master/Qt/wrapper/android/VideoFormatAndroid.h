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
#include "VideoFormatAndroidLoader.h"
#include "../VideoFormat.h"

namespace AMPLoader{
    template <class JNI>
    class VideoFormatAndroid : public IGetJNIObject<JNI>, public MediaPack::IVideoFormat {
        REMAP_TYPES(JNI);
        typedef VideoFormatAndroidLoader<JNI> Loader;

        ObjectReference<JNI> videoFormatRef;
    public:
        VideoFormatAndroid(const std::string & mimeType, int width, int height, const std::shared_ptr<Loader> & formatLoader) 
            : videoFormatRef (formatLoader->CreateInstance(Loader::econstructor0, mimeType, width, height)) {
        }
        VideoFormatAndroid(_jobject videoFormat, const std::shared_ptr<Loader> & formatLoader) 
            : videoFormatRef (formatLoader->LoadInstance(videoFormat)) {
        }
        virtual void release() {
            delete this;
        }
        void setVideoBitRateInKBytes(int bitrate) {
            return videoFormatRef.CallVoidMethod(Loader::esetVideoBitRateInKBytes, bitrate);
        }
        void setVideoFrameRate(int frameRate) {
            return videoFormatRef.CallVoidMethod(Loader::esetVideoFrameRate, frameRate);
        }
        void setVideoIFrameInterval(int iFrameInterval) {
            return videoFormatRef.CallVoidMethod(Loader::esetVideoIFrameInterval, iFrameInterval);
        }
        virtual typename JNI::_jobject GetNativeObject() {
            return videoFormatRef.GetNativeObject();
        }
    };
}
