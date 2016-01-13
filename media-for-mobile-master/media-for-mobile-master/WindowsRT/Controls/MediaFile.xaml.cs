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
using intel.inde.mp.Effects.Video;
using Samples.Common;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace Samples.Controls {
    public sealed partial class MediaFile : UserControl {
        
        StorageFile sourceFile;
        MediaFileInfo mediaFileInfo;

        IVideoFormat videoFormat;
        IAudioFormat audioFormat;

        public MediaFile() {
            this.InitializeComponent();
            
            Preview.AutoPlay = false;

            SegmentSelector.MinValueChanged += OnSegmentValueChanged;
            SegmentSelector.MaxValueChanged += OnSegmentValueChanged;
        }

        public void Seek(TimeSpan position) {
            Preview.SeekAsync(position);
        }

        public void Play() {
            Preview.Play();
        }

        public void Stop() {
            Preview.Stop();            
        }

        public void ShowControls(bool show) {
            Preview.IsPlayPauseVisible = show;
            Preview.IsTimelineVisible = show;

            Preview.AutoHide = !show;
        }

        public StorageFile File {
            get { return sourceFile; }
        }

        /*
        public MediaFileInfo FileInfo {
            get { return mediaFileInfo; }
        }
        */

        public IVideoFormat VideoFormat {
            get { return videoFormat; }
        }

        public IAudioFormat AudioFormat {
            get { return audioFormat; }
        }

        public double Duration {
            get { return mediaFileInfo.getDurationInMicroSec(); }
        }

        public bool EnableSegmentPicker {
            set { SegmentSelector.Visibility = (value) ? Visibility.Visible : Visibility.Collapsed; }
        }

        public double SegmentStart {
            get { return SegmentSelector.Min; }
        }

        public double SegmentEnd {
            get { return SegmentSelector.Max; }
        }

        public void RemoveAllEffects() {
            Preview.RemoveAllEffects();
        }

        public void AddVideoEffect(IVideoEffect effect) {
            Preview.RemoveAllEffects();

            if (effect != null) {
                Preview.AddVideoEffect(effect.getClassId(), false, effect.getProperties());
            }

            if (sourceFile != null) {
                LoadMediaFile(sourceFile);
            }
        }

        private async void OnPickVideo(object sender, RoutedEventArgs e) {
            StorageFile file = await PickFile();

            if (file == null) {
                return;
            }

            LoadMediaFile(file);
        }

        private async Task<StorageFile> PickFile() {

            var picker = new FileOpenPicker {
                SuggestedStartLocation = PickerLocationId.VideosLibrary
            };

            picker.FileTypeFilter.Add(".wmv");
            picker.FileTypeFilter.Add(".mp4");

            return await picker.PickSingleFileAsync();
        }

        public void LoadMediaFile(StorageFile file) {
            IRandomAccessStream stream = file.OpenReadAsync().AsTask().Result;

            if (stream == null) {
                ShowMessageDialog(StringLoader.Get("Invalid_Media_File"));

                return;
            }

            sourceFile = file;

            mediaFileInfo = new MediaFileInfo(sourceFile);
            
            if (UpdateMediaInfo(mediaFileInfo) == false) {
                ShowMessageDialog(StringLoader.Get("Invalid_Media_File"));

                mediaFileInfo = null;
                sourceFile = null;
                stream.Dispose();
                stream = null;

                return;
            }

            Preview.SetSource(stream, sourceFile.ContentType);

            Stop();

            double width = this.ActualWidth;
            double height = this.ActualHeight;

            Preview.Width = width;
            Preview.Height = height;

            SegmentSelector.Width = (width);

            SegmentSelector.Reset();

            Canvas.SetTop(SegmentSelector, height - 100);
        }

        private void OnSegmentValueChanged(object sender, ValueChangedEventArgs args) {
            double value = args.Value;

            long newPos = (long)(value * Preview.Duration.Ticks / 100);

            Seek(TimeSpan.FromTicks(newPos));
        }

        private bool UpdateMediaInfo(MediaFileInfo mediaFileInfo) {
            if (mediaFileInfo == null) {
                return false;
            }

            string videoMimeType = "unknown";
            string audioMimeType = "unknown";

            videoFormat = mediaFileInfo.getVideoFormat();
            audioFormat = mediaFileInfo.getAudioFormat();

            try
            {
                videoMimeType = videoFormat.mimeType.asString();
            }
            catch (Exception) 
            {
                videoFormat = null;
            }

            try 
            {
                audioMimeType = audioFormat.mimeType.asString();
            }
            catch (Exception) 
            {
                audioFormat = null;
            }

            if (videoFormat != null) {
                float frameRate = videoFormat.frameRate.Numerator;

                if (videoFormat.frameRate.Denominator != 0) {
                    frameRate /= videoFormat.frameRate.Denominator;
                }

                VideoCodecText.Text = videoMimeType;
                ResolutionText.Text = String.Format("{0} x {1}", videoFormat.resolution.Width, videoFormat.resolution.Height);
                FramerateText.Text = String.Format("{0}", frameRate);
                BitrateVideoText.Text = String.Format("{0}", videoFormat.bitrate);
            }
            else {
                ResolutionText.Text = "";
                FramerateText.Text = "";
                BitrateVideoText.Text = "";
            }

            if (audioFormat != null) {
                AudioCodecText.Text = audioMimeType;
                ChannelsText.Text = String.Format("{0}", audioFormat.channelCount);
                SamplerateText.Text = String.Format("{0}", audioFormat.sampleRate);
                BitspersampleText.Text = String.Format("{0}", audioFormat.bitsPerSample);
                BitrateAudioText.Text = String.Format("{0}", audioFormat.bitrate);
            }

            if (videoFormat == null && audioFormat == null) {
                return false;
            }

            Info.Visibility = Visibility.Visible;

            return true;
        }

        private void ShowMessageDialog(String message) {
            var msgDialog = new MessageDialog();

            msgDialog.Show("Intel Media For Mobile", message);
        }
    }
}
