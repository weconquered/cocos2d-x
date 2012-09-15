#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../../plugin/IAP/IAP.h"

class HelloWorld
 : public cocos2d::CCLayer
 , public cocos2d::iap::IAPLoginDelegete
 , public cocos2d::iap::IAPRequestProductsDelegate
 , public cocos2d::iap::IAPTransactionDelegate
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

    virtual void onIAPLoginCompleted();
    virtual void onIAPLoginFailed();

    virtual void onIAPRequestProductsCompleted(cocos2d::CCArray* productsId, cocos2d::CCArray* invalidProductsId = NULL);
    virtual void onIAPRequestProductsFailed(cocos2d::iap::IAPProductsRequestErrorCode errorCode, const char* errorMsg);

    virtual void onTransactionFailed(cocos2d::iap::IAPTransaction* pTransaction);
    virtual void onTransactionCompleted(cocos2d::iap::IAPTransaction* pTransaction);
    virtual void onTransactionRestored(cocos2d::iap::IAPTransaction* pTransaction);
};

#endif // __HELLOWORLD_SCENE_H__
