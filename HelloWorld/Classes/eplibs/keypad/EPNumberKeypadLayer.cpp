//
//  EPNumberKeypad.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPNumberKeypadLayer.h"
#include "EPSpriteButton.h"
#include "EPNotificationCenter.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define KEYPAD_TEXTURE                    "keypad/4444_cash_keypad.plist"
#else
    #define KEYPAD_TEXTURE                    "4444_cash_keypad.plist"
#endif

#define KEYPAD_UNIT_WIDTH         (106)
#define KEYPAD_UNIT_HEIGHT        (52)

using namespace cocos2d;

const char* kKeypadWillShowNotification = "KeypadWillShowNotification";
const char* kKeypadDidShowNotification = "KeypadDidShowNotification";
const char* kKeypadWillHideNotification = "KeypadWillHideNotification";
const char* kKeypadDidHideNotification = "KeypadDidHideNotification";
const char* kKeypadPressNotification = "KeypadPressNotification";
const char* kKeypadShowNotification = "KeypadShowNotification";
const char* kKeypadHideNotification = "KeypadHideNotification";

typedef struct
{
    float x;
    float y;
    const char *frame;
}KeypadUnit;

static KeypadUnit layoutPosition[kNumberKeypadMax] = 
{
    { 0.0,-1.5,"keypad_0.png"},
    {-1.0, 1.5,"keypad_1.png"},
    { 0.0, 1.5,"keypad_2.png"},
    { 1.0, 1.5,"keypad_3.png"},
    {-1.0, 0.5,"keypad_4.png"},
    { 0.0, 0.5,"keypad_5.png"},
    { 1.0, 0.5,"keypad_6.png"},
    {-1.0,-0.5,"keypad_7.png"},
    { 0.0,-0.5,"keypad_8.png"},
    { 1.0,-0.5,"keypad_9.png"},
    {-1.0,-1.5,"keypad_done.png"},
    { 1.0,-1.5,"keypad_cancel.png"},
};

EPNumberKeypadLayer::EPNumberKeypadLayer()
:m_tex(NULL)
,m_keypadEnabled(false)
{
}

EPNumberKeypadLayer::~EPNumberKeypadLayer()
{
    EPNotificationCenter::sharedNotifCenter()->removeObserver(this, kKeypadShowNotification);
    EPNotificationCenter::sharedNotifCenter()->removeObserver(this, kKeypadHideNotification);
    
    this->unloadResource();
    
    CC_SAFE_RELEASE(m_items)
}

EPNumberKeypadLayer *EPNumberKeypadLayer::keypad()
{
    EPNumberKeypadLayer *keypad = new EPNumberKeypadLayer;
    keypad->init();
    keypad->autorelease();
    return keypad;
}

bool EPNumberKeypadLayer::init()
{
	if ( !EPGestureRecognizerLayer::init() )
	{
		return false;
	} 
    
    EPNotificationCenter::sharedNotifCenter()->addObserver(this, 
                                                           callfuncO_selector(EPNumberKeypadLayer::show), 
                                                           kKeypadShowNotification,
                                                           NULL);

    EPNotificationCenter::sharedNotifCenter()->addObserver(this, 
                                                           callfuncO_selector(EPNumberKeypadLayer::hide), 
                                                           kKeypadHideNotification,
                                                           NULL);

    m_items = CCArray::arrayWithCapacity(12);
    m_items->retain();
    
    this->loadResource();
    this->initKeypadLayout();
	return true;
}

void EPNumberKeypadLayer::initKeypadLayout()
{
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_keypad = CCNode::node();
    m_keypad->setIsVisible(false);
    m_keypad->setPosition(ccp(winSize.width/2,cc_scale_ipad(-145)));
    this->addChild(m_keypad);
    
    for (int i=0 ; i<kNumberKeypadMax ; i++)
    {
        CCMenuItem *item = EPSpriteButton::itemWithSpriteFrame(frameCache->spriteFrameByName("keypad_number_bg.png"),
                                                               frameCache->spriteFrameByName("keypad_number_bg2.png"), 
                                                               frameCache->spriteFrameByName(layoutPosition[i].frame));
        item->setPosition(ccp(cc_scale_ipad(layoutPosition[i].x * KEYPAD_UNIT_WIDTH),
                              cc_scale_ipad(layoutPosition[i].y * KEYPAD_UNIT_HEIGHT)));
        item->setTag(i);
        m_keypad->addChild(item);
        m_items->addObject(item);
        this->addTapRecognizer(item);
    }
}

