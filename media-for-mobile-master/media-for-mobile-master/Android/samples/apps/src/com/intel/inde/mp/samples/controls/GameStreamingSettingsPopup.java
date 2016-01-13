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

package com.intel.inde.mp.samples.controls;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import com.intel.inde.mp.StreamingParameters;
import com.intel.inde.mp.samples.GameRenderer;
import com.intel.inde.mp.samples.R;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class GameStreamingSettingsPopup extends Popup {

    private Context context;
    private Spinner renderingMethodList;

    public interface GameStreamingSettings {
        public void onRenderMethodChanged(GameRenderer.RenderingMethod method);
        public void onStreamingParamsChanged(StreamingParameters parameters);
    }

    GameStreamingSettings eventsListener;

    public GameStreamingSettingsPopup(Context context) {
        super(context);
        this.context = context;

        LayoutInflater inflator = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        setContentView(inflator.inflate(R.layout.popup_game_streaming_settings, null));

        fillRenderMethodsList();
    }

    public void setEventListener(GameStreamingSettings listener) {
        eventsListener = listener;
    }

    private void fillRenderMethodsList() {
        renderingMethodList = (Spinner)getContentView().findViewById(R.id.renderMethod);

        List<String> list = new ArrayList<String>();

        list.add("Render Twice");
        list.add("Using Frame Buffer");

        ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item, list);
        dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        renderingMethodList.setAdapter(dataAdapter);
    }

    @Override
    public void onDismiss() {
        int method = renderingMethodList.getSelectedItemPosition();

        GameRenderer.RenderingMethod renderingMethod;

        if (method == 0) {
            renderingMethod = GameRenderer.RenderingMethod.RenderTwice;
        } else {
            renderingMethod = GameRenderer.RenderingMethod.FrameBuffer;
        }

        eventsListener.onRenderMethodChanged(renderingMethod);

        StreamingParameters parameters = new StreamingParameters();

        parameters.Host = ((EditText)getContentView().findViewById(R.id.host)).getText().toString();
        parameters.Port = Integer.parseInt(((EditText)getContentView().findViewById(R.id.port)).getText().toString());
        parameters.ApplicationName = ((EditText)getContentView().findViewById(R.id.applicationName)).getText().toString();
        parameters.StreamName = ((EditText)getContentView().findViewById(R.id.streamName)).getText().toString();

        parameters.isToPublishAudio = false;
        parameters.isToPublishVideo = true;

        eventsListener.onStreamingParamsChanged(parameters);
    }

    public void setSettings(GameRenderer.RenderingMethod method, StreamingParameters params) {

        if (method == GameRenderer.RenderingMethod.RenderTwice) {
            renderingMethodList.setSelection(0);
        } else {
            renderingMethodList.setSelection(1);
        }

        ((EditText)getContentView().findViewById(R.id.host)).setText(params.Host);
        ((EditText)getContentView().findViewById(R.id.port)).setText(String.valueOf(params.Port));
        ((EditText)getContentView().findViewById(R.id.applicationName)).setText(params.ApplicationName);
        ((EditText)getContentView().findViewById(R.id.streamName)).setText(params.StreamName);
    }
}
