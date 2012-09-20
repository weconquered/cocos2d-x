#include "IAPCMGC.h"
#include "jni/JniHelper.h"
#include "jni/MessageJni.h"

namespace cocos2d { namespace plugin {

IAPCMGC* IAPCMGC::create(CCArray* pProductArray, const char* pszAppName, const char* pszCompanyName, const char* pszTelNumber)
{
    IAPCMGC* pRet = new IAPCMGC();
    if (pRet != NULL && pRet->init(pProductArray, pszAppName, pszCompanyName, pszTelNumber))
    {

    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IAPCMGC::init(CCArray* pProductArray, const char* pszAppName, const char* pszCompanyName, const char* pszTelNumber)
{
    CCAssert(pProductArray != NULL, "product array mustn't be NULL!");
    CCAssert(pszAppName != NULL && strlen(pszAppName) > 0, "Invalid app name!");
    CCAssert(pszCompanyName != NULL && strlen(pszCompanyName) > 0, "Invalid companyName!");
    CCAssert(pszTelNumber != NULL && strlen(pszTelNumber) > 0, "Invalid telephone number!");

    bool bRet = false;
    JniMethodInfo t;
    do 
    {
        CC_BREAK_IF(!initWithProductArray(pProductArray));
     
        CC_BREAK_IF(!JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/iap/CMGC/CMGCIAPAdapter"
            , "initialize"
            , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z"));

        jstring strAppName = t.env->NewStringUTF(pszAppName);
        jstring strCompanyName = t.env->NewStringUTF(pszCompanyName);
        jstring strTelNumber = t.env->NewStringUTF(pszTelNumber);

        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, strAppName, strCompanyName, strTelNumber);
        bRet = ret ? true : false; 
        // t.env->DeleteLocalRef(t.classID);

    } while(0);

    return bRet;
}

void IAPCMGC::onNotifyGameExitJNI()
{
    CCAssert(m_pDelegate != NULL, "IAP delegate was NULL");
    m_pDelegate->onNotifyGameExit();
    terminateProcessJNI();
}

}} // namespace cocos2d { namespace plugin {
