#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../../plugin/IAP/IAP.h"

class HelloWorld
 : public cocos2d::CCLayer
 , public cocos2d::plugin::IAPDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(cocos2d::CCObject* pSender);
    void menuCallbackReset(cocos2d::CCObject* pSender);
    void menuCallbackLoadProducts(cocos2d::CCObject* pSender);
    void menuCallbackPurchaseProduct(cocos2d::CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void onLogonFinished(cocos2d::plugin::ReturnVal r);
    virtual void onLoadProductsFinished(cocos2d::plugin::ReturnVal r, cocos2d::CCArray* pProductArray);
    virtual void onTransactionFinished(cocos2d::plugin::ReturnVal r, cocos2d::plugin::IAPTransaction* pTransaction);
    virtual void onNotifyGameExit();

private:
    cocos2d::CCMenu* m_pMenu1;
    cocos2d::CCMenu* m_pMenu2;
    cocos2d::CCMenuItemLabel* m_pLoadItem;
    cocos2d::CCMenuItemLabel* m_pResetItem;
};

#endif // __HELLOWORLD_SCENE_H__
