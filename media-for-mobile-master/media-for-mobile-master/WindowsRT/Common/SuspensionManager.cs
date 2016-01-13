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
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace Samples.Common
{
    internal sealed class SuspensionManager
    {
        private static Dictionary<string, object> sessionState = new Dictionary<string, object>();
        private static List<Type> knownTypes = new List<Type>();
        private const string sessionStateFilename = "_sessionState.xml";

        private static DependencyProperty FrameSessionStateKeyProperty = DependencyProperty.RegisterAttached("_FrameSessionStateKey", typeof(String), typeof(SuspensionManager), null);
        private static DependencyProperty FrameSessionStateProperty = DependencyProperty.RegisterAttached("_FrameSessionState", typeof(Dictionary<String, Object>), typeof(SuspensionManager), null);
        private static List<WeakReference<Frame>> registeredFrames = new List<WeakReference<Frame>>();

        public static Dictionary<string, object> SessionState
        {
            get { return sessionState; }
        }

        public static List<Type> KnownTypes
        {
            get { return knownTypes; }
        }

        public static async Task SaveAsync()
        {
            foreach (var weakFrameReference in registeredFrames)
            {
                Frame frame;
        
                if (weakFrameReference.TryGetTarget(out frame))
                {
                    SaveFrameNavigationState(frame);
                }
            }

            MemoryStream sessionData = new MemoryStream();
            DataContractSerializer serializer = new DataContractSerializer(typeof(Dictionary<string, object>), knownTypes);
            serializer.WriteObject(sessionData, sessionState);

            StorageFile file = await ApplicationData.Current.LocalFolder.CreateFileAsync(sessionStateFilename, CreationCollisionOption.ReplaceExisting);
            
            using (Stream fileStream = await file.OpenStreamForWriteAsync())
            {
                sessionData.Seek(0, SeekOrigin.Begin);
                await sessionData.CopyToAsync(fileStream);
                await fileStream.FlushAsync();
            }
        }

        public static async Task RestoreAsync()
        {
            sessionState = new Dictionary<String, Object>();

            StorageFile file = await ApplicationData.Current.LocalFolder.GetFileAsync(sessionStateFilename);
            
            using (IInputStream inStream = await file.OpenSequentialReadAsync())
            {
                DataContractSerializer serializer = new DataContractSerializer(typeof(Dictionary<string, object>), knownTypes);
                sessionState = (Dictionary<string, object>)serializer.ReadObject(inStream.AsStreamForRead());
            }

            foreach (var weakFrameReference in registeredFrames)
            {
                Frame frame;
            
                if (weakFrameReference.TryGetTarget(out frame))
                {
                    frame.ClearValue(FrameSessionStateProperty);
                    RestoreFrameNavigationState(frame);
                }
            }
        }

        public static void RegisterFrame(Frame frame, String sessionStateKey)
        {
            if (frame.GetValue(FrameSessionStateKeyProperty) != null)
            {
                throw new InvalidOperationException("Frames can only be registered to one session state key");
            }

            if (frame.GetValue(FrameSessionStateProperty) != null)
            {
                throw new InvalidOperationException("Frames must be either be registered before accessing frame session state, or not registered at all");
            }

            frame.SetValue(FrameSessionStateKeyProperty, sessionStateKey);
            registeredFrames.Add(new WeakReference<Frame>(frame));
                        
            RestoreFrameNavigationState(frame);
        }

        public static void UnregisterFrame(Frame frame)
        {
            SessionState.Remove((String)frame.GetValue(FrameSessionStateKeyProperty));
            
            registeredFrames.RemoveAll((weakFrameReference) =>
            {
                Frame testFrame;
                return !weakFrameReference.TryGetTarget(out testFrame) || testFrame == frame;
            });
        }

        public static Dictionary<String, Object> SessionStateForFrame(Frame frame)
        {
            var frameState = (Dictionary<String, Object>)frame.GetValue(FrameSessionStateProperty);

            if (frameState == null)
            {
                var frameSessionKey = (String)frame.GetValue(FrameSessionStateKeyProperty);

                if (frameSessionKey != null)
                {
                    if (!sessionState.ContainsKey(frameSessionKey))
                    {
                        sessionState[frameSessionKey] = new Dictionary<String, Object>();
                    }

                    frameState = (Dictionary<String, Object>)sessionState[frameSessionKey];
                }
                else
                {
                    frameState = new Dictionary<String, Object>();
                }

                frame.SetValue(FrameSessionStateProperty, frameState);
            }

            return frameState;
        }

        private static void RestoreFrameNavigationState(Frame frame)
        {
            var frameState = SessionStateForFrame(frame);

            if (frameState.ContainsKey("Navigation"))
            {
                frame.SetNavigationState((String)frameState["Navigation"]);
            }
        }

        private static void SaveFrameNavigationState(Frame frame)
        {
            var frameState = SessionStateForFrame(frame);

            frameState["Navigation"] = frame.GetNavigationState();
        }
    }
}
