//
//  EPTextField.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPTextField.h"
#include "EPNumberKeypadLayer.h"
#include "EPNotificationCenter.h"

using namespace cocos2d;

#define TEXTFIELD_MAX_CHARS     10

const char* kTextFieldShouldReturn = "TextFieldShouldReturn";
const char* kTextFieldShouldChangeCharacters = "TextFieldShouldChangeCharacters";

EPTextField::EPTextField()
:m_numberKeypad(NULL)
,m_maxChars(TEXTFIELD_MAX_CHARS)
{
}

EPTextField::~EPTextField()
{
    EPNotificationCenter::sharedNotifCenter()->removeObserver(this, kKeypadPressNotification);
    EPNotificationCenter::sharedNotifCenter()->removeObserver(this, kKeypadDidHideNotification);
    CC_SAFE_DELETE(m_text)
}

EPTextField *EPTextField::textfield(eKeypadType type)
{
    EPTextField *textfield = new EPTextField;
    textfield->init(type);
    textfield->autorelease();
    return textfield;
}

bool EPTextField::init(eKeypadType type)
{
    
    EPNotificationCenter::sharedNotifCenter()->addObserver(this, 
                                                           callfuncO_selector(EPTextField::keypadPressed), 
                                                           kKeypadPressNotification,
                                                           NULL);

    EPNotificationCenter::sharedNotifCenter()->addObserver(this, 
                                                           callfuncO_selector(EPTextField::keypadReturn), 
                                                           kKeypadDidHideNotification,
                                                           NULL);

    m_type = type;    
    m_text = new CCString(0);
    
    switch(m_type)
    {
        case kKeypadTypeNumberPad:
            this->initNumberKeypad();
            break;
    }
	return true;
}

void EPTextField::onExit()
{
    CCNode::onExit();
}

void EPTextField::onEnterTransitionDidFinish()
{
    CCNode::onEnterTransitionDidFinish();
}

void EPTextField::startInput()
{
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadShowNotification);
}

void EPTextField::keypadPressed(CCObject *object)
{
    switch(m_type)
    {
        case kKeypadTypeNumberPad:
            this->numberKeypadHandler(object);
            break;
    }
}

void EPTextField::keypadReturn(CCObject *object)
{
    this->post(kTextFieldShouldReturn);
    this->removeFromParentAndCleanup(true);
}

void EPTextField::post(const char* name)
{
    CCString *obj = new CCString(m_text->toStdString().c_str());
    obj->autorelease();
    EPNotificationCenter::sharedNotifCenter()->postNotification(name, obj);
}

int EPTextField::getMaxChars()
{
    return m_maxChars;
}

void EPTextField::setMaxChars(int value)
{
    m_maxChars = value;
}

//
// Number Keypad
//
void EPTextField::initNumberKeypad()
{
    m_numberKeypad = EPNumberKeypadLayer::keypad();
    this->addChild(m_numberKeypad);
}

void EPTextField::numberKeypadHandler(cocos2d::CCObject *object)
{
    CCString *num = (CCString*) object;

    int n1 = num->toInt();
    int n2 = m_text->toInt();
    

    if (n1 >= kNumberKeypad0 && n1 <= kNumberKeypad9)
    {
        if (strlen(m_text->toStdString().c_str()) > m_maxChars)
            return;
        
        int n3 = n2 * 10 + n1;
        m_text->assignInt(n3);
        
        this->post(kTextFieldShouldChangeCharacters);
    }
    else if (n1 == kNumberKeypadBackspace)
    {
        int n3 = n2 / 10;
        m_text->assignInt(n3);
        
        this->post(kTextFieldShouldChangeCharacters);
    }
    else if (n1 == kNumberKeypadDone)
    {
        EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadHideNotification);
    }    
}
                                          
