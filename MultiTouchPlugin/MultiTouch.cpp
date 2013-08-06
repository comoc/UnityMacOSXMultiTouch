//
//  MultiTouch.cpp
//  MultiTouchPlugin
//
//  Created by Akihiro Komori on 8/7/13.
//  Copyright (c) 2013 Akihiro Komori. All rights reserved.
//

#include "MultiTouch.h"
#include <pthread.h>
#include "MultitouchSupport.h"
#include <float.h>

namespace
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    MTDeviceRef mt = 0;
    const int MAX_FINGERS = 11;
    double timeStampSrc = 0.0;
    double timeStampDest = 0.0;
    int fingerCountSrc = 0;
    int fingerCountDest = 0;
    Finger fingersSrc[MAX_FINGERS];
    Finger fingersDest[MAX_FINGERS];
    const float UNDEFINED_VALUE = FLT_MIN;
}

#ifdef __cplusplus
extern "C" {
#endif

    int ContactCallbackFunction(int device, Finger *data, int nFingers, double timestamp, int frame)
    {
        pthread_mutex_lock(&mutex);
        timeStampSrc = timestamp;
        for (int i = 0; i < nFingers; i++)
        {
            fingersSrc[i] = data[i];
            fingerCountSrc = i + 1;
            if (i >= MAX_FINGERS)
                break;
        }
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    
    void MultiTouch_Initialize()
    {
        if (mt != 0)
            return;
        
        mt = MTDeviceCreateDefault();
        if (mt != 0)
        {
            MTRegisterContactFrameCallback(mt, ContactCallbackFunction);
            MTDeviceStart(mt);
        }
   
    }
    void MultiTouch_Finalize()
    {
        if (mt != 0)
        {
            MTDeviceStop(mt);
            MTDeviceRelease(mt);
            mt = 0;
        }
    }
    double MultiTouch_ReadUpdate()
    {
        pthread_mutex_lock(&mutex);
        timeStampDest = timeStampSrc;
        fingerCountDest = fingerCountSrc;
        for (int i = 0; i < fingerCountSrc; i++)
        {
            fingersDest[i] = fingersSrc[i];
        }
        pthread_mutex_unlock(&mutex);
        
        return timeStampDest;
    }
    
    int MultiTouch_ReadFingerCount()
    {
        return fingerCountDest;
    }
    
    float MultiTouch_ReadFingerPositionX(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].normalized.pos.x;
    }
    
    float MultiTouch_ReadFingerPositionY(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].normalized.pos.y;
    }

    float MultiTouch_ReadFingerVelocityX(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].normalized.vel.x;
    }
    
    float MultiTouch_ReadFingerVelocityY(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].normalized.vel.y;
    }
    
    float MultiTouch_ReadSize(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].size;
    }
    
    float MultiTouch_ReadAngle(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].angle;
    }
    
    float MultiTouch_ReadMajorAxis(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].majorAxis;
    }
    
    float MultiTouch_ReadMinorAxis(int index)
    {
        if (index < 0 || index >= fingerCountDest)
            return UNDEFINED_VALUE;
        return fingersDest[index].minorAxis;
    }
#ifdef __cplusplus
}
#endif
