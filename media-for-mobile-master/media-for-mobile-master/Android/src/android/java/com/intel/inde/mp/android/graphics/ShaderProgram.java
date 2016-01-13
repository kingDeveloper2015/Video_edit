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
import com.intel.inde.mp.domain.graphics.IEglUtil;
import com.intel.inde.mp.domain.graphics.IShaderProgram;

import java.util.HashMap;

public class ShaderProgram implements IShaderProgram {
    private static int INVALID_VALUE = -1;
    private int programHandle = INVALID_VALUE;
    private HashMap<String, Integer> attributes = new HashMap<String, Integer>();
    private IEglUtil eglUtil;

    public ShaderProgram(IEglUtil eglUtil) {
        this.eglUtil = eglUtil;
    }

    public void create(String vertexCode, String fragmentCode) {
        int vertexShader = INVALID_VALUE;
        if (vertexCode != null) {
            vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexCode);
        }
        if (vertexShader == 0) {
            programHandle = 0;
            return;
        }

        int fragmentShader = INVALID_VALUE;
        if (fragmentCode != null) {
            fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentCode);
        }
        if (fragmentShader == 0) {
            programHandle = 0;
            return;
        }

        programHandle = GLES20.glCreateProgram();
        eglUtil.checkEglError("glCreateProgram");

        GLES20.glAttachShader(programHandle, vertexShader);
        eglUtil.checkEglError("glAttachShader");

        GLES20.glAttachShader(programHandle, fragmentShader);
        eglUtil.checkEglError("glAttachShader");

        GLES20.glLinkProgram(programHandle);
    }

    public void use() {
        GLES20.glUseProgram(programHandle);
    }

    public void unUse() {
        GLES20.glUseProgram(0);
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        eglUtil.checkEglError("glCreateShader");

        GLES20.glShaderSource(shader, shaderCode);
        eglUtil.checkEglError("glShaderSource");

        GLES20.glCompileShader(shader);
        eglUtil.checkEglError("glCompileShader");

        int[] status = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, status, 0);
        if (status[0] == 0) {
            String info = GLES20.glGetShaderInfoLog(shader);
            GLES20.glDeleteShader(shader);
            throw new IllegalArgumentException("Shader compilation failed with: " + info);
        }

        return shader;
    }

    public int getAttributeLocation(String attribute) {
        if (attributes.containsKey(attribute)) {
            return attributes.get(attribute);
        }
        int location = GLES20.glGetAttribLocation(programHandle, attribute);
        eglUtil.checkEglError("glGetAttribLocation " + attribute);
        if (location == -1) {
            location = GLES20.glGetUniformLocation(programHandle, attribute);
            eglUtil.checkEglError("glGetUniformLocation " + attribute);
        }
        if (location == -1) {
            throw new IllegalStateException("Can't find a location for attribute " + attribute);
        }
        attributes.put(attribute, location);
        return location;
    }

    public int getProgramHandle() {
        return programHandle;
    }
}
