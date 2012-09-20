#include "IAPUserCenter.h"
#include "jni/JniHelper.h"
namespace cocos2d { namespace plugin {

IAPUserCenter* IAPUserCenter::create(CCArray* pProductArray, const char* pAppKey, const char* pszSecretKey)
{
    IAPUserCenter* pRet = new IAPUserCenter();
    if (pRet != NULL && pRet->init(pProductArray, pAppKey, pszSecretKey))
    {

    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IAPUserCenter::init(CCArray* pProductArray, const char* pAppKey, const char* pszSecretKey)
{
    CCAssert(pProductArray != NULL, "product array mustn't be NULL!");
    CCAssert(pAppKey != NULL && strlen(pAppKey) > 0, "Invalid app key!");
    CCAssert(pszSecretKey != NULL && strlen(pszSecretKey) > 0, "Invalid secret key!");
    bool bRet = false;
    JniMethodInfo t;

    do 
    {

        CC_BREAK_IF(!initWithProductArray(pProductArray));

        CC_BREAK_IF(!JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/iap/UserCenter/UserCenterIAPAdapter"
            , "initialize"
            , "(Ljava/lang/String;Ljava/lang/String;)Z"));

        jstring strAppKey = t.env->NewStringUTF(pAppKey);
        jstring strSecretKey = t.env->NewStringUTF(pszSecretKey);
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, strAppKey, strSecretKey);

        bRet = ret ? true : false; 
        // t.env->DeleteLocalRef(t.classID);
    }while (0);
    return bRet;
}

}} // namespace cocos2d { namespace plugin {