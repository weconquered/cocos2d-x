//
//  LongPressGestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "LongPressGestureRecognizer.h"

using namespace cocos2d;

LongPressGestureRecognizer::LongPressGestureRecognizer()
:m_fInterval(1.0)
,m_fCount(0)
,m_nPressedCount(0)
{
}

LongPressGestureRecognizer *LongPressGestureRecognizer::recognizer()
{
    LongPressGestureRecognizer *recognizer = new LongPressGestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

eGestureRecognizerType LongPressGestureRecognizer::getRecognizerType()
{
    return kLongPressGestureRecognizer;
}

float LongPressGestureRecognizer::getInterval()
{
    return m_fInterval;
}

void LongPressGestureRecognizer::setInterval(float interval)
{
    m_fInterval = interval;
    m_fCount = m_fInterval;
    m_nPressedCount = 0;
}

bool LongPressGestureRecognizer::countdown(ccTime dt)
{
    m_fCount -= dt;
    if (m_fCount < 0)
    {
        m_fCount = m_fInterval;
        m_nPressedCount++;
        return true;
    }
    return false;
}

int LongPressGestureRecognizer::getPressedCount()
{
    return m_nPressedCount;
}
