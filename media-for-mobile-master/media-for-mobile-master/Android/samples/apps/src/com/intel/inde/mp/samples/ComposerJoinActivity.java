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

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import com.intel.inde.mp.Uri;
import com.intel.inde.mp.samples.controls.TimelineItem;

public class ComposerJoinActivity extends ActivityWithTimeline implements View.OnClickListener {
    TimelineItem item1 = null;
    TimelineItem item2 = null;

    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.composer_join_activity);

        init();
    }

    private void init() {
        item1 = (TimelineItem) findViewById(R.id.timelineItem1);
        item1.setEventsListener(this);
        item1.enableSegmentPicker(false);

        item2 = (TimelineItem) findViewById(R.id.timelineItem2);
        item2.setEventsListener(this);
        item2.enableSegmentPicker(false);

        findViewById(R.id.action).setOnClickListener(this);
    }

    public void action() {
        String mediaFileName1 = item1.getMediaFileName();
        String mediaFileName2 = item2.getMediaFileName();

        if (mediaFileName1 == null || mediaFileName2 == null) {
            showToast("Please select valid video files first.");

            return;
        }

        item1.stopVideoView();
        item2.stopVideoView();

        Intent intent = new Intent();
        intent.setClass(this, ComposerJoinCoreActivity.class);

        Bundle bundle = new Bundle();
        bundle.putString("srcMediaName1", item1.getMediaFileName());
        intent.putExtras(bundle);
        bundle.putString("srcMediaName2", item2.getMediaFileName());
        intent.putExtras(bundle);
        bundle.putString("dstMediaPath", item1.genDstPath(item1.getMediaFileName(), "joined"));
        intent.putExtras(bundle);
        bundle.putString("srcUri1", item1.getUri().getString());
        intent.putExtras(bundle);
        bundle.putString("srcUri2", item2.getUri().getString());
        intent.putExtras(bundle);

        startActivity(intent);
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();

        if (item1 != null) {
            item1.updateView();
        }

        if (item2 != null) {
            item2.updateView();
        }
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();

        switch (id) {
            case R.id.action: {
                action();
            }
            break;
        }
    }
}
