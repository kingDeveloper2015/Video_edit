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
import android.widget.TextView;
import com.intel.inde.mp.MediaComposer;
import com.intel.inde.mp.MediaFileInfo;
import com.intel.inde.mp.android.AndroidMediaObjectFactory;
import com.intel.inde.mp.android.ResamplerAndroid;

import java.io.IOException;
import java.util.concurrent.TimeUnit;

public class ComposerJoinCoreActivity extends ComposerTranscodeCoreActivity {

    @Override
    protected void getActivityInputs() {
        Bundle b = getIntent().getExtras();
        srcMediaName1 = b.getString("srcMediaName1");
        srcMediaName2 = b.getString("srcMediaName2");
        dstMediaPath = b.getString("dstMediaPath");
        mediaUri1 = new com.intel.inde.mp.Uri(b.getString("srcUri1"));
        mediaUri2 = new com.intel.inde.mp.Uri(b.getString("srcUri2"));
    }

    @Override
    protected void setTranscodeParameters(MediaComposer mediaComposer) throws IOException {
        mediaComposer.addSourceFile(mediaUri1);
        mediaComposer.setTargetFile(dstMediaPath);

        configureVideoEncoder(mediaComposer, videoWidthOut, videoHeightOut);
        configureAudioEncoder(mediaComposer);

        mediaComposer.addSourceFile(mediaUri2);
    }

    @Override
    protected void printDuration() {
        long duration1 = duration;
        long duration2 = 0;

        try {
            MediaFileInfo mediaFileInfo = new MediaFileInfo(new AndroidMediaObjectFactory(getApplicationContext()));
            mediaFileInfo.setUri(mediaUri2);
            duration2 = mediaFileInfo.getDurationInMicroSec();
        } catch (Exception e) {
            e.printStackTrace();
        }

        TextView v = (TextView) findViewById(R.id.durationInfo);
        v.setText(String.format("durationSrc1 = %d sec\n", TimeUnit.MICROSECONDS.toSeconds(duration1)));
        v.append(String.format("durationSrc2 = %d sec\n", TimeUnit.MICROSECONDS.toSeconds(duration2)));
    }

    @Override
    protected void getDstDuration() {
        try {
            mediaFileInfo = new MediaFileInfo(new AndroidMediaObjectFactory(getApplicationContext()));
            mediaFileInfo.setUri(mediaUri2);

            duration += mediaFileInfo.getDurationInMicroSec();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void printPaths() {
        pathInfo.setText(String.format("srcMediaFileName1 = %s\nsrcMediaFileName2 = %s\ndstMediaPath = %s\n", srcMediaName1, srcMediaName2, dstMediaPath));
    }
}


