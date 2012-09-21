package org.cocos2dx.iap.UserCenter;

import org.cocos2dx.iap.IAPProducts;
import org.cocos2dx.iap.IAPWrapper;
import org.cocos2dx.iap.TransactionInfo;
import org.cocos2dx.iap.Wrapper;

import com.chukong.usercenter.InitHelper;
import com.chukong.usercenter.ProductInfo;
import com.chukong.usercenter.ResultFlag;
import com.chukong.usercenter.SingleGamePay;
import android.os.Handler;

public class UserCenterIAPAdapter implements org.cocos2dx.iap.IAPAdapter {
	
	private static UserCenterIAPAdapter mAdapter = null;
	private static String mAppKey = null;
	private static String mSecretKey = null;
	
    private static void LogD(String msg) {
		Wrapper.LogD("UserCenter-IAPAdapter", msg);
	}
    
    private static String mProductIdentifier = null;
    private static SingleGamePay mSingleGamePay = null;

    private static Handler mInitResultHandler = null;
    private static Handler mPayResultHandler = null;

	public static boolean initialize(String appKey, String secretKey) {
		boolean ret = false;
		mAppKey = appKey;
		mSecretKey = secretKey;
		
		Wrapper.postEventToMainThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					mAdapter = new UserCenterIAPAdapter();
					mInitResultHandler = new Handler() {
				    	public void handleMessage(android.os.Message msg) {
				    		switch (msg.what) {
				    		case ResultFlag.INIT_SUCCESS:
				    			mSingleGamePay = new SingleGamePay(Wrapper.getActivity());
				    			break;
				    		case ResultFlag.INIT_FAILED:
				    			LogD("Init UserCenter Sdk fails.");
				    		default:
				    			break;
				    		}
				    	}
					};
					
					mPayResultHandler = new Handler() {
				    	public void handleMessage(android.os.Message msg) {
				    		switch (msg.what) {
				    		case ResultFlag.RQF_PAY_SUCCEED:
				    			IAPWrapper.finishTransaction(new TransactionInfo(mProductIdentifier), true, IAPWrapper.kErrorNone);
				    			break;
				    		case ResultFlag.RQF_PAY_CANCLE:
				    			IAPWrapper.finishTransaction(new TransactionInfo(mProductIdentifier), false, IAPWrapper.kErrorUserCancelled);
				    			break;
				    		case ResultFlag.RQF_PAY_FAILED:
				    		case ResultFlag.RQF_PAY_KEEP:
				    		default:
				    			IAPWrapper.finishTransaction(new TransactionInfo(mProductIdentifier), false, IAPWrapper.kErrorPurchaseFailed);
				    			break;
				    		}
				    	}	
					};
					
					InitHelper init = new InitHelper(Wrapper.getActivity());
					init.initSDK(mAppKey, mSecretKey, mInitResultHandler);
					IAPWrapper.setIAPAdapter(mAdapter);
					
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
				
		ret = true;
		return ret;
	}

//	@Override
//	public void notifyServiceInvalid() {
//		LogD("notifyServiceInvalid");
//		Wrapper.postEventToMainThread(new Runnable() {
//            @Override
//            public void run() {
//            	String tip = Wrapper.getActivity().getResources().getString(R.string.ccxiap_strNetworkUnReachable);
//            	String imsi = Wrapper.getImsiNumber();
//            	if (null != imsi && imsi.startsWith("46001")) {
//            		// 是中国联通用户 
//            		tip += Wrapper.getActivity().getResources().getString(R.string.ccxiap_strUnicomTip);
//            	}
//            	Toast.makeText(Wrapper.getActivity(), tip, Toast.LENGTH_SHORT).show();
//        	}
//        });
//	}

	public void notifyIAPToExit() {
		IAPWrapper.notifyGameExit();
	}
	
	@Override
	public void loadProducts(String[] productIds) {
		LogD("loadProducts : " + productIds);
        IAPWrapper.finishLoadProducts(productIds, true, IAPWrapper.kErrorNone);
	}
 

	@Override
	public void purchaseProduct(String productIdentifier) {
		LogD("purchaseProduct : " + productIdentifier);

		mProductIdentifier = productIdentifier;

		final float fPrice = IAPProducts.getProductPrice(mProductIdentifier);
		if (0.0f == fPrice) {
			IAPWrapper.finishTransaction(new TransactionInfo(mProductIdentifier), false, IAPWrapper.kErrorProductPriceInvalid);
			return;
		}
		
		final String productName = IAPProducts.getProductName(mProductIdentifier);

		Wrapper.postEventToMainThread(new Runnable() {
            @Override
            public void run() {
				//ProductInfo productInfo = new ProductInfo("600", "0.01", "600金币");
            	// LogD("product price = " + fPrice + ",name="+IAPProducts.getProductName(mProductIdentifier));
            	ProductInfo productInfo = new ProductInfo("", ""+fPrice, productName);
            	if (mSingleGamePay != null) {
            		mSingleGamePay.startNologinPay(productInfo, mPayResultHandler);
            	}
            	else {
            		IAPWrapper.finishTransaction(new TransactionInfo(mProductIdentifier), false, IAPWrapper.kErrorServiceInvalid);
            	}
            }
		});
	}

	@Override
	public boolean isServiceValid() {
		boolean ret = false;
		do {
			// 检查IMSI
			if (null == mSingleGamePay)
			{
				Wrapper.postEventToMainThread(new Runnable() {
					@Override
					public void run() {
						InitHelper init = new InitHelper(Wrapper.getActivity());
						init.initSDK(mAppKey, mSecretKey, mInitResultHandler);
					}
				});
				break;
			}
			
			ret = true;
		} while (false);
		LogD("service return:"+ret);
		return ret;
	}

	@Override
	public String getAdapterName() {
		return "UserCenter";
	}
}


