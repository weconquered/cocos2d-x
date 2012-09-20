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
	
	private static void LogD(String msg) {
		Wrapper.LogD("IAPWrapper", msg);
	}
	
	////////////////////////////////////////////////////////////////////////
	// IAPWrapper
	private static native void nativeFinishLogon(boolean isSucceed, int errorCode);
	private static native void nativeFinishLoadProducts(String[] products, boolean isSucceed, int errorCode);
	private static native void nativeFinishTransaction(String productId, boolean isSucceed, int errorCode);
	private static native void nativeNotifyGameExit();


	public static boolean enabled() {
		return true;
	}

	private static IAPAdapter mCurrentAdapter = null;

	public static void setIAPAdapter(IAPAdapter adapter) {
		mCurrentAdapter = adapter;
	}
	
	public static void purchaseProduct(String productIdentifier) {
		LogD("purchaseProduct:" + productIdentifier);

		if (!isServiceValid()) {
			nativeFinishTransaction(productIdentifier, false, kErrorServiceInvalid);
			return;
		}
		
		if (null != mCurrentAdapter) {
			if (enabled()) {
				mCurrentAdapter.purchaseProduct(productIdentifier);
			}
		}
	}

	public static boolean isServiceValid() {
		LogD("isServiceVaild");
		if (null != mCurrentAdapter) {
			if (enabled()) {
				return mCurrentAdapter.isServiceValid();
			}
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
			if (enabled()) mCurrentAdapter.notifyIAPToExit();
		}
	}

	public static void loadProduct(String product) {
		
		LogD("loadProduct" + product);
		String[] productIds = {product};
		
		if (null == mCurrentAdapter) {
			nativeFinishLoadProducts(productIds, false, kErrorServiceInvalid);
			return;
		}
		
		if (false == enabled()) return;
		
		mCurrentAdapter.loadProduct(product);
	}
	
	public static void finishLogon(final boolean isSucceed, final int errorCode) {
		LogD("finishLogon");
		if (null == mCurrentAdapter) return;
		if (false == enabled()) return;
		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	nativeFinishLogon(isSucceed, errorCode);
   	            }
		});
	}
	
	public static void finishLoadProducts(final String[] products, final boolean isSucceed, final int errorCode) {
		LogD("finishLoadProducts:" + products + ";isSucceed:" + isSucceed + ";errorCode:"+errorCode);
		if (null == mCurrentAdapter) return;
		if (false == enabled()) return;
		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	nativeFinishLoadProducts(products, isSucceed, errorCode);
   	            }
		});
	}
	
	public static void finishTransaction(final String productIdentifier, final boolean isSucceed, final int errorCode) {
		LogD("finishTransaction:" + productIdentifier + ";isSucceed:" + isSucceed + ";errorCode:"+errorCode);
		if (false == enabled()) return;
		Wrapper.postEventToGLThread(new Runnable() {
   	            @Override
   	            public void run() {
   	            	LogD("GL finishTransaction:" + productIdentifier + ";isSucceed:" + isSucceed + ";errorCode:"+errorCode);
   	            	nativeFinishTransaction(productIdentifier, isSucceed, errorCode);
   	            }
		});
	}
	
	public static void notifyGameExit() {
		LogD("notifyGameExit");
        nativeNotifyGameExit();
	}
}
