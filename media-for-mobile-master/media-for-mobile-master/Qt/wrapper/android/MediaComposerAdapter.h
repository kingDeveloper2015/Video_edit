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
#include "../MediaComposer.h"
#include "TypeRemap.h"
#include "AdapterBase.h"
#include "VideoEffectImpl.h"

namespace AMPLoader {
    template <class JNI>
    class MediaComposerAdapter : public MediaPack::IMediaComposer, public AdapterBase<JNI>
    {
        REMAP_TYPES(JNI);
        typedef AdapterBase<JNI> base;

        MediaPack::ProgressListener listener;
        AndroidMediaObjectFactory<JNI> factory;
        ProgressListener<JNI> progressListenerTmpl;
        MediaComposerAndroid<JNI> composer;

    public:
	    MediaComposerAdapter(const MediaPack::ProgressListener & listener)
            : listener(listener)
            , factory(*g_allAMPClasses->getFactory().get<FactoryLoader_t>())
            , progressListenerTmpl(base::jni, *(this->listener), progressListenerMap, *g_allAMPClasses->getFactory().template get<ProgressListenerLoader_t>())
            , composer(base::jni, videoEffectMap, factory, progressListenerTmpl, g_allAMPClasses->getFactory())
        {
	    }

        virtual void release()
        {
            delete this;
        }

        virtual void start()
        {
            composer.start();
        }

        virtual void stop()
        {
            composer.stop();
        }

        std::vector<MediaPack::MediaFile> getSourceFiles() const{
            return composer.getSourceFiles();
        }

        virtual void addSourceFile( const std::string &url )
        {
            composer.addSourceFile(getExternalMoviesFolder(url));
        }

        virtual void setTargetFile( const std::string &url )
        {
            composer.setTargetFile(getExternalMoviesFolder(url));
        }

        virtual void setTargetVideoFormat( const MediaPack::VideoFormat& videoFormat )
        {
            try {
                const VideoFormatAndroid<JNI> &vFormat = *dynamic_cast<const VideoFormatAndroid<JNI> *>(videoFormat.get());
                composer.setTargetVideoFormat(const_cast<VideoFormatAndroid<JNI> &>(vFormat));
            }catch(std::bad_cast & badCast){
                base::jni.log.E("MediaComposerAdapter", "input VideoFormat type is invalid: %d", badCast.what());
                throw;
            }
        }
        virtual void setTargetAudioFormat(const MediaPack::AudioFormat & audioFormat) {
            try {
                const AudioFormatAndroid<JNI> &aFormat = *dynamic_cast<const AudioFormatAndroid<JNI> *>(audioFormat.get());
                composer.setTargetAudioFormat(const_cast<AudioFormatAndroid<JNI> &>(aFormat));
            }catch(std::bad_cast & badCast){
                base::jni.log.E("MediaComposerAdapter", "input AudioFormat type is invalid: %d", badCast.what());
                throw;
            }
        }
        std::string getExternalMoviesFolder(const std::string & fileName) {

            _ClassLoader cl(base::jni);
            NativeJNI_t::_jclass EnvironmentClass = cl.LoadClass("android/os/Environment");
            NativeJNI_t::_jmethodID methods [] = {cl.LoadStaticMethod(EnvironmentClass, "getExternalStoragePublicDirectory",  "(Ljava/lang/String;)Ljava/io/File;")};
            NativeJNI_t::_jfieldID fields [] = {cl.LoadStaticField(EnvironmentClass, "DIRECTORY_MOVIES",  "Ljava/lang/String;")};

            StaticClassReference<NativeJNI_t> Environment(base::jni, EnvironmentClass, methods, fields, base::adapter);
            NativeJNI_t::_jobject file = Environment.CallStaticObjectMethod(0, Environment.GetStaticObjectField((int)0));

            NativeJNI_t::_jclass FileClass = cl.LoadClass("java/io/File");
            NativeJNI_t::_jmethodID methods2 [2] ;

            ConcreetClassLoader<NativeJNI_t> concreeLoader(base::jni, cl, base::adapter, "java/io/File", methods2);
            methods2[0] = cl.LoadConstructor(FileClass, "(Ljava/io/File;Ljava/lang/String;)V");
            methods2[1] = cl.LoadMethod(FileClass, "getPath", "()Ljava/lang/String;");

            ObjectReference<NativeJNI_t> fileObject(concreeLoader.CreateInstance(0, file, fileName.c_str()));

            return JavaStringAdapter<NativeJNI_t>(base::jni).convert(fileObject.CallObjectMethod(1));

        }

        void addVideoEffect(const MediaPack::VideoEffect &effect){
            try {
                const VideoEffectImpl &videoEffect = *dynamic_cast<const VideoEffectImpl *>(effect.get());
                composer.addVideoEffect(const_cast<VideoEffectImpl &>(videoEffect).get());
            }catch(std::bad_cast & badCast){
                base::jni.log.E("MediaComposerAdapter", "input AudioFormat type is invalid: %d", badCast.what());
                throw;
            }
        }
    };

}
