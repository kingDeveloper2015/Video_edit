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

package org.qtproject.qt5.android.bindings;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.content.Context;
import android.database.Cursor;
import android.provider.MediaStore;

public class FilePicker {

    public static Intent createChooseVideoIntent(int androidSdkVersion) {
        Intent intent;
        if (androidSdkVersion >= 19) {
            intent = new Intent("android.intent.action.OPEN_DOCUMENT");
            // We can't use Intent.ACTION_OPEN_DOCUMENT constant with API level below 18
        } else {
            intent = new Intent(Intent.ACTION_GET_CONTENT);
        }
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("video/mp4");
        return intent;
     }

     public static String getFileName(Context context, Uri uri) {
         String fileName = "";
         Cursor cursor = null;
         try {
             String[] projection = { MediaStore.Video.Media.DISPLAY_NAME };
             cursor = context.getContentResolver().query(uri, projection, null, null, null);
             int nameIndex = cursor.getColumnIndexOrThrow(MediaStore.Video.Media.DISPLAY_NAME);
             cursor.moveToFirst();
             fileName = cursor.getString(nameIndex);
         } finally {
             if (cursor != null) {
                 cursor.close();
             }
         }
         return fileName;
     }
}

