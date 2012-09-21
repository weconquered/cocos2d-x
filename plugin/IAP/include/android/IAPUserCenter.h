#ifndef __IAPUSERCENTER_H__
#define __IAPUSERCENTER_H__

#include "IAP.h"

namespace cocos2d { namespace plugin {

class IAPUserCenter : public IAPProtocol
{
public:
    static IAPUserCenter* create(CCArray* pProductArray, const char* pAppKey, const char* pszSecretKey);
private:
    bool init(CCArray* pProductArray, const char* pAppKey, const char* pszSecretKey);
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAPUSERCENTER_H__ */
