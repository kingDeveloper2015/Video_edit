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
#include "ClassLoader.h"
#include "JavaTypeAdapter.h"
#include "ThreadLocalPort.h"

namespace AMPLoader {
    #define NO_CLASS
    #define COMA(X) X,
    #define LASTCOMA(X) X
    #define JNI_ENV_CLASSES(CLASS) \
        CLASS COMA(Logger) \
        CLASS COMA(JavaVM) \
        CLASS COMA(JNIEnv) \
        CLASS COMA(jclass) \
        CLASS COMA(jmethodID) \
        CLASS COMA(jfieldID) \
        CLASS COMA(jobject) \
        CLASS COMA(jstring) \
        CLASS COMA(jlong) \
        CLASS COMA(jfloat) \
        CLASS COMA(jfloatArray)\
        CLASS LASTCOMA(jobjectArray)
    

    template <JNI_ENV_CLASSES(class)>
    struct NativeJNIEnv {
        #undef  COMA
        #undef  LASTCOMA
        #define COMA(X) X _##X;
        #define LASTCOMA(X) X _##X;

        JNI_ENV_CLASSES(typedef) 

    private:
        //env isnot stable so it always has to be refreshed via refreshenv()
        _JNIEnv * env;
    public:

        _JavaVM & vm;
        _Logger & log;
        

        class AttachThread {
             void * stub[1];
            _JavaVM & vm;
            _Logger & log;
        public:
            AttachThread(_JavaVM & vm , _Logger & log, _JNIEnv *&env) : vm(vm), log(log) {
                log.D("NativeJNI", "AttachThread()this=%p, vm=%p",this, &vm);
                if (vm.AttachCurrentThread(&env, NULL) != 0) {
                    log.E("NativeJNI", "Failed to AttachCurrentThread()");
                    throw KnownException();
                }
            }

            ~AttachThread() {
                log.D("NativeJNI", "~AttachThread()this=%p, vm=%p",this, &vm);
                vm.DetachCurrentThread();
            }
        };

        NativeJNIEnv(_JavaVM & vm, _Logger & log) : vm(vm), log(log) {
        }

        _JNIEnv * RefreshEnv() {
            int envStat = vm.GetEnv((void **)&env, JNI_VERSION_1_6);
            if (JNI_EDETACHED == envStat) {
                log.D("NativeJNI", "GetEnv: not attached vm=%p", &vm);
                //if require need to extend support non c++11 conformat compilers, 
                //example: for 4.6 tool chain need to use placement new a
                thread_local AttachThread detach(vm, log, env);
            }else if (envStat == JNI_EVERSION) {
                log.E("NativeJNI", "GetEnv: version not supported");
                throw KnownException();
            } else if (envStat != JNI_OK) {
                log.E("NativeJNI", "GetEnv: unknown error");
                throw KnownException();
            }
            
            return env;
        }
    };
    
    #undef  COMA
    #undef  LASTCOMA
    #define COMA(X) X,
    #define LASTCOMA(X) X

    template <JNI_ENV_CLASSES(class) >
    struct NativeJNI 
        : public NativeJNIEnv<JNI_ENV_CLASSES(NO_CLASS)> {

        typedef NativeJNIEnv<JNI_ENV_CLASSES(NO_CLASS)> base;

    
        REMAP_ENV_TYPES_T(base);

        typedef ClassLoader<base> _ClassLoader;
        typedef JavaTypeAdapter<NativeJNI> _TypeAdapter;

        NativeJNI(_JavaVM & vm, _Logger & log) : base(vm, log) {
        }
    };
}
