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
#include "../IprogressListener.h"
#include "../MediaFile.h"
#include "ClassLoader.h"
#include "IAndroidMediaObjectFactory.h"
#include "MediaComposerLoader.h"
#include "VideoFormatAndroid.h"
#include "VideoEffectAndroid.h"
#include "ProgressListener.h"
#include "AudioFormatAndroid.h"
#include "MediaFileAndroid.h"
#include "ListLoader.h"
#include "IVideoEffectAndroid.h"

namespace AMPLoader {

    template <class JNI>
    class MediaComposerAndroid : private MediaPack::no_copy
    {
        REMAP_TYPES(JNI);
        typedef MediaComposerLoader<JNI> Loader;
        typedef VideoFormatAndroid<JNI> VideoFormat_t;
        typedef AudioFormatAndroid<JNI> AudioFormat_t;
        typedef VideoEffectAndroid<JNI> VideoEffect_t;
        typedef JavaObjectsMap <JNI, VideoEffectAndroid<JNI>*> VideoEffectsMap;
        typedef ProgressListener<JNI> ProgressListener_t;

        ClassLoadersFactory<JNI> &loaders;
        
        ProgressListener_t progressListener;
        mutable ObjectReference<JNI> mediaComposerRef;
        JNI &jni;
        VideoEffectsMap &effectsMap;
        std::shared_ptr<VideoEffect_t > effectPassedToComposer;

    public:
        MediaComposerAndroid(JNI & jni, 
                VideoEffectsMap &effectsMap, 
                IAndroidMediaObjectFactory<JNI> & factory, 
                ProgressListener_t progressListener,
                ClassLoadersFactory<JNI> &loaders) 
            : progressListener(progressListener)
            , mediaComposerRef (loaders.template get<Loader>()->CreateInstance(Loader::econstructor0, &factory, &progressListener))
            , jni(jni)
            , effectsMap(effectsMap) 
            , loaders(loaders) {
        }
        void addSourceFile(const std::string & filePath) {
            mediaComposerRef.CallVoidMethod(Loader::eaddSourceFile, filePath.c_str());
        }
        std::vector<MediaPack::MediaFile> getSourceFiles() const {
            auto jlist = mediaComposerRef.CallObjectMethod(Loader::egetSourceFiles);
            
            ObjectReference<JNI> javaList(loaders.template get<ListLoader<JNI>>()->LoadInstance(jlist));
            
            JavaArrayAdapter<JNI> adapter(jni);
            
            auto sourceFiles = adapter.convert((_jobjectArray)javaList.CallObjectMethod(ListLoader<JNI>::etoArray));
            
            std::vector<MediaPack::MediaFile> convertedList;

            for(auto &item : sourceFiles) {
                convertedList.push_back(MediaPack::MediaFile(new MediaFileAndroid<JNI>(jni, item, loaders)));
            }
            return convertedList;
        }
        void setTargetFile(const std::string & filePath) {
            mediaComposerRef.CallVoidMethod(Loader::esetTargetFile, filePath.c_str());
        }
        void setTargetVideoFormat(VideoFormat_t & format) {
            mediaComposerRef.CallVoidMethod(Loader::esetTargetVideoFormat, &format);
        }
        void setTargetAudioFormat(AudioFormat_t & format) {
            mediaComposerRef.CallVoidMethod(Loader::esetTargetAudioFormat, (IGetJNIObject<JNI>*)&format);
        }
        void addVideoEffect(std::shared_ptr<IVideoEffectAndroid> newEffect) {
            effectPassedToComposer.reset(new VideoEffect_t(jni, loaders, newEffect, effectsMap));
            mediaComposerRef.CallVoidMethod(Loader::eaddVideoEffect, effectPassedToComposer.get());
        }
        void start() {
            mediaComposerRef.CallVoidMethod(Loader::estart);
        }
        void stop() {
            mediaComposerRef.CallVoidMethod(Loader::estop);
        }
    };
}
