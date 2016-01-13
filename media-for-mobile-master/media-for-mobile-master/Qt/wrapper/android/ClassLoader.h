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
#include <stdarg.h>
#include "TypeRemap.h"

namespace AMPLoader {

    class KnownException{};
    class RethrowedException{};

   

    template <class JNI>
    class ClassLoader : private MediaPack::no_copy {
        REMAP_ENV_TYPES_T(JNI);

        JNI  &jni;
        _Logger &log;

        const char *TAG() {
            static const char tag [] = "ClassLoader";
            return tag;
        }
        struct MethodIdGetter {
            typedef _jmethodID TReturn;

            TReturn operator ()(_JNIEnv & env , _jclass classID, const char* methodName, const char* signature) const {
                return env.GetMethodID(classID, methodName, signature);
            }
        };
        struct StaticMethodIDGetter {
            typedef _jmethodID TReturn;

            TReturn operator ()(_JNIEnv & env, _jclass classID, const char* methodName, const char* signature) const {
                return env.GetStaticMethodID(classID, methodName, signature);
            }
        };
        struct StaticFieldIDGetter {
            typedef _jfieldID TReturn;

            TReturn operator ()(_JNIEnv & env, _jclass classID, const char* methodName, const char* signature) const {
                return env.GetStaticFieldID(classID, methodName, signature);
            }
        };
        struct FieldIDGetter {
            typedef _jfieldID TReturn;

            TReturn operator ()(_JNIEnv & env, _jclass classID, const char* methodName, const char* signature) const {
                return env.GetFieldID(classID, methodName, signature);
            }
        };

        _JNIEnv &env() {
            return *jni.RefreshEnv();
        }
    
    public:

        ClassLoader(JNI& jni) 
            : jni(jni)
            , log(jni.log) {
        }

        _jclass LoadClass(const char * classPath) {
            _jclass classID = 0;
            try
            {
                classID = env().FindClass(classPath);
            }
            catch (...)
            {}

            if (!classID) {
                log.E(TAG(), "Can't find class %s", classPath);
                throw KnownException();
            }

            // Keep a global reference to it
            classID = NewGlobalReference(classID);

            log.D(TAG(), "%s class loaded", classPath);

            return classID;
        }
        
        _jmethodID LoadConstructor(_jclass classID, const char* signature) {
            return LoadMethod(classID, "<init>", signature);
        }

        _jobject CreateInstanceV(_jclass classID, _jmethodID methodid, va_list args) {
            _jobject newObj = 0;
            try{
                newObj = env().NewObjectV(classID, methodid, args);
            }catch(...){}
            if (!newObj) {
                log.E(TAG(), "Cannot create new object");
                throw new KnownException;
            }
            log.D(TAG(), "[NewObject]");

            return NewGlobalReference(newObj);

        }

        template <class T>
        T NewGlobalReference(T newObj )
        {
            T addReffedObject = 0;

            try{
                addReffedObject = static_cast<T>(env().NewGlobalRef(newObj));
            }catch(...){}
            if (!addReffedObject) {
                log.E(TAG(), "Cannot do NewGlobalRef");
                throw new KnownException;
            }
            log.D(TAG(), "[NewGlobalRef] : %p", addReffedObject);

            return addReffedObject;
        }

        _jobject CreateInstance(_jclass classID, _jmethodID methodid, ...) {
            va_list args;
            va_start(args, methodid);
            return CreateInstanceV(classID, methodid, args);
        }

        void ReleaseInstance(_jobject object) {
            try
            {
                env().DeleteGlobalRef(object);
            }
            catch (...)
            {
                log.E(TAG(), "Cannot do DeleteGlobalRef");
                throw;
            }
        }

        _jmethodID LoadMethod(_jclass classID, const char* methodName, const char* signature) {
            return LoadMethod(classID, methodName, signature, MethodIdGetter());
        }
        _jmethodID LoadStaticMethod(_jclass classID, const char* methodName, const char* signature) {
            return LoadMethod(classID, methodName, signature, StaticMethodIDGetter());
        }

        _jfieldID LoadStaticField(_jclass classID, const char* methodName, const char* signature) {
            return LoadMethod(classID, methodName, signature, StaticFieldIDGetter());
        }

        _jfieldID LoadField(_jclass classID, const char* methodName, const char* signature) {
            return LoadMethod(classID, methodName, signature, FieldIDGetter());
        }


        template <class MethodIdGetter>
        typename MethodIdGetter::TReturn LoadMethod(_jclass classID, const char* methodName, const char* signature, const MethodIdGetter & getMethod) {
            typename MethodIdGetter::TReturn method = 0;
            try{
                // Search for its constructor
                method = getMethod(env(), classID, methodName, signature);
            }catch(...){}

            if (!method) {
                log.E(TAG(), "Can't find %s method with signature: %s", methodName, signature);
                throw KnownException();
            }

            log.D(TAG(), "%s %s method loaded", methodName, signature);

            return method;
        }
    };

    
}
