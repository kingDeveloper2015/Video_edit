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
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.PopupWindow;
import com.intel.inde.mp.samples.R;

public class Popup extends PopupWindow implements OnTouchListener, PopupWindow.OnDismissListener {
    protected Context context;
    protected View contentView;

    public Popup(Context context) {
        super(context);

        this.context = context;

        init();
    }

    public void show(View anchor, boolean center) {
        contentView.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED);

        int contentWidth = contentView.getMeasuredWidth();
        int contentHeight = contentView.getMeasuredHeight();

        DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        int screenWidth = metrics.widthPixels;
        int screenHeight = metrics.heightPixels;

        int xPos = 0;
        int yPos = 0;

        boolean onTop = true;
        boolean onLeft = true;

        int gravity = Gravity.NO_GRAVITY;

        if (center) {
            xPos = 0;
            yPos = 0;

            gravity = Gravity.CENTER;
        } else {
            int[] location = new int[2];

            anchor.getLocationOnScreen(location);

            Rect anchorRect = new Rect(location[0], location[1], location[0] + anchor.getWidth(), location[1] + anchor.getHeight());

            if ((anchorRect.left + contentWidth) > screenWidth) {
                if (anchorRect.right - contentWidth < 0) {
                    xPos = screenWidth - contentWidth - 4;
                } else {
                    xPos = anchorRect.right - contentWidth;
                }

                onLeft = false;
            } else {
                xPos = anchorRect.left;
                onLeft = true;
            }

            if (anchorRect.top < screenHeight / 2) {
                yPos = anchorRect.bottom + 4;
                onTop = true;
            } else {
                yPos = anchorRect.top - contentHeight - 4;
                onTop = false;
            }

            if (onTop) {
                if (onLeft) {
                    setAnimationStyle(R.style.Popups_Down_Left);
                } else {
                    setAnimationStyle(R.style.Popups_Down_Right);
                }
            } else {
                if (onLeft) {
                    setAnimationStyle(R.style.Popups_Up_Left);
                } else {
                    setAnimationStyle(R.style.Popups_Up_Right);
                }
            }
        }

        setOnDismissListener(this);

        onShow();

        showAtLocation(anchor, gravity, xPos, yPos);
    }

    public void hide() {
        dismiss();
    }

    private void init() {
        setTouchInterceptor(this);

        setWidth(WindowManager.LayoutParams.WRAP_CONTENT);
        setHeight(WindowManager.LayoutParams.WRAP_CONTENT);
        setTouchable(true);
        setFocusable(true);
        setOutsideTouchable(true);

        setBackgroundDrawable(context.getResources().getDrawable(R.drawable.background_popup));
    }

    public void setContentView(View view) {
        super.setContentView(view);

        contentView = view;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_OUTSIDE) {
            dismiss();

            return true;
        }

        return false;
    }

    public void onShow() {
    }

    @Override
    public void onDismiss() {

    }
}
