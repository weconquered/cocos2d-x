package org.cocos2dx.iap.CMGC;

import org.cocos2dx.iap.IAPWrapper;
import org.cocos2dx.iap.IAPProducts;
import org.cocos2dx.iap.Wrapper;
import cn.emagsoftware.gamebilling.api.GameInterface;
import cn.emagsoftware.gamebilling.api.GameInterface.BillingCallback;
import cn.emagsoftware.gamebilling.api.GameInterface.GameExitCallback;

public class CMGCIAPAdapter implements org.cocos2dx.iap.IAPAdapter{

	private static CMGCIAPAdapter mAdapter = null;

	private static void LogD(String msg) {
		Wrapper.LogD("ChinaMobile-IAPAdapter", msg);
	}
	
	private static String mProductIdentifier;
	public static String getCurrentProductId() { return mProductIdentifier; }
	
	public static CMGCIAPAdapter getInstance() {
    	return mAdapter;
    }
	
	public static boolean initialize(String appName, String companyName, String telNumber) {
		boolean ret = false;
		final String strAppName = appName;
		final String strCompanyName = companyName;
		final String strTelNumber = telNumber;
		Wrapper.postEventToMainThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					mAdapter = new CMGCIAPAdapter();
					GameInterface.initializeApp(Wrapper.getActivity(), strAppName, strCompanyName, strTelNumber);
					IAPWrapper.setIAPAdapter(mAdapter);
				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		ret = true;
		return ret;
	}

	@Override
	public void loadProduct(String product) {
		LogD("loadProduct : " + product);
		final String[] productIds = {product};
        IAPWrapper.finishLoadProducts(productIds, true, IAPWrapper.kErrorNone);
	}
	
	@Override
	public void purchaseProduct(String productIdentifier) {
		LogD("purchaseProduct:" + productIdentifier);
		if (null == productIdentifier || null == Wrapper.getActivity()) {
			IAPWrapper.finishTransaction(productIdentifier, false, IAPWrapper.kErrorProductIdInvalid);
			return;
		}
		mProductIdentifier = productIdentifier;
		Wrapper.postEventToMainThread(new Runnable() {
            @Override
            public void run() {
        		BillingCallback callback = new GameInterface.BillingCallback() {
        			@Override
        			public void onUserOperCancel() {
        				// User cancels operation.
        				if (null == mProductIdentifier) {
        					IAPWrapper.finishTransaction(mProductIdentifier, false, IAPWrapper.kErrorProductIdInvalid);
        					return;
        				}
        				LogD("onUserOperCancel" + mProductIdentifier);
        				
        				IAPWrapper.finishTransaction(mProductIdentifier, false, IAPWrapper.kErrorUserCancelled);
        				mProductIdentifier = null;	
        			}
        			
        			@Override
        			public void onBillingSuccess() {
        				// Purchase succeed.
        				if (null == mProductIdentifier) return;
        				LogD("onBillingSuccess" + mProductIdentifier);
        				IAPWrapper.finishTransaction(mProductIdentifier, true, IAPWrapper.kErrorNone);
		        		mProductIdentifier = null;
        			}
        			
        			@Override
        			public void onBillingFail() {
        				// Purchase fails.
        				if (null == mProductIdentifier) return;
        				LogD("onBillingFail" + mProductIdentifier);
        				IAPWrapper.finishTransaction(mProductIdentifier, false, IAPWrapper.kErrorPurchaseFailed);
		        		mProductIdentifier = null;
        			}
        		};
        		LogD("before doBilling");
        		GameInterface.doBilling(true, true, IAPProducts.getProductInfoByKey(mProductIdentifier, "CMGCSMSKey"), callback);
        		LogD("after doBilling");
            }
        });
	}

	@Override
	public boolean isServiceValid() {
		boolean ret = false;
		do {
			if (null == Wrapper.getActivity()) break;
			String imsi = Wrapper.getImsiNumber();
			// Check whether sms works.
			if (null == imsi || imsi.length() <= 10) break;

			if(imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007")) {
				ret = true;
			}
		} while (false);
		return ret;
	}

	public void notifyIAPToExit() {

			GameInterface.exit(new GameExitCallback() {
				@Override
				public void onConfirmExit() {
					Wrapper.postEventToMainThread(new Runnable() {
						@Override
						public void run() {
							IAPWrapper.notifyGameExit();
						}
					});
				}

				@Override
				public void onCancelExit() {
				}
			}); 
	}
	
	@Override
	public String getAdapterName() {
		return "CMGC";
	}
}
