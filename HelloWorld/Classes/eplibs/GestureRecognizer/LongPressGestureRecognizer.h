//
//  LongPressGestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __LONGPRESS_GESTURE_RECOGNIZER_H__
#define __LONGPRESS_GESTURE_RECOGNIZER_H__

#include "GestureRecognizer.h"

class /*CC_DLL*/ LongPressGestureRecognizer : public GestureRecognizer
{
public:
    LongPressGestureRecognizer();
    static LongPressGestureRecognizer* recognizer();
    
    eGestureRecognizerType getRecognizerType();

    CC_PROPERTY(float, m_fInterval, Interval)
    
    CC_PROPERTY_READONLY(int, m_nPressedCount, PressedCount)
    
    bool countdown(cocos2d::ccTime dt);
private:
    float m_fCount;
};

#endif//__LONGPRESS_GESTURE_RECOGNIZER_H__