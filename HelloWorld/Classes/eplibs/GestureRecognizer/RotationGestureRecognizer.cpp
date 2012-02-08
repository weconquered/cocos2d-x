//
//  RotationGestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "RotationGestureRecognizer.h"

using namespace cocos2d;

#define ROTATION_DETECTION_ANGLE   2.0

#define ABS(f) ((f)<0)?-(f):(f)

RotationGestureRecognizer *RotationGestureRecognizer::recognizer()
{
    RotationGestureRecognizer *recognizer = new RotationGestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

eGestureRecognizerType RotationGestureRecognizer::getRecognizerType()
{
    return kRotationGestureRecognizer;
}

void RotationGestureRecognizer::reset()
{
    PinchGestureRecognizer::reset();
    m_fPrevAngle = 0;
    m_fStartAngle = 0;
    m_fGestureRotation = 0;
}

void RotationGestureRecognizer::setNode(CCNode *node)
{
    PinchGestureRecognizer::setNode(node);
    m_fGestureRotation = node->getRotation();
}

float RotationGestureRecognizer::getGestureRotation()
{
    return m_fGestureRotation;
}

void RotationGestureRecognizer::setControlTouches(CCSet *pTouches)
{
    PinchGestureRecognizer::setControlTouches(pTouches);
    
    if (this->controlIsReady())
    {   
        m_fPrevAngle = this->calculateAngle();
        m_fStartAngle = m_fPrevAngle;
    }
}

bool RotationGestureRecognizer::updateTouches(CCSet *pTouches)
{
    PinchGestureRecognizer::updateTouches(pTouches);
    
    float currAngle = this->calculateAngle();

    if (ABS(currAngle - m_fPrevAngle) > ROTATION_DETECTION_ANGLE)
    {
        m_fGestureRotation = m_fPrevAngle - currAngle;
        m_fPrevAngle = currAngle;
        return true;
    }
    
    return false;
}

float RotationGestureRecognizer::calculateAngle()
{
    CCPoint b,c;
    
    b = m_touchPoint1;
    c = m_touchPoint2;
        
    float bc = this->calculateDistance(b, c);
    float cos = (b.x - c.x) / bc;
    float radius = acos(cos);
    float degree = 180 / (M_PI / radius);
        
    float y = b.y - c.y;
    float x = b.x - c.x;

    if (y < 0)
    {
        degree = 360 - degree;
    }
    else if(y == 0 && x < 0)
    {
        degree = 180;
    }
    
    return degree;
}

