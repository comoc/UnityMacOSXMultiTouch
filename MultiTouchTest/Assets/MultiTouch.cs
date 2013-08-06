using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class MultiTouch : MonoBehaviour {
	
	[DllImport ("MultiTouchPlugin")]
    private static extern void MultiTouch_Initialize();
	[DllImport ("MultiTouchPlugin")]
    private static extern void MultiTouch_Finalize();
	[DllImport ("MultiTouchPlugin")]
    private static extern double MultiTouch_ReadUpdate();
	[DllImport ("MultiTouchPlugin")]
    private static extern int MultiTouch_ReadFingerCount();
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadFingerPositionX(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadFingerPositionY(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadFingerVelocityX(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadFingerVelocityY(int index);    
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadSize(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadAngle(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadMajorAxis(int index);
	[DllImport ("MultiTouchPlugin")]
    private static extern float MultiTouch_ReadMinorAxis(int index);
	
	private static double timeStampOld = 0.0;
	
	// Use this for initialization
	void Start () {
		MultiTouch_Initialize();
	}
	
	// Update is called once per frame
	void Update () {
		double d = MultiTouch_ReadUpdate();
		if (d != timeStampOld) {
			int count = MultiTouch_ReadFingerCount();
			timeStampOld = d;
			print("timeStamp:" + d + " fingerCount:" + count);
			for (int i = 0; i < count; i++) {
				print("i:" + i + " x:" + MultiTouch_ReadFingerPositionX(i) + " y:" + MultiTouch_ReadFingerPositionY(i));	
			}
		}
	}
	
	~MultiTouch() {
		MultiTouch_Finalize();
	}
}
