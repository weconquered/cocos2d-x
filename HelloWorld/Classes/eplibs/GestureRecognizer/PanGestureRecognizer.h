//
//  PanGestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __PAN_GESTURE_RECOGNIZER_H__
#define __PAN_GESTURE_RECOGNIZER_H__

#include "GestureRecognizer.h"

class /*CC_DLL*/ PanGestureRecognizer : public GestureRecognizer
{
public:
    PanGestureRecognizer();
    
    static PanGestureRecognizer* recognizer();
    
    eGestureRecognizerType getRecognizerType();
    
    void reset();
    
    bool updateTranslateView(cocos2d::CCSet *pTouches);
    
    CC_PROPERTY_READONLY(cocos2d::CCPoint, m_translationView, TranslationView)
    
private:
    cocos2d::CCPoint m_prevPoint;
};

#endif//__PAN_GESTURER_ECOGNIZER_H__