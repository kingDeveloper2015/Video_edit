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
#include "JavaObjectToNativeObjectMap.h"
#include "PairLoader.h"
#include <memory>
#include "JNIVideoEffectLoader.h"
#include "TimeStampSegmentTmpl.h"
#include "ResolutionLoader.h"
#include "JavaArrayAdapter.h"
#include "IVideoEffectAndroid.h"

namespace AMPLoader {
    template <class JNI>
    class VideoEffectAndroid : public IGetJNIObject<JNI> {
        REMAP_TYPES(JNI);
        typedef JavaObjectsMap <JNI, VideoEffectAndroid<JNI>*> EffectsStorage;
        std::shared_ptr<IVideoEffectAndroid> target;
        JNI *jni;
        ClassLoadersFactory<JNI> & factory;
        std::shared_ptr<JNIVideoEffectLoader<JNI>> videoEffectloader;
        std::shared_ptr<PairLoader<JNI>> pairLoader;
        std::shared_ptr<ResolutionLoader<JNI>> resolutionLoader;

        ObjectReference<JNI> videoEffectLoaded;
        std::shared_ptr<TimeStampSegmentTmpl<JNI> >lastSegment;
    public:
        VideoEffectAndroid(JNI &jni, ClassLoadersFactory<JNI> & factory, std::shared_ptr<IVideoEffectAndroid> target, EffectsStorage &storage)
            : target (target)
            , jni(&jni)
            , factory(factory)
            , videoEffectloader(factory.template get<JNIVideoEffectLoader<JNI>>())
            , pairLoader(factory.template get<PairLoader<JNI>>())
            , resolutionLoader(factory.template get<ResolutionLoader<JNI>>())
            , videoEffectLoaded(videoEffectloader->CreateInstance(JNIVideoEffectLoader<JNI>::econstructor0, (_jlong)this)) {
         
            storage[(typename JNI::_jlong)this] = this;
        }

        virtual _jobject GetNativeObject() {
            return videoEffectLoaded.GetNativeObject();
        }

        std::shared_ptr<TimeStampSegmentTmpl<JNI>> getSegment() {
            auto segment = target->getSegment();
            jni->log.D("VideoEffect<JNI>", "target->getSegment() returned: %lld-%lld", segment.first, segment.second);
            lastSegment = std::make_shared<TimeStampSegmentTmpl<JNI>> (*jni, factory, segment);
            return lastSegment;
        }

        void start() {
            target->start();
        }

        void applyEffect( int inTextureId, MediaPack::TimeStamp timeProgress, _jfloatArray transformMatrix) {
            auto matrix = JavaArrayAdapter<JNI>(*jni).convert(transformMatrix);
            jni->log.D("VideoEffect<JNI>", "transform={%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f}"
                , matrix[0]
                , matrix[1]
                , matrix[2]
                , matrix[3]
                , matrix[4]
                , matrix[5]
                , matrix[6]
                , matrix[7]
                , matrix[8]
                , matrix[9]
                , matrix[10]
                , matrix[11]
                , matrix[12]
                , matrix[13]
                , matrix[14]
                , matrix[15]);
            target->applyEffect(inTextureId, timeProgress, &*matrix.begin());
        }

        void setInputResolution(_jobject resolution) {
            jni->log.D("VideoEffect<JNI>", "setInputResolution() resolution=0x%p", resolution);
            auto resoluTionFromJava = resolutionLoader->LoadInstance(resolution);
            
            target->setInputResolution(MediaPack::Resolution(
                resoluTionFromJava.CallIntMethod(ResolutionLoader<JNI>::ewidth), 
                resoluTionFromJava.CallIntMethod(ResolutionLoader<JNI>::eheight)));
        }

        bool fitToCurrentSurface( bool should ) {
            return target->fitToCurrentSurface(should);
        }
    };
}
