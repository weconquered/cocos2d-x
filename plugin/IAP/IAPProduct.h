#ifndef __IAPPRODUCT_H__
#define __IAPPRODUCT_H__

#include "cocos2d.h"

namespace cocos2d { namespace plugin {

class IAPProduct : public CCObject
{
public:
    static IAPProduct* create(const char* productIdentifier,
                             const char* localizedTitle,
                             const char* localizedDescription,
                             float price,
                             const char* priceLocale);
    
    const std::string& getProductIdentifier(void);
    const std::string& getLocalizedTitle(void);
    const std::string& getLocalizedDescription(void);
    float getPrice(void);
    const std::string& getPriceLocale(void);
    
private:
    IAPProduct(void) {}
    bool init(const char* productIdentifier,
                    const char* localizedTitle,
                    const char* localizedDescription,
                    float price,
                    const char* priceLocale);
    
    std::string m_productIdentifier;
    std::string m_localizedTitle;
    std::string m_localizedDescription;
    float       m_price;
    std::string m_priceLocale;
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAPPRODUCT_H__ */
