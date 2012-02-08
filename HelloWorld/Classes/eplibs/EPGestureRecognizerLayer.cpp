//
//  EPGestureRecognizerLayer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPGestureRecognizerLayer.h"

using namespace cocos2d;

#define LONG_PRESS_DEFAULT_INTERVAL  1.0

EPGestureRecognizerLayer::EPGestureRecognizerLayer()
:m_pTapRecognizers(NULL)
,m_pLongPressRecognizers(NULL)
,m_pPanRecognizers(NULL)
,m_pPinchRecognizers(NULL)
,m_pRotationRecognizers(NULL)
,m_pTapNodes(NULL)
,m_pLongPressNodes(NULL)
,m_pPanNodes(NULL)
,m_pPinchNode(NULL)
,m_pRotationNode(NULL)
,m_fLongPressInterval(LONG_PRESS_DEFAULT_INTERVAL)
,m_bTransitionDone(false)
,m_bPause(false)
,m_nTouchCount(0)
,m_nNumberOfTapsRequired(1)
,m_nNumberOfLongPressRequired(1)
,m_nNumberOfPansRequired(1)
{
}

EPGestureRecognizerLayer::~EPGestureRecognizerLayer()
{
    CC_SAFE_RELEASE(m_pTapNodes);
    CC_SAFE_RELEASE(m_pTapRecognizers);
    
    CC_SAFE_RELEASE(m_pLongPressNodes);
    CC_SAFE_RELEASE(m_pLongPressRecognizers);

    CC_SAFE_RELEASE(m_pPanNodes);
    CC_SAFE_RELEASE(m_pPanRecognizers);
    
    CC_SAFE_RELEASE(m_pPinchRecognizers);
    
    CC_SAFE_RELEASE(m_pRotationRecognizers);
}

EPGestureRecognizerLayer* EPGestureRecognizerLayer::node()
{
    EPGestureRecognizerLayer *layer = new EPGestureRecognizerLayer();
    if (layer && layer->init()) 
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer)
    return 0;
}

void EPGestureRecognizerLayer::initRecognizerArray()
{
    m_pTapNodes = CCArray::arrayWithCapacity(3);
    m_pTapNodes->retain();

    m_pTapRecognizers = CCArray::arrayWithCapacity(3);
    m_pTapRecognizers->retain();

    m_pLongPressNodes = CCArray::arrayWithCapacity(3);
    m_pLongPressNodes->retain();

    m_pLongPressRecognizers = CCArray::arrayWithCapacity(3);
    m_pLongPressRecognizers->retain();
    
    m_pPanNodes = CCArray::arrayWithCapacity(3);
    m_pPanNodes->retain();

    m_pPanRecognizers = CCArray::arrayWithCapacity(3);
    m_pPanRecognizers->retain();

    m_pPinchRecognizers = CCArray::arrayWithCapacity(3);
    m_pPinchRecognizers->retain();
    
    m_pRotationRecognizers = CCArray::arrayWithCapacity(3);
    m_pRotationRecognizers->retain();
}

bool EPGestureRecognizerLayer::init()
{
    if (CCLayer::init())
    {        
        this->m_bIsTouchEnabled = true;
        m_bTabEnabled = false;
        
        // menu in the center of the screen
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        
        this->m_bIsRelativeAnchorPoint = true;
        setAnchorPoint(ccp(0.5f, 0.5f));
        this->setContentSize(s);
        
        // XXX: in v0.7, winSize should return the visible size
        // XXX: so the bar calculation should be done there
        CCRect r;
        CCApplication::sharedApplication().statusBarFrame(&r);
        ccDeviceOrientation orientation = CCDirector::sharedDirector()->getDeviceOrientation();
        if (orientation == CCDeviceOrientationLandscapeLeft || orientation == CCDeviceOrientationLandscapeRight)
        {
            s.height -= r.size.width;
        }
        else
        {
            s.height -= r.size.height;
        }
        setPosition(ccp(s.width/2, s.height/2));
        
        this->initRecognizerArray();
        
        return true;
    }
    
    return false;
}

