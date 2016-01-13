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

package com.intel.inde.mp.effects;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.SystemClock;
import com.intel.inde.mp.domain.graphics.IEglUtil;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class TextOverlayEffect extends OverlayEffect {

    private Paint paint;

    private float nextUpdate = 0.0f;
    private float cpuUsage = 0.0f;
    ExecutorService executor = Executors.newSingleThreadExecutor();
    int defaultTextSize = 40;
    float defaultWidth = 1280;
    float defaultHeight = 720;


    private long msPerFrame = 1;
    private long l;
    static final int window = 10;
    ArrayList<Long> lst = new ArrayList<Long>();


    public synchronized double getFps() {
        long sum = 0;

        for (Long aLong : lst) {
            sum += aLong;
        }
        return 1e9*lst.size() / sum;
    }

    public TextOverlayEffect(int angle, IEglUtil eglUtil) {
        super(angle, eglUtil);

        paint = new Paint();
        paint.setColor(Color.YELLOW);
        paint.setAlpha(230);
        paint.setTextSize(defaultTextSize);                 // defaultTextSize = 40
        paint.setStyle(Paint.Style.FILL);
        paint.setAntiAlias(true);
    }

    @Override
    protected void drawCanvas(Canvas canvas) {
        update();

        float width = canvas.getWidth();
        float height = canvas.getHeight();


        // try to choose smallest
        float scalePosition = (width/defaultWidth < height/defaultHeight) ? width/defaultWidth : height/defaultHeight;
        float scaleText = (width/defaultWidth + height/defaultHeight) / 2;
        float textSize = defaultTextSize * scaleText;

        paint.setTextSize(textSize);


        canvas.drawText(String.format("FPS: %.0f", getFps()), width - 350 * scalePosition, 60 * scalePosition, paint);
        canvas.drawText(String.format("CPU: %.0f", cpuUsage * 100) + "%", width - 350 * scalePosition, 110 * scalePosition, paint);
    }

    private void update() {
        long currentTime = System.nanoTime();
        msPerFrame = currentTime - l;
        l = currentTime;
        synchronized (this) {
            lst.add(msPerFrame);
            if (lst.size() > window) {
                lst.remove(0);
                }
            }


        if (SystemClock.currentThreadTimeMillis() > nextUpdate * 1000) {
            executor.execute(new Runnable() {
                public void run() {
                    cpuUsage = readCpuUsage();
                }
            });
        }
    }

    private float readCpuUsage() {
        RandomAccessFile reader = null;
        try {
            reader = new RandomAccessFile("/proc/stat", "r");
            String line = reader.readLine();
            line = line.replace("  ", " ");

            String[] split = line.split(" ");

            long idleTimeInitial = Long.parseLong(split[4]);
            long busyTimeInitial = Long.parseLong(split[1]) + Long.parseLong(split[2]) + Long.parseLong(split[3])
                    + Long.parseLong(split[5]) + Long.parseLong(split[6]) + Long.parseLong(split[7]);

            try {
                Thread.sleep(300);
            } catch (Exception e) {}

            reader.seek(0);
            line = reader.readLine();
            line = line.replace("  ", " ");
            reader.close();

            split = line.split(" ");

            long idleTimeFinal = Long.parseLong(split[4]);
            long busyTimeFinal = Long.parseLong(split[1]) + Long.parseLong(split[2]) + Long.parseLong(split[3])
                    + Long.parseLong(split[5]) + Long.parseLong(split[6]) + Long.parseLong(split[7]);

            return (float)(busyTimeFinal - busyTimeInitial) / ((busyTimeFinal + idleTimeFinal) - (busyTimeInitial + idleTimeInitial));

        } catch (IOException ex) {
            ex.printStackTrace();
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return 0;
    }
}
