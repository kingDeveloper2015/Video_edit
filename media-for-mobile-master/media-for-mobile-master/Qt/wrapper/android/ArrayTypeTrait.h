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

template <class JNI, class T>
class ArrayTypeTrait {
};

template <class JNI>
class ArrayTypeTrait<JNI, typename JNI::_jfloatArray>
{
    REMAP_ENV_TYPES_T(JNI);
    JNI &jni;
public:
    ArrayTypeTrait(JNI &jni) : jni(jni){}
    std::vector<float> convert(_jfloatArray javaFloatArray) const {
        std::vector<float> data;
        auto len = jni.RefreshEnv()->GetArrayLength(javaFloatArray);
        auto element = jni.RefreshEnv()->GetFloatArrayElements(javaFloatArray, 0);
        data.insert(data.end(), element, element + len);
        return data;

    }
};

template <class JNI>
class ArrayTypeTrait<JNI, typename JNI::_jobjectArray>
{
    REMAP_ENV_TYPES_T(JNI);
    JNI &jni;
public:

    ArrayTypeTrait(JNI &jni) : jni(jni){}
    std::vector<_jobject> convert(_jobjectArray javaObjectArray) const {
        std::vector<_jobject> data;
        auto len = jni.RefreshEnv()->GetArrayLength(javaObjectArray);
        for (int i = 0; i < len; i++) {
            auto element = jni.RefreshEnv()->GetObjectArrayElement(javaObjectArray, i);
            data.insert(data.end(), element);
        }
        return data;
    }
};


