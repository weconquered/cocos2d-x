//
//  MainLayer.cpp
//  helloccb
//
//  Created by James Chen on 4/21/13.
//
//

#include "MainLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool MainLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTopleftBtn", CCMenuItemImage*, this->mTopleftBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTestTitleLabelTTF", CCLabelTTF *, this->mTestTitleLabelTTF);
    
    return false;
}

bool MainLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
    if (pTarget == this)
    {
        if (0 == strcmp(pMemberVariableName, "mCustomPropertyInt"))
        {
            CCLog("mCustomPropertyInt = %d", pCCBValue->getIntValue());
            bRet = true;
        }
//        else if ( 0 == strcmp(pMemberVariableName, "mCustomPropertyFloat"))
//        {
//            this->mCustomPropertyFloat = pCCBValue->getFloatValue();
//            CCLog("mCustomPropertyFloat = %f", mCustomPropertyFloat);
//            bRet = true;
//        }
//        else if ( 0  == strcmp(pMemberVariableName, "mCustomPropertyBoolean"))
//        {
//            this->mCustomPropertyBoolean = pCCBValue->getBoolValue();
//            CCLog("mCustomPropertyBoolean = %d", mCustomPropertyBoolean);
//            bRet = true;
//        }
//        else if ( 0  == strcmp(pMemberVariableName, "mCustomPropertyString"))
//        {
//            this->mCustomPropertyString = pCCBValue->getStringValue();
//            CCLog("mCustomPropertyString = %s", mCustomPropertyString.c_str());
//            bRet = true;
//        }
        
    }
    
    return bRet;
}

SEL_MenuHandler MainLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPressButtonTopLeft", MainLayer::onTopLeftMenuClicked);
    
    return NULL;
}

SEL_CCControlHandler MainLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

void MainLayer::onTopLeftMenuClicked(cocos2d::CCObject * pSender)
{
    CCLog("onTopLeftMenuClicked...");
    mTopleftBtn->setScale(2.0f);
}
