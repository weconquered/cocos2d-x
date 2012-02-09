//
//  EPScrollListLayer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/7.
//  Copyright 2011年 Erawppa Co., Ltd.. All rights reserved.
//

#include "EPScrollListLayer.h"

using namespace cocos2d;

enum 
{
	kCCScrollLayerStateIdle,
	kCCScrollLayerStateSliding,
    kCCScrollLayerStateDebouncing,
    kCCScrollLayerStateClicking,
};

EPScrollListLayer::EPScrollListLayer()
:m_target(NULL)
,m_tapSelector(NULL)
,m_touchBeginSelector(NULL)
,m_touchEndSelector(NULL)
,m_nState(kCCScrollLayerStateIdle)
,m_tapLayer(NULL)
,m_vertical(true)
,m_tapEnable(true)
{
}

EPScrollListLayer::~EPScrollListLayer()
{
}

EPScrollListLayer *EPScrollListLayer::nodeWithLayers(CCArray *layers, 
                                                     CCSize size, 
                                                     CCPoint offset,
                                                     bool vertical)
{
    EPScrollListLayer *layer = new EPScrollListLayer;
    layer->initWithLayers(layers, size, offset,vertical);
    layer->autorelease();
    return layer;
}

bool EPScrollListLayer::initWithLayers(CCArray *layers, 
                                       CCSize size, 
                                       CCPoint offset,
                                       bool vertical)
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->setContentSize(size);
    
    // test start
//    static int aaa = 0;
//    CCLayerColor *c = NULL;
//    if (aaa == 0)
//        c = CCLayerColor::layerWithColor(ccc4(255, 0, 0, 255));
//    else if (aaa == 1)
//        c = CCLayerColor::layerWithColor(ccc4(0, 255, 0, 255));
//    else if (aaa == 2)
//        c = CCLayerColor::layerWithColor(ccc4(0, 0, 255, 255));
//    else if (aaa == 3)
//        c = CCLayerColor::layerWithColor(ccc4(0, 255, 255, 255));
//    else if (aaa == 4)
//        c = CCLayerColor::layerWithColor(ccc4(255, 0, 255, 255));
//    else
//        c = CCLayerColor::layerWithColor(ccc4(255, 255, 0, 255));
//    
//    aaa++;
//    aaa = (aaa > 5) ? 0 : aaa;
//    c->setContentSize(getContentSize());
//    this->addChild(c);
    // test end
    
//    m_tListRect = CCRectMake(winSize.width/2 - size.width/2, 
//                             winSize.height/2 - size.height/2, 
//                             size.width, size.height);
    
    m_tListRect = CCRectMake(0,0, 
                             size.width, size.height);

    
    m_tListRect.origin = ccpAdd(m_tListRect.origin, offset); 
    
    m_vertical = vertical;
    
    this->setIsTouchEnabled(true);
    
    m_nState = kCCScrollLayerStateIdle;
    
    if (vertical)
    {
        this->initVerticalListNode(layers);
        this->initVerticalIndicator();
    }
    else
    {
        this->initHorizontalListNode(layers);
        this->initHorizontalIndicator();
    }
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

void EPScrollListLayer::initHorizontalListNode(cocos2d::CCArray *layers)
{
    int w = 0;
    CCObject* obj = NULL;
    
    if (layers)
    {
        CCARRAY_FOREACH(layers, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            w += l->getContentSize().width;
        }
    }
    
    m_tMaxListContentSize = CCSizeMake(w, this->getContentSize().height);
    
    m_pList = CCNode::node();
    m_pList->setAnchorPoint(ccp(0,0.5));
    m_pList->setContentSize(m_tMaxListContentSize);
    m_pList->setPosition(ccp(0,0));
    this->addChild(m_pList);

    if (layers)
    {
        w = 0;
        CCARRAY_FOREACH(layers, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setAnchorPoint(ccp(0.5,0.5));
            l->setPosition(ccp(w,l->getContentSize().height/2));
            m_pList->addChild(l);
            w += l->getContentSize().width;
        }
    }
}

void EPScrollListLayer::initVerticalListNode(CCArray *layers)
{
    int h = 0;
    
    CCObject *obj = NULL;
    if (layers)
    {
        CCARRAY_FOREACH(layers, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            h += l->getContentSize().height;
        }
    }
    
    m_tMaxListContentSize = CCSizeMake(this->getContentSize().width, h);
    
    m_pList = CCNode::node();
    m_pList->setAnchorPoint(ccp(0.5,1.0));
    m_pList->setContentSize(m_tMaxListContentSize);
    m_pList->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height));
    this->addChild(m_pList);
    
    if (layers)
    {
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
}

