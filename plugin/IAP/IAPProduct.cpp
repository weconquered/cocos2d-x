#include "IAPProduct.h"

namespace cocos2d { namespace iap {

IAPProduct* IAPProduct::create(const char* productIdentifier,
                                          const char* localizedTitle,
                                          const char* localizedDescription,
                                          float price,
                                          const char* priceLocale)
{
    IAPProduct* product = new IAPProduct();
    if (product != NULL && product->init(productIdentifier, localizedTitle, localizedDescription, price, priceLocale))
    {
        product->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(product);
    }
    return product;
}

bool IAPProduct::init(const char* productIdentifier,
                    const char* localizedTitle,
                    const char* localizedDescription,
                    float price,
                    const char* priceLocale)
{
    m_productIdentifier = productIdentifier ? productIdentifier : "";
    m_localizedTitle = localizedTitle ? localizedTitle : "";
    m_localizedDescription = localizedDescription ? localizedDescription : "";
    m_price = price;
    m_priceLocale = priceLocale ? priceLocale : "";
    return true;
}


const std::string& IAPProduct::getProductIdentifier(void)
{
    return m_productIdentifier;
}

const std::string& IAPProduct::getLocalizedTitle(void)
{
    return m_localizedTitle;
}

const std::string& IAPProduct::getLocalizedDescription(void)
{
    return m_localizedDescription;
}

float IAPProduct::getPrice(void)
{
    return m_price;
}

const std::string& IAPProduct::getPriceLocale(void)
{
    return m_priceLocale;
}

}}