void EPGestureRecognizerLayer::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCLayer::onExit();
    m_bTransitionDone = false;
    m_bTabEnabled = false;
    
    if (m_pLongPressRecognizers->count() > 0)
        this->unschedule(schedule_selector(EPGestureRecognizerLayer::longPressedTimer));
}

void EPGestureRecognizerLayer::onEnterTransitionDidFinish()
{
//    CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this, 0);
    CCLayer::onEnterTransitionDidFinish();
    m_bTransitionDone = true;
    m_bTabEnabled = true;
}

void EPGestureRecognizerLayer::startGestureRecognizer()
{
    m_bPause = false;
}

void EPGestureRecognizerLayer::stopGestureRecognizer()
{
    m_bPause = true;
}

#pragma mark -
#pragma mark Internal
GestureRecognizer* EPGestureRecognizerLayer::nodeForTouch(CCArray *pArrays, CCSet *pTouches)
{    
    if (pArrays && pArrays->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pArrays, pObject)
        {
            GestureRecognizer* pRecognizer = (GestureRecognizer*) pObject;
            
            if (!pRecognizer)
                continue;
            
            GestureRecognizer* pTouchedRecognizer = pRecognizer->nodeForTouch(pTouches);
            if (pTouchedRecognizer)
                return pTouchedRecognizer;
        }
    }
    
    return NULL;
}

bool EPGestureRecognizerLayer::isVisibled()
{
    if (!this->getIsVisible() || !m_bTransitionDone)
        return false;
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->getIsVisible() == false)
        {
            return false;
        }
    }
    
    return true;
}

void EPGestureRecognizerLayer::defaultTouchesCancelled(CCArray *pArrays, CCSet *pTouches)
{
    CCArray *removeList = CCArray::arrayWithCapacity(4);
    
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(pArrays, pObject)
    {
        GestureRecognizer* pRecognizer = (GestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->containsTouch(pTouches))
        {
            removeList->addObject(pObject);
        }
    }
    
    CCARRAY_FOREACH(removeList, pObject)
    {
        m_pTapNodes->removeObject(pObject);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// TapGestureRecognizer
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void EPGestureRecognizerLayer::tapTouchesBegan(CCSet *pTouches)
{
    if (m_pTapRecognizers->count() == 0 || m_nTouchCount > m_nNumberOfTapsRequired)
        return;
    
    if (m_pTapNodes->count() > m_nNumberOfTapsRequired)
        return;
    
    TapGestureRecognizer *pRecognizer = (TapGestureRecognizer*)this->nodeForTouch(m_pTapRecognizers,pTouches);
    if (pRecognizer)
    {
        pRecognizer->setGestureRecognizerState(kGestureRecognizerStateBegan);
        m_pTapNodes->addObject(pRecognizer);
    }        
}

void EPGestureRecognizerLayer::tapTouchesMoved(CCSet *pTouches)
{
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pTapNodes, pObject)
    {
        TapGestureRecognizer* pRecognizer = (TapGestureRecognizer*) pObject;
        if (pRecognizer && 
            pRecognizer->getGestureRecognizerState() == kGestureRecognizerStateBegan &&
            pRecognizer->containsTouch(pTouches))
        {
            if (!pRecognizer->nodeForTouch(pTouches))
            {
                pRecognizer->setGestureRecognizerState(kGestureRecognizerStateChanged);
            }            
        }
    }
}

void EPGestureRecognizerLayer::tapTouchesEnded(CCSet *pTouches)
{
    CCArray *removeList = CCArray::arrayWithCapacity(4);

    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pTapNodes, pObject)
    {
        TapGestureRecognizer* pRecognizer = (TapGestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->containsTouch(pTouches))
        {
            if (pRecognizer->getGestureRecognizerState() == kGestureRecognizerStateBegan)
            {
                pRecognizer->setGestureRecognizerState(kGestureRecognizerStateEnded);
                if (this->gestureRecognizer(pRecognizer))
                    this->performTap(pRecognizer, pRecognizer->getNode());
            }

            pRecognizer->reset();
            removeList->addObject(pObject);
        }
    }
    
    CCARRAY_FOREACH(removeList, pObject)
    {
        m_pTapNodes->removeObject(pObject);
    }
}

