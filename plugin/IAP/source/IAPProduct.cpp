#include "IAPProduct.h"

namespace cocos2d { namespace plugin {

IAPProduct::IAPProduct()
{

}

IAPProduct::~IAPProduct()
{
    CC_SAFE_RELEASE(m_pProductInfoTable);
}

IAPProduct* IAPProduct::create(CCDictionary* pProductInfoTable)
{
    IAPProduct* product = new IAPProduct();
    if (product != NULL && product->init(pProductInfoTable))
    {
        product->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(product);
    }
    return product;
}

bool IAPProduct::init(CCDictionary* pProductInfoTable)
{
    m_pProductInfoTable = pProductInfoTable;
    CC_SAFE_RETAIN(m_pProductInfoTable);

    return true;
}


const char* IAPProduct::getProductId(void)
{
    if (m_pProductInfoTable != NULL)
    {
        CCString* pId = (CCString*)m_pProductInfoTable->objectForKey("productId");
        return (pId != NULL ? pId->getCString() : NULL);
    }
    return NULL;
}

float IAPProduct::getPrice(void)
{
    if (m_pProductInfoTable != NULL)
    {
        CCString* pId = (CCString*)m_pProductInfoTable->objectForKey("productPrice");
        return (pId != NULL ? atof(pId->getCString()) : 0.0f);
    }
    return 0.0f;
}

CCDictionary* IAPProduct::getProductInfoTable()
{
    return m_pProductInfoTable;
}

}}
