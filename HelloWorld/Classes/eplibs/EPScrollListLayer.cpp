//
//  EPScrollListLayer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPScrollListLayer.h"

using namespace cocos2d;

enum 
{
	kCCScrollLayerStateIdle,
	kCCScrollLayerStateSliding,
    kCCScrollLayerStateDebouncing
};

EPScrollListLayer::EPScrollListLayer()
{
}

EPScrollListLayer::~EPScrollListLayer()
{
}

EPScrollListLayer *EPScrollListLayer::nodeWithLayers(CCArray *layers, CCSize size, CCPoint offset)
{
    EPScrollListLayer *layer = new EPScrollListLayer;
    layer->initWithLayers(layers, size, offset);
    layer->autorelease();
    return layer;
}

bool EPScrollListLayer::initWithLayers(CCArray *layers, CCSize size, CCPoint offset)
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->setAnchorPoint(ccp(0.5,0.5));
    this->setIsRelativeAnchorPoint(true);
    this->setContentSize(size);
    this->setPosition(ccpAdd(ccp(winSize.width/2, winSize.height/2),offset));
    
    m_tListRect = CCRectMake(winSize.width/2 - size.width/2, 
                             winSize.height/2 - size.height/2, 
                             size.width, size.height);
    
    m_tListRect.origin = ccpAdd(m_tListRect.origin, offset); 
    
    this->setIsTouchEnabled(true);
    
    m_nState = kCCScrollLayerStateIdle;
    
    this->initListNode(layers);
    this->initIndicator();
	return true;
}


void EPScrollListLayer::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void EPScrollListLayer::onEnterTransitionDidFinish()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
    CCLayer::onEnterTransitionDidFinish();
}

void EPScrollListLayer::initListNode(CCArray *layers)
{
    int h = 0;
    CCObject *obj = NULL;
    
    CCARRAY_FOREACH(layers, obj)
    {
        CCLayer *l = (CCLayer*)obj;
        h += l->getContentSize().height;
    }
    
    m_tMaxListContentSize = CCSizeMake(this->getContentSize().width, h);
    
    m_pList = CCNode::node();
    m_pList->setAnchorPoint(ccp(0.5,1.0));
    m_pList->setContentSize(m_tMaxListContentSize);
    m_pList->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height));
    this->addChild(m_pList);
    

    CCARRAY_FOREACH(layers, obj)
    {
        CCLayer *l = (CCLayer*)obj;
        l->setAnchorPoint(ccp(0.5,0.5));
        l->setPosition(ccp((m_pList->getContentSize().width - l->getContentSize().width)/2,
                           (h - l->getContentSize().height)));
        m_pList->addChild(l);
        h -= l->getContentSize().height;
    }
}

void EPScrollListLayer::initIndicator()
{
    float factor = this->getContentSize().height / m_pList->getContentSize().height;

    m_pIndicator = CCLayerColor::layerWithColor(ccc4(150, 150, 150, 200));
    m_pIndicator->setContentSize(CCSizeMake(cc_scale_ipad(5), this->getContentSize().height * factor));
    m_pIndicator->setPosition(ccp(this->getContentSize().width - m_pIndicator->getContentSize().width * 2,
                                  this->getContentSize().height - m_pIndicator->getContentSize().height));
    m_pIndicator->setIsVisible(false);
    this->addChild(m_pIndicator);
}

bool EPScrollListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->locationInView(pTouch->view());
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

    if (m_nState == kCCScrollLayerStateIdle && CCRect::CCRectContainsPoint(m_tListRect, touchPoint))
    {
        m_fStartSwipe = touchPoint.y;
        m_nState = kCCScrollLayerStateSliding;
        if (m_pList->getContentSize().height > this->getContentSize().height)
            m_pIndicator->setIsVisible(true);
    }
	return true;
}

void EPScrollListLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nState == kCCScrollLayerStateIdle)
        return;
    
    CCPoint touchPoint = pTouch->locationInView(pTouch->view());
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    float distance = m_fStartSwipe - touchPoint.y;
    m_fStartSwipe = touchPoint.y;
    this->moveListTo(ccp(0,distance));
}

void EPScrollListLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nState == kCCScrollLayerStateSliding)
    {
        m_nState = kCCScrollLayerStateIdle;
        m_pIndicator->setIsVisible(false);
    }
}

void EPScrollListLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nState == kCCScrollLayerStateSliding)
    {
        m_nState = kCCScrollLayerStateIdle;
        m_pIndicator->setIsVisible(false);
    }
}

void EPScrollListLayer::listDebounceDone()
{
    m_nState = kCCScrollLayerStateIdle;
    m_pIndicator->setIsVisible(false);
}

void EPScrollListLayer::moveListTo(CCPoint point)
{
    if (m_nState != kCCScrollLayerStateSliding)
        return;
    
    m_pList->setPosition(ccpSub(m_pList->getPosition(), point));
    
    CCPoint listPosition = m_pList->getPosition();
    CCSize listContentSize = m_pList->getContentSize();
    CCSize thisContentSize = this->getContentSize();
    
    if (listPosition.y < thisContentSize.height)
    {
        bool debounce = false;
        if (thisContentSize.height - listPosition.y > 10)
            debounce = true;
            
        m_pList->setPosition(ccp(listPosition.x,thisContentSize.height));
        if (debounce)
        {
            m_nState = kCCScrollLayerStateDebouncing;
            CCFiniteTimeAction *seq = CCSequence::actions
            (
             CCMoveBy::actionWithDuration(0.1, ccp(0,cc_scale_ipad(-20))),
             CCMoveBy::actionWithDuration(0.2, ccp(0,cc_scale_ipad(20))),
             CCCallFunc::actionWithTarget(this, callfunc_selector(EPScrollListLayer::listDebounceDone)),
             NULL
             );            
            m_pList->runAction(seq);
        }
    }
    else if (listPosition.y > listContentSize.height)
    {
        bool debounce = false;
        if (listPosition.y - thisContentSize.height > 10)
            debounce = true;
            
            if (listContentSize.height > thisContentSize.height)
                m_pList->setPosition(ccp(listPosition.x,listContentSize.height));
            else
                m_pList->setPosition(ccp(listPosition.x,thisContentSize.height));
                    
            if (debounce || listContentSize.height < thisContentSize.height)
            {
                m_nState = kCCScrollLayerStateDebouncing;
                CCFiniteTimeAction *seq = CCSequence::actions
                (
                 CCMoveBy::actionWithDuration(0.1, ccp(0,cc_scale_ipad(20))),
                 CCMoveBy::actionWithDuration(0.2, ccp(0,cc_scale_ipad(-20))),
                 CCCallFunc::actionWithTarget(this, callfunc_selector(EPScrollListLayer::listDebounceDone)),
                 NULL
                 );            
                m_pList->runAction(seq);                
            }
    }
    
    if (listContentSize.height > thisContentSize.height)
    {
        float factor = thisContentSize.height / listContentSize.height;
        float offset = factor * (listPosition.y - thisContentSize.height);
        m_pIndicator->setPosition(ccp(m_pIndicator->getPosition().x,
                                      (thisContentSize.height - m_pIndicator->getContentSize().height) - offset));
    }
}

void EPScrollListLayer::visit()
{
    if (!m_bIsVisible)
        return;
    
    CCPoint worldPt = m_tListRect.origin;//CCDirector::sharedDirector()->convertToUI(m_tListRect.origin);
    CCSize size;
    
    if (CC_CONTENT_SCALE_FACTOR() != 1)
    {
        size.height = m_tListRect.size.height * CC_CONTENT_SCALE_FACTOR();
        size.width = m_tListRect.size.width * CC_CONTENT_SCALE_FACTOR();
        worldPt.y *= CC_CONTENT_SCALE_FACTOR();
    }
    else
    {
        size.width = m_tListRect.size.width;
        size.height = m_tListRect.size.height;
    }
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(worldPt.x,worldPt.y,size.width,size.height);
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
}
