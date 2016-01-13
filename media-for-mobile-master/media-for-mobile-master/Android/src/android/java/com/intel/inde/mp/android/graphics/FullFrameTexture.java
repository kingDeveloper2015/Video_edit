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

package com.intel.inde.mp.android.graphics;

import android.opengl.GLES20;
import android.opengl.Matrix;

import java.nio.ByteBuffer;

public class FullFrameTexture {
    private static final String VERTEXT_SHADER =
        "uniform mat4 uOrientationM;\n" +
            "uniform mat4 uTransformM;\n" +
            "attribute vec2 aPosition;\n" +
            "varying vec2 vTextureCoord;\n" +
            "void main() {\n" +
            "gl_Position = vec4(aPosition, 0.0, 1.0);\n" +
            "vTextureCoord = (uTransformM * ((uOrientationM * gl_Position + 1.0) * 0.5)).xy;" +
            "}";

    private static final String FRAGMENT_SHADER =
        "precision mediump float;\n" +
            "uniform sampler2D sTexture;\n" +
            "varying vec2 vTextureCoord;\n" +
            "void main() {\n" +
            "gl_FragColor = texture2D(sTexture, vTextureCoord);\n" +
            "}";

    private final byte[] FULL_QUAD_COORDINATES = {-1, 1, -1, -1, 1, 1, 1, -1};

    private ShaderProgram shader;

    private ByteBuffer fullQuadVertices;

    private final float[] orientationMatrix = new float[16];
    private final float[] transformMatrix = new float[16];

    public FullFrameTexture() {
        if (shader != null) {
            shader = null;
        }

        shader = new ShaderProgram(EglUtil.getInstance());

        shader.create(VERTEXT_SHADER, FRAGMENT_SHADER);

        fullQuadVertices = ByteBuffer.allocateDirect(4 * 2);

        fullQuadVertices.put(FULL_QUAD_COORDINATES).position(0);

        Matrix.setRotateM(orientationMatrix, 0, 0, 0f, 0f, 1f);
        Matrix.setIdentityM(transformMatrix, 0);
    }

    public void release() {
        shader = null;
        fullQuadVertices = null;
    }

    public void draw(int textureId) {
        shader.use();

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);

        int uOrientationM = shader.getAttributeLocation("uOrientationM");
        int uTransformM = shader.getAttributeLocation("uTransformM");

        GLES20.glUniformMatrix4fv(uOrientationM, 1, false, orientationMatrix, 0);
        GLES20.glUniformMatrix4fv(uTransformM, 1, false, transformMatrix, 0);

        // Trigger actual rendering.
        renderQuad(shader.getAttributeLocation("aPosition"));

        shader.unUse();
    }

    private void renderQuad(int aPosition) {
        GLES20.glVertexAttribPointer(aPosition, 2, GLES20.GL_BYTE, false, 0, fullQuadVertices);
        GLES20.glEnableVertexAttribArray(aPosition);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
    }
}
