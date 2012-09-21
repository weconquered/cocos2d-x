#ifndef __IAPPRODUCT_H__
#define __IAPPRODUCT_H__

#include "cocos2d.h"

namespace cocos2d { namespace plugin {

class IAPProduct : public CCObject
{
public:
    virtual ~IAPProduct();
    static IAPProduct* create(CCDictionary* pProductInfoTable);
    
    const char* getProductId();
    float getPrice();

    CCDictionary* getProductInfoTable();

private:
    IAPProduct();
    bool init(CCDictionary* pProductInfoTable);
    
    CCDictionary* m_pProductInfoTable;
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAPPRODUCT_H__ */
