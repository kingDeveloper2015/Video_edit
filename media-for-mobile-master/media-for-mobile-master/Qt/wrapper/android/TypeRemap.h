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


//remaps only native jni.h types
#define REMAP_ENV_TYPES_INTERNAL(JNI, TYPEDEF_TYPENAME)\
    TYPEDEF_TYPENAME JNI::_jobjectArray _jobjectArray;\
    TYPEDEF_TYPENAME JNI::_jfloatArray _jfloatArray;\
    TYPEDEF_TYPENAME JNI::_jfloat _jfloat;\
    TYPEDEF_TYPENAME JNI::_jlong _jlong;\
    TYPEDEF_TYPENAME JNI::_jstring _jstring;\
    TYPEDEF_TYPENAME JNI::_jobject _jobject;\
    TYPEDEF_TYPENAME JNI::_Logger _Logger;\
    TYPEDEF_TYPENAME JNI::_JavaVM _JavaVM;\
    TYPEDEF_TYPENAME JNI::_JNIEnv _JNIEnv;\
    TYPEDEF_TYPENAME JNI::_jclass _jclass;\
    TYPEDEF_TYPENAME JNI::_jmethodID _jmethodID;\
    TYPEDEF_TYPENAME JNI::_jfieldID _jfieldID;

#define REMAP_ENV_TYPES_T(JNI) REMAP_ENV_TYPES_INTERNAL(JNI, typedef typename)
#define REMAP_ENV_TYPES(JNI) REMAP_ENV_TYPES_INTERNAL(JNI, typedef )

//remaps also infrastructure types
#define REMAP_TYPES(JNI)\
    REMAP_ENV_TYPES_T(JNI)\
    typedef typename JNI::_ClassLoader _ClassLoader;\
    typedef typename JNI::_TypeAdapter _TypeAdapter;
