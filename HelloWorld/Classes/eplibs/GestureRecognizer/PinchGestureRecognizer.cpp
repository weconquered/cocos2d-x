//
//  PinchGestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "PinchGestureRecognizer.h"

using namespace cocos2d;

#define PINCH_DETECTION_SCALE   0.01

#define ABS(f) ((f)<0)?-(f):(f)

PinchGestureRecognizer::PinchGestureRecognizer()
:m_nTouchView1(-1)
,m_nTouchView2(-1)
,m_fDistance(0)
,m_fPinchScale(1.0)
{
}

PinchGestureRecognizer *PinchGestureRecognizer::recognizer()
{
    PinchGestureRecognizer *recognizer = new PinchGestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

eGestureRecognizerType PinchGestureRecognizer::getRecognizerType()
{
    return kPinchGestureRecognizer;
}

void PinchGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    m_nTouchView1 = -1;
    m_nTouchView2 = -1;
    m_fDistance = 0;
    
    m_touchPoint1 = CCPointZero;
    m_touchPoint2 = CCPointZero;
}

void PinchGestureRecognizer::setNode(CCNode *node)
{
    GestureRecognizer::setNode(node);
    m_fPinchScale = node->getScale();
}

void PinchGestureRecognizer::setControlTouches(cocos2d::CCSet *pTouches)
{
    CCSetIterator it;
    
    for( it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        if (*it)
        {
            CCTouch *touch = (CCTouch *)*it;
            CCPoint touchLocation = touch->locationInView(touch->view());
            touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

            if (m_nTouchView1 == -1)
            {
                m_nTouchView1 = touch->view();
                m_touchPoint1 = touchLocation;
            }
            else if (m_nTouchView2 == -1)
            {
                m_nTouchView2 = touch->view();
                m_touchPoint2 = touchLocation;
            }
        }
    }
    
    if (this->controlIsReady())
    {
        m_fDistance = this->calculateDistance(m_touchPoint1,m_touchPoint2);    
    }
}

bool PinchGestureRecognizer::updateTouches(cocos2d::CCSet *pTouches)
{
    CCSetIterator it;
    
    for( it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        if (*it)
        {
            CCTouch *touch = (CCTouch *)*it;
            CCPoint touchLocation = touch->locationInView(touch->view());
            touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

            if (m_nTouchView1 == touch->view())
            {
                m_touchPoint1 = touchLocation;
            }
            else if (m_nTouchView2 == touch->view())
            {
                m_touchPoint2 = touchLocation;
            }
        }
    }

    float distance = this->calculateDistance(m_touchPoint1,m_touchPoint2);    
    float s = distance / m_fDistance;
    float diff = m_fPinchScale - s;
    if (ABS(diff) >= PINCH_DETECTION_SCALE)
    {
        m_fDistance = distance;
        m_fPinchScale *= s;
        return true;
    }

    return false;
}

bool PinchGestureRecognizer::controlIsReady()
{
    if (m_nTouchView1 >= 0 && m_nTouchView2 >= 0)
        return true;
    return false;
}

float PinchGestureRecognizer::getPinchScale()
{
    return m_fPinchScale;
}

float PinchGestureRecognizer::calculateDistance(CCPoint a,CCPoint b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    return (float)sqrt(x * x + y * y);
}