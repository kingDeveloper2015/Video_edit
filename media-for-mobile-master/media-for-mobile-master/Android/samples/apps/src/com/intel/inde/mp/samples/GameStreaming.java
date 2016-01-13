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

import android.content.Context;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import com.intel.inde.mp.StreamingParameters;
import com.intel.inde.mp.samples.controls.GameCaptureSettingsPopup;
import com.intel.inde.mp.samples.controls.GameStreamingSettingsPopup;

public class GameStreaming extends GameCapturing implements GameStreamingSettingsPopup.GameStreamingSettings {

    StreamingParameters parameters;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        parameters = new StreamingParameters();

        parameters.Host = getString(R.string.streaming_server_default_ip);
        parameters.Port = Integer.parseInt(getString(R.string.streaming_server_default_port));
        parameters.ApplicationName = getString(R.string.streaming_server_default_app);
        parameters.StreamName = getString(R.string.streaming_server_default_stream);

        parameters.isToPublishAudio = false;
        parameters.isToPublishVideo = true;
    }

    private StreamingParameters prepareStreamingParams() throws Exception {
        return parameters;
    }

    @Override
    protected void startCapturing() {
        try {
            gameRenderer.startCapturing(prepareStreamingParams());
        } catch (Exception e) {
            showToast("Failed to setup streaming parameters.");
            e.printStackTrace();
        }
    }

    public void showSettings(View view) {
        GameStreamingSettingsPopup settingsPopup = new GameStreamingSettingsPopup(this);

        settingsPopup.setEventListener(this);
        settingsPopup.setSettings(renderingMethod, parameters);
        settingsPopup.show(view, false);
    }

    @Override
    public void onStreamingParamsChanged(StreamingParameters parameters) {
        this.parameters = parameters;
    }
}
