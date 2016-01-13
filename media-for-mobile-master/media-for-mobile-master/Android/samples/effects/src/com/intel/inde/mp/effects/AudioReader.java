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
import com.intel.inde.mp.Uri;

import com.intel.inde.mp.android.AudioFormatAndroid;
import com.intel.inde.mp.android.ResamplerAndroid;
import com.intel.inde.mp.domain.Resampler;

import java.io.IOException;
import java.nio.ByteBuffer;

public class AudioReader {
    protected static final int TIMEOUT_USEC = 10000;

    protected MediaExtractor audioExtractor;
    protected MediaCodec audioDecoder;
    protected boolean noEOS = true;
    protected Resampler resampler = null;
    protected AudioReader audioReader;

    protected ByteBuffer[] audioDecoderInputBuffers = null;
    protected ByteBuffer[] audioDecoderOutputBuffers = null;
    protected ByteBuffer[] audioEncoderInputBuffers = null;
    protected ByteBuffer[] audioEncoderOutputBuffers = null;
    protected MediaCodec.BufferInfo audioDecoderOutputBufferInfo = null;
    protected MediaFormat decoderOutputAudioFormat = null;
    protected Uri uri;
    protected AudioFormat primaryAudioFormat;
    protected MediaFormat inputFormat;

    protected ByteBuffer resamplerBuffer;
    protected int resamplerBufferPosition = 0;
    protected int resamplerBufferLimit = 0;
    protected final int frameSize = 2048;
    protected final int maxDeltaHz = 12*2;

    final int targetSampleRate = 48000;
    final int targetChannelCount = 2;


    public void setFileUri(Uri uri) {
        this.uri = uri;
    }

    public void start(Context context, AudioFormat mediaFormat) {
        primaryAudioFormat = mediaFormat;
        audioExtractor = createExtractor(context);

        int audioInputTrack = getAndSelectAudioTrackIndex(audioExtractor);
        inputFormat = audioExtractor.getTrackFormat(audioInputTrack);

        int primarySampleRate = primaryAudioFormat.getAudioSampleRateInHz();
        int secondarySampleRate = inputFormat.getInteger("sample-rate");

        int primaryChanelCount = primaryAudioFormat.getAudioChannelCount();
        int secondaryChanelCount = inputFormat.getInteger("channel-count");

        boolean sampleRateSatisfy = false;
        boolean channelCountSatisfy = false;

        if ((primarySampleRate == targetSampleRate) && (secondarySampleRate == targetSampleRate)){
            sampleRateSatisfy = true;
        }
        if((primaryChanelCount == targetChannelCount)&&(secondaryChanelCount == targetChannelCount)){
            channelCountSatisfy = true;
        }

        // if sampleRate and channelCount both of streams satisfy the target parameters
        // when use standart audio effect applying
        if(sampleRateSatisfy && channelCountSatisfy){
            audioReader = new EqualRateAudioReader(audioExtractor, inputFormat);
        }
        else{
            audioReader = new DifferentRateAudioReader(audioExtractor, inputFormat, secondarySampleRate, secondaryChanelCount);
        }

        audioReader.start(context, mediaFormat);
    }

    public boolean read(ByteBuffer byteBuffer) {
        return audioReader.read(byteBuffer);
    }

    protected MediaExtractor createExtractor(Context context) {
        MediaExtractor extractor;
        extractor = new MediaExtractor();
        try {
            extractor.setDataSource(context, android.net.Uri.parse(uri.getString()), null);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return extractor;
    }

    protected int getAndSelectAudioTrackIndex(MediaExtractor extractor) {
        for (int index = 0; index < extractor.getTrackCount(); ++index) {
            if (isAudioFormat(extractor.getTrackFormat(index))) {
                extractor.selectTrack(index);
                return index;
            }
        }
        return -1;
    }

    protected static boolean isAudioFormat(MediaFormat format) {
        return getMimeTypeFor(format).startsWith("audio/");
    }

    protected static String getMimeTypeFor(MediaFormat format) {
        return format.getString(MediaFormat.KEY_MIME);
    }

    protected MediaCodec createAudioDecoder(MediaFormat inputFormat) {
        MediaCodec decoder = MediaCodec.createDecoderByType(getMimeTypeFor(inputFormat));
        decoder.configure(inputFormat, null, null, 0);
        decoder.start();
        return decoder;
    }

    public MediaFormat getDecoderOutputAudioFormat() {
        return decoderOutputAudioFormat;
    }
}
