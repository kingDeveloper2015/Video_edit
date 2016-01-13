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
import android.media.MediaFormat;
import android.util.Log;
import com.intel.inde.mp.AudioFormat;
import com.intel.inde.mp.Uri;
import com.intel.inde.mp.android.AudioFormatAndroid;

import java.nio.ByteBuffer;

public class SubstituteAudioEffect extends AudioEffect {
    private AudioReader reader = new AudioReader();
    private ByteBuffer byteBuffer = ByteBuffer.allocate(1024 * 1024);
    private Uri uri;
    private AudioFormatAndroid audioFormat;

    @Override
    public void applyEffect(ByteBuffer input, long timeProgress) {
        if (reader.read(byteBuffer)) {

            audioFormat = new AudioFormatAndroid("audio/mp4a-latm", 48000, 2);

            // there may be problem in allocating memory
            // take a look of all data copying correctly
            if (input.capacity() < byteBuffer.limit()){
                input = ByteBuffer.allocateDirect(byteBuffer.limit() + 2);
            }

            byteBuffer.position(0);

            input.position(0);
            input.limit(byteBuffer.limit());
            input.put(byteBuffer);
        }
    }

    public void setFileUri(Context context, Uri uri, AudioFormat mediaFormat) {
        this.uri = uri;

        reader.setFileUri(uri);
        reader.start(context, mediaFormat);
    }

    public Uri getFileUri() {
        return uri;
    }

    public com.intel.inde.mp.domain.MediaFormat getMediaFormat() {
        return audioFormat;
    }
}
