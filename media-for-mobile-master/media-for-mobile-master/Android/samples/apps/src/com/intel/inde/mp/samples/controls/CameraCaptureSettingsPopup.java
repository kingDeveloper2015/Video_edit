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
import android.hardware.Camera;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.Spinner;
import com.intel.inde.mp.samples.R;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CameraCaptureSettingsPopup extends Popup {

    private Context context;
    List<Camera.Size> supportedResolutions;

    public interface CameraCaptureSettings {
        public void displayResolutionChanged(int width, int height);

        public void videoResolutionChanged(int width, int height);

        public void audioRecordChanged(boolean bState);
    }

    CameraCaptureSettings eventsListener;

    public CameraCaptureSettingsPopup(Context context, List<Camera.Size> resolutions, CameraCaptureSettings listener) {
        super(context);

        supportedResolutions = resolutions;
        this.context = context;
        eventsListener = listener;

        LayoutInflater inflator = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        setContentView(inflator.inflate(R.layout.popup_camera_capture_settings, null));

        setupCameraResolutionsSpinner();
        setupOutputResolutionsSpinner();

        final CheckBox audioRecord = (CheckBox) getContentView().findViewById(R.id.recordAudio);
        audioRecord.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                eventsListener.audioRecordChanged(audioRecord.isChecked());
            }
        });
    }

    private void setupCameraResolutionsSpinner() {
        Spinner spinner = (Spinner) getContentView().findViewById(R.id.cam_resolutions);
        List<String> spinner_data = new ArrayList();

        for (Camera.Size sz : supportedResolutions) {
            spinner_data.add(sz.width + " x " + sz.height);
        }

        ArrayAdapter<String> adapter = new ArrayAdapter(context, android.R.layout.simple_spinner_item, spinner_data);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long id) {

                eventsListener.displayResolutionChanged(supportedResolutions.get((int) id).width, supportedResolutions.get((int) id).height);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
            }
        });
    }

    private void setupOutputResolutionsSpinner() {
        Spinner spinner = (Spinner) getContentView().findViewById(R.id.encoded_resolutions);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(context, R.array.frame_size_values, android.R.layout.simple_spinner_item);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            final Pattern pattern = Pattern.compile("(\\d*)\\s*[xX]\\s*(\\d*)");

            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long id) {
                String resolution = adapterView.getAdapter().getItem((int) id).toString();
                Matcher matcher = pattern.matcher(resolution);
                if (!matcher.find()) {
                    throw new RuntimeException("Invalid resolution string: " + resolution);
                }
                eventsListener.videoResolutionChanged(new Integer(matcher.group(1)), new Integer(matcher.group(2)));
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
            }
        });

        spinner.setSelection(0);
    }
}
