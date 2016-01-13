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
#include "IVideoEffectAndroid.h"
#include "EglUtilLoader.h"
#include "VideoEffectLoader.h"
#include <memory>

namespace AMPLoader {

    template <class JNI>
    class VideoEffectBase : public IVideoEffectAndroid {
        REMAP_TYPES(JNI);
        _TypeAdapter adapter;
        _ClassLoader cl;
        ObjectReference<JNI> eglUtil;
        ObjectReference<JNI> videoEffectLoaded;
        std::shared_ptr<ObjectReference<JNI>> resolutionJNI;
        MediaPack::TimeSegment segment;
        bool fitToContext;
        JNI &jni;
    public:
        VideoEffectBase(JNI &jni)
            : jni(jni)
            , adapter(jni)
            , cl(jni)
            , eglUtil(
                g_allAMPClasses->getFactory().template get<EglUtilLoader<JNI>>()->CreateInstanceByStaticMethod(EglUtilLoader<JNI>::egetInstance))
            , videoEffectLoaded(
                g_allAMPClasses->getFactory().template get<VideoEffectLoader<JNI>>()->CreateInstance(VideoEffectLoader<JNI>::econstructor0, 0, (IGetJNIObject<JNI>*)&eglUtil)) 
            , fitToContext(true)
        {
        }

        void setFragmentShader(const std::string &shader) {
            videoEffectLoaded.CallVoidMethod(VideoEffectLoader<JNI>::esetFragmentShader, shader);
        }

        void setVertexShader(const std::string &shader){
            videoEffectLoaded.CallVoidMethod(VideoEffectLoader<JNI>::esetVertexShader, shader);
        }

        void setSegment(const MediaPack::TimeSegment &segment) {
            this->segment = segment;
        }

        virtual MediaPack::TimeSegment getSegment() {
            return segment;
        }

        virtual void start()
        {
            //implemented in Java completely
            videoEffectLoaded.CallVoidMethod(VideoEffectLoader<JNI>::estart);
        }

        virtual void applyEffect( int inTextureId, MediaPack::TimeStamp timeProgress, float *transformMatrix )
        {
            //implemented in Java completely
            std::vector<float> matrix(transformMatrix, transformMatrix + 16);
            videoEffectLoaded.CallVoidMethod(VideoEffectLoader<JNI>::eapplyEffect, inTextureId, timeProgress, &matrix);
        }

        virtual void setInputResolution( MediaPack::Resolution resolution )
        {
            jni.log.D("VideoEffectBase","setInputResolution(%dx%d)", resolution.width(), resolution.height());
            //implemented in Java completely
            resolutionJNI = std::make_shared<ObjectReference<JNI>>(
                g_allAMPClasses->getFactory().template get<ResolutionLoader<JNI>>()->CreateInstance(ResolutionLoader<JNI>::econstructor0, resolution.width(), resolution.height()));

            videoEffectLoaded.CallVoidMethod(VideoEffectLoader<JNI>::esetInputResolution, (IGetJNIObject<JNI>*)resolutionJNI.get());
        }

        virtual bool fitToCurrentSurface( bool should )
        {
            bool toRet = fitToContext;
            fitToContext = should;
            return toRet;
        }
    };
}