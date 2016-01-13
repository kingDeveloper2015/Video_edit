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

#ifndef MPTRANSCODER_H
#define MPTRANSCODER_H

#include "../wrapper/MediaComposer.h"
#include <QQuickItem>

namespace MediaPack
{
    class IFilePicker;
}

class MPTranscoder : public QQuickItem, public MediaPack::IProgressListener
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString input MEMBER m_input NOTIFY inputChanged)
    Q_PROPERTY(QString output MEMBER m_output NOTIFY outputChanged)

public:
    explicit MPTranscoder(QQuickItem *parent = 0);

    bool isRunning() const { return m_isRunning; }
    float progress() const { return m_progress; }
    QString status() const { return m_status; }

    Q_INVOKABLE void start();

signals:
    void isRunningChanged();
    void progressChanged();
    void statusChanged();
    void inputChanged();
    void outputChanged();

private slots:
    void setInputFile(const QString &file);

private:
    bool m_isRunning;
    float m_progress;
    QString m_status;
    QString m_input;
    QString m_output;
    MediaPack::MediaComposer m_composer;

    // IProgressListener interface
    void release(){}
    void onMediaStart();
    void onMediaProgress(float progress);
    void onMediaDone();
    void onMediaPause();
    void onMediaStop();
    void onError(const std::string &exception);  
};

#endif // MPTRANSCODER_H
