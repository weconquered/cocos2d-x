#ifndef __IAPCHINATELECOM_H__
#define __IAPCHINATELECOM_H__

#include "IAP.h"

namespace cocos2d { namespace plugin {

class IAPChinaTelecom : public IAPProtocol
{
public:
    static IAPChinaTelecom* create(CCArray* pProductArray, const char* pszFromer);
private:
    bool init(CCArray* pProductArray, const char* pszFromer);
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAPCHINATELECOM_H__ */