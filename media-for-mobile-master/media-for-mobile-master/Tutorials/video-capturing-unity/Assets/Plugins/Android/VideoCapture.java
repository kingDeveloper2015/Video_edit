// Copyright (c) 2015, Intel Corporation
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

package com.intel.inde.mp.samples.unity;

import android.content.Context;
import android.util.Log;

import com.intel.inde.mp.*;
import com.intel.inde.mp.android.AndroidMediaObjectFactory;
import com.intel.inde.mp.android.AudioFormatAndroid;
import com.intel.inde.mp.android.VideoFormatAndroid;

import java.io.IOException;

public class VideoCapture
{
    private static final String TAG = "VideoCapture";

    private static final String Codec = "video/avc";
    private static int IFrameInterval = 1;

    private static final Object syncObject = new Object();
    private static volatile VideoCapture videoCapture;

    private static VideoFormat videoFormat;
    private static int videoWidth;
    private static int videoHeight;
    private GLCapture capturer;

    private boolean isConfigured;
    private boolean isStarted;
    private long framesCaptured;
	private Context context;
	private IProgressListener progressListener;

    public VideoCapture(Context context, IProgressListener progressListener)
    {
		this.context = context;
        this.progressListener = progressListener;
    }
    
    public static void init(int width, int height, int frameRate, int bitRate)
    {
    	videoWidth = width;
    	videoHeight = height;
    	
    	videoFormat = new VideoFormatAndroid(Codec, videoWidth, videoHeight);
    	videoFormat.setVideoFrameRate(frameRate);
        videoFormat.setVideoBitRateInKBytes(bitRate);
        videoFormat.setVideoIFrameInterval(IFrameInterval);
    }

    public void start(String videoPath) throws IOException
    {
        if (isStarted())
            throw new IllegalStateException(TAG + " already started!");

        capturer = new GLCapture(new AndroidMediaObjectFactory(context), progressListener);
        capturer.setTargetFile(videoPath);
        capturer.setTargetVideoFormat(videoFormat);

        AudioFormat audioFormat = new AudioFormatAndroid("audio/mp4a-latm", 44100, 2);
        capturer.setTargetAudioFormat(audioFormat);

        capturer.start();

        isStarted = true;
        isConfigured = false;
        framesCaptured = 0;
    }    
    
    public void stop()
    {
        if (!isStarted())
            throw new IllegalStateException(TAG + " not started or already stopped!");

        try {
            capturer.stop();
            isStarted = false;
        } catch (Exception ex) {
        	Log.e(TAG, "--- Exception: GLCapture can't stop");
        }

        capturer = null;
        isConfigured = false;
    }

    private void configure()
    {
        if (isConfigured())
            return;

        try {
            capturer.setSurfaceSize(videoWidth, videoHeight);
            isConfigured = true;
        } catch (Exception ex) {
        }
    }

    public void beginCaptureFrame()
    {
        if (!isStarted())
            return;

        configure();
        if (!isConfigured())
        	return;

        capturer.beginCaptureFrame();
    }

    public void endCaptureFrame()
    {
        if (!isStarted() || !isConfigured())
            return;

        capturer.endCaptureFrame();
        framesCaptured++;
    }

    public boolean isStarted()
    {
        return isStarted;
    }

    public boolean isConfigured()
    {
        return isConfigured;
    }

}
