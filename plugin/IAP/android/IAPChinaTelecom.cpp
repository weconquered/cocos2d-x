#include "IAPChinaTelecom.h"
#include "jni/JniHelper.h"
namespace cocos2d { namespace plugin {

IAPChinaTelecom* IAPChinaTelecom::create(CCArray* pProductArray, const char* pszFromer)
{
    IAPChinaTelecom* pRet = new IAPChinaTelecom();
    if (pRet != NULL && pRet->init(pProductArray, pszFromer))
    {

    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IAPChinaTelecom::init(CCArray* pProductArray, const char* pszFromer)
{
    CCAssert(pProductArray != NULL, "product array mustn't be NULL!");
    CCAssert(pszFromer != NULL && strlen(pszFromer) > 0, "Invalid source!");

    bool bRet = false;
    JniMethodInfo t;

    do 
    {
        CC_BREAK_IF(!initWithProductArray(pProductArray));

        CC_BREAK_IF(!JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/iap/ChinaTelecom/ChinaTelecomIAPAdapter"
            , "initialize"
            , "(Ljava/lang/String;)Z"));

        jstring strFromer = t.env->NewStringUTF(pszFromer);
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, strFromer);
        bRet = ret ? true : false; 
        // t.env->DeleteLocalRef(t.classID);

    } while(0);

    return bRet;
}

}} // namespace cocos2d { namespace plugin {