void EPScrollListLayer::initHorizontalIndicator()
{
    float factor = this->getContentSize().width / m_pList->getContentSize().width;
    
    m_pIndicator = CCLayerColor::layerWithColor(ccc4(200, 200, 200, 200));
    m_pIndicator->setContentSize(CCSizeMake(this->getContentSize().width * factor,cc_scale_ipad(5)));
    m_pIndicator->setPosition(ccp(0,0));
    m_pIndicator->setIsVisible(false);
    this->addChild(m_pIndicator);
}

void EPScrollListLayer::initVerticalIndicator()
{
    float factor = this->getContentSize().height / m_pList->getContentSize().height;

    m_pIndicator = CCLayerColor::layerWithColor(ccc4(200, 200, 200, 200));
    m_pIndicator->setContentSize(CCSizeMake(cc_scale_ipad(5), this->getContentSize().height * factor));
    m_pIndicator->setPosition(ccp(this->getContentSize().width - m_pIndicator->getContentSize().width,
                                  this->getContentSize().height - m_pIndicator->getContentSize().height));
    m_pIndicator->setIsVisible(false);
    this->addChild(m_pIndicator);
}

CCLayer* EPScrollListLayer::touchForLayer(CCPoint touchPoint)
{
    CCObject* obj = NULL;
    CCArray* children = m_pList->getChildren();
    CCARRAY_FOREACH(children, obj)
    {
        CCLayer* layer = (CCLayer*)obj;
        CCPoint local = layer->convertToNodeSpace(touchPoint);
        CCRect r = layer->nodeRect();
        r.origin = CCPointZero;
        
        if (CCRect::CCRectContainsPoint(r, local))
        {
            if (layer->getTouchEnabled())
                return layer;
            else
            {
                //check children
                CCObject* childObj = NULL;
                CCArray* childrenArray = layer->getChildren();
                CCARRAY_FOREACH(childrenArray, childObj)
                {
                    CCLayer* childLayer = (CCLayer*)childObj;
                    CCPoint local = childLayer->convertToNodeSpace(touchPoint);
                    CCRect r = childLayer->nodeRect();
                    r.origin = CCPointZero;
                    if (CCRect::CCRectContainsPoint(r, local))
                    {
                        return childLayer;
                    }
                }
            }
        }
    }
    return NULL;
}

bool EPScrollListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    for (CCNode* n = this ; n != NULL ; n = n->getParent())
    {
        if (!n->getIsVisible())
            return false;
    }
    
    CCPoint touchPoint = pTouch->locationInView(pTouch->view());
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

    CCPoint local = this->convertToNodeSpace(touchPoint);
    CCRect r = this->nodeRect();
    r.origin = CCPointZero;
    
    if (m_nState == kCCScrollLayerStateIdle && CCRect::CCRectContainsPoint(r, local))
    {        
        m_tapLayer = touchForLayer(touchPoint);
        if (m_vertical)
        {
            m_fStartSwipe = touchPoint.y;
            m_nState = kCCScrollLayerStateSliding;
            if (m_pList->getContentSize().height > this->getContentSize().height)
                m_pIndicator->setIsVisible(true);
        }
        else
        {
            m_fStartSwipe = touchPoint.x;
            m_nState = kCCScrollLayerStateSliding;
            if (m_pList->getContentSize().width > this->getContentSize().width)
                m_pIndicator->setIsVisible(true);
        }
        
        m_fUniqueStartSwipe = m_fStartSwipe;
        
        if (m_target && m_touchBeginSelector && m_tapLayer)
        {
            (m_target->*m_touchBeginSelector)(m_tapLayer);
        }        
    }
    
    
	return true;
}

void EPScrollListLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    for (CCNode* n = this ; n != NULL ; n = n->getParent())
    {
        if (!n->getIsVisible())
            return;
    }

    if (m_nState == kCCScrollLayerStateIdle)
        return;
    
    CCPoint touchPoint = pTouch->locationInView(pTouch->view());
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    float distance = 0;
    
    if (m_vertical)
    {
        distance = m_fStartSwipe - touchPoint.y;
        m_fStartSwipe = touchPoint.y;
        this->moveVerticalListTo(ccp(0,distance));        
    }
    else
    {
        distance = touchPoint.x - m_fStartSwipe;
        m_fStartSwipe = touchPoint.x;
        this->moveHorizontalListTo(ccp(distance,0));        
    }
    
    std::vector<EPScrollListLayer*>::iterator it;
    for (it = m_syncScrolls.begin() ; it != m_syncScrolls.end() ; ++it)
    {
        EPScrollListLayer* scrollLayer = (EPScrollListLayer*)*it;
        scrollLayer->move(distance);
    }
}

void EPScrollListLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    for (CCNode* n = this ; n != NULL ; n = n->getParent())
    {
        if (!n->getIsVisible())
            return;
    }

    if (m_target && m_touchEndSelector && m_tapLayer)
    {
        (m_target->*m_touchEndSelector)(m_tapLayer);
    }        

    CCPoint touchPoint = pTouch->locationInView(pTouch->view());
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

    if (m_nState == kCCScrollLayerStateSliding)
    {
        m_nState = kCCScrollLayerStateIdle;
        m_pIndicator->setIsVisible(false);
        
        float distance = 0;
        
        if (m_vertical)
            distance = m_fUniqueStartSwipe - touchPoint.y;
        else
            distance = m_fUniqueStartSwipe - touchPoint.x;
        
        distance = (distance < 0) ? -distance : distance ;
        
        CCLayer* releaseTapLayer = touchForLayer(touchPoint);
        if (m_tapLayer && m_tapLayer == releaseTapLayer && distance < 10 && m_tapEnable)
        {
            m_nState = kCCScrollLayerStateClicking;
            CCFiniteTimeAction *seq = CCSequence::actions
            (
             CCScaleTo::actionWithDuration(0.1, m_tapLayer->getScale() - 0.1),
             CCScaleTo::actionWithDuration(0.1, m_tapLayer->getScale() + 0.1),
             CCScaleTo::actionWithDuration(0.1, m_tapLayer->getScale()),
             CCCallFuncN::actionWithTarget(this, callfuncN_selector(EPScrollListLayer::pressedLayer)),
             NULL
             );
            m_tapLayer->runAction(seq);
        }
        
        m_fUniqueStartSwipe = 0;
        m_tapLayer = NULL;
    }       
}

void EPScrollListLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{    
    for (CCNode* n = this ; n != NULL ; n = n->getParent())
    {
        if (!n->getIsVisible())
            return;
    }

    if (m_target && m_touchEndSelector && m_tapLayer)
    {
        (m_target->*m_touchEndSelector)(m_tapLayer);
    }        

    if (m_nState == kCCScrollLayerStateSliding)
    {
        m_nState = kCCScrollLayerStateIdle;
        m_pIndicator->setIsVisible(false);
    }
}

void EPScrollListLayer::pressedLayer(CCNode* sender)
{
    m_nState = kCCScrollLayerStateIdle;
    
    if (m_target)
    {
        (m_target->*m_tapSelector)(sender);
    }
}

void EPScrollListLayer::listDebounceDone()
{
    m_nState = kCCScrollLayerStateIdle;
    m_pIndicator->setIsVisible(false);
}

void EPScrollListLayer::move(float distance)
{
    if (m_vertical)
    {
        CCPoint point = ccp(0,distance);
        m_pList->setPosition(ccpSub(m_pList->getPosition(), point));
        
        CCPoint listPosition = m_pList->getPosition();
        CCSize listContentSize = m_pList->getContentSize();
        CCSize thisContentSize = this->getContentSize();
        
        if (listPosition.y < thisContentSize.height)
        {
            m_pList->setPosition(ccp(listPosition.x,thisContentSize.height));
        }
        else if (listPosition.y > listContentSize.height)
        {
            if (listContentSize.height > thisContentSize.height)
                m_pList->setPosition(ccp(listPosition.x,listContentSize.height));
            else
                m_pList->setPosition(ccp(listPosition.x,thisContentSize.height));            
        }
    }
    else
    {
        CCPoint point = ccp(distance,0);
        m_pList->setPosition(ccpAdd(m_pList->getPosition(), point));
        
        CCPoint listPosition = m_pList->getPosition();
        CCSize listContentSize = m_pList->getContentSize();
        CCSize thisContentSize = this->getContentSize();
        
        if (listPosition.x > 0)
        {
            bool debounce = false;
            if (thisContentSize.width - listPosition.x > 10)
                debounce = true;
            
            m_pList->setPosition(ccp(0,listPosition.y));
        }
        else if (listPosition.x < (thisContentSize.width - listContentSize.width))
        {
            bool debounce = false;
            if (listPosition.x - (thisContentSize.width - listContentSize.width) < -10)
                debounce = true;
            
            if (listContentSize.width > thisContentSize.width)
                m_pList->setPosition(ccp((thisContentSize.width - listContentSize.width),listPosition.y));
            else
                m_pList->setPosition(ccp(0,listPosition.y));
        }
    }
}