void EPNumberKeypadLayer::loadResource()
{
//    const char *pszPath = CCFileUtils::fullPathFromRelativePath(KEYPAD_TEXTURE);
//    string texturePath("");
//    
//    texturePath = pszPath;
//    
//    // remove .xxx
//    size_t startPos = texturePath.find_last_of("."); 
//    texturePath = texturePath.erase(startPos);
//    
//    // append .png
//    texturePath = texturePath.append(".png");
//    
//	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str(),0);    
//    
//    texture->setPVRImagesHavePremultipliedAlpha(true);
//    if (STR_HAS_PREFIX(KEYPAD_TEXTURE,"4444"))
//        texture->setDefaultAlphaPixelFormat(kTexture2DPixelFormat_RGBA4444);
//    else if (STR_HAS_PREFIX(KEYPAD_TEXTURE,"565"))
//        texture->setDefaultAlphaPixelFormat(kTexture2DPixelFormat_RGB565);
//    else if (STR_HAS_PREFIX(KEYPAD_TEXTURE,"5A1"))
//        texture->setDefaultAlphaPixelFormat(kTexture2DPixelFormat_RGB5A1);
//    else
//        texture->setDefaultAlphaPixelFormat(kTexture2DPixelFormat_RGBA8888);
//    
//    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
//    frameCache->addSpriteFramesWithFile(KEYPAD_TEXTURE, texture); 
//    
//    m_tex = texture;
}

void EPNumberKeypadLayer::unloadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(KEYPAD_TEXTURE);
    
    if (m_tex)
        CCTextureCache::sharedTextureCache()->removeTexture(m_tex);
}

void EPNumberKeypadLayer::onExit()
{
    EPGestureRecognizerLayer::onExit();
    m_keypadEnabled = false;
}

void EPNumberKeypadLayer::onEnterTransitionDidFinish()
{
    EPGestureRecognizerLayer::onEnterTransitionDidFinish();
}

void EPNumberKeypadLayer::showDone()
{
    m_keypadEnabled = true;
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadDidShowNotification);
}

void EPNumberKeypadLayer::hideDone()
{
    m_keypadEnabled = false;
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadDidHideNotification);
}

void EPNumberKeypadLayer::show(CCObject *object)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCFiniteTimeAction *seq = CCSequence::actions
    (
     CCShow::action(),
     CCMoveTo::actionWithDuration(0.3, ccp(winSize.width/2,cc_scale_ipad(145))),
     CCCallFunc::actionWithTarget(this, callfunc_selector(EPNumberKeypadLayer::showDone)),
     NULL
     );
    m_keypad->runAction(seq);
    
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadWillShowNotification);
}

void EPNumberKeypadLayer::hide(CCObject *object)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCFiniteTimeAction *seq = CCSequence::actions
    (
     CCMoveTo::actionWithDuration(0.3, ccp(winSize.width/2,cc_scale_ipad(-145))),
     CCHide::action(),
     CCCallFunc::actionWithTarget(this, callfunc_selector(EPNumberKeypadLayer::hideDone)),
     NULL
     );
    m_keypad->runAction(seq);
    
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadWillHideNotification);
}

void EPNumberKeypadLayer::pressAction(CCNode *sender)
{
    CCMenuItem *item = (CCMenuItem*) sender;
    
    item->unselected();
    m_bTabEnabled = true;
    
    CCString *num = new CCString(item->getTag());
    num->autorelease();
    EPNotificationCenter::sharedNotifCenter()->postNotification(kKeypadPressNotification, num);
}

#pragma mark -
#pragma mark GestureRecognizer
void EPNumberKeypadLayer::pressButton(CCNode *node, SEL_CallFuncN selector)
{
    CCFiniteTimeAction *seq = CCSequence::actions
    (
     CCDelayTime::actionWithDuration(0.1),
     CCCallFuncN::actionWithTarget(this, selector),
     NULL
     );
    node->runAction(seq);
}

bool EPNumberKeypadLayer::gestureRecognizer(GestureRecognizer *gestureRecognizer)
{
    if (gestureRecognizer->getRecognizerType() == kTapGestureRecognizer)
        return true;
    return false;
}

void EPNumberKeypadLayer::performTap(TapGestureRecognizer *recognizer ,CCNode *node)
{
    if (!m_bTabEnabled)
        return;
    
    CCObject *obj;
    CCARRAY_FOREACH(m_items, obj)
    {
        CCMenuItem *item = (CCMenuItem*)obj;
        if (node == item)
        {
            m_bTabEnabled = false;
            item->selected();
            this->pressButton(item, callfuncN_selector(EPNumberKeypadLayer::pressAction));
            break;
        }
    }
}