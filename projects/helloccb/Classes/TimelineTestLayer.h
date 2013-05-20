//
//  TimelineTestLayer.h
//  helloccb
//
//  Created by James Chen on 5/19/13.
//
//

#ifndef __helloccb__TimelineTestLayer__
#define __helloccb__TimelineTestLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

class TimelineTestLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TimelineTestLayer, create);
    TimelineTestLayer();
    virtual ~TimelineTestLayer();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCallback1(CCNode* sender);
    void onCallback2(CCNode* sender);
    void onAnimationFinished();
    void setAnimationManganger(cocos2d::extension::CCBAnimationManager* manager);
private:
    cocos2d::extension::CCBAnimationManager* m_animationMangager;
    cocos2d::CCSprite* m_aniSp;
};
#endif /* defined(__helloccb__TimelineTestLayer__) */
