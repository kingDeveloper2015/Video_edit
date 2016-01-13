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

import com.intel.inde.mp.IProgressListener;
import com.intel.inde.mp.domain.Resolution;
import com.intel.inde.mp.android.graphics.FullFrameTexture;

import android.opengl.GLES20;
import android.os.Environment;
import android.util.Log;
import android.content.Context;

import java.io.IOException;
import java.io.File;

public class Capturing
{
	private static final String TAG = "Capturing";
	
	private static FullFrameTexture texture;
	
	private VideoCapture videoCapture;
	private int width = 0;
	private int height = 0;
	
	private int videoWidth = 0;
	private int videoHeight = 0;
	private int videoFrameRate = 0;
	
	private long nextCaptureTime = 0;
	private long startTime = 0;
	
	private static Capturing instance = null;
	
	private SharedContext sharedContext = null;
    private EncodeThread encodeThread = null;
	private boolean finalizeFrame = false;
	private boolean isRunning = false;
	
	private IProgressListener progressListener = new IProgressListener() {
        @Override
        public void onMediaStart() {
        	startTime = System.nanoTime();
        	nextCaptureTime = 0;
        	encodeThread.start();
        	isRunning = true;
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
    
    private class EncodeThread extends Thread
    {
    	private static final String TAG = "EncodeThread";
    	
    	private SharedContext sharedContext;  	
    	private boolean isStopped = false;
		private int textureID;
    	private boolean newFrameIsAvailable = false;
		
    	EncodeThread(SharedContext sharedContext) {
    		super();
    		this.sharedContext = sharedContext;
    	}
    	
		@Override
		public void run() {
			while (!isStopped) {
				if (newFrameIsAvailable) {
					synchronized (videoCapture) {
						sharedContext.makeCurrent();
						videoCapture.beginCaptureFrame();
						GLES20.glViewport(0, 0, videoWidth, videoHeight);
						texture.draw(textureID);
						videoCapture.endCaptureFrame();
						newFrameIsAvailable = false;
						sharedContext.doneCurrent();
					}
				}
			}
			isStopped = false;
			synchronized (videoCapture) {
				videoCapture.stop();
			}
		}
		
		public void queryStop() {
			isStopped = true;
		}

		public void pushFrame(int textureID) {
			this.textureID = textureID;
			newFrameIsAvailable = true;
		}
    }
	
    public Capturing(Context context, int width, int height)
    {
		videoCapture = new VideoCapture(context, progressListener);
		
		this.width = width;
		this.height = height;
		
		texture = new FullFrameTexture();
		sharedContext = new SharedContext();
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
		Log.d(TAG, "--- initCapturing: " + width + "x" + height + ", " + frameRate + ", " + bitRate);
    	videoFrameRate = frameRate;
        VideoCapture.init(width, height, frameRate, bitRate);
        videoWidth = width;
    	videoHeight = height;

    	encodeThread = new EncodeThread(sharedContext);
    }

    public void startCapturing(final String videoPath)
    {
        if (videoCapture == null) {
            return;
        }

		(new Thread() {
			public void run() {
				Log.d(TAG, "--- startCapturing");
		        synchronized (videoCapture) {
		            try {
		                videoCapture.start(videoPath);
		            } catch (IOException e) {
		            	Log.e(TAG, "--- startCapturing error");
		            }
		        }
			}
		}).start();
    }
	
	public void captureFrame(int textureID)
	{
		encodeThread.pushFrame(textureID);
	}

    public void stopCapturing()
    {
		Log.d(TAG, "--- stopCapturing");
    	isRunning = false;
		
    	if (finalizeFrame) {
    		finalizeFrame = false;
    	}
        encodeThread.queryStop();
    }
    
    public boolean isRunning()
    {
    	return isRunning;
    }

}
