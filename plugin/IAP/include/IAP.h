#ifndef __IAP_H__
#define __IAP_H__

#include "IAPProduct.h"
#include "IAPTransaction.h"

namespace cocos2d { namespace plugin {


enum ErrorCode {
    kErrorNone = 0,
    kErrorUnknown,
    kErrorServiceInvalid,
    kErrorPreviousRequestUncompleted,
    kErrorUserCancelled,
    kErrorTimeout,
    kErrorProductIdInvalid,
    kErrorProductPriceInvalid,
    kErrorPurchaseFailed,
    kErrorSmsKeyInvalid
};

struct ReturnVal {
    bool isSucceed;
    ErrorCode errorCode;
};


class IAPDelegate
{
public:
    virtual void onLogonFinished(ReturnVal r) = 0;
    /** The callback function of finishing loading products
     * @param productArray Its element type is 'IAPProduct*'.
     */
    virtual void onLoadProductsFinished(ReturnVal r, CCArray* productArray) = 0;
    virtual void onTransactionFinished(ReturnVal r, IAPTransaction* pTransaction) = 0;
    /** User needs to release game resources in this delegate method */
    virtual void onNotifyGameExit() = 0;
};

class IAPProtocol
{
public:
    virtual ~IAPProtocol();

    /** Set IAP delegate, it must be invoked before you call other methods */
    virtual void setDelegate(IAPDelegate* pDelegate);

    /** Get IAP delegate */
    virtual IAPDelegate* getDelegate();

    /** Check whether user needs to login IAP platform. It return false by default. */
    virtual bool isLogonNeeded();

    /** Login IAP platform */
	virtual void login();

    /** load some products */
    virtual bool loadProducts(CCArray* productIds);
    
    /** Cancel Load products operation */
    virtual void cancelLoadProducts();

    /** @brief purchase just one product
     *  @param productId Product identifier string.
     */
    virtual bool purchaseProduct(const char* productId);
    
    /** @brief purchase some products. It return false by default.
     *         If the new IAP system support this feature, this method needs to be overrided.
     *  @param productIds an Array of CCString which stores the name of product.
     */
    virtual bool purchaseSomeProducts(CCArray* productIds);

    /** Check whether the iap service is ready for use */
    virtual bool isServiceValid();

    /** Notify iap platform to exit.
     *  On CMCC's IAP system, the exit activity which belongs to sdk will be shown.
     *  If you want to put your games to CMGC store, you should invoke this method when 'exit' button was clicked.
     */
    virtual void notifyIAPToExit();

    /** Internal functions */
    virtual CCArray* getProductsArray();

    /** For porting to a new android iap platform, override them if you want to custom these methods. */
    virtual void onLoginFinishedJNI(ReturnVal r);

    /** The callback function of finishing loading products from JNI.
     * @param productArray Its element type is 'IAPProduct*'.
     */
    virtual void onLoadProductsFinishedJNI(ReturnVal r, CCArray* productArray);
    virtual void onTransactionFinishedJNI(ReturnVal r, IAPTransaction* pTransaction);
    virtual void onNotifyGameExitJNI();

protected:
	IAPProtocol();
    bool initWithProductArray(CCArray* pArray);
    IAPDelegate* m_pDelegate;
    CCArray* m_pProductsArray;
};

}} // namespace cocos2d { namespace plugin {

#endif /* __IAP_H__ */
