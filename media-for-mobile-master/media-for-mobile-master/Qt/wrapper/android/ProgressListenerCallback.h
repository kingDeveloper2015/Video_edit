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
#include <functional>
#include "VideoEffectAndroid.h"

namespace AMPLoader {

    namespace detail {
        
        template <class Logger, class jlong>
        inline void reportNoMapEntryFoundForIProgressListener(const  Logger & logger, jlong thiz) {
            logger.E("JNIProgressListener","No C++ IProgressListener with objectid=%lld found", thiz);
            throw 1;
        }

        template <class Logger, class jlong>
        inline void reportNoMapEntryFoundForIVideoEffect(const  Logger & logger, jlong thiz) {
            logger.E("JNIVideoEffect","No C++ IVideoEfect with objectid=%lld found", thiz);
            throw 1;
        }

        
        template <class Storage, class ErrReporter, class jlong, class T>
        inline auto callMethodInMAP(Storage &storage, const ErrReporter &reportError, jlong thiz, const T &caller) 
            -> decltype(caller(((typename Storage::iterator)0)->second)){
            typename Storage::iterator x = storage.find(thiz);
            if (x == storage.end()) {
                reportError();
            }
            return caller(x->second);
        }
    }

    template <class Storage, class Logger, class MemFunPtr, class jlong>
    inline void callProgressListener(Storage &storage, const Logger & logger, jlong thiz, MemFunPtr ptr) {
        detail::callMethodInMAP(storage, [logger, thiz]() {
                detail::reportNoMapEntryFoundForIProgressListener(logger, thiz);
            }, thiz, [ptr](ProgressListener<NativeJNI_t>*listener){
                (listener->*ptr)();});
    }

    template <class Storage, class Logger, class MemFunPtr, class Args>
    inline void callProgressListener(Storage &storage, const Logger & logger, jlong thiz, MemFunPtr ptr, Args args) {
        detail::callMethodInMAP(storage,[logger, thiz]() {
            detail::reportNoMapEntryFoundForIProgressListener(logger, thiz);
        }, thiz, [ptr, args](ProgressListener<NativeJNI_t>*listener) {
            (listener->*ptr)(args);});
    }
    
    template <class Storage, class Logger, class MemFunPtr>
    inline auto callVideoEffect(Storage &storage, const Logger & logger, jlong thiz, MemFunPtr ptr) -> decltype(((VideoEffectAndroid<NativeJNI_t>*)0->*ptr)()) {
        return detail::callMethodInMAP(storage, [logger, thiz](){
                detail::reportNoMapEntryFoundForIVideoEffect(logger, thiz);
            }, thiz, [ptr](VideoEffectAndroid<NativeJNI_t>* effect) {
                return (effect->*ptr)();
        });
    }

    template <class Storage, class Logger, class MemFunPtr, class Arg0>
    inline auto callVideoEffect(Storage &storage, const Logger & logger, jlong thiz, MemFunPtr ptr
        , Arg0 arg0) -> decltype(((VideoEffectAndroid<NativeJNI_t>*)0->*ptr)(arg0)) {
            return detail::callMethodInMAP(storage, [logger, thiz](){
                detail::reportNoMapEntryFoundForIVideoEffect(logger, thiz);
            }, thiz, [ptr, arg0](VideoEffectAndroid<NativeJNI_t>* effect) {
                return (effect->*ptr)(arg0);
            });
    }

    template <class Storage, class Logger, class MemFunPtr, class Arg0, class Arg1, class Arg2>
    inline auto callVideoEffect(Storage &storage, const Logger & logger, jlong thiz, MemFunPtr ptr
        , Arg0 arg0, Arg1 arg1, Arg2 arg2) -> decltype(((VideoEffectAndroid<NativeJNI_t>*)0->*ptr)(arg0, arg1, arg2)) {
        return detail::callMethodInMAP(storage, [logger, thiz](){
                detail::reportNoMapEntryFoundForIVideoEffect(logger, thiz);
            }, thiz, [ptr, arg0, arg1, arg2](VideoEffectAndroid<NativeJNI_t>* effect) {
               return (effect->*ptr)(arg0, arg1, arg2);
        });
    }
}
