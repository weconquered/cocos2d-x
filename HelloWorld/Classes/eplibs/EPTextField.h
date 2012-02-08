//
//  EPTextField.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __EPTEXTFIELD_H__
#define __EPTEXTFIELD_H__

#include "cocos2d.h"

class EPNumberKeypadLayer;

extern const char* kTextFieldShouldReturn;
extern const char* kTextFieldShouldChangeCharacters;

typedef enum
{
    kKeypadTypeNumberPad = 0,
}eKeypadType;

class EPTextField : public cocos2d::CCNode
{
public:
    EPTextField();
    
    ~EPTextField();
    
    void onExit();
    
    void onEnterTransitionDidFinish();
    
    bool init(eKeypadType type);
        
    static EPTextField *textfield(eKeypadType type);

    void startInput();
    
    CC_PROPERTY(int, m_maxChars, MaxChars)
        
private:
    void keypadPressed(cocos2d::CCObject *object);
    void keypadReturn(cocos2d::CCObject *object);
    void post(const char* name);
    
    eKeypadType m_type;
    cocos2d::CCString *m_text;
    //
    // number keypad
    //
private:
    EPNumberKeypadLayer *m_numberKeypad;
    void initNumberKeypad();
    void numberKeypadHandler(cocos2d::CCObject *object);
};

#endif//__EPTEXTFIELD_H__
