//
//  TimelineTestLayer.cpp
//  helloccb
//
//  Created by James Chen on 5/19/13.
//
//

#include "TimelineTestLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

void TimelineTestLayer::setAnimationManganger(cocos2d::extension::CCBAnimationManager* manager)
{
    m_animationMangager = manager;
    m_animationMangager->setAnimationCompletedCallback(this, callfunc_selector(TimelineTestLayer::onAnimationFinished));
}

void TimelineTestLayer::onAnimationFinished()
{
    CCLOG("onAnimationFinished.......");
    m_aniSp->stopAllActions();
}
TimelineTestLayer::TimelineTestLayer()
: m_animationMangager(NULL)
//: _helloLabel(NULL)
{}

TimelineTestLayer::~TimelineTestLayer()
{
//    CC_SAFE_RELEASE(_helloLabel);
    CocosDenshion::SimpleAudioEngine::end();
}

SEL_MenuHandler TimelineTestLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

SEL_CCControlHandler TimelineTestLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

SEL_CallFuncN TimelineTestLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "onCallback1", TimelineTestLayer::onCallback1);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "onCallback2", TimelineTestLayer::onCallback2);
    return NULL;
}

bool TimelineTestLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "helloLabel", CCLabelTTF *, this->_helloLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGrossini", CCSprite *, this->m_aniSp);
    CCLOG("var: %s", pMemberVariableName);
    return false;
}

void TimelineTestLayer::onCallback1(CCNode* sender)
{
    CCLOG("onCallback1");
    static bool bFirst = true;
    if (bFirst)
    {
        m_animationMangager->runAnimationsForSequenceNamed("grossini_animation");
        bFirst = false;
    }
    
    // Rotate the label when the button is pressed
//    this->_helloLabel->runAction(CCRotateBy::create(1,360));
//    this->_helloLabel->setString("Callback 1");
}

void TimelineTestLayer::onCallback2(CCNode* sender)
{
    CCLOG("onCallback2");
    // Rotate the label when the button is pressed
//    this->_helloLabel->runAction(CCRotateBy::create(1,-360));
//    this->_helloLabel->setString("Callback 2");
}
