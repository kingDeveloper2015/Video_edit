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
import android.content.pm.ActivityInfo;
import android.hardware.Camera;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import com.intel.inde.mp.CameraCapture;
import com.intel.inde.mp.IProgressListener;
import com.intel.inde.mp.IVideoEffect;
import com.intel.inde.mp.StreamingParameters;
import com.intel.inde.mp.android.AndroidMediaObjectFactory;
import com.intel.inde.mp.android.AudioFormatAndroid;
import com.intel.inde.mp.android.VideoFormatAndroid;
import com.intel.inde.mp.android.graphics.VideoEffect;
import com.intel.inde.mp.domain.IPreview;
import com.intel.inde.mp.domain.Pair;
import com.intel.inde.mp.domain.Resolution;
import com.intel.inde.mp.effects.GrayScaleEffect;
import com.intel.inde.mp.effects.InverseEffect;
import com.intel.inde.mp.effects.SepiaEffect;
import com.intel.inde.mp.samples.controls.StreamingSettingsPopup;

import java.util.ArrayList;
import java.util.List;

public class CameraStreamerActivity extends ActivityWithTimeline implements StreamingSettingsPopup.CameraStreamingSettings {
    StreamingParameters parameters;

    public IProgressListener progressListener = new IProgressListener() {
        @Override
        public void onMediaStart() {}

        @Override
        public void onMediaProgress(float progress) {}

        @Override
        public void onMediaDone() {
            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        inProgress = false;
                        onStreamingDone();
                    }
                });
            } catch (Exception e) {}
        }

        @Override
        public void onMediaPause() {}

        @Override
        public void onMediaStop() {
            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        inProgress = false;
                    }
                });
            } catch (Exception e) {
            }
        }

        @Override
        public void onError(Exception exception) {
            try {
                final Exception e = exception;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        inProgress = false;
                        onStreamingDone();

                        String message = (e.getMessage() != null) ? e.getMessage() : e.toString();
                        showMessageBox("Capturing failed" + "\n" + message, null);
                    }
                });
            } catch (Exception e) {
            }
        }
    };

    class AllEffects implements IVideoEffect {
        private Pair<Long, Long> segment = new Pair<Long, Long>(0l, 0l);
        private ArrayList<IVideoEffect> videoEffects = new ArrayList<IVideoEffect>();
        private int activeEffectId;
        private long l;
        private long msPerFrame = 1;
        ArrayList<Long> lst = new ArrayList<Long>();
        static final int window = 10;

        @Override
        public Pair<Long, Long> getSegment() {
            return segment;
        }

        @Override
        public void setSegment(Pair<Long, Long> segment) {
        }

        @Override
        public void start() {
            for (IVideoEffect effect : videoEffects) {
                effect.start();
            }
        }

        @Override
        public void applyEffect(int inTextureId, long timeProgress, float[] transformMatrix) {
            long currentTime = System.nanoTime();
            msPerFrame = currentTime - l;
            l = currentTime;
            lst.add(msPerFrame);
            if (lst.size() > window) {
                lst.remove(0);
            }

            videoEffects.get(activeEffectId).applyEffect(inTextureId, timeProgress, transformMatrix);
        }

        @Override
        public void setInputResolution(Resolution resolution) {
            for (IVideoEffect videoEffect : videoEffects) {
                videoEffect.setInputResolution(resolution);
            }
        }

        @Override
        public boolean fitToCurrentSurface(boolean should) {
            boolean fitValue = false;
            for (IVideoEffect videoEffect : videoEffects) {
                fitValue = videoEffect.fitToCurrentSurface(should);
            }
            return fitValue;
        }

        public void setActiveEffectId(int activeEffectId) {
            this.activeEffectId = activeEffectId;
        }

        public int getActiveEffectId() {
            return activeEffectId;
        }

        public ArrayList<IVideoEffect> getVideoEffects() {
            return videoEffects;
        }
    }

    Camera camera = null;
    private int camera_type = 0;
    CameraCapture capture = null;
    private IPreview preview;

    private AndroidMediaObjectFactory factory;
    private int activeEffectId;
    AllEffects allEffects = new AllEffects();
    private GLSurfaceView surfaceView;

    private AudioFormatAndroid audioFormat;
    private VideoFormatAndroid videoFormat;

    private boolean inProgress = false;
    private boolean isActive = false;


    public void onCreate(Bundle icicle) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        Intent intent = getIntent();
        camera_type = intent.getIntExtra("CAMERA_TYPE", 0);

        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.camera_streamer_activity);

        camera = createCamera();

        factory = new AndroidMediaObjectFactory(getApplicationContext());

        parameters = new StreamingParameters();

        parameters.Host = getString(R.string.streaming_server_default_ip);
        parameters.Port = Integer.parseInt(getString(R.string.streaming_server_default_port));
        parameters.ApplicationName = getString(R.string.streaming_server_default_app);
        parameters.StreamName = getString(R.string.streaming_server_default_stream);

        parameters.isToPublishAudio = false;
        parameters.isToPublishVideo = true;

        configureEffects(factory);
        createCapturePipeline();
        createPreview();
    }

    private void configureEffects(final AndroidMediaObjectFactory factory) {

        ArrayList<IVideoEffect> videoEffects = allEffects.getVideoEffects();

        if(videoEffects != null) { videoEffects.clear(); }

        allEffects.getVideoEffects().add(new VideoEffect(0, factory.getEglUtil()) {
        });
        allEffects.getVideoEffects().add(new GrayScaleEffect(0, factory.getEglUtil()));
        allEffects.getVideoEffects().add(new SepiaEffect(0, factory.getEglUtil()));
        allEffects.getVideoEffects().add(new InverseEffect(0, factory.getEglUtil()));
    }

    @Override
    protected void onPause() {
        if (isActive) {
            stopStreaming();
            if (camera != null) {

                saveSettings();

                destroyPreview();
                destroyCamera();
                destroyCapturePipeline();
            }
            isActive = false;
        }

        super.onPause();
    }

    @Override
    protected void onResume() {
        if (!isActive) {
            if  (camera == null) {
                camera = createCamera();

                factory = new AndroidMediaObjectFactory(getApplicationContext());
                createCapturePipeline();

                configureEffects(factory);
                createPreview();
            }
            isActive = true;
        }

        restoreSettings();
        super.onResume();
    }

    private void saveSettings() {
        //Save camera effect settings
        activeEffectId = allEffects.getActiveEffectId();
    }

    private void restoreSettings() {
        //Restore saved effect settings
        allEffects.setActiveEffectId(activeEffectId);
        preview.setActiveEffect(allEffects);
    }


    private void createCapturePipeline() {
        capture = new CameraCapture(factory, progressListener);
        if (allEffects != null) {
            capture.addVideoEffect(allEffects);
        }
    }

    private void destroyCapturePipeline() {
        capture = null;
    }

    private void createPreview() {
        surfaceView = new GLSurfaceView(getApplicationContext());

        ((RelativeLayout)findViewById(R.id.streamer_layout)).addView(surfaceView, 0);
        preview = capture.createPreview(surfaceView, camera);
        preview.start();
    }

    private void destroyPreview() {
        preview.stop();
        preview = null;
        ((RelativeLayout)findViewById(R.id.streamer_layout)).removeView(surfaceView);
        surfaceView = null;
    }

    private StreamingParameters prepareStreamingParams() {
        return parameters;
    }

    private void configureMediaStreamFormat() {

        videoFormat = new VideoFormatAndroid("video/avc", 640, 480);
        videoFormat.setVideoBitRateInKBytes(1000);
        videoFormat.setVideoFrameRate(25);
        videoFormat.setVideoIFrameInterval(1);

        audioFormat = new AudioFormatAndroid("audio/mp4a-latm", 44100, 1);
    }

    private Camera createCamera() {
        Camera camera = Camera.open(camera_type);

        List<Camera.Size> supportedResolutions = camera.getParameters().getSupportedPreviewSizes();

        Camera.Size maxCameraResolution = supportedResolutions.get(0);

        for (Camera.Size size : supportedResolutions) {

            if (maxCameraResolution.width < size.width) {
                maxCameraResolution = size;
            }
        }

        Camera.Parameters parameters = camera.getParameters();
        parameters.setPreviewSize(maxCameraResolution.width, maxCameraResolution.height);
        parameters.setRecordingHint(true);
        CameraUtils utils = new CameraUtils(parameters).invoke();
        parameters.setPreviewFpsRange(utils.getMaxFps0(), utils.getMaxFps1());


        camera.setParameters(parameters);

        return camera;
    }

    private void destroyCamera() {
        camera.release();
        camera = null;
    }

    public void startStreaming() {
        updateUI(true);

        configureMediaStreamFormat();
        capture.setTargetVideoFormat(videoFormat);
        capture.setTargetAudioFormat(audioFormat);
        capture.setTargetConnection(prepareStreamingParams());

        captureStart();
        inProgress = true;
    }


    private void captureStart ()
    {
        configureMediaStreamFormat();

        capture.start();
    }

    public void stopStreaming() {
        updateUI(false);
        if (inProgress && capture != null) {
            capture.stop();

            configureEffects(factory);
            preview.setActiveEffect(allEffects);
        }
        inProgress = false;
    }

    public void onStreamingDone() {

        updateUI(false);
    }

    private void updateUI(boolean inProgress) {
        ImageButton settingsButton = (ImageButton)findViewById(R.id.settings);
        ImageButton captureButton = (ImageButton)findViewById(R.id.start);
        ImageButton changeCameraButton = (ImageButton) findViewById(R.id.change_camera);

        ScrollView container = (ScrollView)findViewById(R.id.effectsContainer);

        if (inProgress == false) {
            captureButton.setImageResource(R.drawable.rec_inact);
            settingsButton.setVisibility(View.VISIBLE);
            container.setVisibility(View.VISIBLE);
            changeCameraButton.setVisibility(View.VISIBLE);
        } else {
            captureButton.setImageResource(R.drawable.rec_act);
            settingsButton.setVisibility(View.INVISIBLE);
            container.setVisibility(View.INVISIBLE);
            changeCameraButton.setVisibility(View.INVISIBLE);
        }
    }

    public void changeCamera(View view) {
        if (camera_type == 0) {
            camera_type = 1;
        } else {
            camera_type = 0;
        }

        if (camera_type >= Camera.getNumberOfCameras())
            camera_type -= Camera.getNumberOfCameras();

        if (camera != null) {
            Intent intent = getIntent();
            intent.putExtra("CAMERA_TYPE", camera_type);
            overridePendingTransition(0,0);
            finish();
            overridePendingTransition(0,0);
            startActivity(intent);
        }
    }

    public void onSettings(View view) {
        StreamingSettingsPopup settingsPopup = new StreamingSettingsPopup(this);
        settingsPopup.setEventListener(this);
        settingsPopup.setSettings(parameters);

        settingsPopup.show(view, false);
    }

    public void onStreaming(View view) {
        if (inProgress) {
            stopStreaming();
        } else {
            startStreaming();
        }
    }

    public void onClickEffect(View v) {
        if(inProgress) {
            return;
        }

        switch (v.getId()) {
            default: {
                String tag = (String) v.getTag();

                if(tag != null)
                {
                   allEffects.setActiveEffectId(Integer.parseInt(tag));
                   preview.setActiveEffect(allEffects);
                }
            }
            break;
        }
    }

    @Override
    public void onStreamingParamsChanged(StreamingParameters parameters) {
        this.parameters = parameters;
    }
}
