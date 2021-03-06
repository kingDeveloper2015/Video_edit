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
#include "../IProgressListener.h"
#include "JNIProgressListenerLoader.h"

namespace AMPLoader{

    template<class JNI>
    class ProgressListener : public IGetJNIObject<JNI> {
        REMAP_TYPES(JNI);

        JNI *pJni;
        MediaPack::IProgressListener *target;
        ObjectReference<JNI> progressListenerJNI;


    public:
        typedef JavaObjectsMap <JNI, ProgressListener<JNI>*> Storage;

        ProgressListener(JNI &jni, MediaPack::IProgressListener &target, Storage & storage, JNIProgressListenerLoader<JNI> & loader) 
            : pJni(&jni)
            , target(&target)
            , progressListenerJNI(loader.CreateInstance(JNIProgressListenerLoader<JNI>::econstructor0, (_jlong)this)) {

            storage[(typename JNI::_jlong)this] = this;
        }

        virtual _jobject GetNativeObject()
        {
            return progressListenerJNI.GetNativeObject();
        }

        void onMediaStart()
        {
            target->onMediaStart();
        }

        void onMediaProgress( float progress )
        {
            target->onMediaProgress(progress);
        }

        void onMediaDone()
        {
            target->onMediaDone();
        }

        void onMediaPause()
        {
            target->onMediaPause();
        }

        void onMediaStop()
        {
            target->onMediaStop();
        }

        void onError( const std::string &exception )
        {
            target->onError(exception);
        }
    };
}
