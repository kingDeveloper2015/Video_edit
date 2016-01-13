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
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.*;
import android.widget.*;
import com.intel.inde.mp.StreamingParameters;

public class VideoStreamPlayerActivity extends ActivityWithTimeline {

    private ToggleButton buttonStart;
    private VideoView videoView;
    private boolean inProgress = false;
    private LinearLayout linearLayout;
    private ProgressBar progressBar = null;

    private TableLayout table;

    private static final int UNCHECK_BUTTON = 2;
    private static final int ENABLE_BUTTON = 3;
    private static final int DISABLE_BUTTON = 4;

    final private Handler uiHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case UNCHECK_BUTTON: {
                    buttonStart.setChecked(false);
                    break;
                }
                case ENABLE_BUTTON: {
                    buttonStart.setEnabled(true);
                    break;
                }
                case DISABLE_BUTTON: {
                    buttonStart.setEnabled(false);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    };

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        requestWindowFeature(Window.FEATURE_NO_TITLE);

        setContentView(R.layout.video_stream_player_activity);

        if(!inProgress) {
            setupUI();
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if(inProgress) {
            videoView.resume();
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        if(inProgress) {
            videoView.pause();
        }
    }

    @Override
    public void onDestroy() {
        super.onPause();
        if(inProgress) {
            videoView.stopPlayback();
        }
    }

    protected void setupUI()
    {
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        table = (TableLayout)inflater.inflate(R.layout.streaming_params, null);

        if(table == null) {
            showMessageBox("Failed to setup UI.", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    finish();
                }
            });
        } else {
            linearLayout = (LinearLayout) findViewById(R.id.linearLayout);
            linearLayout.addView(table, 0);
            findViewById(R.id.host).requestFocus(Gravity.END);
        }

        buttonStart = (ToggleButton) findViewById(R.id.toggleButtonStart);
        buttonStart.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                startStreaming(isChecked);
            }
        });

        videoView = (VideoView) findViewById(R.id.video_view);
    }

    private StreamingParameters prepareStreamingParams() {
        StreamingParameters parameters = new StreamingParameters();

        try {
            parameters.Host = ((EditText) findViewById(R.id.host)).getText().toString();
            parameters.Port = Integer.parseInt(((EditText) findViewById(R.id.port)).getText().toString());
            parameters.ApplicationName = ((EditText) findViewById(R.id.applicationName)).getText().toString();
            parameters.StreamName = ((EditText) findViewById(R.id.streamName)).getText().toString();
        }
        catch (NullPointerException e) {
            showMessageBox(e.getMessage(), new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    finish();
                }
            });
        }

        return parameters;
    }

    public void startStreaming(boolean startStreaming) {

        inProgress = startStreaming;

        updateUI(startStreaming);

        if(startStreaming) {

            uiHandler.sendMessage(uiHandler.obtainMessage(DISABLE_BUTTON));

            StreamingParameters streamingParameters = prepareStreamingParams();
            String path = "http://" + streamingParameters.Host + ":"+ streamingParameters.Port + "/" +
                    streamingParameters.ApplicationName + "/" +
                    streamingParameters.StreamName + "/playlist.m3u8";

            progressBar = new ProgressBar(getApplicationContext(), null, android.R.attr.progressBarStyleLarge);
            linearLayout.addView(progressBar, 0);

            videoView.setVideoPath(path);

            videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer arg) {

                    if(progressBar != null) {
                        linearLayout.removeView(progressBar);
                    }
                    uiHandler.sendMessage(uiHandler.obtainMessage(ENABLE_BUTTON));

                    videoView.start();
                }
            });

            videoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {
                @Override
                public boolean onError(MediaPlayer mp, int what, int extra) {

                    if(progressBar != null) {
                        linearLayout.removeView(progressBar);
                    }
                    uiHandler.sendMessage(uiHandler.obtainMessage(ENABLE_BUTTON));

                    uiHandler.sendMessage(uiHandler.obtainMessage(UNCHECK_BUTTON));

                    String message = "Can't play the video.\n Unknown error.";

                    if(extra == MediaPlayer.MEDIA_ERROR_IO) {
                        message = "Can't play the video.\n File or network related error.";
                    }
                    if(extra == MediaPlayer.MEDIA_ERROR_TIMED_OUT) {
                        message = "Can't play the video.\nTimeout error.";
                    }

                    final String uiMessage = message;

                    try {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {

                                showMessageBox(uiMessage, new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                    }
                                });

                                table.setVisibility(View.VISIBLE);
                            }
                        });
                    } catch (Exception ex) {
                    }

                    return true;
                }
            });
        }
        else {
            videoView.stopPlayback();
        }
    }

    private void updateUI(boolean inProgress) {

        if(inProgress) {
            table.setVisibility(View.INVISIBLE);
        }
        else {
            table.setVisibility(View.VISIBLE);
        }
    }
}
