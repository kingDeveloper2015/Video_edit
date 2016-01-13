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

package com.intel.inde.mp.effects;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import com.intel.inde.mp.android.graphics.VideoEffect;
import com.intel.inde.mp.domain.graphics.IEglUtil;

public abstract class OverlayEffect extends VideoEffect {

    private int oTextureHandle;
    private int[] textures = new int[1];

    Bitmap bitmap = null;
    int inputBitmapWidth = 1280; // default resolution
    int inputBitmapHeight = 720;


    public OverlayEffect(int angle, IEglUtil eglUtil) {
        super(angle, eglUtil);
        bitmap = Bitmap.createBitmap(inputBitmapWidth, inputBitmapHeight, Bitmap.Config.ARGB_8888);
        setFragmentShader(getFragmentShader());
    }

    protected String getFragmentShader() {
        return "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "varying vec2 vTextureCoord;\n" +
                "uniform samplerExternalOES sTexture;\n" +
                "uniform sampler2D oTexture;\n" +
                "void main() {\n" +
                "  vec4 bg_color = texture2D(sTexture, vTextureCoord);\n" +
                "  vec4 fg_color = texture2D(oTexture, vTextureCoord);\n" +
                "  float colorR = (1.0 - fg_color.a) * bg_color.r + fg_color.a * fg_color.r;\n" +
                "  float colorG = (1.0 - fg_color.a) * bg_color.g + fg_color.a * fg_color.g;\n" +
                "  float colorB = (1.0 - fg_color.a) * bg_color.b + fg_color.a * fg_color.b;\n" +
                "  gl_FragColor = vec4(colorR, colorG, colorB, bg_color.a);\n" +
                "}\n";

    }

    @Override
    public void start() {
        super.start();
        oTextureHandle = shaderProgram.getAttributeLocation("oTexture");

        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
    }

    @Override
    protected void addEffectSpecific() {
        if (bitmap.getWidth()!= inputResolution.width()) {
            bitmap = Bitmap.createBitmap(inputResolution.width(), inputResolution.height(), Bitmap.Config.ARGB_8888);
        } else if (bitmap.getHeight() != inputResolution.height()) {
            bitmap = Bitmap.createBitmap(inputResolution.width(), inputResolution.height(), Bitmap.Config.ARGB_8888);
        }

        bitmap.eraseColor(Color.argb(0, 0, 0, 0));

        Canvas bitmapCanvas = new Canvas(bitmap);

        drawCanvas(bitmapCanvas);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        checkGlError("glBindTexture");


        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, bitmap, 0);
        checkGlError("texImage2d");
        GLES20.glUniform1i(oTextureHandle, 1);
        checkGlError("oTextureHandle - glUniform1i");
    }

    protected void finalize() {
    }

    protected abstract void drawCanvas(Canvas canvas);
}
