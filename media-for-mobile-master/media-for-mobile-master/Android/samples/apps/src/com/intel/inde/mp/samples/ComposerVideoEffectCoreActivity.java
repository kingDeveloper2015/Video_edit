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

package com.intel.inde.mp.samples;

import android.os.Bundle;
import com.intel.inde.mp.IVideoEffect;
import com.intel.inde.mp.MediaComposer;
import com.intel.inde.mp.Uri;
import com.intel.inde.mp.domain.Pair;
import com.intel.inde.mp.effects.*;

import java.io.IOException;


public class ComposerVideoEffectCoreActivity extends ComposerTranscodeCoreActivity {

    private int effectIndex;

    @Override
    protected void getActivityInputs() {

        Bundle b = getIntent().getExtras();
        srcMediaName1 = b.getString("srcMediaName1");
        dstMediaPath = b.getString("dstMediaPath");
        mediaUri1 = new Uri(b.getString("srcUri1"));

        effectIndex = b.getInt("effectIndex");
    }

    @Override
    protected void setTranscodeParameters(MediaComposer mediaComposer) throws IOException {
        mediaComposer.addSourceFile(mediaUri1);
        mediaComposer.setTargetFile(dstMediaPath);

        configureVideoEncoder(mediaComposer, videoWidthOut, videoHeightOut);
        configureAudioEncoder(mediaComposer);

        configureVideoEffect(mediaComposer);
    }

    private void configureVideoEffect(MediaComposer mediaComposer) {
        IVideoEffect effect = null;

        switch (effectIndex) {
            case 0:
                effect = new SepiaEffect(0, factory.getEglUtil());
                break;
            case 1:
                effect = new GrayScaleEffect(0, factory.getEglUtil());
                break;
            case 2:
                effect = new InverseEffect(0, factory.getEglUtil());
                break;
            case 3:
                effect = new TextOverlayEffect(0, factory.getEglUtil());
                break;
            default:
                break;
        }

        if (effect != null) {
            effect.setSegment(new Pair<Long, Long>(0l, 0l)); // Apply to the entire stream
            mediaComposer.addVideoEffect(effect);
        }
    }

    @Override
    protected void printEffectDetails() {
        effectDetails.append(String.format("Video effect = %s\n", getVideoEffectName(effectIndex)));
    }

    private String getVideoEffectName(int videoEffectIndex) {
        switch (videoEffectIndex) {
            case 0:
                return "Sepia";
            case 1:
                return "Grayscale";
            case 2:
                return "Inverse";
            case 3:
                return "Text Overlay";
            default:
                return "Unknown";
        }
    }
}


