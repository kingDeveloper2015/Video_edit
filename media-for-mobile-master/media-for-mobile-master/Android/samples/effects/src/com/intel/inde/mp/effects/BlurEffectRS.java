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

import android.content.Context;
import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.ScriptIntrinsicBlur;
import com.intel.inde.mp.android.graphics.VideoEffect;
import com.intel.inde.mp.domain.Resolution;
import com.intel.inde.mp.domain.graphics.IEglUtil;
import com.intel.inde.mp.domain.graphics.TextureRenderer;
import com.intel.inde.mp.domain.pipeline.TriangleVerticesCalculator;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class BlurEffectRS extends VideoEffect {
    protected FloatBuffer triangleVertices2D;
    private IntBuffer buffer;
    private int width, height;
    private RenderScript renderScript;
    private Allocation allocationIn;
    private Allocation allocationOut;
    private Bitmap inputBitmap;
    private Bitmap outputBitmap;
    private TextureRenderer textureRenderer;
    private int rsOutTexture;
    private ScriptIntrinsicBlur intrinsicBlur;
    private int radius;

    public BlurEffectRS(int angle, Context context, IEglUtil eglUtil, int radius) {
        super(angle, eglUtil);
        this.radius = radius;
        renderScript = RenderScript.create(context);
        textureRenderer = new TextureRenderer(eglUtil);
    }

    @Override
    public void start() {
        super.start();
        prepareDrawOutput();
    }

    @Override
    public void setInputResolution(Resolution resolution) {

        super.setInputResolution(resolution);
        textureRenderer.setInputSize(resolution.width(), resolution.height());
    }

    @Override
    public void applyEffect(int inputTextureId, long l, float[] floats) {
        boolean wasFit = fitToCurrentSurface(false);
        super.applyEffect(inputTextureId, l, floats);

        fitToCurrentSurface(wasFit);

        if (inputResolution.width() != width || inputResolution.height() != height) {
            width = inputResolution.width();
            height = inputResolution.height();
            outputBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            inputBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            allocationOut = Allocation.createFromBitmap(renderScript, outputBitmap);
            buffer = IntBuffer.wrap(new int[width * height]);
        }

        buffer.position(0);
        GLES20.glReadPixels(0, 0, width, height, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, buffer);
        buffer.rewind();

        checkGlError("GLES20.glReadPixels");

        applyRenderScriptEffect();

        textureRenderer.drawFrame2D(floats, rsOutTexture, 0f, wasFit);
    }

    private void prepareDrawOutput() {

        textureRenderer.surfaceCreated();

        triangleVertices2D = ByteBuffer.allocateDirect(TriangleVerticesCalculator.getDefaultTriangleVerticesData().length * FLOAT_SIZE_BYTES)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        triangleVertices2D.put(TriangleVerticesCalculator.getDefaultTriangleVerticesData()).position(0);

        rsOutTexture = eglUtil.createTexture(GLES20.GL_TEXTURE_2D);

        intrinsicBlur = ScriptIntrinsicBlur.create(renderScript, Element.U8_4(renderScript));
    }

    private void applyRenderScriptEffect() {
        inputBitmap.copyPixelsFromBuffer(buffer);

        allocationIn = Allocation.createFromBitmap(renderScript, inputBitmap);

        intrinsicBlur.setRadius(radius);
        intrinsicBlur.setInput(allocationIn);
        intrinsicBlur.forEach(allocationOut);

        allocationOut.copyTo(outputBitmap);

        GLES20.glViewport(0, 0, width, height);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, rsOutTexture);
        checkGlError("GLES20.glBindTexture");
        //for the debug purpose, you may try to output the inputBitmap
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, inputBitmap, 0);
        checkGlError("GLUtils.texImage2D");

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
    }
}
