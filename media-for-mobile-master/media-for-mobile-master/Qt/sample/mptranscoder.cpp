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

#include "mptranscoder.h"
#include "../wrapper/ProgressListener.h"

using namespace std;
using namespace MediaPack;

/****************************************
 * Transcoder
 */
MPTranscoder::MPTranscoder(QQuickItem *parent) :
    QQuickItem(parent),
    m_isRunning(false),
    m_progress(0.0),
    m_status("Status"),
    m_input("input.mp4"),
    m_output("output.mp4")
{
}

void MPTranscoder::start()
{
    m_composer = IMediaComposer::create(ProgressListener(new ProgressListenerWrapper(*this)));

    m_composer->addSourceFile(m_input.toStdString());
    m_composer->setTargetFile(m_output.toStdString());

    VideoFormat videoFormat = IVideoFormat::create(MIMETypeAVC, 640, 480);
    videoFormat->setVideoBitRateInKBytes(1500);
    videoFormat->setVideoFrameRate(25);
    videoFormat->setVideoIFrameInterval(1);
    m_composer->setTargetVideoFormat(videoFormat);

    AudioFormat audioFormat = IAudioFormat::create(MIMETypeAAC, 48000, 2);
    audioFormat->setAudioBitrateInBytes(96 * 1024);
    m_composer->setTargetAudioFormat(audioFormat);

    m_composer->start();
}

void MPTranscoder::setInputFile(const QString &file)
{
    m_input = file;
    emit inputChanged();
}

void MPTranscoder::onMediaStart()
{
    m_status = "Transcoding...";
    emit statusChanged();

    m_progress = 0.0f;
    emit progressChanged();

    m_isRunning = true;
    emit isRunningChanged();
}

void MPTranscoder::onMediaProgress(float progress)
{
    m_progress = progress;
    emit progressChanged();
}

void MPTranscoder::onMediaDone()
{
    m_status = "Finished!";
    emit statusChanged();

    m_isRunning = false;
    emit isRunningChanged();
}

void MPTranscoder::onMediaPause()
{
    m_status = "Paused...";
    emit statusChanged();
}

void MPTranscoder::onMediaStop()
{
    m_status = "Stopped";
    emit statusChanged();
}

void MPTranscoder::onError(const std::string &exception)
{
    m_status = exception.c_str();
    emit statusChanged();
}
