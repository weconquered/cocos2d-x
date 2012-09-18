#ifndef __IAPTRANSACTION_H__
#define __IAPTRANSACTION_H__

#include "cocos2d.h"

namespace cocos2d { namespace iap {

class IAPTransaction : public CCObject
{
public:
    static IAPTransaction* create(const char* productId);
    const char* getProductIdentifier();
private:
    IAPTransaction();
    bool init(const char* productId);
    std::string m_productIdentifier;
};

}}

#endif /* __IAPTRANSACTION_H__ */
