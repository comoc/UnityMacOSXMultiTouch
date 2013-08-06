//
//  MultiTouch.h
//  MultiTouchPlugin
//
//  Created by Akihiro Komori on 8/7/13.
//  Copyright (c) 2013 Akihiro Komori. All rights reserved.
//

#ifndef MultiTouchPlugin_MultiTouch_h
#define MultiTouchPlugin_MultiTouch_h

#ifdef __cplusplus
extern "C" {
#endif
    void MultiTouch_Initialize();
    void MultiTouch_Finalize();
    double MultiTouch_ReadUpdate();
    int MultiTouch_ReadFingerCount();
    float MultiTouch_ReadFingerPositionX(int index);
    float MultiTouch_ReadFingerPositionY(int index);
    float MultiTouch_ReadFingerVelocityX(int index);
    float MultiTouch_ReadFingerVelocityY(int index);    
    float MultiTouch_ReadSize(int index);
    float MultiTouch_ReadAngle(int index);
    float MultiTouch_ReadMajorAxis(int index);
    float MultiTouch_ReadMinorAxis(int index);
#ifdef __cplusplus
}
#endif

#endif
