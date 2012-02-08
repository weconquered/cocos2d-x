//
//  GestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __GESTURE_RECOGNIZER_H__
#define __GESTURE_RECOGNIZER_H__

#include "cocos2d.h"

typedef enum
{
    kGestureRecognizer = 0,
    kTapGestureRecognizer,
    kLongPressGestureRecognizer,
    kPanGestureRecognizer,
    kPinchGestureRecognizer,
    kRotationGestureRecognizer,
}eGestureRecognizerType;

typedef enum
{
    kGestureRecognizerStatePossible = 0,
    kGestureRecognizerStateBegan,
    kGestureRecognizerStateChanged,
    kGestureRecognizerStateEnded,
    kGestureRecognizerStateCancelled,
}eGestureRecognizerState;

class /*CC_DLL*/ GestureRecognizer : public cocos2d::CCObject
{
public:
    GestureRecognizer();
    ~GestureRecognizer();
    
    static GestureRecognizer* recognizer();
    
    virtual eGestureRecognizerType getRecognizerType();
    
    GestureRecognizer* nodeForTouch(cocos2d::CCSet *pTouches);
    bool containsTouch(cocos2d::CCSet *pTouches);
    
    void reset();
    
    void init();        
    
    CC_PROPERTY(eGestureRecognizerState, m_eRecognizerState, GestureRecognizerState);
    CC_PROPERTY(cocos2d::CCNode*, m_pNode, Node);
    CC_PROPERTY(int, m_nTouchView, TouchView);
};
#endif//__GESTURE_RECOGNIZER_H__