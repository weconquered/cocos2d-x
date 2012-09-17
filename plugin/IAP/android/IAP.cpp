#include "IAP.h"
#include <android/log.h>
#include "jni/JniHelper.h"
//  IAPWrapper.java methods

using namespace cocos2d;
using namespace cocos2d::iap;


extern "C"
{

jstring Java_org_cocos2dx_iap_IAPWrapper_nativeGetProductInfo(JNIEnv* env, jobject thiz, jstring text)
{
    const char* pszID = env->GetStringUTFChars(text, NULL);
    return env->NewStringUTF(pszID);
 //   std::string strRet = ProductInfo::getProductInfo(pszID);
 //   return env->NewStringUTF(strRet.c_str());
}

bool Java_org_cocos2dx_iap_IAPWrapper_nativeIAPEnabled(JNIEnv*  env, jobject thiz)
{
    return true;//cjh (! ModuleCustomConfig::disableIAP);
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidLoginFailed(JNIEnv*  env, jobject thiz)
{
    if (! IAP::getInstance()->getLoginDelegate())
    {
        CCLog("Havn't set IAPLoginDelegate yet!!!!!!!!!");
        return;
    }
    IAP::getInstance()->getLoginDelegate()->onIAPLoginFailed();
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidLoginSuccess(JNIEnv*  env, jobject thiz)
{
    if (! IAP::getInstance()->getLoginDelegate())
    {
        CCLog("Havn't set IAPLoginDelegate yet!!!!!!!!!");
        return;
    }
    IAP::getInstance()->getLoginDelegate()->onIAPLoginCompleted();
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidReceivedProducts(JNIEnv*  env, jobject thiz, jstring products)
{
    if (! IAP::getInstance()->getRequestProductsDelegate())
    {
        CCLog("Havn't set IAPRequestProductsDelegate yet!!!!!!!!!");
        return;
    }

    static CCString pStr;
    static CCArray items;
    if (items.count() == 0)
    {
        items.addObject(&pStr);
    }
    pStr.m_sString = cocos2d::JniHelper::jstring2string(products);
    CCLog("Java_org_cocos2dx_iap_IAPWrapper_nativeDidReceivedProducts : %s", pStr.m_sString.c_str());

    IAP::getInstance()->getRequestProductsDelegate()->onIAPRequestProductsCompleted(&items);
    items.removeAllObjects();
    pStr.m_sString = "";
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidCompleteTransaction(JNIEnv*  env, jobject thiz, jstring productIdentifier)
{
    if (! IAP::getInstance()->getTransactionDelegate())
    {
        CCLog("Havn't set IAPTransactionDelegate yet!!!!!!!!!");
        return;
    }

    std::string item = cocos2d::JniHelper::jstring2string(productIdentifier);
    IAP::getInstance()->getTransactionDelegate()->onTransactionCompleted(IAPTransaction::create(item.c_str()));
}

void Java_org_cocos2dx_iap_IAPWrapper_nativeDidFailedTransaction(JNIEnv*  env, jobject thiz, jstring productIdentifier)
{
    if (! IAP::getInstance()->getTransactionDelegate())
    {
        CCLog("Havn't set IAPTransactionDelegate yet!!!!!!!!!");
        return;
    }

    std::string item = cocos2d::JniHelper::jstring2string(productIdentifier);
    IAP::getInstance()->getTransactionDelegate()->onTransactionFailed(IAPTransaction::create(item.c_str()));
}

} // extern "C"


namespace cocos2d { namespace iap {

IAP* IAP::getInstance()
{
    static IAP s_iapInstance;
    return &s_iapInstance;
}

IAP::IAP()
{

}

IAP::~IAP()
{

}

void IAP::setPlatform(IAPPlatform platform)
{
    m_platform = platform;
}

bool IAP::login(IAPLoginDelegete* pDelegate)
{

    m_pLoginDelegate = pDelegate;
    return true;
}

bool IAP::loadOneProduct(const char* productId, IAPPayMode payMode, IAPRequestProductsDelegate* pDelegate)
{
    m_pRequestProductsDelegate = pDelegate;
    if (productId == NULL || strlen(productId) <= 0 || pDelegate == NULL || payMode < kIAPPayModeDefault || payMode >= kIAPPayModeMax)
    {
        CCLog("loadOneProduct: invaild parameters!");
        return false;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
        , "org/cocos2dx/iap/IAPWrapper"
        , "requestProductData"
        , "(Ljava/lang/String;I)V"))
    {
        jstring stringProduct = t.env->NewStringUTF(productId);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringProduct, payMode);
        // t.env->DeleteLocalRef(t.classID);
    }
    return true;
}

/** Load products */
bool IAP::loadProducts(CCArray* productsId, IAPPayMode payMode, IAPRequestProductsDelegate* pDelegate)
{
    CCAssert(0, "not implement!");
    return false;
}

/** Cancel Load products operation */
void IAP::cancelLoadProducts()
{

}

/** @brief purchase just one product
 *  @param 
 */
bool IAP::purchaseOneProduct(const char* productId, IAPTransactionDelegate* pDelegate)
{
    if (productId != NULL && strlen(productId) > 0)
    {       
        m_pTransactionDelegate = pDelegate;
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

/** @brief purchase more than one product.
 *  @param productIds an Array of CCString which stores the name of product.
 */
bool IAP::purchaseProducts(CCArray* productIds, IAPTransactionDelegate* pDelegate)
{
    CCAssert(0, "not implement!");
    if (productIds != NULL && productIds->count() > 0)
    {
        m_pTransactionDelegate = pDelegate;
        return true;
    }
    CCLog("Invaild parameters in purchaseProducts");
    return false;
}

/** Check whether the network is ready for being used */
bool IAP::isNetworkReachable()
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

/** @brief The default notification when network is unreachable, this method is often invoked after isNetworkReachable.
 *   On android, it shows a tocast view to notify user that network wasn't connected. 
 *   If user wants to customize the notification, just ignores this method.
 */
void IAP::networkUnReachableNotify()
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

IAPLoginDelegete* IAP::getLoginDelegate()
{
    return m_pLoginDelegate;
}

IAPRequestProductsDelegate* IAP::getRequestProductsDelegate()
{
    return m_pRequestProductsDelegate;
}

IAPTransactionDelegate* IAP::getTransactionDelegate()
{
    return m_pTransactionDelegate;
}

}}
