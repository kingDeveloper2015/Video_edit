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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

namespace Samples.Common
{
    [Windows.Foundation.Metadata.WebHostHidden]
    public class NavigationHelper : DependencyObject
    {
        private Page Page { get; set; }
        private Frame Frame { get { return this.Page.Frame; } }

        public NavigationHelper(Page page)
        {
            this.Page = page;

            this.Page.Loaded += (sender, e) =>
            {
                if (this.Page.ActualHeight == Window.Current.Bounds.Height &&
                    this.Page.ActualWidth == Window.Current.Bounds.Width)
                {
                    Window.Current.CoreWindow.Dispatcher.AcceleratorKeyActivated +=
                        CoreDispatcher_AcceleratorKeyActivated;

                    Window.Current.CoreWindow.PointerPressed +=
                        this.CoreWindow_PointerPressed;
                }
            };

            this.Page.Unloaded += (sender, e) =>
            {
                Window.Current.CoreWindow.Dispatcher.AcceleratorKeyActivated -=
                    CoreDispatcher_AcceleratorKeyActivated;
                Window.Current.CoreWindow.PointerPressed -=
                    this.CoreWindow_PointerPressed;
            };
        }

        #region Navigation support

        RelayCommand _goBackCommand;
        RelayCommand _goForwardCommand;

        public RelayCommand GoBackCommand
        {
            get
            {
                if (_goBackCommand == null)
                {
                    _goBackCommand = new RelayCommand(
                        () => this.GoBack(),
                        () => this.CanGoBack());
                }
                return _goBackCommand;
            }
            set
            {
                _goBackCommand = value;
            }
        }

        public RelayCommand GoForwardCommand
        {
            get
            {
                if (_goForwardCommand == null)
                {
                    _goForwardCommand = new RelayCommand(
                        () => this.GoForward(),
                        () => this.CanGoForward());
                }
                return _goForwardCommand;
            }
        }

        public virtual bool CanGoBack()
        {
            return this.Frame != null && this.Frame.CanGoBack;
        }

        public virtual bool CanGoForward()
        {
            return this.Frame != null && this.Frame.CanGoForward;
        }

        public virtual void GoBack()
        {
            if (this.Frame != null && this.Frame.CanGoBack) this.Frame.GoBack();
        }

        public virtual void GoForward()
        {
            if (this.Frame != null && this.Frame.CanGoForward) this.Frame.GoForward();
        }

        private void CoreDispatcher_AcceleratorKeyActivated(CoreDispatcher sender, AcceleratorKeyEventArgs e)
        {
            var virtualKey = e.VirtualKey;

            if ((e.EventType == CoreAcceleratorKeyEventType.SystemKeyDown ||
                e.EventType == CoreAcceleratorKeyEventType.KeyDown) &&
                (virtualKey == VirtualKey.Left || virtualKey == VirtualKey.Right ||
                (int)virtualKey == 166 || (int)virtualKey == 167))
            {
                var coreWindow = Window.Current.CoreWindow;
                var downState = CoreVirtualKeyStates.Down;
                bool menuKey = (coreWindow.GetKeyState(VirtualKey.Menu) & downState) == downState;
                bool controlKey = (coreWindow.GetKeyState(VirtualKey.Control) & downState) == downState;
                bool shiftKey = (coreWindow.GetKeyState(VirtualKey.Shift) & downState) == downState;
                bool noModifiers = !menuKey && !controlKey && !shiftKey;
                bool onlyAlt = menuKey && !controlKey && !shiftKey;

                if (((int)virtualKey == 166 && noModifiers) ||
                    (virtualKey == VirtualKey.Left && onlyAlt))
                {
                    e.Handled = true;

                    this.GoBackCommand.Execute(null);
                }
                else if (((int)virtualKey == 167 && noModifiers) ||
                    (virtualKey == VirtualKey.Right && onlyAlt))
                {
                    e.Handled = true;

                    this.GoForwardCommand.Execute(null);
                }
            }
        }

        private void CoreWindow_PointerPressed(CoreWindow sender, PointerEventArgs e)
        {
            var properties = e.CurrentPoint.Properties;

            if (properties.IsLeftButtonPressed || properties.IsRightButtonPressed || properties.IsMiddleButtonPressed) {
                return;
            }

            bool backPressed = properties.IsXButton1Pressed;
            bool forwardPressed = properties.IsXButton2Pressed;

            if (backPressed ^ forwardPressed)
            {
                e.Handled = true;

                if (backPressed) this.GoBackCommand.Execute(null);
                if (forwardPressed) this.GoForwardCommand.Execute(null);
            }
        }

        #endregion

        #region Process lifetime management

        private String _pageKey;

        public event LoadStateEventHandler LoadState;
        public event SaveStateEventHandler SaveState;

        public void OnNavigatedTo(NavigationEventArgs e)
        {
            var frameState = SuspensionManager.SessionStateForFrame(this.Frame);

            this._pageKey = "Page-" + this.Frame.BackStackDepth;

            if (e.NavigationMode == NavigationMode.New)
            {
                var nextPageKey = this._pageKey;
                int nextPageIndex = this.Frame.BackStackDepth;

                while (frameState.Remove(nextPageKey))
                {
                    nextPageIndex++;
                    nextPageKey = "Page-" + nextPageIndex;
                }

                if (this.LoadState != null)
                {
                    this.LoadState(this, new LoadStateEventArgs(e.Parameter, null));
                }
            }
            else
            {
                if (this.LoadState != null)
                {
                    this.LoadState(this, new LoadStateEventArgs(e.Parameter, (Dictionary<String, Object>)frameState[this._pageKey]));
                }
            }
        }

        public void OnNavigatedFrom(NavigationEventArgs e)
        {
            var frameState = SuspensionManager.SessionStateForFrame(this.Frame);
            var pageState = new Dictionary<String, Object>();

            if (this.SaveState != null)
            {
                this.SaveState(this, new SaveStateEventArgs(pageState));
            }
            frameState[_pageKey] = pageState;
        }

        #endregion
    }

    public delegate void LoadStateEventHandler(object sender, LoadStateEventArgs e);
    public delegate void SaveStateEventHandler(object sender, SaveStateEventArgs e);

    public class LoadStateEventArgs : EventArgs
    {
        public Object NavigationParameter { get; private set; }
        public Dictionary<string, Object> PageState { get; private set; }

        public LoadStateEventArgs(Object navigationParameter, Dictionary<string, Object> pageState)
            : base()
        {
            this.NavigationParameter = navigationParameter;
            this.PageState = pageState;
        }
    }

    public class SaveStateEventArgs : EventArgs
    {
        public Dictionary<string, Object> PageState { get; private set; }

        public SaveStateEventArgs(Dictionary<string, Object> pageState)
            : base()
        {
            this.PageState = pageState;
        }
    }
}
