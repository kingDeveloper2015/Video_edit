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

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace Samples.Controls {
    public class ValueChangedEventArgs : EventArgs {
        public ValueChangedEventArgs(double value) {
            m_value = value;
        }

        private double m_value;

        public double Value {
            get { return m_value; }
        }
    }

    public sealed partial class RangeSelector : UserControl {

        int HandleSize;

        public RangeSelector() {
            this.InitializeComponent();

            this.SizeChanged += OnSizeChanged;
        }

        public event EventHandler<ValueChangedEventArgs> MinValueChanged;
        public event EventHandler<ValueChangedEventArgs> MaxValueChanged;

        public double Min {
            get { return PositionToValue(LeftHandleTransform.TranslateX); }
        }

        public double Max {
            get { return PositionToValue(RightHandleTransform.TranslateX); }
        }

        public void Reset() {
            BackgroundHolder.Width = this.Width;

            LeftHandleTransform.TranslateX = 0;
            RightHandleTransform.TranslateX = (this.Width - HandleSize);

            UpdateInner();
        }

        private void RangeSelector_Loaded(object sender, RoutedEventArgs e) {
            HandleSize = (int)Height;

            LeftHandleTransform.TranslateX = 0;
            RightHandleTransform.TranslateX = (Width - HandleSize);
        }

        private void Handle_ManipulationDelta(object sender, ManipulationDeltaRoutedEventArgs e) {
            var transform = (sender as Grid).RenderTransform as CompositeTransform;
            var translate = (LeftHandle.RenderTransform as CompositeTransform).TranslateX;

            double translateX = transform.TranslateX + e.Delta.Translation.X;

            if (transform == LeftHandleTransform) {
                if (translateX < 0 || ((translateX + HandleSize) > RightHandleTransform.TranslateX)) {
                    return;
                }

                transform.TranslateX = translateX;

                if (MinValueChanged != null) {
                    MinValueChanged(this, new ValueChangedEventArgs(Min));
                }
            }
            else if (transform == RightHandleTransform) {
                if ((translateX + HandleSize) > Width || (translateX < LeftHandleTransform.TranslateX + HandleSize)) {
                    return;
                }

                transform.TranslateX = translateX;

                if (MaxValueChanged != null) {
                    MaxValueChanged(this, new ValueChangedEventArgs(Max));
                }
            }

            UpdateInner();
        }

        private void OnClickInside(object sender, TappedRoutedEventArgs e) {
            Point pos = e.GetPosition(RootGrid);

            double distFromLeft = Math.Abs(LeftHandleTransform.TranslateX - pos.X);
            double distFromRight = Math.Abs(RightHandleTransform.TranslateX - pos.X);

            if (distFromLeft < distFromRight) {
                LeftHandleTransform.TranslateX = pos.X;

                if (MinValueChanged != null) {
                    MinValueChanged(this, new ValueChangedEventArgs(Min));
                }
            }
            else {
                RightHandleTransform.TranslateX = pos.X;

                if (MaxValueChanged != null) {
                    MaxValueChanged(this, new ValueChangedEventArgs(Max));
                }
            }

            UpdateInner();            
        }

        public void OnSizeChanged(object sender, SizeChangedEventArgs e) {
            Reset();
        }

        private void UpdateInner() {
            InnerTransform.TranslateX = LeftHandleTransform.TranslateX;
            Inner.Width = (RightHandleTransform.TranslateX - LeftHandleTransform.TranslateX);
        }

        private double PositionToValue(double position) {
            return ((position * 100) / Width);
        }
    }
}
