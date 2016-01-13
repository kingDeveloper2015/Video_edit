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

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

public class DemoListAdapter extends ArrayAdapter<DemoListItem> {

    private List<DemoListItem> mList;
    private Activity mContext;

    public DemoListAdapter(Activity context, List<DemoListItem> list) {
        super(context, R.layout.sample_list_item, list);

        mContext = context;
        mList = list;
    }

    static class ViewHolder {
        protected TextView mTitle;
    }

    public View getView(int position, View convertView, ViewGroup parent) {

        DemoListItem item = mList.get(position);

        View view = null;

        if (convertView == null) {
            LayoutInflater inflater = mContext.getLayoutInflater();

            view = inflater.inflate(R.layout.sample_list_item, null);

            ViewHolder viewHolder = new ViewHolder();

            viewHolder.mTitle = (TextView) view.findViewById(R.id.itemTitle);

            view.setTag(viewHolder);
        } else {
            view = convertView;
        }

        ViewHolder viewHolder = (ViewHolder) view.getTag();

        viewHolder.mTitle.setText(item.getTitle());

        return view;
    }

    public void updateDisplay() {
        this.notifyDataSetChanged();
    }

    @Override
    public int getCount() {
        return mList.size();
    }
}
