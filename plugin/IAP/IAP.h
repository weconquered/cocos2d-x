#ifndef __IAP_H__
#define __IAP_H__

#include "IAPProduct.h"
#include "IAPTransaction.h"

namespace cocos2d { namespace iap {


enum {
    kResultSuccess = 0,
    kResultFail,
};

typedef int Result;

enum {
    kErrorNone = 0,
    kErrorUnknown,
    kErrorServiceNotReachable,
    kErrorPreviousRequestNotCompleted,
    kErrorUserCancel,
};

typedef int ErrorCode;

struct ReturnVal {
    Result result;
    ErrorCode errorCode;

    ReturnVal(Result ret, ErrorCode err) {
        this->result = ret;
        this->errorCode = err;
    }

    bool isSucceed() {
        return this->result == kResultSuccess ? true : false;
    }
};


class IAPDelegate
{
public:
    virtual void onLoginFinished(ReturnVal r) = 0;
    virtual void onLoadProductsFinished(ReturnVal r, CCArray* productsId, CCArray* invalidProductsId = NULL) = 0;
    virtual void onTransactionFinished(ReturnVal r, IAPTransaction* pTransaction) = 0;
    /** User need to release game resources in this delegate method */
    virtual void onNotifyGameExit() = 0;
};


enum {
    kIAPPayModeAuto = 0,
    kIAPPayModeSms,
    kIAPPayModeOther,
    kIAPPayModeMax
};

typedef int IAPPayMode;

class IAP
{
public:
    ~IAP();

	static IAP* getInstance();
	
    bool initWithProductArray(CCArray* pArray);

    void setDelegate(IAPDelegate* pDelegate);
    IAPDelegate* getDelegate();

    void setPayMode(IAPPayMode payMode);

	bool login();
	
    /** Load one product */
    bool loadProduct(const char* productId);

    /** load some products */
    bool loadSomeProducts(CCArray* productsId);
    
    /** Cancel Load products operation */
    void cancelLoadProducts();

    /** @brief purchase just one product
     *  @param productId Product identifier string.
     */
    bool purchaseProduct(const char* productId);
    
    /** @brief purchase some products.
     *  @param productIds an Array of CCString which stores the name of product.
     */
    bool purchaseSomeProducts(CCArray* productIds);

    /** Check whether the iap service is ready for use */
    bool isServiceReachable();
    
    /** @brief The default notification when iap service is unreachable, this method is often invoked after isServiceReachable.
     *   On android, it shows a tocast view to notify user that iap service wasn't connected. 
     *   If user wants to customize the notification, just ignores this method.
     */
    void notifyServiceUnReachable();

    /** @brief Notify iap platform to exit.
        
    */
    void notifyIAPToExit();

    /** internal function */
    CCArray* getProductsArray();
private:
	IAP();

    IAPDelegate* m_pDelegate;
    CCArray* m_pProductsArray;
};

}}

#endif /* __IAP_H__ */
