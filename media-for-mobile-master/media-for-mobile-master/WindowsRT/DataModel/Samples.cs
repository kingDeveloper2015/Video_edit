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

using Samples.Common;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Resources;
using Windows.ApplicationModel.Resources.Core;

namespace Samples.DataModel {

    public class SampleItem {
        public SampleItem(Type classType, String uniqueId, String title, String imagePath) {
            this.ClassType = classType;
            this.UniqueId = uniqueId;
            this.Title = title;
            this.ImagePath = imagePath;
        }

        public Type ClassType { get; private set; }
        public string UniqueId { get; private set; }
        public string Title { get; private set; }
        public string ImagePath { get; private set; }

        public override string ToString() {
            return this.Title;
        }
    }

    public class SampleGroup {
        public SampleGroup(String uniqueId, String title) {
            this.UniqueId = uniqueId;
            this.Title = title;
            this.Items = new ObservableCollection<SampleItem>();
        }

        public string UniqueId { get; private set; }
        public string Title { get; private set; }

        public ObservableCollection<SampleItem> Items { get; private set; }

        public override string ToString() {
            return this.Title;
        }
    }

    class SampleDataSource 
    {        
        private ObservableCollection<SampleGroup> groups = new ObservableCollection<SampleGroup>();

        public ObservableCollection<SampleGroup> Groups {
            get { return this.groups; }
        }

        public SampleDataSource() {
            SampleGroup groupVideoProcessing = new SampleGroup("Group_Transcode", StringLoader.Get("Video_Processing"));

            SampleItem itemTranscode = new SampleItem(typeof(Samples.TranscodePage), "Item_Transcode", StringLoader.Get("Transcode"), "Assets/Icons/Transcode.png");
            groupVideoProcessing.Items.Add(itemTranscode);

            SampleItem itemCut = new SampleItem(typeof(Samples.CutPage), "Item_Cut", StringLoader.Get("Cut"), "Assets/Icons/Cut.png");
            groupVideoProcessing.Items.Add(itemCut);

            SampleItem itemJoin = new SampleItem(typeof(Samples.JoinPage), "Item_Join", StringLoader.Get("Join"), "Assets/Icons/Join.png");
            groupVideoProcessing.Items.Add(itemJoin);

            SampleItem itemVideoEffect = new SampleItem(typeof(Samples.VideoEffectPage), "Item_Video_Effect", StringLoader.Get("Video_Effect"), "Assets/Icons/Effect.png");
            groupVideoProcessing.Items.Add(itemVideoEffect);

            groups.Add(groupVideoProcessing);

            SampleGroup groupCapturing = new SampleGroup("Group_Capturing", StringLoader.Get("Capturing"));

            SampleItem itemCapturing = new SampleItem(typeof(Samples.CameraPage), "Item_Camera", StringLoader.Get("Camera_Capturing"), "Assets/Icons/Camera.png");
            groupCapturing.Items.Add(itemCapturing);

            groups.Add(groupCapturing);
        }
    }
}