void EPGestureRecognizerLayer::tapTouchesCancelled(CCSet *pTouches)
{
    this->defaultTouchesCancelled(m_pTapNodes,pTouches);
}

int EPGestureRecognizerLayer::getNumberOfTapsRequired()
{
    return m_nNumberOfTapsRequired;
}

void EPGestureRecognizerLayer::setNumberOfTapsRequired(int num)
{
    m_nNumberOfTapsRequired = num;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// LongPressGestureRecognizer
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void EPGestureRecognizerLayer::longPressTouchesBegan(CCSet *pTouches)
{
    if (m_pLongPressRecognizers->count() == 0 || m_nTouchCount > m_nNumberOfLongPressRequired)
        return;
    
    if (m_pLongPressNodes->count() > m_nNumberOfLongPressRequired)
        return;
    
    LongPressGestureRecognizer *pRecognizer = (LongPressGestureRecognizer*)this->nodeForTouch(m_pLongPressRecognizers,pTouches);
    if (pRecognizer)
    {
        pRecognizer->setGestureRecognizerState(kGestureRecognizerStateBegan);
        pRecognizer->setInterval(m_fLongPressInterval);
        
        if (this->gestureRecognizer(pRecognizer))
            this->performLongPress(pRecognizer, pRecognizer->getNode());
        
        m_pLongPressNodes->addObject(pRecognizer);
    }       
}

void EPGestureRecognizerLayer::longPressTouchesMoved(CCSet *pTouches)
{
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pLongPressNodes, pObject)
    {
        LongPressGestureRecognizer* pRecognizer = (LongPressGestureRecognizer*) pObject;
        if (pRecognizer && 
            pRecognizer->getGestureRecognizerState() == kGestureRecognizerStateBegan &&
            pRecognizer->containsTouch(pTouches))
        {
            if (!pRecognizer->nodeForTouch(pTouches))
            {
                pRecognizer->setGestureRecognizerState(kGestureRecognizerStateChanged);
            }            
        }
    }
}

void EPGestureRecognizerLayer::longPressTouchesEnded(CCSet *pTouches)
{
    CCArray *removeList = CCArray::arrayWithCapacity(4);
    
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pLongPressNodes, pObject)
    {
        GestureRecognizer* pRecognizer = (GestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->containsTouch(pTouches))
        {
            pRecognizer->reset();
            removeList->addObject(pObject);
        }
    }
    
    CCARRAY_FOREACH(removeList, pObject)
    {
        m_pLongPressNodes->removeObject(pObject);
    }
}

void EPGestureRecognizerLayer::longPressTouchesCancelled(CCSet *pTouches)
{
    this->defaultTouchesCancelled(m_pLongPressNodes,pTouches);
}

void EPGestureRecognizerLayer::longPressedTimer(ccTime dt)
{
    if (m_nTouchCount == 0)
        return;
    
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pLongPressNodes, pObject)
    {
        LongPressGestureRecognizer* pRecognizer = (LongPressGestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->getGestureRecognizerState() == kGestureRecognizerStateBegan)
        {
            if (pRecognizer->countdown(dt))
            {
                if (this->gestureRecognizer(pRecognizer))
                    this->performLongPress(pRecognizer, pRecognizer->getNode());
            }
        }
    }
}

void EPGestureRecognizerLayer::setLongPressInterval(ccTime interval)
{
    m_fLongPressInterval = interval;
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pLongPressNodes, pObject)
    {
        LongPressGestureRecognizer* pRecognizer = (LongPressGestureRecognizer*) pObject;
        pRecognizer->setInterval(m_fLongPressInterval);
    }
}

