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
    void menuCloseCallback(CCObject* pSender);
    void testIAP(cocos2d::CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void onLogonFinished(cocos2d::plugin::ReturnVal r);
    virtual void onLoadProductsFinished(cocos2d::plugin::ReturnVal r, cocos2d::CCArray* productsId, cocos2d::CCArray* invalidProductsId = NULL);
    virtual void onTransactionFinished(cocos2d::plugin::ReturnVal r, cocos2d::plugin::IAPTransaction* pTransaction);
    virtual void onNotifyGameExit();
};

#endif // __HELLOWORLD_SCENE_H__
