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
#include "../MediaFile.h"
#include "ClassLoaderFactory.h"
#include "MediaFileLoader.h"
#include "JavaStringAdapter.h"
#include "TimeStampSegmentTmpl.h"

namespace AMPLoader {
    template <class JNI>
    class MediaFileAndroid : public MediaPack::IMediaFile {
        REMAP_TYPES(JNI);
        typedef MediaFileLoader<JNI> Loader;
        ClassLoadersFactory<JNI> &loaders;
        JNI &jni;
        
        mutable ObjectReference<JNI> mediaFileRef;


    public:
        MediaFileAndroid (JNI &jni, _jobject instance, ClassLoadersFactory<JNI> &loaders)
            : jni(jni)
            , loaders(loaders)
            , mediaFileRef(loaders.template get<Loader>()->LoadInstance(instance)) {
        }
        virtual void release() {
            delete this;
        }

        virtual void addSegment( MediaPack::TimeSegment segment ) {
            TimeStampSegmentTmpl<JNI> segmentToJava(jni, loaders, segment);
            mediaFileRef.CallVoidMethod(Loader::eaddSegment, &segmentToJava);
        }

        virtual std::vector<MediaPack::TimeSegment>  getSegments() const {
            auto segmentCollection = mediaFileRef.CallObjectMethod(Loader::egetSegments);
            
            auto &pairLoader = *loaders.template get<PairLoader<JNI>>();
            auto &longLoader = *loaders.template get<LongLoader<JNI>>();
            auto &collectionLoader = *loaders.template get<CollectionLoader<JNI>>();

            ObjectReference<JNI> collection(collectionLoader.LoadInstance(segmentCollection));
            auto segmentsWithPair = JavaArrayAdapter<JNI>(jni).convert((_jobjectArray)collection.CallObjectMethod(CollectionLoader<JNI>::etoArray));
            std::vector<MediaPack::TimeSegment> segments;

            for(auto &item : segmentsWithPair) {

                ObjectReference<JNI> segment = pairLoader.LoadInstance(item);
                
                auto left = longLoader.LoadInstance(segment.GetObjectField(PairLoader<JNI>::eleft));
                auto leftJlong = left.CallLongMethod(LongLoader<JNI>::elongValue);

                auto right = longLoader.LoadInstance(segment.GetObjectField(PairLoader<JNI>::eright));
                auto rightJlong = right.CallLongMethod(LongLoader<JNI>::elongValue);

                segments.push_back(MediaPack::TimeSegment(leftJlong, rightJlong));
            }

            return segments;
        }

        virtual void insertSegment( int index, MediaPack::TimeSegment segment ) {
            TimeStampSegmentTmpl<JNI> segmentToJava(jni, loaders, segment);
            mediaFileRef.CallVoidMethod(Loader::einsertSegment, index, &segmentToJava);
        }

        virtual void removeSegment( int index ) {
            mediaFileRef.CallVoidMethod(Loader::eremoveSegment, index);
        }

        virtual int getVideoTracksCount() {
            return mediaFileRef.CallIntMethod(Loader::egetVideoTracksCount);
        }

        virtual int getAudioTracksCount() {
            return mediaFileRef.CallIntMethod(Loader::egetAudioTracksCount);
        }

        virtual MediaPack::VideoFormat getVideoFormat( int index ) {
            auto  videoFormat = mediaFileRef.CallObjectMethod(Loader::egetVideoFormat, index);
            auto  videoFormatLoader = loaders.template get<VideoFormatAndroidLoader<JNI>>();
            
            MediaPack::VideoFormat formatLoaded(new VideoFormatAndroid<JNI>(videoFormat, videoFormatLoader));
            return formatLoaded;
        }

        virtual MediaPack::AudioFormat getAudioFormat( int index ) {
            auto audioFormat = mediaFileRef.CallObjectMethod(Loader::egetAudioFormat, index);
            auto audioFormatLoader = loaders.template get<AudioFormatAndroidLoader<JNI>>();

            MediaPack::AudioFormat formatLoaded(new AudioFormatAndroid<JNI>(audioFormat, audioFormatLoader));
            return formatLoaded;
        }

        virtual MediaPack::TimeStamp getDurationInMicroSec() {
            return mediaFileRef.CallLongMethod(Loader::egetDurationInMicroSec);
        }

        virtual MediaPack::TimeStamp getSegmentsDurationInMicroSec() {
            return mediaFileRef.CallLongMethod(Loader::egetSegmentsDurationInMicroSec);
        }

        virtual int getRotation() {
            return mediaFileRef.CallIntMethod(Loader::egetRotation);
        }

        virtual std::string getFilePath() {
            auto getFileResult = mediaFileRef.CallObjectMethod(Loader::egetFilePath);
            return JavaStringAdapter<JNI>(jni).convert(getFileResult);
        }

    };
}
