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
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import com.intel.inde.mp.samples.R;

public class PlaybackToolbar extends LinearLayout implements View.OnClickListener, SeekBar.OnSeekBarChangeListener
{
	//////////////////////////////////////////////////////////////////////
	// Callback Interface
	//////////////////////////////////////////////////////////////////////
	
	public interface OnCommandListener
	{
		void onPlaybackToolbarPlay();
		void onPlaybackToolbarPause();

		void onPlaybackToolbarPositionChanged(long value);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Controls
	//////////////////////////////////////////////////////////////////////
	
	private ImageButton mPlayButton;
	private ImageButton mPauseButton;
	private TextView mPlayTimeText;
	private SeekBar mPlayProgress;

	//////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////
	
	private long mDuration = 0;
	private long mPosition = 0;

	private OnCommandListener mCallback;
	
	public PlaybackToolbar(Context context)
	{
		super(context);
	}
	
	public PlaybackToolbar(Context context, AttributeSet attrs) 
	{
        super(context, attrs);
 
		LayoutInflater li = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        li.inflate(R.layout.playback_toolbar, this, true);
        
        bindControls();
        
        setDuration(0);
        setPosition(0);
    }
	
	private void bindControls()
	{
		mPlayButton = (ImageButton) findViewById(R.id.playButton);		
		mPlayButton.setOnClickListener(this);
		
		mPauseButton = (ImageButton) findViewById(R.id.pauseButton);		
		mPauseButton.setOnClickListener(this);

		mPlayTimeText = (TextView)findViewById(R.id.playTimeText);
		
		mPlayProgress = (SeekBar)findViewById(R.id.playProgress);
		mPlayProgress.setOnSeekBarChangeListener(this);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Interface
	//////////////////////////////////////////////////////////////////////
	
	public void setOnCommandListener(OnCommandListener listener)
	{
		mCallback = listener;
	}
	
	public void setPlayState()
	{
		mPlayButton.setVisibility(View.INVISIBLE);
		mPauseButton.setVisibility(View.VISIBLE);
	}
	
	public void setPauseState()
	{
		mPlayButton.setVisibility(View.VISIBLE);
		mPauseButton.setVisibility(View.INVISIBLE);
	}
	
	public void setPosition(long position)
	{
		mPosition = position;
		
		updateControls();
	}
	
	public void setDuration(long duration)
	{
		mDuration = duration;
		
		if(mDuration > 0)
		{
			mPlayProgress.setMax(100);
		}
		
		updateControls();
	}
	
	public void showToolbar(Boolean show)
	{
		TranslateAnimation anim = null;	
		
		if(show)
        {
			setVisibility(View.VISIBLE);
			anim = new TranslateAnimation(0.0f, 0.0f, this.getHeight(), 0.0f);
        }
		else
		{
			anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, this.getHeight());
			
			anim.setAnimationListener(toolbarAnimationListener);
		}
		
        anim.setDuration(300);
        anim.setInterpolator(new AccelerateInterpolator(1.0f));
        this.startAnimation(anim);
	}
	
	public void toggleToolbar()
	{
		showToolbar(getVisibility() != View.VISIBLE);
	}
	
	//////////////////////////////////////////////////////////////////////
	// View.OnClickListener Implementation
	//////////////////////////////////////////////////////////////////////
	
	public void onClick(View v) 
	{
		switch (v.getId()) 
		{
			case R.id.playButton:
			{
				onMediaPlay();
			}
			break;
			
			case R.id.pauseButton: 
			{
				onMediaPause();
			}
			break;
		}
	}
	
	//////////////////////////////////////////////////////////////////////
	// SeekBar.OnSeekBarChangeListener Implementation
	//////////////////////////////////////////////////////////////////////
	
	@Override
	public void onProgressChanged(SeekBar view, int value, boolean user)
	{		
		if(user == false)
		{
			return;
		}
		
		long newPosition = (long)Math.round((mDuration * value) / 100f);
		
		mCallback.onPlaybackToolbarPositionChanged(newPosition);
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar)
	{		
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar)
	{
	}
	
	//////////////////////////////////////////////////////////////////////
	// Click Handlers Implementation
	//////////////////////////////////////////////////////////////////////
	
	private void onMediaPlay()
	{
		mCallback.onPlaybackToolbarPlay();
	}
	
	private void onMediaPause()
	{
		mCallback.onPlaybackToolbarPause();
	}

	//////////////////////////////////////////////////////////////////////
	// Show\Hide Animation
	//////////////////////////////////////////////////////////////////////
	
	Animation.AnimationListener toolbarAnimationListener = new Animation.AnimationListener() 
	{
	    public void onAnimationEnd(Animation animation) 
	    {
	    	setVisibility(View.GONE);
	    }

	    @Override
	    public void onAnimationRepeat(Animation animation) 
	    {
	    }

	    @Override
	    public void onAnimationStart(Animation animation) 
	    {
	    }
	};
	
	//////////////////////////////////////////////////////////////////////
	// Update UI
	//////////////////////////////////////////////////////////////////////
	
	private void updateControls()
	{
        long duration = mPosition / 1000;

        long h = duration / 3600;
        long m = (duration - h * 3600) / 60;
        long s = duration - (h * 3600 + m * 60);

        String time;

        if (h == 0)
        {
            time = asTwoDigit(m) + ":" + asTwoDigit(s);
        }
        else
        {
            time = asTwoDigit(h) + ":" + asTwoDigit(m) + ":" + asTwoDigit(s);
        }
	    
	    mPlayTimeText.setText(time);
	    
	    int playProgress = 0;
	    
	    if(mDuration != 0)
	    {
	    	playProgress = (int)((mPosition * 100) / mDuration);
	    }	    
	    
	    mPlayProgress.setProgress(playProgress);
	}

    private String asTwoDigit(long digit)
    {
        String value = "";

        if (digit < 10)
        {
            value = "0";
        }

        value += String.valueOf(digit);

        return value;
    }

}
