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

using intel.inde.mp;
using intel.inde.mp.domain;
using Samples.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text.RegularExpressions;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace Samples {
    public partial class JoinPage : BasePage {
        public JoinPage() {
            this.InitializeComponent();

            Progress.Click = OnStop;
        }

        private void OnStop(object sender, EventArgs e) {
            mediaComposer.stop();
        }

        private void OnTranscode(object sender, RoutedEventArgs e) {
            if (Preview.File == null || Preview2.File == null) {
                ShowMessageDialog(StringLoader.Get("Please_Pick_Media_File"));

                return;
            }

            Preview.Stop();
            Preview2.Stop();

            destinationFileName = Preview.File.Name + ".joined.mp4";

            mediaComposer = new MediaComposer(this);

            SetTranscodeParameters();

            mediaComposer.setTargetFile(new File(KnownFolders.VideosLibrary, destinationFileName));

            DoTranscode();
        }

        protected void SetTranscodeParameters() {
            IVideoFormat videoFormat = Preview.VideoFormat;
            IAudioFormat audioFormat = Preview.AudioFormat;

            mediaComposer.addSourceFile(new File(Preview.File));
            mediaComposer.addSourceFile(new File(Preview2.File));

            ComboBoxItem framerateItem = (ComboBoxItem)FramerateSelect.SelectedItem;
            String videoProfile = ((ComboBoxItem)ProfileSelect.SelectedItem).Content.ToString();

            var match = Regex.Match(videoProfile, @"\(([0-9]+)x([0-9]+)\)");

            uint width = uint.Parse(match.Groups[1].Value);
            uint height = uint.Parse(match.Groups[2].Value);
            uint framerate = uint.Parse(framerateItem.Content.ToString());
            uint bitrate = (uint)(BitrateSelect.Value * 1000);

            if (videoFormat != null) {
                var vFormat = new WinRtVideoFormat(videoMimeType, width, height) {
                    bitrate = bitrate
                };

                vFormat.frameRate.Numerator = framerate;
                vFormat.frameRate.Denominator = 1;

                mediaComposer.setTargetVideoFormat(vFormat);
            }

            if (audioFormat != null) {
                var aFormat = new WinRtAudioFormat(audioMimeType) {
                    bitrate = audioFormat.bitrate,
                    bitsPerSample = audioFormat.bitsPerSample,
                    channelCount = audioFormat.channelCount,
                    sampleRate = audioFormat.sampleRate
                };

                mediaComposer.setTargetAudioFormat(aFormat);
            }
        }

        protected override void UpdateUiState(PageState newState) {
            pageState = newState;

            Setting.IsEnabled = (pageState != PageState.InProgress);
            Transcode.IsEnabled = (pageState != PageState.InProgress);

            ProgressParent.Visibility = (pageState != PageState.InProgress) ? Windows.UI.Xaml.Visibility.Collapsed : Windows.UI.Xaml.Visibility.Visible;

            Preview.IsEnabled = (pageState != PageState.InProgress);
            Preview.ShowControls(Preview.IsEnabled);

            Preview2.IsEnabled = (pageState != PageState.InProgress);
            Preview2.ShowControls(Preview2.IsEnabled);
        }

        protected override void OnTranscodeProgress(double progress) {
            Progress.Value = progress;
        }

        protected async override void OnTranscodeStop() {
            if (navigatedOut == false) {
                StorageFile file = await KnownFolders.VideosLibrary.GetFileAsync(destinationFileName);
                ShowVideoInPopup(file);
            }
        }
    }
}