ccTime EPGestureRecognizerLayer::getLongPressInterval()
{
    return m_fLongPressInterval;
}

void EPGestureRecognizerLayer::setNumberOfLongPressRequired(int num)
{
    m_nNumberOfLongPressRequired = num;
}

int EPGestureRecognizerLayer::getNumberOfLongPressRequired()
{
    return m_nNumberOfLongPressRequired;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// PanGestureRecognizer
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void EPGestureRecognizerLayer::panTouchesBegan(CCSet *pTouches)
{
    if (m_pPanRecognizers->count() == 0 || m_nTouchCount > m_nNumberOfPansRequired)
        return;
    
    if (m_pPanNodes->count() > m_nNumberOfPansRequired)
        return;
    
    PanGestureRecognizer *pRecognizer = (PanGestureRecognizer*)this->nodeForTouch(m_pPanRecognizers,pTouches);
    if (pRecognizer)
    {
        pRecognizer->setGestureRecognizerState(kGestureRecognizerStateBegan);
        pRecognizer->updateTranslateView(pTouches);
        
        if (this->gestureRecognizer(pRecognizer))
            this->performPan(pRecognizer, pRecognizer->getNode());

        m_pPanNodes->addObject(pRecognizer);
    }        
}

void EPGestureRecognizerLayer::panTouchesMoved(CCSet *pTouches)
{
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pPanNodes, pObject)
    {
        PanGestureRecognizer* pRecognizer = (PanGestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->containsTouch(pTouches))
        {
            pRecognizer->setGestureRecognizerState(kGestureRecognizerStateChanged);
            
            if (pRecognizer->updateTranslateView(pTouches))
            {
                if (this->gestureRecognizer(pRecognizer))
                    this->performPan(pRecognizer, pRecognizer->getNode());
            }
        }
    }    
}

void EPGestureRecognizerLayer::panTouchesEnded(CCSet *pTouches)
{
    CCArray *removeList = CCArray::arrayWithCapacity(4);
    
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pPanNodes, pObject)
    {
        PanGestureRecognizer* pRecognizer = (PanGestureRecognizer*) pObject;
        if (pRecognizer && pRecognizer->containsTouch(pTouches))
        {
            pRecognizer->setGestureRecognizerState(kGestureRecognizerStateEnded);
            pRecognizer->updateTranslateView(pTouches);
            if (this->gestureRecognizer(pRecognizer))
                this->performPan(pRecognizer, pRecognizer->getNode());
            
            pRecognizer->reset();
            removeList->addObject(pObject);
        }
    }
    
    CCARRAY_FOREACH(removeList, pObject)
    {
        m_pPanNodes->removeObject(pObject);
    }    
}

void EPGestureRecognizerLayer::panTouchesCancelled(CCSet *pTouches)
{
    this->defaultTouchesCancelled(m_pPanNodes,pTouches);
}

int EPGestureRecognizerLayer::getNumberOfPansRequired()
{
    return m_nNumberOfPansRequired;
}

