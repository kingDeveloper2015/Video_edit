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

import android.opengl.GLES20;
import com.intel.inde.mp.android.graphics.VideoEffect;
import com.intel.inde.mp.domain.graphics.IEglUtil;

public class SepiaEffect extends VideoEffect {
    public SepiaEffect(int angle, IEglUtil eglUtil) {
        super(angle, eglUtil);
        setFragmentShader(getFragmentShader());
    }

    protected String getFragmentShader() {
        return "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "varying vec2 vTextureCoord;\n" +
                "uniform mat3 uWeightsMatrix;\n" +
                "uniform samplerExternalOES sTexture;\n" +
                "void main() {\n" +
                "  vec4 color = texture2D(sTexture, vTextureCoord);\n" +
                "  vec3 color_new = min(uWeightsMatrix * color.rgb, 1.0);\n" +
                "  gl_FragColor = vec4(color_new.rgb, color.a);\n" +
                "}\n";

    }

    protected float[] getWeights() {
        return new float[]{
                805.0f / 2048.0f, 715.0f / 2048.0f, 557.0f / 2048.0f,
                1575.0f / 2048.0f, 1405.0f / 2048.0f, 1097.0f / 2048.0f,
                387.0f / 2048.0f, 344.0f / 2048.0f, 268.0f / 2048.0f
        };
    }

    protected int weightsMatrixHandle;

    @Override
    public void start() {
        super.start();
        weightsMatrixHandle = shaderProgram.getAttributeLocation("uWeightsMatrix");
    }

    @Override
    protected void addEffectSpecific() {
        GLES20.glUniformMatrix3fv(weightsMatrixHandle, 1, false, getWeights(), 0);
    }
}
