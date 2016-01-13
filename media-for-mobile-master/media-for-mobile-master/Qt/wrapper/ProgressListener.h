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
#include "IProgressListener.h"
#include "no_copy.h"

namespace MediaPack
{
    /*
    * To use ProgressListener where shared_ptr instantiation is not possible
    */
    class ProgressListenerWrapper : public IProgressListener, private no_copy
    {
        MediaPack::IProgressListener & target;
    public:
        ProgressListenerWrapper(MediaPack::IProgressListener & target)
            : target(target){}
        virtual ~ProgressListenerWrapper() {}
        virtual void release() {delete this;}
        virtual void onMediaStart () {target.onMediaStart();}
        virtual void onMediaProgress (float progress)   { target.onMediaProgress(progress);}
        virtual void onMediaDone () { target.onMediaDone();}
        virtual void onMediaPause () { target.onMediaPause();}
        virtual void onMediaStop () { target.onMediaStop();}
        virtual void onError (const std::string &exceptionString) { target.onError(exceptionString);}
    };
}
