//
//  RotationGestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __ROTATION_GESTURE_RECOGNIZER_H__
#define __ROTATION_GESTURE_RECOGNIZER_H__

#include "PinchGestureRecognizer.h"

class /*CC_DLL*/ RotationGestureRecognizer : public PinchGestureRecognizer
{
public:
    static RotationGestureRecognizer* recognizer();
    
    void reset();
    
    void setNode(cocos2d::CCNode *node);
    
    void setControlTouches(cocos2d::CCSet *pTouches);
    
    bool updateTouches(cocos2d::CCSet *pTouches);
    
    eGestureRecognizerType getRecognizerType();
    
    CC_PROPERTY_READONLY(float, m_fGestureRotation, GestureRotation)
private:
    float calculateAngle();
    
    float m_fPrevAngle;
    
    float m_fStartAngle;
};

#endif//__ROTATION_GESTURE_RECOGNIZER_H__