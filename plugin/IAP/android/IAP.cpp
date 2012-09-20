#include "IAP.h"
#include <android/log.h>
#include "jni/JniHelper.h"
#include "json/json.h"
//  IAPWrapper.java methods

using namespace cocos2d;
using namespace cocos2d::plugin;

static IAPProtocol* s_pIAPInstance = NULL;

extern "C"
{

void Java_org_cocos2dx_iap_IAPWrapper_nativeFinishLogon(JNIEnv*  env, jobject thiz, jboolean isSucceed, jint errorCode)
{
    CCAssert(s_pIAPInstance->getDelegate() != NULL, "IAP Delegate wasn't set");
    ReturnVal r;
    r.isSucceed = isSucceed;
    r.errorCode = (ErrorCode)errorCode;
    s_pIAPInstance->onLoginFinishedJNI(r);
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeFinishLoadProducts(JNIEnv*  env, jobject thiz, jobjectArray products, jboolean isSucceed, jint errorCode)
{
    CCAssert(s_pIAPInstance->getDelegate() != NULL, "IAP Delegate wasn't set");

    ReturnVal r;
    r.isSucceed = isSucceed;
    r.errorCode = (ErrorCode)errorCode;

    CCArray* pProductIdArray = NULL;
    do 
    {
        if (isSucceed)
        {
            int len = env->GetArrayLength(products);

            if (len <= 0) 
            {
                CCLog("productIds didn't contain any string");
                r.isSucceed = false;
                r.errorCode = kErrorUnknown;
                break;
            }
            pProductIdArray = CCArray::create();
            for (int i = 0; i < len; ++i)
            {
                jstring str = (jstring)env->GetObjectArrayElement(products, i);
                CCString* pStr = CCString::create(cocos2d::JniHelper::jstring2string(str));
                CCLog("Java_org_cocos2dx_iap_IAPWrapper_nativeDidReceivedProducts : %s", pStr->getCString());
                pProductIdArray->addObject(pStr);
            }
        }
    } while (false);

    s_pIAPInstance->onLoadProductsFinishedJNI(r, pProductIdArray);
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeFinishTransaction(JNIEnv*  env, jobject thiz, jstring productId, jboolean isSucceed, jint errorCode)
{
    CCAssert(s_pIAPInstance->getDelegate() != NULL, "IAP Delegate wasn't set");

    std::string item = cocos2d::JniHelper::jstring2string(productId);
    ReturnVal r;
    r.isSucceed = isSucceed;
    r.errorCode = (ErrorCode)errorCode;
    s_pIAPInstance->onTransactionFinishedJNI(r, IAPTransaction::create(item.c_str()));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeNotifyGameExit(JNIEnv*  env, jobject thiz)
{
    CCAssert(s_pIAPInstance->getDelegate() != NULL, "IAP Delegate wasn't set");
    s_pIAPInstance->onNotifyGameExitJNI();
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
    
    CCArray* productArray = s_pIAPInstance->getProductsArray();
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

    CCLOG("---- ret : %s ----", retValue.toStyledString().c_str());
    return retValue.toStyledString();
}


jstring Java_org_cocos2dx_iap_IAPProducts_nativeGetProductInfo(JNIEnv* env, jobject thiz, jstring text)
{
    const char* pszID = env->GetStringUTFChars(text, NULL);
    std::string strRet = getProductInfo(pszID);
    return env->NewStringUTF(strRet.c_str());
}

} // extern "C"


namespace cocos2d { namespace plugin {

IAPProtocol::IAPProtocol()
: m_pDelegate(NULL)
, m_pProductsArray(NULL)
{
    s_pIAPInstance = this;
}

IAPProtocol::~IAPProtocol()
{
    CC_SAFE_RELEASE(m_pProductsArray);
}

bool IAPProtocol::initWithProductArray(CCArray* pArray)
{
    bool bRet = false;
    m_pProductsArray = (CCArray*)pArray->copy();
    if (m_pProductsArray != NULL && m_pProductsArray->count() > 0)
    {
        bRet = true;
    }
    return bRet;
}

CCArray* IAPProtocol::getProductsArray()
{
    return m_pProductsArray;
}

IAPDelegate* IAPProtocol::getDelegate()
{
    return m_pDelegate;
}

void IAPProtocol::setDelegate(IAPDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
}

bool IAPProtocol::isLogonNeeded()
{
    return false;
}

void IAPProtocol::login()
{
}

bool IAPProtocol::loadProduct(const char* productId)
{
    if (productId == NULL || strlen(productId) <= 0 )
    {
        CCLog("loadProduct: invaild parameters!");
        return false;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "loadProduct"
        , "(Ljava/lang/String;)V"))
    {
        jstring stringProduct = t.env->NewStringUTF(productId);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringProduct);
        // t.env->DeleteLocalRef(t.classID);
    }
    return true;
}

bool IAPProtocol::loadSomeProducts(CCArray* productsId)
{
    CCLOG("IAPProtocol::loadSomeProducts shouldn't be invoked! ");
    return false;
}

void IAPProtocol::cancelLoadProducts()
{

}

bool IAPProtocol::purchaseProduct(const char* productId)
{
    if (productId != NULL && strlen(productId) > 0)
    {       
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/iap/IAPWrapper"
            , "purchaseProduct"
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

bool IAPProtocol::purchaseSomeProducts(CCArray* productIds)
{
    CCLOG("IAPProtocol::purchaseSomeProducts shouldn't be invoked! ");
    return false;
}

bool IAPProtocol::isServiceValid()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "isServiceValid"
        , "()Z"))
    {
        bool ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        // t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

/** @brief The default notification when iap service is invalid, 
 *         this method is often invoked after isServiceReachable.
 *   On android, it shows a tocast view to notify user that iap service wasn't connected. 
 *   If you want to customize the notification, just ignores this method.
 */
// void IAPProtocol::notifyServiceInvaild()
// {
//     JniMethodInfo t;
//     if (JniHelper::getStaticMethodInfo(t
//         , "org/cocos2dx/iap/IAPWrapper"
//         , "notifyServiceInvaild"
//         , "()V"))
//     {
//         t.env->CallStaticVoidMethod(t.classID, t.methodID);
//         // t.env->DeleteLocalRef(t.classID);
//     }
// }

void IAPProtocol::notifyIAPToExit()
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

void IAPProtocol::onLoginFinishedJNI(ReturnVal r)
{
    CCAssert(m_pDelegate != NULL, "IAP delegate was NULL");
    m_pDelegate->onLogonFinished(r);
}

void IAPProtocol::onLoadProductsFinishedJNI(ReturnVal r, CCArray* productsId, CCArray* invalidProductsId/* = NULL */)
{
    CCAssert(m_pDelegate != NULL, "IAP delegate was NULL");
    m_pDelegate->onLoadProductsFinished(r, productsId, invalidProductsId);
}

void IAPProtocol::onTransactionFinishedJNI(ReturnVal r, IAPTransaction* pTransaction)
{
    CCAssert(m_pDelegate != NULL, "IAP delegate was NULL");
    m_pDelegate->onTransactionFinished(r, pTransaction);
}

void IAPProtocol::onNotifyGameExitJNI()
{
    CCAssert(m_pDelegate != NULL, "IAP delegate was NULL");
    m_pDelegate->onNotifyGameExit();
}

}} // namespace cocos2d { namespace plugin {
