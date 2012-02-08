//
//  PanGestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "PanGestureRecognizer.h"

using namespace cocos2d;

#define PAN_DETECTION_DISTANCE  8

PanGestureRecognizer::PanGestureRecognizer()
{
}

PanGestureRecognizer *PanGestureRecognizer::recognizer()
{
    PanGestureRecognizer *recognizer = new PanGestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

eGestureRecognizerType PanGestureRecognizer::getRecognizerType()
{
    return kPanGestureRecognizer;
}

void PanGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    
    m_translationView = CCPointMake(-1, -1);
    m_prevPoint = CCPointZero;
}

bool PanGestureRecognizer::updateTranslateView(cocos2d::CCSet *pTouches)
{
    CCSetIterator it;
    
    for( it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        if (*it)
        {
            CCTouch *touch = (CCTouch *)*it;
            
            if (touch->view() == m_nTouchView)
            {
                CCPoint touchLocation = touch->locationInView(touch->view());
                touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

                if (m_translationView.x == -1 && m_translationView.y == -1)
                {
                    m_translationView = touchLocation;
                    m_prevPoint = touchLocation;
                    return true;
                }
                else
                {
                    float x = touchLocation.x - m_prevPoint.x;
                    float y = touchLocation.y - m_prevPoint.y;
                    float length = (float)sqrt(x * x + y * y);

                    if (length > PAN_DETECTION_DISTANCE)
                    {
                        m_prevPoint = m_translationView;
                        m_translationView = touchLocation;
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

CCPoint PanGestureRecognizer::getTranslationView()
{
    return m_translationView;
}