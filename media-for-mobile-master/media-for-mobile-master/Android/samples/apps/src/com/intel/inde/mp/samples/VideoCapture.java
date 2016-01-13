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

package com.intel.inde.mp.samples;

import android.content.Context;
import com.intel.inde.mp.*;
import com.intel.inde.mp.android.AndroidMediaObjectFactory;
import com.intel.inde.mp.android.AudioFormatAndroid;
import com.intel.inde.mp.android.VideoFormatAndroid;

import java.io.IOException;

public class VideoCapture {
    private static final String TAG = "VideoCapture";

    private static final int width = 1280;
    private static final int height = 720;
    private static final int frameRate = 30;
    private static final int iFrameInterval = 1;
    private static final int bitRate = 3000;
    private static final String codec = "video/avc";

    private static final Object syncObject = new Object();

    private VideoFormat videoFormat;
    private GLCapture capture;

    private boolean isStarted;
    private boolean isConfigured;
    private Context context;
    private IProgressListener progressListener;

    public VideoCapture(Context context, IProgressListener progressListener) {
        this.context = context;
        this.progressListener = progressListener;
        initVideoFormat();
    }

    public void start(String videoPath) throws IOException {
        if (isStarted()) {
            throw new IllegalStateException(TAG + " already started!");
        }

        capture = new GLCapture(new AndroidMediaObjectFactory(context), progressListener);

        capture.setTargetFile(videoPath);
        capture.setTargetVideoFormat(videoFormat);

        AudioFormat audioFormat = new AudioFormatAndroid("audio/mp4a-latm", 44100, 1);
        capture.setTargetAudioFormat(audioFormat);

        capture.start();

        isStarted = true;
        isConfigured = false;
    }

    public void start(StreamingParameters params) throws IOException {
        if (isStarted()) {
            throw new IllegalStateException(TAG + " already started!");
        }

        capture = new GLCapture(new AndroidMediaObjectFactory(context), progressListener);

        capture.setTargetConnection(params);
        capture.setTargetVideoFormat(videoFormat);

        capture.start();

        isStarted = true;
        isConfigured = false;
    }

    public void stop() {
        if (!isStarted()) {
            throw new IllegalStateException(TAG + " not started or already stopped!");
        }

        capture.stop();
        capture = null;
        isConfigured = false;
    }

    private boolean configure() {
        if (isConfigured()) {
            return true;
        }

        try {
            capture.setSurfaceSize(width, height);
            isConfigured = true;
        } catch (Exception ex) {

        }

        return isConfigured;
    }

    public boolean beginCaptureFrame() {
        if (!isStarted()) {
            return false;
        }

        if (!isConfigured()) {
            if (!configure()) {
                return false;
            }
        }

        capture.beginCaptureFrame();

        return true;
    }

    public void endCaptureFrame() {
        if (!isStarted()) {
            return;
        }

        if (!isConfigured()) {
            return;
        }

        capture.endCaptureFrame();
    }

    public boolean isStarted() {
        if (capture == null) {
            return false;
        }

        return isStarted;
    }

    public boolean isConfigured() {
        return isConfigured;
    }

    public int getFrameWidth() {
        return width;
    }

    public int getFrameHeight() {
        return height;
    }

    private void initVideoFormat() {
        videoFormat = new VideoFormatAndroid(codec, width, height);

        videoFormat.setVideoBitRateInKBytes(bitRate);
        videoFormat.setVideoFrameRate(frameRate);
        videoFormat.setVideoIFrameInterval(iFrameInterval);
    }
}
