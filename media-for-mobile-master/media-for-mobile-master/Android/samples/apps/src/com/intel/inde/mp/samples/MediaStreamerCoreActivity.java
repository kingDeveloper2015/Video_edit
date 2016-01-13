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
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.*;
import android.widget.*;
import com.intel.inde.mp.*;
import com.intel.inde.mp.android.AndroidMediaObjectFactory;
import com.intel.inde.mp.domain.ISurfaceWrapper;
import com.intel.inde.mp.samples.controls.TranscodeSurfaceView;

import java.nio.ByteBuffer;
import java.util.Timer;
import java.util.TimerTask;

public class MediaStreamerCoreActivity extends ActivityWithTimeline implements SurfaceHolder.Callback {

    public static final int UPDATE_TIMER = 1;

    private boolean inProgress;

    private ToggleButton buttonStart;
    private ProgressBar progressBar;
    private long startTime;
    private TextView time;
    private Timer timer;

    private TableLayout table;

    final Handler uiHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case UPDATE_TIMER: {
                    updateTimer();
                }
                break;
            }
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Media File Info
    private long duration;

    private Uri mediaUri;
    private int videoWidthIn;

    private int videoHeightIn;

    private boolean isActive = false;

    // Media Composer parameters and logic

    private MediaFileInfo mediaFileInfo;
    protected MediaStreamer mediaStreamer;
    public IProgressListener streamingProgressListener = new IProgressListener() {
        @Override
        public void onMediaStart() {
            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        startTime = System.currentTimeMillis();

                        timer = new Timer();
                        timer.schedule(new UpdateTimerTask(), 0, 1000);
                    }
                });
            } catch (Exception ex) {
            }
        }

        @Override
        public void onMediaProgress(float progress) {

            final float mediaProgress = progress;
            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBar.setProgress((int) (progressBar.getMax() * mediaProgress));
                    }
                });
            } catch (Exception ex) {
            }
        }

        @Override
        public void onMediaDone() {

            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {

                        if (timer != null) {
                            timer.cancel();
                        }

                        inProgress = false;
                        if(!isActive) {
                            return;
                        }
                        onStreamingDone();
                    }
                });
            } catch (Exception ex) {
            }
        }

        @Override
        public void onMediaPause() {
        }

        @Override
        public void onMediaStop() {
            try {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        inProgress = false;
                    }
                });
            } catch (Exception ex) {
            }
        }

        @Override
        public void onError(Exception exception) {
            try {
                final Exception e = exception;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (!isActive) {
                            return;
                        }
                        updateUI(false);
                        String message = (e.getMessage() != null) ? e.getMessage() : e.toString();
                        showMessageBox("Streaming failed." + "\n" + message, null);
                    }
                });
            } catch (Exception e) {
            }
        }
    };

    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        requestWindowFeature(Window.FEATURE_NO_TITLE);

        setContentView(R.layout.media_streamer_activity);

        getFileInfo();

        TranscodeSurfaceView surfaceView = (TranscodeSurfaceView) findViewById(R.id.surfaceView);
        surfaceView.getHolder().addCallback(this);
        surfaceView.setImageSize(videoWidthIn, videoHeightIn);

        setupUI();
    }

    @Override
    public void onPause() {
        isActive = false;
        super.onPause();
    }

    @Override
    protected void onResume() {
        isActive = true;
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        startStreaming();
        super.onDestroy();
    }

    protected void setupUI()
    {
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        table = (TableLayout)inflater.inflate(R.layout.streaming_params, null);
        ((LinearLayout)findViewById(R.id.linearLayout)).addView(table, 1);
        findViewById(R.id.host).requestFocus(Gravity.END);

        progressBar = (ProgressBar) findViewById(R.id.progressBar);
        progressBar.setMax(1000);

        buttonStart = (ToggleButton) findViewById(R.id.toggleButtonStart);
        buttonStart.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    startStreaming();
                } else {
                    stopStreaming();
                }
            }
        });

        time = (TextView) findViewById(R.id.time);
    }

    protected void getActivityInputs() {

        Bundle b = getIntent().getExtras();
        mediaUri = new com.intel.inde.mp.Uri(b.getString("srcUri1"));
    }

    protected void getFileInfo() {

        getActivityInputs();

        try {
            mediaFileInfo = new MediaFileInfo(new AndroidMediaObjectFactory(getApplicationContext()));
            mediaFileInfo.setUri(mediaUri);

            duration = mediaFileInfo.getDurationInMicroSec();

            VideoFormat videoFormat = (VideoFormat) mediaFileInfo.getVideoFormat();

            videoWidthIn = videoFormat.getVideoFrameSize().width();
            videoHeightIn = videoFormat.getVideoFrameSize().height();

        } catch (Exception e) {

            updateUI(false);

            String message = "Failed to get file info.";
            showMessageBox(message, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                }
            });
        }
    }

    protected void displayVideoFrame(SurfaceHolder holder) {
        try {
            ISurfaceWrapper surface = AndroidMediaObjectFactory.Converter.convert(holder.getSurface());
            mediaFileInfo.setOutputSurface(surface);

            ByteBuffer buffer = ByteBuffer.allocate(1);
            mediaFileInfo.getFrameAtPosition(duration/10, buffer);

        } catch (Exception e) {

            updateUI(false);

            String message = "Failed to display a frame.";
            showMessageBox(message, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                }
            });
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        displayVideoFrame(holder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    private StreamingParameters prepareStreamingParams() {
        StreamingParameters parameters = new StreamingParameters();
        parameters.Host = ((EditText) findViewById(R.id.host)).getText().toString();
        parameters.Port = Integer.parseInt(((EditText) findViewById(R.id.port)).getText().toString());
        parameters.ApplicationName = ((EditText) findViewById(R.id.applicationName)).getText().toString();
        parameters.StreamName = ((EditText) findViewById(R.id.streamName)).getText().toString();

        parameters.isToPublishAudio = true;
        parameters.isToPublishVideo = true;

        parameters.streamingFromFile = true;

        return parameters;
    }

    public void startStreaming() {

        updateUI(true);

        try {
            mediaStreamer = new MediaStreamer(new AndroidMediaObjectFactory(getApplicationContext()), streamingProgressListener);
            mediaStreamer.addSourceFile(mediaUri);
            mediaStreamer.setTargetConnection(prepareStreamingParams());
            mediaStreamer.start();

            inProgress = true;

        } catch (Exception e) {

            updateUI(false);

            String message = "Failed to start media streamer.";
            showMessageBox(message, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                }
            });
        }
    }

    private void stopStreaming() {
        updateUI(false);

        if(inProgress && mediaStreamer != null) {
            mediaStreamer.stop();
        }
    }

    private void updateUI(boolean inProgress) {

        if(inProgress) {

            progressBar.setVisibility(View.VISIBLE);

            table.setVisibility(View.GONE);
        } else {

            progressBar.setVisibility(View.GONE);

            buttonStart.setVisibility(View.INVISIBLE);
        }
    }

    public void onStreamingDone() {

        updateUI(false);

        showMessageBox("Streaming finished.", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
            }
        });
    }

    public void updateTimer() {
        String timeText = Format.duration(System.currentTimeMillis() - startTime);
        time.setText(timeText);
    }

    private class UpdateTimerTask extends TimerTask {
        @Override
        public void run() {
            uiHandler.sendMessage(uiHandler.obtainMessage(UPDATE_TIMER));
        }
    }
}