void EPScrollListLayer::moveHorizontalListTo(cocos2d::CCPoint point)
{
    if (m_nState != kCCScrollLayerStateSliding)
        return;
    
    m_pList->setPosition(ccpAdd(m_pList->getPosition(), point));
    
    CCPoint listPosition = m_pList->getPosition();
    CCSize listContentSize = m_pList->getContentSize();
    CCSize thisContentSize = this->getContentSize();
    
    if (listPosition.x > 0)
    {
        bool debounce = false;
        if (thisContentSize.width - listPosition.x > 10)
            debounce = true;
        
        m_pList->setPosition(ccp(0,listPosition.y));
        if (debounce)
        {
            m_nState = kCCScrollLayerStateDebouncing;
            CCFiniteTimeAction *seq = CCSequence::actions
            (
             CCMoveBy::actionWithDuration(0.1, ccp(cc_scale_ipad(20),0)),
             CCMoveBy::actionWithDuration(0.2, ccp(cc_scale_ipad(-20),0)),
             CCCallFunc::actionWithTarget(this, callfunc_selector(EPScrollListLayer::listDebounceDone)),
             NULL
             );            
            m_pList->runAction(seq);
        }
    }
    else if (listPosition.x < (thisContentSize.width - listContentSize.width))
    {
        bool debounce = false;
        if (listPosition.x - (thisContentSize.width - listContentSize.width) < -10)
            debounce = true;
        
        if (listContentSize.width > thisContentSize.width)
            m_pList->setPosition(ccp((thisContentSize.width - listContentSize.width),listPosition.y));
        else
            m_pList->setPosition(ccp(0,listPosition.y));
        
        if (debounce || listContentSize.width < thisContentSize.width)
        {
            m_nState = kCCScrollLayerStateDebouncing;
            CCFiniteTimeAction *seq = CCSequence::actions
            (
             CCMoveBy::actionWithDuration(0.1, ccp(cc_scale_ipad(-20),0)),
             CCMoveBy::actionWithDuration(0.2, ccp(cc_scale_ipad(20),0)),
             CCCallFunc::actionWithTarget(this, callfunc_selector(EPScrollListLayer::listDebounceDone)),
             NULL
             );            
            m_pList->runAction(seq);                
        }
    }

    if (listContentSize.width > thisContentSize.width)
    {
        float factor = thisContentSize.width / listContentSize.width;        
        float offset = factor * listPosition.x;
        offset = (offset < 0) ? -offset : offset ;
        
        float pos = (thisContentSize.width - m_pIndicator->getContentSize().width) - offset;
        pos = m_pIndicator->getContentSize().width - pos;
        m_pIndicator->setPosition(ccp(pos,m_pIndicator->getPosition().y));
    }
}

void EPScrollListLayer::moveVerticalListTo(CCPoint point)
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
    