void EPGestureRecognizerLayer::setNumberOfPansRequired(int num)
{
    m_nNumberOfPansRequired = num;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// PinchGestureRecognizer
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void EPGestureRecognizerLayer::pinchTouchesBegan(CCSet *pTouches)
{
    if (m_pPinchRecognizers->count() == 0 || m_nTouchCount > 2)
        return;
    
    PinchGestureRecognizer *pRecognizer = (PinchGestureRecognizer*)this->nodeForTouch(m_pPinchRecognizers,pTouches);
    
    if ((!m_pPinchNode && pRecognizer) ||
        (m_pPinchNode && m_pPinchNode == pRecognizer))
    {
        pRecognizer->setGestureRecognizerState(kGestureRecognizerStateBegan);
        pRecognizer->setControlTouches(pTouches);
        m_pPinchNode = pRecognizer;
        if (this->gestureRecognizer(m_pPinchNode) && m_pPinchNode->controlIsReady())
            this->performPinch(m_pPinchNode, m_pPinchNode->getNode());
    }        
}

void EPGestureRecognizerLayer::pinchTouchesMoved(CCSet *pTouches)
{
    if (!m_pPinchNode || !m_pPinchNode->controlIsReady())
        return;
    
    if (m_pPinchNode->updateTouches(pTouches))
    {
        m_pPinchNode->setGestureRecognizerState(kGestureRecognizerStateChanged);
        if (this->gestureRecognizer(m_pPinchNode))
            this->performPinch(m_pPinchNode, m_pPinchNode->getNode());
    }
}

void EPGestureRecognizerLayer::pinchTouchesEnded(CCSet *pTouches)
{
    if (!m_pPinchNode)
        return;
    
    m_pPinchNode->setGestureRecognizerState(kGestureRecognizerStateEnded);
    if (this->gestureRecognizer(m_pPinchNode) && m_pPinchNode->controlIsReady())
        this->performPinch(m_pPinchNode, m_pPinchNode->getNode());
    
    m_pPinchNode->reset();
    m_pPinchNode = NULL;
}

void EPGestureRecognizerLayer::pinchTouchesCancelled(CCSet *pTouches)
{
    if (!m_pPinchNode)
        return;

    m_pPinchNode->setGestureRecognizerState(kGestureRecognizerStateCancelled);
    if (this->gestureRecognizer(m_pPinchNode))
        this->performPinch(m_pPinchNode, m_pPinchNode->getNode());

    m_pPinchNode->reset();
    m_pPinchNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// RotationGestureRecognizer
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void EPGestureRecognizerLayer::rotationTouchesBegan(CCSet *pTouches)
{
    if (m_pRotationRecognizers->count() == 0 || m_nTouchCount > 2)
        return;
    
    RotationGestureRecognizer *pRecognizer = (RotationGestureRecognizer*)this->nodeForTouch(m_pRotationRecognizers,pTouches);
    
    if ((!m_pRotationNode && pRecognizer) ||
        (m_pRotationNode && m_pRotationNode == pRecognizer))
    {
        pRecognizer->setGestureRecognizerState(kGestureRecognizerStateBegan);
        pRecognizer->setControlTouches(pTouches);
        m_pRotationNode = pRecognizer;
        if (this->gestureRecognizer(m_pRotationNode) && m_pRotationNode->controlIsReady())
            this->performRotation(m_pRotationNode, m_pRotationNode->getNode());
    }        
}

void EPGestureRecognizerLayer::rotationTouchesMoved(CCSet *pTouches)
{
    if (!m_pRotationNode || !m_pRotationNode->controlIsReady())
        return;
    
    if (m_pRotationNode->updateTouches(pTouches))
    {
        m_pRotationNode->setGestureRecognizerState(kGestureRecognizerStateChanged);
        if (this->gestureRecognizer(m_pRotationNode))
            this->performRotation(m_pRotationNode, m_pRotationNode->getNode());
    }
}

void EPGestureRecognizerLayer::rotationTouchesEnded(CCSet *pTouches)
{
    if (!m_pRotationNode)
        return;
    
    m_pRotationNode->setGestureRecognizerState(kGestureRecognizerStateEnded);
    if (this->gestureRecognizer(m_pRotationNode) && m_pRotationNode->controlIsReady())
        this->performRotation(m_pRotationNode, m_pRotationNode->getNode());

    m_pRotationNode->reset();
    m_pRotationNode = NULL;
}

void EPGestureRecognizerLayer::rotationTouchesCancelled(CCSet *pTouches)
{
    if (!m_pRotationNode)
        return;

    m_pRotationNode->setGestureRecognizerState(kGestureRecognizerStateCancelled);
    if (this->gestureRecognizer(m_pRotationNode))
        this->performRotation(m_pRotationNode, m_pRotationNode->getNode());

    m_pRotationNode->reset();
    m_pRotationNode = NULL;
}

#pragma mark -
#pragma mark Event
void EPGestureRecognizerLayer::registerWithTouchDispatcher()
{
//    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, true);
    CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this, 0);
}

void EPGestureRecognizerLayer::ccTouchesBegan(CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    m_nTouchCount += pTouches->count();

    if (m_bPause)
        return;
        
    if (!this->isVisibled())
        return;
    
    this->tapTouchesBegan(pTouches);
    this->longPressTouchesBegan(pTouches);
    this->panTouchesBegan(pTouches);
    this->pinchTouchesBegan(pTouches);
    this->rotationTouchesBegan(pTouches);
}

void EPGestureRecognizerLayer::ccTouchesMoved(CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (m_bPause)
        return;

    this->tapTouchesMoved(pTouches);
    this->longPressTouchesMoved(pTouches);
    this->panTouchesMoved(pTouches);
    this->pinchTouchesMoved(pTouches);
    this->rotationTouchesMoved(pTouches);
}

void EPGestureRecognizerLayer::ccTouchesEnded(CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    m_nTouchCount -= pTouches->count();

    if (m_bPause)
        return;
   
    this->tapTouchesEnded(pTouches);
    this->longPressTouchesEnded(pTouches);
    this->panTouchesEnded(pTouches);
    this->pinchTouchesEnded(pTouches);
    this->rotationTouchesEnded(pTouches);
}

void EPGestureRecognizerLayer::ccTouchesCancelled(CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    m_nTouchCount -= pTouches->count();
    
    if (m_bPause)
        return;
    
    this->tapTouchesCancelled(pTouches);
    this->longPressTouchesCancelled(pTouches);
    this->panTouchesCancelled(pTouches);
    this->pinchTouchesCancelled(pTouches);
    this->rotationTouchesCancelled(pTouches);
}

#pragma mark -
#pragma mark GestureRecognizer Functions
void EPGestureRecognizerLayer::addTapRecognizer(CCNode *node)
{
    if (!node)
        return;
    
    TapGestureRecognizer *recognizer = TapGestureRecognizer::recognizer();
    recognizer->setNode(node);
    m_pTapRecognizers->addObject(recognizer);
}

void EPGestureRecognizerLayer::addLongPressRecognizer(CCNode *node)
{
    if (!node)
        return;

    if (m_pLongPressRecognizers->count() == 0)
    {
        this->schedule(schedule_selector(EPGestureRecognizerLayer::longPressedTimer), 0.1);
    }
    
    LongPressGestureRecognizer *recognizer = LongPressGestureRecognizer::recognizer();
    recognizer->setNode(node);
    recognizer->setInterval(m_fLongPressInterval);
    m_pLongPressRecognizers->addObject(recognizer);
}

void EPGestureRecognizerLayer::addPanRecognizer(CCNode *node)
{
    if (!node)
        return;
    
    PanGestureRecognizer *recognizer = PanGestureRecognizer::recognizer();
    recognizer->setNode(node);
    m_pPanRecognizers->addObject(recognizer);
}

void EPGestureRecognizerLayer::addPinchRecognizer(CCNode *node)
{
    if (!node)
        return;
    
    PinchGestureRecognizer *recognizer = PinchGestureRecognizer::recognizer();
    recognizer->setNode(node);
    m_pPinchRecognizers->addObject(recognizer);
}

void EPGestureRecognizerLayer::addRotationRecognizer(CCNode *node)
{
    if (!node)
        return;
    
    RotationGestureRecognizer *recognizer = RotationGestureRecognizer::recognizer();
    recognizer->setNode(node);
    m_pRotationRecognizers->addObject(recognizer);
}

#pragma mark -
#pragma mark Button Effects
void EPGestureRecognizerLayer::pressButton(CCNode *node, SEL_CallFuncN selector)
{
    CCFiniteTimeAction *seq = CCSequence::actions
    (
     CCScaleTo::actionWithDuration(0.1, node->getScale() + 0.1),
     CCScaleTo::actionWithDuration(0.1, node->getScale()),
     CCCallFuncN::actionWithTarget(this, selector),
     NULL
     );
    node->runAction(seq);
}