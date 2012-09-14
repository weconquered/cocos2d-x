#ifndef __IAP_H__
#define __IAP_H__

namespace cocos2d { namespace iap {

/////////////////////////////////////////////////////////////////////////
//  IAP mode 
/////////////////////////////////////////////////////////////////////////
enum {
	kIAPModeAll = 0,      // IAP mode wasn't set, the default value is kIAPModeAll.
	kIAPModeChinaTelecom,
	kIAPModeChinaMobile,
	kIAPModePunchBox
};

typedef int IAPMode;

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
    kIAPProductsRequestErrorPreviousRequestNotCompleted = 0;
    kIAPProductsRequestErrorUserCancel;
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
     virtual void onTransactionFailed(const char* proIdentifier) = 0;
     virtual void onTransactionCompleted(const char* proIdentifier) = 0;
     virtual void onTransactionRestored(const char* proIdentifier) = 0;

};


class IAP
{
public:
	static IAP* getInstance();
	void setIAPMode(IAPMode mode);
	bool login();
	void loadProducts(CCArray* productsId, ); 

private:
	IAP();
};

}}

#endif /* __IAP_H__ */
