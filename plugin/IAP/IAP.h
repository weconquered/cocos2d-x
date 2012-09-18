#ifndef __IAP_H__
#define __IAP_H__

#include "IAPProduct.h"
#include "IAPTransaction.h"

namespace cocos2d { namespace iap {

/////////////////////////////////////////////////////////////////////////
//  IAP mode 
/////////////////////////////////////////////////////////////////////////
enum {
	kIAPPlatformAll = 0,      // IAP mode wasn't set, the default value is kIAPModeAll.
	kIAPPlatformChinaTelecom,
	kIAPPlatformChinaMobile,
	kIAPPlatformUserCenter
};

typedef int IAPPlatform;



/////////////////////////////////////////////////////////////////////////
//  IAP Login 
/////////////////////////////////////////////////////////////////////////
class IAPLoginDelegete
{
public:
	virtual void onIAPLoginCompleted() = 0;
	virtual void onIAPLoginFailed() = 0;
};

/////////////////////////////////////////////////////////////////////////
//  IAP Request 
/////////////////////////////////////////////////////////////////////////
enum {
    kIAPProductsRequestErrorPreviousRequestNotCompleted = 0,
    kIAPProductsRequestErrorUserCancel
};

typedef int IAPProductsRequestErrorCode;

class IAPRequestProductsDelegate
{
public:
    /* @brief Products request completed callback function.
     * @param productsId        Array of vaild product identifiers, its element type is IAPProduct.
     * @param invalidProductsId Array of invaild product identifiers, its element type is IAPProduct.
     */
	virtual void onIAPRequestProductsCompleted(CCArray* productsId, CCArray* invalidProductsId = NULL) = 0;
	virtual void onIAPRequestProductsFailed(IAPProductsRequestErrorCode errorCode, const char* errorMsg) = 0;
};

/////////////////////////////////////////////////////////////////////////
//  IAP TransactionDelegate 
/////////////////////////////////////////////////////////////////////////
class IAPTransactionDelegate
{
public:
     virtual void onTransactionFailed(IAPTransaction* pTransaction) = 0;
     virtual void onTransactionCompleted(IAPTransaction* pTransaction) = 0;
     virtual void onTransactionRestored(IAPTransaction* pTransaction) = 0;
};

enum {
    kIAPPayModeDefault = 0,
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
	
  //  void setPlatform(IAPPlatform mode);

	bool login(IAPLoginDelegete* pDelegate);
	
    /** Load products */
    bool loadOneProduct(const char* productId, IAPPayMode payMode, IAPRequestProductsDelegate* pDelegate);
    bool loadProducts(CCArray* productsId, IAPPayMode payMode, IAPRequestProductsDelegate* pDelegate);
    
    /** Cancel Load products operation */
    void cancelLoadProducts();

    /** @brief purchase just one product
     *  @param 
     */
    bool purchaseOneProduct(const char* productId, IAPTransactionDelegate* pDelegate);
    
    /** @brief purchase more than one product.
     *  @param productIds an Array of CCString which stores the name of product.
     */
    bool purchaseProducts(CCArray* productIds, IAPTransactionDelegate* pDelegate);

    /** Check whether the network is ready for being used */
    bool isNetworkReachable();
    
    /** @brief The default notification when network is unreachable, this method is often invoked after isNetworkReachable.
     *   On android, it shows a tocast view to notify user that network wasn't connected. 
     *   If user wants to customize the notification, just ignores this method.
     */
    void networkUnReachableNotify();

    /** internal functions */
    IAPLoginDelegete* getLoginDelegate();
    IAPRequestProductsDelegate* getRequestProductsDelegate();
    IAPTransactionDelegate* getTransactionDelegate();
private:
	IAP();

    IAPPlatform m_platform;
    IAPLoginDelegete* m_pLoginDelegate;
    IAPRequestProductsDelegate* m_pRequestProductsDelegate;
    IAPTransactionDelegate* m_pTransactionDelegate;
};

}}

#endif /* __IAP_H__ */
