#ifndef __IAPCMGC_H__
#define __IAPCMGC_H__

#include "IAP.h"

namespace cocos2d { namespace plugin {

class IAPCMGC : public IAPProtocol
{
public:
    static IAPCMGC* create(CCArray* pProductArray, const char* pszAppName, const char* pszCompanyName, const char* pszTelNumber);
    virtual void onNotifyGameExitJNI();
private:
    bool init(CCArray* pProductArray, const char* pszAppName, const char* pszCompanyName, const char* pszTelNumber);
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAPCMGC_H__ */
