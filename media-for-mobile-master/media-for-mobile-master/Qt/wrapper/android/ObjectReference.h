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

#include "../no_copy.h"
#include "JNIGlobalReferenceWrapper.h"
#include <memory>
#include "IGetJniObject.h"
#include "TypeRemap.h"

namespace AMPLoader {
    template <class JNI>
    class ObjectReference  : public IGetJNIObject<JNI> {
        REMAP_TYPES(JNI);

        JNI *pJni;
        _jmethodID *methods;
        std::shared_ptr<JNIGlobalReferenceWrapper<JNI> > createdInstance;
        _TypeAdapter adapter;

    public:
        ObjectReference(JNI &jni, _jmethodID* methods, _jobject instance, const _TypeAdapter &adapter) 
            : pJni(&jni)
            , methods(methods)
            , createdInstance(new JNIGlobalReferenceWrapper<JNI>(jni, instance))
            , adapter(adapter) {
        }

        _jobject CallObjectMethod(int methodId) {
            return CallObjectMethodV(methodId);
        }
        template <class T>
        _jobject CallObjectMethod(int methodId, T arg) {
            return CallObjectMethodV(methodId, adapter.convert(arg));
        }

        void CallVoidMethod(int methodId) {
            CallVoidMethodV(methodId);
        }
        template<class T>
        void CallVoidMethod(int methodId, T arg) {
            CallVoidMethodV(methodId, adapter.convert(arg));
        }
        template<class T, class T1>
        void CallVoidMethod(int methodId, T arg, T1 arg1) {
            CallVoidMethodV(methodId
                , adapter.convert(arg)
                , adapter.convert(arg1));
        }
        template<class T, class T1, class T2 >
        void CallVoidMethod(int methodId, T arg, T1 arg1, T2 arg2) {
            CallVoidMethodV(methodId
            , adapter.convert(arg)
            , adapter.convert(arg1)
            , adapter.convert(arg2));
        }

        int CallIntMethod(int methodId) {
           return pJni->RefreshEnv()->CallIntMethod(GetNativeObject(), methods[methodId]);
        }
        long long CallLongMethod(int methodId) {
            return pJni->RefreshEnv()->CallLongMethod(GetNativeObject(), methods[methodId]);
        }

        _jobject GetObjectField(int fieldId) {
            return pJni->RefreshEnv()->GetObjectField(GetNativeObject(), (_jfieldID)methods[fieldId]);
        }

        virtual _jobject GetNativeObject() {
            return createdInstance->GetNativeObject();
        }
    private:
        _jobject  CallObjectMethodV(int methoidID, ...) {
            va_list args;
            va_start(args, methoidID);

            return pJni->RefreshEnv()->CallObjectMethodV(GetNativeObject(), methods[methoidID], args);
        }
        void  CallVoidMethodV(int methoidID, ...) {
            va_list args;
            va_start(args, methoidID);

            pJni->RefreshEnv()->CallVoidMethodV(GetNativeObject(), methods[methoidID], args);
        }
    };
}
