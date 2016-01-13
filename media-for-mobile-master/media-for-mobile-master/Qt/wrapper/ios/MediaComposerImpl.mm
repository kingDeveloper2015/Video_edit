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

#include "MediaComposerImpl.h"
#include "VideoFormatImpl.h"
#include "AudioFormatImpl.h"
#include "VideoEffectImpl.h"
#include "MediaFileImpl.h"
#import <AssetsLibrary/AssetsLibrary.h>

namespace MediaPack
{
    MediaComposerImpl::MediaComposerImpl(const ProgressListener &listener)
    {
        // Save listener C++ shared_ptr object
        __block ProgressListener listenerCpp = listener;

        // Create listener ObjC object
        MPProgressListenerHelper *listenerObjc = [MPProgressListenerHelper new];
        listenerObjc.onMediaStartBlock = ^{ listenerCpp->onMediaStart(); };
        listenerObjc.onMediaPauseBlock = ^{ listenerCpp->onMediaPause(); };
        listenerObjc.onMediaStopBlock  = ^{ listenerCpp->onMediaStop(); };
        listenerObjc.onMediaDoneBlock  = ^{ listenerCpp->onMediaDone(); };
        listenerObjc.onMediaProgressBlock  = ^(float progress){ listenerCpp->onMediaProgress(progress); };

        listenerObjc.onErrorBlock  = ^(NSException *exc){
            std::string str_exc([[exc reason] UTF8String]);
            listenerCpp->onError(str_exc);
        };

        _self = [MPMediaComposer mediaComposerWithProgressListener:listenerObjc];
    }

    void MediaComposerImpl::release()
    {
        delete this;
    }

    void MediaComposerImpl::start(void)
    {
        [_self start];
    }

    void MediaComposerImpl::stop(void)
    {
        [_self stop];
    }

    void MediaComposerImpl::addSourceFile(const std::string &input)
    {
        NSURL *inURL = [NSURL URLWithString:[NSString stringWithUTF8String:input.c_str()]];
        [_self addSourceFile:inURL];
    }

    void MediaComposerImpl::setTargetFile(const std::string &output)
    {
        NSString *localPath = [NSString stringWithFormat:@"Documents/%@", [NSString stringWithUTF8String:output.c_str()]];
        NSString *outputPath = [NSHomeDirectory() stringByAppendingPathComponent:localPath];
        unlink([outputPath UTF8String]);
        NSURL *outURL = [NSURL fileURLWithPath:outputPath];

        [_self setTargetFile:outURL];
    }

    void MediaComposerImpl::setTargetVideoFormat(const VideoFormat &videoFormat)
    {
        VideoFormatImpl *videoFormatImpl = dynamic_cast<VideoFormatImpl*>(videoFormat.get());
        [_self setTargetVideoFormat:videoFormatImpl->getNativeObj()];
    }

    void MediaComposerImpl::setTargetAudioFormat(const AudioFormat &audioFormat)
    {
        AudioFormatImpl *audioFormatImpl = dynamic_cast<AudioFormatImpl*>(audioFormat.get());
        [_self setTargetAudioFormat:audioFormatImpl->getNativeObj()];
    }

    void MediaComposerImpl::addVideoEffect(const VideoEffect & effect)
    {
        VideoEffectImpl *videoEffectImpl = dynamic_cast<VideoEffectImpl*>(effect.get());
        [_self addVideoEffect:videoEffectImpl->getNativeObj()];
    }

    std::vector<MediaFile> MediaComposerImpl::getSourceFiles() const
    {
        std::vector<MediaFile> list;

        NSArray *listObjc = [_self getSourceFiles];
        for (MPMediaFile *fileObjc in listObjc)
        {
            MediaFile file = MediaFile(new MediaFileImpl(fileObjc));
            list.push_back(file);
        }

        return list;
    }
}
