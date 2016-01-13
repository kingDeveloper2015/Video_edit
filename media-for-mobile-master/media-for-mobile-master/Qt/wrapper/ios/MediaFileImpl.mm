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

#include "MediaFileImpl.h"

namespace MediaPack
{
    MediaFileImpl::MediaFileImpl(MPMediaFile *ref)
    {
        _self = ref;
    }
    
    void MediaFileImpl::release()
    {
        delete this;
    }

    TimeStamp MediaFileImpl::getDurationInMicroSec()
    {
        return [_self getDurationInMicroSec];
    }
    
    TimeStamp MediaFileImpl::getSegmentsDurationInMicroSec()
    {
        return [_self getSegmentsDurationInMicroSec];
    }
    
    void MediaFileImpl::addSegment(TimeSegment segment)
    {
        [_self addSegment:[MPPair pairWithLeft:(long)segment.first right:(long)segment.second]];
    }
    
    std::vector<TimeSegment> MediaFileImpl::getSegments() const
    {
        std::vector<TimeSegment> list;
        
        NSArray *listObjc = [_self getSegments];
        for (MPPair *segmentObjc in listObjc)
        {
            TimeSegment segment = TimeSegment([segmentObjc left], [segmentObjc right]);
            list.push_back(segment);
        }
        
        return list;
    }
    
    /***************************************/
    // Not Implemented
    /***************************************/
    
    void MediaFileImpl::insertSegment(int index, TimeSegment segment)
    {
        // Not Implemented
    }
    
    void MediaFileImpl::removeSegment(int index)
    {
        // Not Implemented
    }
    
    int MediaFileImpl::getVideoTracksCount()
    {
        // Not Implemented
        return 0;
    }
    
    int MediaFileImpl::getAudioTracksCount()
    {
        // Not Implemented
        return 0;
    }
    
    VideoFormat MediaFileImpl::getVideoFormat(int index)
    {
        // Not Implemented
        return nil;
    }
    
    AudioFormat MediaFileImpl::getAudioFormat(int index)
    {
        // Not Implemented
        return nil;
    }
    
    int MediaFileImpl::getRotation()
    {
        return 0;
    }
    
    std::string MediaFileImpl::getFilePath()
    {
        return "Not Implemented";
    }
}