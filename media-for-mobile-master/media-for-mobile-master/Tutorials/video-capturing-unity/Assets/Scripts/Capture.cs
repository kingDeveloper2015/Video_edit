// Copyright (c) 2015, Intel Corporation
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

using UnityEngine;
using System.Collections;
using System.IO;
using System;

[RequireComponent(typeof(Camera))]
public class Capture : MonoBehaviour
{
	public int videoWidth = 720;
	public int videoHeight = 1094;
	public int videoFrameRate = 15;
	public int videoBitRate = 3000;
	
	private string videoDir;
	public string fileName = "game_capturing-";

	private IntPtr capturingObject = IntPtr.Zero;
	private float startTime = 0.0f;
	private float nextCaptureTime = 0.0f;
	public bool isRunning { get; private set; }
	
	private AndroidJavaObject playerActivityContext = null;
	
	private static IntPtr constructorMethodID = IntPtr.Zero;
	private static IntPtr initCapturingMethodID = IntPtr.Zero;
	private static IntPtr startCapturingMethodID = IntPtr.Zero;
	private static IntPtr captureFrameMethodID = IntPtr.Zero;
	private static IntPtr stopCapturingMethodID = IntPtr.Zero;
	
	private static IntPtr getDirectoryDCIMMethodID = IntPtr.Zero;
	
	void Start()
	{
		if (!Application.isEditor) {
			// First, obtain the current activity context
			using (AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
				playerActivityContext = jc.GetStatic<AndroidJavaObject>("currentActivity");
			}
			
			// Search for our class
			IntPtr classID = AndroidJNI.FindClass("com/intel/inde/mp/samples/unity/Capturing"); // com.intel.inde.mp.samples.unity // com/intel/penelope/Capturing
			
			// Search for it's contructor
			constructorMethodID = AndroidJNI.GetMethodID(classID, "<init>", "(Landroid/content/Context;II)V");
			if (constructorMethodID == IntPtr.Zero) {
				Debug.LogError("Can't find Capturing constructor.");
				return;
			}
			
			// Register our methods
			initCapturingMethodID = AndroidJNI.GetMethodID(classID, "initCapturing", "(IIII)V");
			if (initCapturingMethodID == IntPtr.Zero) {
				Debug.LogError("Can't find initCapturing() method.");
				return;
			}
			startCapturingMethodID = AndroidJNI.GetMethodID(classID, "startCapturing", "(Ljava/lang/String;)V");
			if (startCapturingMethodID == IntPtr.Zero) {
				Debug.LogError("Can't find startCapturing() method.");
				return;
			}
			captureFrameMethodID = AndroidJNI.GetMethodID(classID, "captureFrame", "(I)V");
			if (captureFrameMethodID == IntPtr.Zero) {
				Debug.LogError("Can't find captureFrame() method.");
				return;
			}
			stopCapturingMethodID = AndroidJNI.GetMethodID(classID, "stopCapturing", "()V");
			if (stopCapturingMethodID == IntPtr.Zero) {
				Debug.LogError("Can't find stopCapturingMethodID() method.");
				return;
			}
			
			// Register and call our static method
			getDirectoryDCIMMethodID = AndroidJNI.GetStaticMethodID(classID, "getDirectoryDCIM", "()Ljava/lang/String;");
			jvalue[] args = new jvalue[0];
			videoDir = AndroidJNI.CallStaticStringMethod(classID, getDirectoryDCIMMethodID, args);
			
			// Create Capturing object
			jvalue[] constructorParameters = AndroidJNIHelper.CreateJNIArgArray(new object [] {
				playerActivityContext, Screen.width, Screen.height
			});
			IntPtr local_capturingObject = AndroidJNI.NewObject(classID, constructorMethodID, constructorParameters);
			if (local_capturingObject == IntPtr.Zero) {
				Debug.LogError("--- Can't create Capturing object.");
				return;
			}
			// Keep a global reference to it
			capturingObject = AndroidJNI.NewGlobalRef(local_capturingObject);
			AndroidJNI.DeleteLocalRef(local_capturingObject);
			
			AndroidJNI.DeleteLocalRef(classID);
		}
		isRunning = false;
		Debug.Log("--- videoFrameRate = " + videoFrameRate);
		Debug.Log("--- 1.0f / videoFrameRate = " + 1.0f / videoFrameRate);
	}
	
	void OnRenderImage(RenderTexture src, RenderTexture dest)
	{
		Graphics.Blit(src, dest);
		if (isRunning) {
			float elapsedTime = Time.time - startTime;
			if (elapsedTime >= nextCaptureTime) {
				CaptureFrame(src.GetNativeTexturePtr().ToInt32());
				nextCaptureTime += 1.0f / videoFrameRate;
			}
		}
	}
	
	public void StartCapturing()
	{
		if (capturingObject == IntPtr.Zero)
			return;
		
		jvalue[] videoParameters =  new jvalue[4];
		videoParameters[0].i = videoWidth;
		videoParameters[1].i = videoHeight;
		videoParameters[2].i = videoFrameRate;
		videoParameters[3].i = videoBitRate;
		AndroidJNI.CallVoidMethod(capturingObject, initCapturingMethodID, videoParameters);
		DateTime date = DateTime.Now;
		string fullFileName = fileName + date.ToString("ddMMyy-hhmmss.fff") + ".mp4";
		jvalue[] args = new jvalue[1];
		args[0].l = AndroidJNI.NewStringUTF(videoDir + fullFileName);
		AndroidJNI.CallVoidMethod(capturingObject, startCapturingMethodID, args);

		startTime = Time.time;
		nextCaptureTime = 0.0f;
		isRunning = true;
	}
	
	private void CaptureFrame(int textureID)
	{
		if (capturingObject == IntPtr.Zero)
			return;
		
		jvalue[] args = new jvalue[1];
		args[0].i = textureID;
		AndroidJNI.CallVoidMethod(capturingObject, captureFrameMethodID, args);
	}
	
	public void StopCapturing()
	{
		isRunning = false;
		
		if (capturingObject == IntPtr.Zero)
			return;
		
		jvalue[] args = new jvalue[0];
		AndroidJNI.CallVoidMethod(capturingObject, stopCapturingMethodID, args);
	}
}
