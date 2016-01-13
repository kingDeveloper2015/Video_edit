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
#include "TypeRemap.h"
#include "IGetJniObject.h"
#include "../TimeStamp.h"
#include "PairLoader.h"
#include "ClassLoaderFactory.h"

namespace AMPLoader
{
    template <class JNI>
    class TimeStampSegmentTmpl : public IGetJNIObject<JNI> {
        REMAP_TYPES(JNI);
        
        ObjectReference<JNI> left;
        ObjectReference<JNI> right;
        ObjectReference<JNI> pairLoaded;

    public:
        TimeStampSegmentTmpl (JNI &jni, ClassLoadersFactory<JNI> &factory, const MediaPack::TimeSegment & segment)
            : left(factory.template get<LongLoader<JNI>>()->makeLong(segment.first))
            , right(factory.template get<LongLoader<JNI>>()->makeLong(segment.second))
            , pairLoaded(factory.template get<PairLoader<JNI>>()->CreateInstance(PairLoader<JNI>::econstructor0, &left, &right)) {
            
            jni.log.D("TimeStampSegmentTmpl","instantiated from segment: %lld-%lld", segment.first, segment.second);
        }

        virtual _jobject GetNativeObject() {
            return pairLoaded.GetNativeObject();
        }
    };
}
