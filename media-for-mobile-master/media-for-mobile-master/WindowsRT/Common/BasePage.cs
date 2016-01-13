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
using Samples.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Navigation;

namespace Samples.Common {
    public enum PageState { None, Ready, InProgress };

    public abstract partial class BasePage : Page, IProgressListener {

        NavigationHelper navigationHelper;
        ObservableDictionary defaultViewModel = new ObservableDictionary();

        readonly CoreDispatcher dispatcher = Window.Current.Dispatcher;

        protected PageState pageState = PageState.None;
        protected bool navigatedOut = false;

        protected MediaComposer mediaComposer;
        protected String destinationFileName;

        protected const string videoMimeType = "video/avc";
        protected const string audioMimeType = "audio/aac";

        public NavigationHelper NavigationHelper {
            get { return this.navigationHelper; }
        }

        public ObservableDictionary DefaultViewModel {
            get { return this.defaultViewModel; }
        }

        protected abstract void UpdateUiState(PageState newState);

        protected abstract void OnTranscodeProgress(double progress);
        protected abstract void OnTranscodeStop();

        public BasePage() {
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += OnLoadState;
        }

        protected async void DoTranscode() {
            try {
                await mediaComposer.start();
            }
            catch (Exception exception) {
                ShowMessageDialog(exception.Message);
                UpdateUiState(PageState.None);
            }
        }

        protected void ShowVideoInPopup(StorageFile file) {
            PlayerPopup player = new PlayerPopup();

            player.Show(file);
        }

        protected void ShowMessageDialog(String message) {
            var msgDialog = new Samples.Controls.MessageDialog();

            msgDialog.Show("Intel Media For Mobile", message);
        }

        #region IProgressListener
        public async void onMediaStart() {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => {
                UpdateUiState(PageState.InProgress);
            });
        }

        public async void onMediaProgress(double progress) {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => {
                OnTranscodeProgress(progress);
            });
        }

        public void onMediaDone() {
        }

        public void onMediaPause() {
        }

        public async void onMediaStop() {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => {
                UpdateUiState(PageState.Ready);
                OnTranscodeStop();
            });
        }

        public async void onError(Exception exception) {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => {
                UpdateUiState(PageState.Ready);
                ShowMessageDialog(exception.Message);
            });
        }
        #endregion

        #region NavigationHelper registration

        protected virtual void OnLoadState(object sender, LoadStateEventArgs e) {
        }

        protected override void OnNavigatedTo(NavigationEventArgs e) {
            navigationHelper.OnNavigatedTo(e);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e) {
            navigatedOut = true;

            if (mediaComposer != null) {
                mediaComposer.stop();
            }

            navigationHelper.OnNavigatedFrom(e);
        }

        #endregion
    }
}
