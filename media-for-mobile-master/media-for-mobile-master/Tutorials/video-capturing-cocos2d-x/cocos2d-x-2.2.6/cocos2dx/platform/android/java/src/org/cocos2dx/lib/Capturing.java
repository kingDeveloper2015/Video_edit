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

package org.cocos2dx.lib;

import com.intel.inde.mp.IProgressListener;
import com.intel.inde.mp.domain.Resolution;
import com.intel.inde.mp.android.graphics.FullFrameTexture;
import com.intel.inde.mp.android.graphics.FrameBuffer;
import com.intel.inde.mp.android.graphics.EglUtil;

import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.os.Environment;
import android.util.Log;
import android.content.Context;
import android.graphics.SurfaceTexture;

import java.io.IOException;
import java.io.File;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

public class Capturing
{
	private static final String TAG = "Capturing";
	
	private static FullFrameTexture texture;
	private FrameBuffer frameBuffer;
	private VideoCapture videoCapture;
	private int width = 0;
	private int height = 0;
	private int videoWidth = 0;
	private int videoHeight = 0;
	private int mVideoFrameRate = 30;
	private long mNextCaptureNanoTime = 0;
	private long mStartNanoTime = 0;
	
	private static Capturing instance = null;
	
	private IProgressListener progressListener = new IProgressListener() {
        @Override
        public void onMediaStart() {
        }

        @Override
        public void onMediaProgress(float progress) {
        }

        @Override
        public void onMediaDone() {
        }

        @Override
        public void onMediaPause() {
        }

        @Override
        public void onMediaStop() {
        }

        @Override
        public void onError(Exception exception) {
        }
    };
	
    public Capturing(Context context, int width, int height)
    {
		videoCapture = new VideoCapture(context, progressListener);
		
	    frameBuffer = new FrameBuffer(EglUtil.getInstance());
		frameBuffer.setResolution(new Resolution(width, height));
		this.width = width;
		this.height = height;
		
		texture = new FullFrameTexture();
		
		instance = this;
    }
    
    public static Capturing getInstance()
    {
    	return instance;
    }

    public static String getDirectoryDCIM()
    {
        return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM) + File.separator;
    }

    public void initCapturing(int width, int height, int frameRate, int bitRate)
    {
    	mVideoFrameRate = frameRate;
        VideoCapture.init(width, height, frameRate, bitRate);
        videoWidth = width;
    	videoHeight = height;
    }

    public void startCapturing(String videoPath)
    {
        if (videoCapture == null) {
            return;
        }
        synchronized (videoCapture) {
            try {
            	mStartNanoTime = System.nanoTime();
                videoCapture.start(videoPath);
            } catch (IOException e) {
            }
        }
    }
	
	public void beginCaptureFrame()
    {
    	frameBuffer.bind();
    }
	
	public void captureFrame(int textureID)
    {
        if (videoCapture == null) {
            return;
        }
        synchronized (videoCapture) {
            videoCapture.beginCaptureFrame();
            GLES20.glViewport(0, 0, videoWidth, videoHeight);
            texture.draw(textureID);
            videoCapture.endCaptureFrame();
        }
    }
	
	public void endCaptureFrame()
    {
    	frameBuffer.unbind();
		int textureID = frameBuffer.getTextureId();
		long elapsedNanoTime = System.nanoTime() - mStartNanoTime;
		if (elapsedNanoTime > mNextCaptureNanoTime) {
			captureFrame(textureID);
			mNextCaptureNanoTime += 1000000000 / mVideoFrameRate;
		}
		GLES20.glViewport(0, 0, width, height);
		texture.draw(textureID);
    }

    public void stopCapturing()
    {
        if (videoCapture == null) {
            return;
        }
        synchronized (videoCapture) {
            if (videoCapture.isStarted()) {
                videoCapture.stop();
            }
        }
    }
    
    public boolean isRunning()
    {
    	return videoCapture.isStarted();
    }

}
