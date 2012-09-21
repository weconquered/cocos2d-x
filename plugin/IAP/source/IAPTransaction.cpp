#include "IAPTransaction.h"

namespace cocos2d { namespace plugin {

IAPTransaction* IAPTransaction::create(const char* productId)
{
    IAPTransaction* pRet = new IAPTransaction();
    if (pRet != NULL && pRet->init(productId))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

IAPTransaction::IAPTransaction()
{

}

bool IAPTransaction::init(const char* productId)
{
    if (productId != NULL && strlen(productId) > 0)
    {
        m_productId = productId;
        return true;
    }
    return false;
}

const char* IAPTransaction::getProductId()
{
    return m_productId.c_str();
}

}}
