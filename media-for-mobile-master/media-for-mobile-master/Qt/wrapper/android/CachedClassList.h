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

#include "AndroidMediaObjectFactoryLoader.h"
#include "JNIProgressListenerLoader.h"
#include "JNIVideoEffectLoader.h"
#include "MediaComposerLoader.h"
#include "VideoEffectLoader.h"
#include "VideoFormatAndroidLoader.h"
#include "EglUtilLoader.h"
#include "AudioFormatAndroidLoader.h"
#include "MediaFileLoader.h"
#include "ListLoader.h"
#include "LongLoader.h"
#include "CollectionLoader.h"
#include "PairLoader.h"
#include "ResolutionLoader.h"



//max 9 types in order to bypass early c++11 compilers limitations
#define CACHED_CLASSES_LIST1()\
    CACHE_CLASS(AndroidMediaObjectFactoryLoader)\
    CACHE_CLASS(JNIProgressListenerLoader) \
    CACHE_CLASS(JNIVideoEffectLoader) \
    CACHE_CLASS(MediaComposerLoader) \
    CACHE_CLASS(PairLoader) \
    CACHE_CLASS(ResolutionLoader) \
    CACHE_CLASS(VideoFormatAndroidLoader)\
    CACHE_CLASS(AudioFormatAndroidLoader)\
    CACHE_CLASS(VideoEffectLoader) 
    

#define CACHED_CLASSES_LIST2()\
    CACHE_CLASS(MediaFileLoader) \
    CACHE_CLASS(ListLoader) \
    CACHE_CLASS(EglUtilLoader) \
    CACHE_CLASS(LongLoader) \
    CACHE_CLASS(CollectionLoader) 
    
