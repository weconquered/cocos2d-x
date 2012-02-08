//
//  GestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "GestureRecognizer.h"

using namespace cocos2d;

GestureRecognizer::GestureRecognizer()
:m_eRecognizerState(kGestureRecognizerStatePossible)
,m_pNode(NULL)
,m_nTouchView(-1)
{
}

GestureRecognizer::~GestureRecognizer()
{
    CC_SAFE_RELEASE(m_pNode);
}

void GestureRecognizer::init()
{
    m_eRecognizerState = kGestureRecognizerStatePossible;
}

void GestureRecognizer::reset()
{
    m_nTouchView = -1;
    m_eRecognizerState = kGestureRecognizerStatePossible;
}

GestureRecognizer *GestureRecognizer::recognizer()
{
    GestureRecognizer *recognizer = new GestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

GestureRecognizer* GestureRecognizer::nodeForTouch(CCSet *pTouches)
{
    CCSetIterator it;
    
    for( it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        if (*it)
        {
            CCTouch *touch = (CCTouch *)*it;
            CCPoint touchLocation = touch->locationInView(touch->view());
            touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

            if (!m_pNode)
                break;
            
            if (m_pNode->getIsVisible() && m_pNode->getTouchEnabled())
            {
                CCPoint local = m_pNode->convertToNodeSpace(touchLocation);
                CCRect r = m_pNode->nodeRect();
                r.origin = CCPointZero;
                
                if (CCRect::CCRectContainsPoint(r, local))
                {
                    this->setTouchView(touch->view());
                    return this;
                }
            }
        }
    }
    
    return NULL;
}

bool GestureRecognizer::containsTouch(cocos2d::CCSet *pTouches)
{
    CCSetIterator it;
    
    for( it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        if (*it)
        {
            CCTouch *touch = (CCTouch *)*it;
            if (m_nTouchView == touch->view())
                return true;            
        }
    }
    
    return false;
}

eGestureRecognizerType GestureRecognizer::getRecognizerType()
{
    return kGestureRecognizer;
}

eGestureRecognizerState GestureRecognizer::getGestureRecognizerState()
{
    return m_eRecognizerState;
}

void GestureRecognizer::setGestureRecognizerState(eGestureRecognizerState state)
{
    m_eRecognizerState = state;
}

CCNode *GestureRecognizer::getNode()
{
    return m_pNode;
}

void GestureRecognizer::setNode(CCNode *node)
{
    m_pNode = node;
    m_pNode->retain();
}

int GestureRecognizer::getTouchView()
{
    return m_nTouchView;
}

void GestureRecognizer::setTouchView(int touchView)
{
    m_nTouchView = touchView;
}
