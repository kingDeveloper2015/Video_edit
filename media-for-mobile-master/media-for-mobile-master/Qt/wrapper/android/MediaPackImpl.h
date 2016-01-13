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
#include "../MediaComposer.h"
#include "MediaComposerAdapter.h"
#include "MediaComposerAndroid.h"
#include "VideoEffectImpl.h"
#include "AndroidTypes.h"
#include <stdexcept>
#include "GrayScaleEffect.h"
#include "AndroidLogger.h"

namespace MediaPack {

	const char MIMETypeAAC[] = { "audio/mp4a-latm" };
    
	MediaComposer IMediaComposer::create(const ProgressListener& progressListener) {
        MediaComposer composerAdapter(new AMPLoader::MediaComposerAdapter<NativeJNI_t>(progressListener));
        return composerAdapter;
    }
    VideoFormat IVideoFormat::create(const std::string & mimeType, int width, int height){
        VideoFormat formatAdapter(new VideoFormat_t(mimeType, width, height, g_allAMPClasses->getFactory().get<VideoFormatLoader_t>()));
        return formatAdapter;
    }
    AudioFormat IAudioFormat::create(const std::string & mimeType, int sampleRate, int numChannels) {
        AudioFormat formatAdapter(new AudioFormat_t(mimeType, sampleRate, numChannels, g_allAMPClasses->getFactory().get<AudioFormatLoader_t>()));
        return formatAdapter;
    }

    VideoEffect IVideoEffect::create(const std::string &videoEffectName) {
        if (videoEffectName == GrayScaleEffect) {
            NativeJNI_t jni(*s_javaVM, AMPLoader::AndroidLogger::instance());
            VideoEffect effectAdapter(new VideoEffectImpl(std::make_shared<AMPLoader::GrayScaleEffect<NativeJNI_t>>(jni)));
            return effectAdapter;
        }
        throw std::runtime_error("unsupported effect");
    }
}
