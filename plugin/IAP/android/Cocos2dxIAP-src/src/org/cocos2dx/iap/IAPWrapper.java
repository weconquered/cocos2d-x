package org.cocos2dx.iap;

public class IAPWrapper {	
	
	public static final int kErrorNone = 0;
	public static final int kErrorUnknown = 1;
	public static final int kErrorServiceInvalid = 2;
	public static final int kErrorPreviousRequestUncompleted = 3;
	public static final int kErrorUserCancelled = 4;
	public static final int kErrorTimeout = 5;
	public static final int kErrorProductIdInvalid = 6;
	public static final int kErrorProductPriceInvalid = 7;
	public static final int kErrorPurchaseFailed = 8;
	public static final int kErrorSmsKeyInvalid = 9;
	
	private static boolean mIsPreviousRequestCompleted = true;
	
	private static void LogD(String msg) {
		Wrapper.LogD("IAPWrapper", msg);
	}
	
	////////////////////////////////////////////////////////////////////////
	// IAPWrapper
	private static native void nativeFinishLogon(boolean isSucceed, int errorCode);
	private static native void nativeFinishLoadProducts(String[] products, boolean isSucceed, int errorCode);
	private static native void nativeFinishTransaction(TransactionInfo info, boolean isSucceed, int errorCode);
	private static native void nativeNotifyGameExit();

	public static void setPreviousRequestCompleted(boolean isCompleted) {
		mIsPreviousRequestCompleted = isCompleted;
	}

	public static boolean isPreviousRequestCompleted() {
		return mIsPreviousRequestCompleted;
	}

	private static IAPAdapter mCurrentAdapter = null;

	public static void setIAPAdapter(IAPAdapter adapter) {
		mCurrentAdapter = adapter;
	}
	
	public static void purchaseProduct(String productId) {
		if (!isPreviousRequestCompleted()) {
			LogD("purchaseProduct, PreviousRequestUncompleted" + productId);
			//nativeFinishTransaction(productIdentifier, false, kErrorPreviousRequestUncompleted);
			return;
		}
		setPreviousRequestCompleted(false);
		
		LogD("purchaseProduct:" + productId);

		if (!isServiceValid()) {
			setPreviousRequestCompleted(true);
			nativeFinishTransaction(new TransactionInfo(productId), false, kErrorServiceInvalid);
			return;
		}
		
		if (null != mCurrentAdapter) {
			mCurrentAdapter.purchaseProduct(productId);
		}
	}

	public static boolean isServiceValid() {
		LogD("isServiceVaild");
		if (null != mCurrentAdapter) {
			return mCurrentAdapter.isServiceValid();
		}
		return false;
	}
	
//	public static void notifyServiceInvaild() {
//		LogD("notifyServiceInvaild");
//		if (null != mCurrentAdapter){
//			if (enabled()) mCurrentAdapter.notifyServiceInvalid();
//		}
//	}
	
	public static void notifyIAPToExit() {
		LogD("notifyIAPToExit");
		
		if (null != mCurrentAdapter){
			mCurrentAdapter.notifyIAPToExit();
		}
	}

	public static void loadProducts(String[] productIds) {
		if (!isPreviousRequestCompleted()) {
			LogD("loadProduct, PreviousRequestUncompleted" + productIds);
			//nativeFinishLoadProducts(null, false, kErrorPreviousRequestUncompleted);
			return;
		}
		setPreviousRequestCompleted(false);
		
		LogD("loadProducts" + productIds);
		
		if (null == mCurrentAdapter) {
			setPreviousRequestCompleted(true);
			nativeFinishLoadProducts(productIds, false, kErrorServiceInvalid);
			return;
		}
		
		mCurrentAdapter.loadProducts(productIds);
	}
	
	public static void finishLogon(final boolean isSucceed, final int errorCode) {
		LogD("finishLogon");

		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	setPreviousRequestCompleted(true);
	   	     		if (null == mCurrentAdapter) {
	   	     			LogD("finishLogon, kErrorServiceInvalid!");
	   	     			nativeFinishLogon(false, kErrorServiceInvalid);
	   	     		}
	   	     		else {
	   	     			nativeFinishLogon(isSucceed, errorCode);
	   	     		}
   	            }
		});
	}
	
	public static void finishLoadProducts(final String[] products, final boolean isSucceed, final int errorCode) {
		LogD("finishLoadProducts:" + products + ";isSucceed:" + isSucceed + ";errorCode:"+errorCode);
		
		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	setPreviousRequestCompleted(true);
   	            	if (null == mCurrentAdapter) {
   	            		LogD("finishLoadProducts, kErrorServiceInvalid");
   	            		nativeFinishLoadProducts(products, false, kErrorServiceInvalid);
   	            	}
   	            	else {
   	            		nativeFinishLoadProducts(products, isSucceed, errorCode);
   	            	}
   	            }
		});
	}
	
	public static void finishTransaction(final TransactionInfo info, final boolean isSucceed, final int errorCode) {
		LogD("finishTransaction:" + info.mProductId + ";isSucceed:" + isSucceed + ";errorCode:"+errorCode);

		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	setPreviousRequestCompleted(true);
   	            	if (null == mCurrentAdapter) {
   	            		LogD("finishTransaction, kErrorServiceInvalid");
   	            		nativeFinishTransaction(info, false, kErrorServiceInvalid);
   	            	}
   	            	else {
   	            		nativeFinishTransaction(info, isSucceed, errorCode);
   	            	}
   	            }
		});
	}
	
	public static void notifyGameExit() {
		LogD("notifyGameExit");
        nativeNotifyGameExit();
	}
}
