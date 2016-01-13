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
#include "TimeStamp.h"
#include "MediaPackPointer.h"
#include "no_copy.h"

#include <vector>

#include "TimeStamp.h"
#include "no_copy.h"
#include "VideoFormat.h"
#include "AudioFormat.h"
#include <string>
#include <vector>

namespace MediaPack {
    class IMediaFile;
    typedef detail::SharedPointer<IMediaFile> MediaFile;

    class IMediaFile : private no_copy {
    protected:
        //deny default constructor
        IMediaFile(){}
    public:
        virtual void release() = 0;
        virtual void addSegment(TimeSegment segment)  = 0;
        virtual std::vector<TimeSegment> getSegments() const = 0;
        virtual void insertSegment(int index, TimeSegment segment) = 0;
        virtual void removeSegment(int index) =0;
        virtual VideoFormat getVideoFormat(int index) = 0;
        virtual AudioFormat getAudioFormat(int index) = 0;
        virtual TimeStamp getDurationInMicroSec()  = 0;
        virtual TimeStamp getSegmentsDurationInMicroSec() = 0;
        virtual int getRotation() = 0;
        virtual std::string getFilePath()  = 0;
    };
}
