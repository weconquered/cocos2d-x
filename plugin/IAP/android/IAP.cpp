#include "IAP.h"
#include <android/log.h>
#include "jni/JniHelper.h"
#include "json/json.h"
//  IAPWrapper.java methods

using namespace cocos2d;
using namespace cocos2d::iap;


extern "C"
{


bool Java_org_cocos2dx_iap_IAPWrapper_nativeIAPEnabled(JNIEnv*  env, jobject thiz)
{
    return true;//cjh (! ModuleCustomConfig::disableIAP);
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidLoginFailed(JNIEnv*  env, jobject thiz)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");
    IAP::getInstance()->getDelegate()->onLoginFinished(ReturnVal(kResultFail, kErrorUnknown));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidLoginSuccess(JNIEnv*  env, jobject thiz)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");
    IAP::getInstance()->getDelegate()->onLoginFinished(ReturnVal(kResultSuccess, kErrorNone));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidReceivedProducts(JNIEnv*  env, jobject thiz, jstring products)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");

    static CCString pStr;
    static CCArray items;
    if (items.count() == 0)
    {
        items.addObject(&pStr);
    }
    pStr.m_sString = cocos2d::JniHelper::jstring2string(products);
    CCLog("Java_org_cocos2dx_iap_IAPWrapper_nativeDidReceivedProducts : %s", pStr.m_sString.c_str());

    IAP::getInstance()->getDelegate()->onLoadProductsFinished(ReturnVal(kResultSuccess, kErrorNone), &items);
    items.removeAllObjects();
    pStr.m_sString = "";
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidCompleteTransaction(JNIEnv*  env, jobject thiz, jstring productIdentifier)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");

    std::string item = cocos2d::JniHelper::jstring2string(productIdentifier);
    IAP::getInstance()->getDelegate()->onTransactionFinished(ReturnVal(kResultSuccess, kErrorNone), IAPTransaction::create(item.c_str()));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidFailedTransaction(JNIEnv*  env, jobject thiz, jstring productIdentifier)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");

    std::string item = cocos2d::JniHelper::jstring2string(productIdentifier);
    IAP::getInstance()->getDelegate()->onTransactionFinished(ReturnVal(kResultFail, kErrorUnknown), IAPTransaction::create(item.c_str()));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeNotifyGameExit(JNIEnv*  env, jobject thiz)
{
    CCAssert(IAP::getInstance()->getDelegate() != NULL, "IAP Delegate wasn't set");
    IAP::getInstance()->getDelegate()->onNotifyGameExit();
}

static CCDictionary* findItemFromProductArray(CCArray* pArray, const char* productId)
{
    CCDictionary* pRet = NULL;
    CCObject* pObj = NULL;
    CCDictionary* pDict = NULL;
    CCString* pstrProductId = NULL;
    CCARRAY_FOREACH(pArray, pObj)
    {
        pDict = (CCDictionary*)pObj;
        pstrProductId = (CCString*)pDict->objectForKey("productId");

        if (0 == strcmp(productId, pstrProductId->getCString()))
        {
            pRet = pDict;
            break;
        }
    }
    return pRet;
}

static std::string getProductInfo(const char* productId)
{
    CCAssert(productId != NULL && strlen(productId) > 0, "invalid product id");
    
    CCArray* productArray = IAP::getInstance()->getProductsArray();
    CCAssert(productArray != NULL, "products array wasn't set");

    CCDictionary* goodInfo = findItemFromProductArray(productArray, productId);
    CCAssert(goodInfo != NULL, "goodInfo must not be NULL!");

    CCArray* pKeys = goodInfo->allKeys();
    CCObject* pObj = NULL;

    Json::Value retValue;
    CCARRAY_FOREACH(pKeys, pObj)
    {
        CCString* pKey = (CCString*) pObj;
        CCString* pValue = (CCString*)(goodInfo->objectForKey(pKey->m_sString));

        retValue[pKey->m_sString.c_str()] = pValue->m_sString;
    }

    CCLog("---- ret : %s ----", retValue.toStyledString().c_str());
    return retValue.toStyledString();
}


jstring Java_org_cocos2dx_iap_IAPWrapper_nativeGetProductInfo(JNIEnv* env, jobject thiz, jstring text)
{
    const char* pszID = env->GetStringUTFChars(text, NULL);
    std::string strRet = getProductInfo(pszID);
    return env->NewStringUTF(strRet.c_str());
}

} // extern "C"


namespace cocos2d { namespace iap {

IAP* IAP::getInstance()
{
    static IAP s_iapInstance;
    return &s_iapInstance;
}


IAP::IAP()
: m_pDelegate(NULL)
, m_pProductsArray(NULL)
{

}

IAP::~IAP()
{
    CC_SAFE_RELEASE(m_pProductsArray);
}

bool IAP::initWithProductArray(CCArray* pArray)
{
    m_pProductsArray = (CCArray*)pArray->copy();
    return true;
}

CCArray* IAP::getProductsArray()
{
    return m_pProductsArray;
}

IAPDelegate* IAP::getDelegate()
{
    return m_pDelegate;
}

void IAP::setDelegate(IAPDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
}

void IAP::setPayMode(IAPPayMode payMode)
{
    CCAssert(payMode >= kIAPPayModeAuto && payMode <= kIAPPayModeMax, "invalid paymode parameter!");
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "setPayMode"
        , "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, payMode);
        // t.env->DeleteLocalRef(t.classID);
    }
}

bool IAP::login()
{
    CCAssert(m_pDelegate != NULL, "IAP Delegate wasn't set");
    m_pDelegate->onLoginFinished(ReturnVal(kResultSuccess, kErrorNone));
    return true;
}

bool IAP::loadProduct(const char* productId)
{
    if (productId == NULL || strlen(productId) <= 0 )
    {
        CCLog("loadProduct: invaild parameters!");
        return false;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "requestProductData"
        , "(Ljava/lang/String;)V"))
    {
        jstring stringProduct = t.env->NewStringUTF(productId);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringProduct);
        // t.env->DeleteLocalRef(t.classID);
    }
    return true;
}

bool IAP::loadSomeProducts(CCArray* productsId)
{
    CCAssert(0, "not implement!");
    return false;
}

void IAP::cancelLoadProducts()
{

}

bool IAP::purchaseProduct(const char* productId)
{
    if (productId != NULL && strlen(productId) > 0)
    {       
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/iap/IAPWrapper"
            , "addPayment"
            , "(Ljava/lang/String;)V"))
        {
            jstring stringProduct = t.env->NewStringUTF(productId);
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringProduct);
            // t.env->DeleteLocalRef(t.classID);
        }

        return true;
    }
    CCLog("Invaild parameters in purchaseOneProduct");
    return false;
}

bool IAP::purchaseSomeProducts(CCArray* productIds)
{
    CCAssert(0, "not implement!");
    if (productIds != NULL && productIds->count() > 0)
    {
        return true;
    }
    CCLog("Invaild parameters in purchaseProducts");
    return false;
}

bool IAP::isServiceReachable()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "networkReachable"
        , "()Z"))
    {
        bool ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        // t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

void IAP::notifyServiceUnReachable()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "networkUnReachableNotify"
        , "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // t.env->DeleteLocalRef(t.classID);
    }
}

void IAP::notifyIAPToExit()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "notifyIAPToExit"
        , "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // t.env->DeleteLocalRef(t.classID);
    }
}

}}
