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

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class SharedContext
{
	private static final String TAG = "SharedContext";
	
	private EGL10 egl;
	private EGLContext eglContext;
	private EGLDisplay eglDisplay;
	EGLConfig auxConfig;
	private EGLSurface auxSurface = null;
	private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	private static final int EGL_OPENGL_ES2_BIT = 4;
	private int[] textures = new int[1];
	private SurfaceTexture surfaceTexture;
	
	SharedContext() {
		egl = (EGL10)EGLContext.getEGL();
		
		eglDisplay = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
		if (eglDisplay == EGL10.EGL_NO_DISPLAY) {
			Log.e(TAG, "--- eglGetDisplay failed: " + GLUtils.getEGLErrorString(egl.eglGetError()));
		}
		
		int[] version = new int[2];
        if (!egl.eglInitialize(eglDisplay, version)) {
            Log.e(TAG, "--- eglInitialize failed: " + GLUtils.getEGLErrorString(egl.eglGetError()));
        }
        
        auxConfig = chooseEglConfig();
        if (auxConfig == null) {
            Log.e(TAG, "--- eglConfig not initialized");
        }
		
		int[] contextAttrs = new int[] {
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL10.EGL_NONE
		};
		
		// Create a shared context for this thread
		EGLContext currentContext = egl.eglGetCurrentContext();
		eglContext = egl.eglCreateContext(eglDisplay, auxConfig, currentContext, contextAttrs);
		if (eglContext != null) {
			Log.d(TAG, "--- eglContext created");
		}
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]);
		surfaceTexture = new SurfaceTexture(textures[0]);
		
		auxSurface = egl.eglCreateWindowSurface(eglDisplay, auxConfig, surfaceTexture, null);
		if (auxSurface == null || auxSurface == EGL10.EGL_NO_SURFACE) {
            Log.e(TAG,"--- createWindowSurface returned error: " + GLUtils.getEGLErrorString(egl.eglGetError()));
        }
	}
	
	final int[] auxConfigAttribs = {
		EGL10.EGL_SURFACE_TYPE, EGL10.EGL_WINDOW_BIT,
		EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL10.EGL_RED_SIZE, 8,
		EGL10.EGL_GREEN_SIZE, 8,
		EGL10.EGL_BLUE_SIZE, 8,
		EGL10.EGL_ALPHA_SIZE, 0,
		EGL10.EGL_DEPTH_SIZE, 0,
		EGL10.EGL_STENCIL_SIZE, 0,
		EGL10.EGL_NONE
	};

	private EGLConfig chooseEglConfig() {
		EGLConfig[] auxConfigs = new EGLConfig[1];
		int[] auxConfigsCount = new int[1];
		Log.d(TAG, "--- chooseEglConfig()");
		if (!egl.eglChooseConfig(eglDisplay, auxConfigAttribs, auxConfigs, 1, auxConfigsCount)) {
			throw new IllegalArgumentException("eglChooseConfig failed " + GLUtils.getEGLErrorString(egl.eglGetError()));
		} else if (auxConfigsCount[0] > 0) {
			return auxConfigs[0];
		}
		return null;
	}

	public void makeCurrent() {
		if (!egl.eglMakeCurrent(eglDisplay, auxSurface, auxSurface, eglContext)) {
			Log.e(TAG, "--- eglMakeCurrent failed: " + GLUtils.getEGLErrorString(egl.eglGetError()));
		}
	}

	public void doneCurrent() {
		if (!egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT)) {
			Log.e(TAG, "--- eglMakeCurrent failed: " + GLUtils.getEGLErrorString(egl.eglGetError()));
		}
	}
}
