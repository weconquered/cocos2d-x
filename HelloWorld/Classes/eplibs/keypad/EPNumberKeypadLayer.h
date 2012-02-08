//
//  EPNumberKeypad.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EP_NUMBER_KEYPAD_H__
#define __EP_NUMBER_KEYPAD_H__

#include "EPGestureRecognizerLayer.h"

extern const char* kKeypadWillShowNotification;
extern const char* kKeypadDidShowNotification;
extern const char* kKeypadWillHideNotification;
extern const char* kKeypadDidHideNotification;
extern const char* kKeypadPressNotification;
extern const char* kKeypadShowNotification;
extern const char* kKeypadHideNotification;

enum
{
    kNumberKeypad0 = 0,
    kNumberKeypad1,
    kNumberKeypad2,
    kNumberKeypad3,
    kNumberKeypad4,
    kNumberKeypad5,
    kNumberKeypad6,
    kNumberKeypad7,
    kNumberKeypad8,
    kNumberKeypad9,
    kNumberKeypadDone,
    kNumberKeypadBackspace,
    kNumberKeypadMax
};

class EPNumberKeypadLayer : public EPGestureRecognizerLayer
{
public:
    EPNumberKeypadLayer();
    
    ~EPNumberKeypadLayer();
    
    void onExit();
    
    void onEnterTransitionDidFinish();
    
    bool init();

    bool gestureRecognizer(GestureRecognizer *gestureRecognizer);
    
    void performTap(TapGestureRecognizer *recognizer ,CCNode *node);
    
    static EPNumberKeypadLayer *keypad();
    
private:
    void loadResource();
    void unloadResource();    
    void initKeypadLayout();
    void showDone();
    void hideDone();
    void show(cocos2d::CCObject *object);
    void hide(cocos2d::CCObject *object);
    void pressAction(cocos2d::CCNode *sender);
    void pressButton(cocos2d::CCNode *node, cocos2d::SEL_CallFuncN selector);
    
    cocos2d::CCNode *m_keypad;
    cocos2d::CCTexture2D *m_tex;
    cocos2d::CCArray *m_items;
    
    bool m_keypadEnabled;
};

#endif//__EP_NUMBER_KEYPAD_H__