//    CCPoint worldPt = m_tListRect.origin;//CCDirector::sharedDirector()->convertToUI(m_tListRect.origin);
    CCPoint worldPt = this->convertToWorldSpace(m_tListRect.origin);
    CCSize size;
    
    if (CC_CONTENT_SCALE_FACTOR() != 1)
    {
        size.height = m_tListRect.size.height * CC_CONTENT_SCALE_FACTOR();
        size.width = m_tListRect.size.width * CC_CONTENT_SCALE_FACTOR();
        worldPt = ccpMult(worldPt, CC_CONTENT_SCALE_FACTOR());
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

void EPScrollListLayer::addLayer(CCLayer* layer)
{
    if (!layer)
        return;
    
    if (m_vertical)
    {
        int h = m_pList->getContentSize().height + layer->getContentSize().height;
        
        m_tMaxListContentSize = CCSizeMake(this->getContentSize().width, h);
        m_pList->setContentSize(m_tMaxListContentSize);
        layer->setAnchorPoint(ccp(0.5,0.5));
        m_pList->addChild(layer);

        CCArray* children = m_pList->getChildren();
        CCObject* obj = NULL;
        CCARRAY_FOREACH(children, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setPosition(ccp((m_pList->getContentSize().width - l->getContentSize().width)/2,
                               (h - l->getContentSize().height)));
            h -= l->getContentSize().height;
        }
    }
    else
    {
        int w = m_pList->getContentSize().width + layer->getContentSize().width;

        m_tMaxListContentSize = CCSizeMake(w, this->getContentSize().height);
        m_pList->setContentSize(m_tMaxListContentSize);
        layer->setAnchorPoint(ccp(0.5,0.5));
        layer->setPosition(ccp(m_pList->getContentSize().width - layer->getContentSize().width,layer->getContentSize().height/2));
        m_pList->addChild(layer);        
    }
}

void EPScrollListLayer::clear()
{
    m_tMaxListContentSize = CCSizeMake(0, this->getContentSize().height);
    m_pList->removeAllChildrenWithCleanup(true);
    m_pList->setContentSize(m_tMaxListContentSize);    
}

void EPScrollListLayer::removeLayer(CCLayer* layer)
{
    if (!layer)
        return;
    
    CCSize layerContentSize = layer->getContentSize();
    m_pList->removeChild(layer, true);
    
    CCArray* children = m_pList->getChildren();    
    CCObject* obj = NULL;
    
    if (m_vertical)
    {
        int h = m_pList->getContentSize().height - layerContentSize.height;
        
        m_tMaxListContentSize = CCSizeMake(this->getContentSize().width, h);
        m_pList->setContentSize(m_tMaxListContentSize);
        m_pList->addChild(layer);
        
        CCARRAY_FOREACH(children, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setPosition(ccp((m_pList->getContentSize().width - l->getContentSize().width)/2,
                               (h - l->getContentSize().height)));
            h -= l->getContentSize().height;
        }
    }
    else
    {
        
        int w = m_pList->getContentSize().width - layerContentSize.width;
        
        m_tMaxListContentSize = CCSizeMake(w, this->getContentSize().height);
        m_pList->setContentSize(m_tMaxListContentSize);
        
        w = 0;
        CCARRAY_FOREACH(children, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setPosition(ccp(w,l->getContentSize().height/2));
            w += l->getContentSize().width;
        }
    }
}

void EPScrollListLayer::insertLayer(cocos2d::CCLayer* layer,int index)
{
    if (index < 0)
        return;
    
    CCArray* children = m_pList->getChildren();
    if (index > children->count())
    {
        addLayer(layer);
        return;
    }
    
    CCArray* newChildren = CCArray::arrayWithCapacity(10);
    CCObject* obj = NULL;
    int i = 0;
    CCARRAY_FOREACH(children, obj)
    {
        if (i == index)
        {
            newChildren->addObject(layer);
        }
        
        newChildren->addObject(obj);
        i++;
    }

    if (m_vertical)
    {
        int h = m_pList->getContentSize().height + layer->getContentSize().height;
        
        m_tMaxListContentSize = CCSizeMake(this->getContentSize().width, h);
        m_pList->setContentSize(m_tMaxListContentSize);
        layer->setAnchorPoint(ccp(0.5,0.5));
        m_pList->addChild(layer);
        
        m_pList->removeAllChildrenWithCleanup(true);
        
        CCARRAY_FOREACH(newChildren, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setPosition(ccp((m_pList->getContentSize().width - l->getContentSize().width)/2,
                               (h - l->getContentSize().height)));
            h -= l->getContentSize().height;
            m_pList->addChild(l);
        }
    }
    else
    {
        
        int w = m_pList->getContentSize().width + layer->getContentSize().width;
        
        m_tMaxListContentSize = CCSizeMake(w, this->getContentSize().height);
        m_pList->setContentSize(m_tMaxListContentSize);
        layer->setAnchorPoint(ccp(0.5,0.5));
        
        m_pList->removeAllChildrenWithCleanup(true);
        
        w = 0;
        CCARRAY_FOREACH(newChildren, obj)
        {
            CCLayer *l = (CCLayer*)obj;
            l->setPosition(ccp(w,l->getContentSize().height/2));
            w += l->getContentSize().width;
            m_pList->addChild(l);
        }
    }
}

void EPScrollListLayer::clearSyncScrollLayer()
{
    m_syncScrolls.clear();
}

void EPScrollListLayer::syncScrollLayer(EPScrollListLayer* scrollLayer)
{
    if (!scrollLayer)
        return;
    
    if (m_vertical != scrollLayer->m_vertical)
        return;
    
    if (m_pList->getChildren()->count() != scrollLayer->m_pList->getChildren()->count())
        return;
    
    m_syncScrolls.push_back(scrollLayer);
}
