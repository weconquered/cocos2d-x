//
//  MainLayer.h
//  helloccb
//
//  Created by James Chen on 4/21/13.
//
//

#ifndef __helloccb__MainLayer__
#define __helloccb__MainLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

class MainLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainLayer, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onTopLeftMenuClicked(cocos2d::CCObject * pSender);
private:
    cocos2d::CCMenuItemImage* mTopleftBtn;
};


#endif /* defined(__helloccb__MainLayer__) */
