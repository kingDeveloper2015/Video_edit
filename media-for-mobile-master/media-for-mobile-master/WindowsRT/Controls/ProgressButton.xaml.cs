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
    public sealed partial class ProgressButton : UserControl {

        EventHandler m_Click;
        
        public ProgressButton() {
            this.InitializeComponent();

            Button.Click += OnButtonClick;
        }

        public EventHandler Click {
            set { m_Click = value; }
        }

        public double Value {
            get { return (double)GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        public double ArcAngle {
            get { return (double)GetValue(ArcAngleProperty); }
            set { SetValue(ArcAngleProperty, value); }
        }

        public int StrokeThickness {
            get { return (int)GetValue(StrokeThicknessProperty); }
            set { SetValue(StrokeThicknessProperty, value); }
        }

        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register("Value", typeof(double), typeof(ProgressButton), new PropertyMetadata(0d, new PropertyChangedCallback(OnValueChanged)));

        public static readonly DependencyProperty ArcAngleProperty =
            DependencyProperty.Register("Angle", typeof(double), typeof(ProgressButton), new PropertyMetadata(0d, new PropertyChangedCallback(OnPropertyChanged)));

        public static readonly DependencyProperty StrokeThicknessProperty =
            DependencyProperty.Register("StrokeThickness", typeof(int), typeof(ProgressButton), new PropertyMetadata(5));

        private static void OnValueChanged(DependencyObject sender, DependencyPropertyChangedEventArgs args) {
            ProgressButton progress = sender as ProgressButton;
            progress.ArcAngle = (progress.Value * 360) / 100;
        }

        private static void OnPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs args) {
            ProgressButton progress = sender as ProgressButton;
            progress.Render();
        }

        public void Render() {

            //BackgroundEllipse.Width = Width;
            //BackgroundEllipse.Height = Height;

            double radius = (Width - StrokeThickness) / 2;

            double angleRad = (Math.PI / 180.0) * (ArcAngle - 90);

            Point startPoint = new Point(radius, 0);

            Point endPoint = new Point(radius * Math.Cos(angleRad), radius * Math.Sin(angleRad));

            endPoint.X += radius;
            endPoint.Y += radius;

            pathRoot.Margin = new Thickness(StrokeThickness / 2, StrokeThickness / 2, 0, 0);
            pathRoot.StrokeThickness = StrokeThickness;

            bool largeArc = (ArcAngle > 180.0);

            Size outerArcSize = new Size(radius, radius);

            pathFigure.StartPoint = startPoint;

            if (startPoint.X == Math.Round(endPoint.X) && startPoint.Y == Math.Round(endPoint.Y)) {
                endPoint.X -= 0.01;
            }

            arcSegment.Point = endPoint;
            arcSegment.Size = outerArcSize;
            arcSegment.IsLargeArc = largeArc;
        }

        private void OnButtonClick(object sender, RoutedEventArgs e) {

            var eventHandler = this.m_Click;

            if (eventHandler != null) {
                eventHandler(this, null);
            }
        }
    }
}
