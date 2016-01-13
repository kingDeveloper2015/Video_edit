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
#include "ObjectReference.h"
#include "../no_copy.h"
#include "TypeRemap.h"
#include <memory>
#include "StaticClassReference.h"

namespace AMPLoader {

    template <class JNI>
    class ConcreetClassLoader : private MediaPack::no_copy {
    protected:
        REMAP_TYPES(JNI);
        
        _ClassLoader &load;
        _jmethodID *methodsPtr;
        JNI &jni;
        _TypeAdapter &adapter;
        _jclass loadedClass;

    public:
        ConcreetClassLoader(JNI &jni, _ClassLoader &load, _TypeAdapter &adapter
                , const char * className
                , _jmethodID * methods)
            : load(load)
            , methodsPtr(methods)
            , jni(jni)
            , adapter(adapter) {
            loadedClass = load.LoadClass(className);
        }

        ObjectReference<JNI>  LoadInstance(_jobject loadFrom) {
            ObjectReference<JNI> newReference(jni, methodsPtr, load.NewGlobalReference(loadFrom), adapter);
            return newReference;
        }

        ObjectReference<JNI>  CreateInstanceByStaticMethod(int factoryMethodId) {
            StaticClassReference<JNI> staticReference(jni, loadedClass, methodsPtr,NULL,  adapter);

            ObjectReference<JNI> reference(
                jni, 
                methodsPtr, 
                load.NewGlobalReference(staticReference.CallStaticObjectMethod(factoryMethodId)),
                adapter);
            return reference;
        }


        ObjectReference<JNI>  CreateInstance(int constructorID) {
            return CreateInstanceV(constructorID);
        }

        template <class T>
        ObjectReference<JNI>  CreateInstance(int constructorID, T arg) {
            return CreateInstanceV(constructorID, adapter.convert(arg));
        }

        template <class T, class T1>
        ObjectReference<JNI>  CreateInstance(int constructorID, T arg, T1 arg1) {
            return CreateInstanceV(constructorID, adapter.convert(arg), adapter.convert(arg1));
        }

        template <class T, class T1, class T2>
        ObjectReference<JNI>  CreateInstance(int constructorID, T arg, T1 arg1, T2 arg2) {
            return CreateInstanceV(constructorID, adapter.convert(arg), adapter.convert(arg1), adapter.convert(arg2));
        }


    private:
        ObjectReference<JNI>  CreateInstanceV(int constructorID, ...) {
            va_list args;
            va_start(args, constructorID);

            ObjectReference<JNI> newReference(jni, methodsPtr, load.CreateInstanceV(loadedClass, methodsPtr[constructorID], args), adapter);
            return newReference;
        }
    };

}
