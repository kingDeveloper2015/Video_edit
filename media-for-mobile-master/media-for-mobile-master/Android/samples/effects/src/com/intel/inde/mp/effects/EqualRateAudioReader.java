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
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import com.intel.inde.mp.AudioFormat;

import java.nio.ByteBuffer;

public class EqualRateAudioReader extends AudioReader {

    public EqualRateAudioReader(MediaExtractor audioExtractor, MediaFormat inputFormat){
        this.audioExtractor = audioExtractor;
        this.inputFormat = inputFormat;
    }

    @Override
    public void start(Context context, AudioFormat mediaFormat) {
        audioDecoder = createAudioDecoder(inputFormat);

        audioDecoderInputBuffers = audioDecoder.getInputBuffers();
        audioDecoderOutputBuffers = audioDecoder.getOutputBuffers();
        audioDecoderOutputBufferInfo = new MediaCodec.BufferInfo();
    }

    public boolean read(ByteBuffer byteBuffer) {
        boolean noData = true;
        while (noData && noEOS) {
            int decoderInputBufferIndex = audioDecoder.dequeueInputBuffer(TIMEOUT_USEC);

            if (decoderInputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {

            }

            if (decoderInputBufferIndex >= 0) {
                ByteBuffer decoderInputBuffer = audioDecoderInputBuffers[decoderInputBufferIndex];
                int size = audioExtractor.readSampleData(decoderInputBuffer, 0);

                long presentationTime = audioExtractor.getSampleTime();

                if (size >= 0) {
                    audioDecoder.queueInputBuffer(decoderInputBufferIndex, 0, size, presentationTime, audioExtractor.getSampleFlags());
                }

                noEOS = audioExtractor.advance();
            }

            int decoderOutputBufferIndex = audioDecoder.dequeueOutputBuffer(audioDecoderOutputBufferInfo, TIMEOUT_USEC);
            if (decoderOutputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {

            }
            if (decoderOutputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                audioDecoderOutputBuffers = audioDecoder.getOutputBuffers();

            }
            if (decoderOutputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                decoderOutputAudioFormat = audioDecoder.getOutputFormat();
            }

            if (decoderOutputBufferIndex >= 0) {
                ByteBuffer decoderOutputBuffer = audioDecoder.getOutputBuffers()[decoderOutputBufferIndex];

                byteBuffer.limit(audioDecoderOutputBufferInfo.size);
                byteBuffer.position(0);
                decoderOutputBuffer.limit(audioDecoderOutputBufferInfo.size);
                decoderOutputBuffer.position(0);
                byteBuffer.put(decoderOutputBuffer);
                audioDecoder.releaseOutputBuffer(decoderOutputBufferIndex, false);

                noData = false;
            }
        }

        return !noData;
    }
}
