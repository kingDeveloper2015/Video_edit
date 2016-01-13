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
#include <memory>
#include "JNIStringWrapper.h"
#include "IGetJniObject.h"
#include "IAndroidMediaObjectFactory.h"
#include "JavaNull.h"
#include "JNIGlobalReferenceWrapper.h"
#include "../IProgressListener.h"
#include "VideoFormatAndroid.h"
#include "VideoEffectAndroid.h"
#include "ProgressListener.h"

namespace AMPLoader {

    template <class JNI, class T>
    struct JavaTypeTrait {
        JavaTypeTrait(JNI & /*jni*/) {}
        
        T convert( T arg) {
            return arg;
        }
    };

    template <class JNI>
    struct JavaTypeTrait<JNI, const char *> {
    private:
        typedef JNIStringWrapper<JNI> SPType;
        std::shared_ptr<SPType> sp;
        JNI & jni;
    public:
        JavaTypeTrait(JNI & jni)  : jni(jni) {
        }
        typename JNI::_jstring convert( const char * arg) {
            sp = std::make_shared<SPType>(jni, arg); 
            return sp->result();
        }
    };

    template <class JNI>
    struct JavaTypeTrait<JNI, std::string> : JavaTypeTrait<JNI, const char *> {
    public:
        JavaTypeTrait(JNI & jni)  :  JavaTypeTrait<JNI, const char *>(jni) {
        }
        typename JNI::_jstring convert( std::string arg) {
            return JavaTypeTrait<JNI, const char *>::convert(arg.c_str());
        }
    };


    template <class JNI>
    struct JavaTypeTrait<JNI, JavaNull> {
    private:
        typedef JNIGlobalReferenceWrapper<JNI> SPType;
        std::shared_ptr<SPType> sp;
        JNI & jni;
    public:
        typedef typename JNI::_jobject type;

        JavaTypeTrait(JNI & jni) : jni(jni) {
        }
        type convert(JavaNull /*arg*/) {
            sp = std::make_shared<SPType>(jni, jni.RefreshEnv()->NewGlobalRef(NULL)); 
            jni.log.D("JavaTypeTrait<JavaNull>", " result=%d", sp->GetNativeObject());
            return sp->GetNativeObject();
        }
    };

    template <class JNI>
    struct JavaTypeTrait<JNI, std::vector<float>* > {
        JNI & jni;
        JavaTypeTrait(JNI & jni) : jni(jni) {}
        typename JNI::_jfloatArray convert(std::vector<float>* arg) {
            auto resultArray = jni.RefreshEnv()->NewFloatArray(arg->size());
            jni.RefreshEnv()->SetFloatArrayRegion(resultArray, 0, arg->size(),&*arg->begin());
            return resultArray;
        }
    };


    template <class JNI>
    struct JavaTypeTrait<JNI, IGetJNIObject<JNI>* > {
        JNI * jni;
    public:
        JavaTypeTrait(JNI & jni) : 
            jni(&jni) {
        }
        typename JNI::_jobject convert(IGetJNIObject<JNI> * arg) {
            jni->log.D("JavaTypeTrait<IGetJNIObject*>", " result=%d", arg->GetNativeObject());
            return arg->GetNativeObject();
        }
    };

    #define DECL_JNIOBJECT(class_type)\
    template <class JNI>\
    struct JavaTypeTrait<JNI, class_type<JNI>* >  : JavaTypeTrait<JNI, IGetJNIObject<JNI>* > {\
        JavaTypeTrait(JNI&jni) : JavaTypeTrait<JNI, IGetJNIObject<JNI>* >(jni){}\
    };

    DECL_JNIOBJECT(TimeStampSegmentTmpl);
    DECL_JNIOBJECT(IAndroidMediaObjectFactory);
    DECL_JNIOBJECT(VideoFormatAndroid);
    DECL_JNIOBJECT(VideoEffectAndroid);
    DECL_JNIOBJECT(ProgressListener);
    DECL_JNIOBJECT(JNIGlobalReferenceWrapper);
    DECL_JNIOBJECT(ObjectReference);


}
