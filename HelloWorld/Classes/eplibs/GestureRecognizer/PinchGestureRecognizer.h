//
//  PinchGestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __PINCH_GESTURE_RECOGNIZER_H__
#define __PINCH_GESTURE_RECOGNIZER_H__

#include "GestureRecognizer.h"

class /*CC_DLL*/ PinchGestureRecognizer : public GestureRecognizer
{
public:
    PinchGestureRecognizer();
    
    static PinchGestureRecognizer* recognizer();
    
    eGestureRecognizerType getRecognizerType();
    
    void reset();
    
    void setNode(cocos2d::CCNode *node);
    
    void setControlTouches(cocos2d::CCSet *pTouches);
    
    bool controlIsReady();
    
    bool updateTouches(cocos2d::CCSet *pTouches);
    
    CC_PROPERTY_READONLY(float, m_fPinchScale, PinchScale)
    
    int m_nTouchView1;
    int m_nTouchView2;
    
    cocos2d::CCPoint m_touchPoint1;
    cocos2d::CCPoint m_touchPoint2;
    
    float m_fDistance;
    
    float calculateDistance(cocos2d::CCPoint a,cocos2d::CCPoint b);
};

#endif//__PINCH_GESTURER_ECOGNIZER_H__