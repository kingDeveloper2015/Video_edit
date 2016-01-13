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
using Samples.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Devices.Enumeration;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media.Capture;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace Samples {
    public sealed partial class CameraPage : BasePage {
        private CameraCapture cameraCapture = new CameraCapture(null);
        private IPreview preview = null;

        private const string captureOutput = "Capture.mp4";

        public CameraPage() {
            this.InitializeComponent();

            UpdateUiState(PageState.None);

            FillDevicesList();

            Window.Current.VisibilityChanged += OnWindowVisibilityChanged;

            VideoSelect.SelectionChanged += OnDeviceChanaged;
            AudioSelect.SelectionChanged += OnDeviceChanaged;

            VideoPreview.AutoHide = true;
            VideoPreview.AutoPlay = false;

            LoadVideoPreview();
        }

        protected override void UpdateUiState(PageState newState) {
            pageState = newState;

            StartRecording.Visibility = (pageState != PageState.InProgress) ? Visibility.Visible : Visibility.Collapsed;
            StopRecording.Visibility = (pageState == PageState.InProgress) ? Visibility.Visible : Visibility.Collapsed;

            StartRecording.IsEnabled = (pageState == PageState.Ready);
            Setting.IsEnabled = (pageState != PageState.InProgress);
            VideoEffect.IsEnabled = (pageState == PageState.Ready);

            VideoPreview.Visibility = (pageState != PageState.InProgress) ? Visibility.Visible : Visibility.Collapsed;
        }

        private async void OnStartRecording(object sender, RoutedEventArgs e) {
            cameraCapture.setTargetFile(new File(KnownFolders.VideosLibrary, captureOutput));

            var winRtVideoFormat = new WinRtVideoFormat("video/avc", 640, 480) { bitrate = 1000000 };

            winRtVideoFormat.frameRate.Numerator = 30;
            winRtVideoFormat.frameRate.Denominator = 1;

            cameraCapture.setTargetVideoFormat(winRtVideoFormat);

            cameraCapture.setTargetAudioFormat(new WinRtAudioFormat("audio/aac") {
                sampleRate = 44100,
                bitrate = 128000,
                bitsPerSample = 16,
                channelCount = 2
            });

            await cameraCapture.start();

            UpdateUiState(PageState.InProgress);
        }

        private async void OnStopRecording(object sender, RoutedEventArgs e) {
            if (cameraCapture != null) {
                await cameraCapture.stop();
            }

            UpdateUiState(PageState.Ready);

            LoadVideoPreview();
        }

        private async void OnWindowVisibilityChanged(object sender, VisibilityChangedEventArgs e) {
            if (e.Visible)
            {
                StartPreview();
            }
            else 
            {
                await cameraCapture.stop();

                StopPreview();
            }
        }

        protected async override void OnNavigatedFrom(NavigationEventArgs e) {
            base.OnNavigatedFrom(e);

            await cameraCapture.stop();

            StopPreview();
        }

        private void StartPreview() {
            int videoDevice = VideoSelect.SelectedIndex;
            int audioDevice = AudioSelect.SelectedIndex;

            if (videoDevice == -1 || audioDevice == -1) { 
                return; 
            }

            ResetEffects();

            UpdateUiState(PageState.None);

            string videoDeviceId = ((ComboBoxItem)VideoSelect.SelectedItem).Name.ToString();
            string audioDeviceId = ((ComboBoxItem)AudioSelect.SelectedItem).Name.ToString();

            MediaCaptureInitializationSettings settings = new MediaCaptureInitializationSettings {                    
                    VideoDeviceId = videoDeviceId,
                    AudioDeviceId = audioDeviceId,
                    StreamingCaptureMode = StreamingCaptureMode.AudioAndVideo,
                };

            IAsyncOperation<IPreview> previewTask;

            previewTask = cameraCapture.createPreview(PreviewWindow, settings);
                       
            if (preview != null) {
                preview.start().AsTask().Wait();
            }

            previewTask.Completed = delegate {
                try {
                    preview = previewTask.GetResults();
                }
                catch (UnauthorizedAccessException) {
                    ShowMessageDialog(StringLoader.Get("Camera_Access_Denied"));
                    return;
                }

                preview.start().AsTask().Wait();

                UpdateUiState(PageState.Ready);
            };
        }

        private async void StopPreview() {
            if (preview != null) {
                await preview.stop();
            }

            preview = null;
        }

        private async void FillDevicesList() {
            DeviceInformationCollection videoDevices = await DeviceInformation.FindAllAsync(DeviceClass.VideoCapture);

            foreach (var device in videoDevices) 
            {
                ComboBoxItem item = new ComboBoxItem();
                item.Content = device.Name;
                item.Name = device.Id;
                VideoSelect.Items.Add(item);
            }

            DeviceInformationCollection audioDevices = await DeviceInformation.FindAllAsync(DeviceClass.AudioCapture);

            foreach (var device in audioDevices) {
                ComboBoxItem item = new ComboBoxItem();
                item.Content = device.Name;
                item.Name = device.Id;
                AudioSelect.Items.Add(item);
            }

            if (VideoSelect.Items.Count > 0) {
                VideoSelect.SelectedIndex = 0;
            }

            if (AudioSelect.Items.Count > 0) {
                AudioSelect.SelectedIndex = 0;
            }
        }

        private void OnDeviceChanaged(object sender, SelectionChangedEventArgs e) {            
            StopPreview();
            StartPreview();
        }

        private async void LoadVideoPreview() {
            StorageFile file;
            IRandomAccessStream stream;

            try {
                file = await KnownFolders.VideosLibrary.GetFileAsync(captureOutput);
            }
            catch (FileNotFoundException) {
                return;
            }

            stream = await file.OpenAsync(FileAccessMode.Read);

            double width = this.ActualWidth / 4;
            double height = this.ActualHeight / 4;

            double padding = 12;

            VideoPreview.Width = width;
            VideoPreview.Height = height;

            Canvas.SetLeft(VideoPreview, this.ActualWidth - width - padding);
            Canvas.SetTop(VideoPreview, padding);

            VideoPreview.Stop();
            VideoPreview.Source = null;
            VideoPreview.SetSource(stream, file.ContentType);
        }

        protected override void OnLoadState(object sender, LoadStateEventArgs e) {
            AppendEffectsMenu("Grayscale");
            AppendEffectsMenu("Sepia");
            AppendEffectsMenu("Poster");
            AppendEffectsMenu("Black_and_White");
            AppendEffectsMenu("Cartoon");
            AppendEffectsMenu("Horizontal_Blur");
            AppendEffectsMenu("Invert");
        }

        private async void OnEffectClicked(object sender, RoutedEventArgs e) {
            ToggleMenuFlyoutItem checkedItem = (ToggleMenuFlyoutItem)sender;

            for (int i = 0; i < EffectsMenu.Items.Count; ++i) {
                ToggleMenuFlyoutItem item = (ToggleMenuFlyoutItem)EffectsMenu.Items[i];

                if (checkedItem != item) {
                    item.IsChecked = false;
                }
            }

            if (checkedItem.IsChecked) {
                await preview.setActiveEffect(SamplesUtils.GetVideoEffectByName(checkedItem.Text));
            }
            else {
                await preview.setActiveEffect(null);
            }
        }

        private void AppendEffectsMenu(String name) {
            ToggleMenuFlyoutItem menuItem = new ToggleMenuFlyoutItem();
            menuItem.Text = StringLoader.Get(name);

            menuItem.Click += OnEffectClicked;

            EffectsMenu.Items.Add(menuItem);
        }

        private void ResetEffects() {
            for (int i = 0; i < EffectsMenu.Items.Count; ++i) {
                ToggleMenuFlyoutItem item = (ToggleMenuFlyoutItem)EffectsMenu.Items[i];

                item.IsChecked = false;
            }
        }

        protected override void OnTranscodeProgress(double progress) {
        }

        protected override void OnTranscodeStop() {
        }
    }
}
