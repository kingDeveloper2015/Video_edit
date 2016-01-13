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
import com.intel.inde.mp.MediaFile;
import com.intel.inde.mp.Uri;
import com.intel.inde.mp.domain.Pair;

import java.io.IOException;

public class ComposerCutCoreActivity extends ComposerTranscodeCoreActivity {

    private long segmentFrom = 0;
    private long segmentTo = 0;

    @Override
    protected void getActivityInputs() {

        Bundle b = getIntent().getExtras();
        srcMediaName1 = b.getString("srcMediaName1");
        dstMediaPath = b.getString("dstMediaPath");
        mediaUri1 = new Uri(b.getString("srcUri1"));

        segmentFrom = b.getLong("segmentFrom");
        segmentTo = b.getLong("segmentTo");
    }

    @Override
    protected void setTranscodeParameters(MediaComposer mediaComposer) throws IOException {
        mediaComposer.addSourceFile(mediaUri1);
        mediaComposer.setTargetFile(dstMediaPath);

        configureVideoEncoder(mediaComposer, videoWidthOut, videoHeightOut);
        configureAudioEncoder(mediaComposer);

        ///////////////////////////

        MediaFile mediaFile = mediaComposer.getSourceFiles().get(0);
        mediaFile.addSegment(new Pair<Long, Long>(segmentFrom, segmentTo));
    }

    @Override
    protected void printDuration() {

    	TextView v = (TextView)findViewById(R.id.durationInfo);
        v.setText(String.format("duration = %.1f sec\n", (float)(segmentTo - segmentFrom)/1e6));
        v.append(String.format("from = %.1f sec\nto = %.1f sec\n", (float)segmentFrom/1e6, (float)segmentTo/1e6));
    }
}

