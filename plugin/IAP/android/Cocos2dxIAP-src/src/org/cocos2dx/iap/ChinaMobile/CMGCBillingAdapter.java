package org.cocos2dx.iap.ChinaMobile;


import org.cocos2dx.iap.IAPWrapper;
import org.cocos2dx.iap.IAPWrapper.IAPAdapter;
import org.cocos2dx.iap.R;
import org.cocos2dx.iap.Wrapper;

import cn.emagsoftware.gamebilling.api.GameInterface;
import cn.emagsoftware.gamebilling.api.GameInterface.BillingCallback;
import cn.emagsoftware.gamebilling.view.BillingView;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.Toast;

public class CMGCBillingAdapter implements IAPAdapter{

    
	private static CMGCBillingAdapter mAdapter = null;
	
//	private static SignalStrengthListener signalStrengthListener = null;
//	private static int singleStrength = 0;
	
//	public static void setSingleStrength(int singleStrengthValue){
//		singleStrength = singleStrengthValue;
//	}
	
	private static void LogD(String msg) {
		Wrapper.LogD("ChinaMobile-IAPAdapter", msg);
	}
	
	private static String mProductIdentifier;
	public static String getCurrentProductId() { return mProductIdentifier; }
	
	public static CMGCBillingAdapter getInstance() {
    	return mAdapter;
    }
	
	public static void initialize(String appName, String companyName, String telNumber) {
		mAdapter = new CMGCBillingAdapter();
        
		try {
			GameInterface.initializeApp(Wrapper.getActivity(), appName, companyName, telNumber);
		}catch (Exception e) {
			e.printStackTrace();
		}
//		signalStrengthListener = new SignalStrengthListener();
//		TelephonyManager phoneMgr = (TelephonyManager)Wrapper.getActivity().getSystemService(Context.TELEPHONY_SERVICE);
//		phoneMgr.listen(signalStrengthListener, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);
	}

	@Override
	public boolean isLogin() {
		// 现在还不需要Login
		return true;
		// 获取本地激活标志，尽可能的防止盗版，绑定IMEI
		//TelephonyManager tm = (TelephonyManager)Wrapper.getActivity().getSystemService(Context.TELEPHONY_SERVICE);
		
		//final String key = "activateflag"+tm.getDeviceId();
		//final SharedPreferences sp = Wrapper.getActivity().getPreferences(Context.MODE_PRIVATE);
		//boolean activate = sp.getBoolean(key, false);
		//return activate;
	}

	@Override
	public void loginAsync() {
		LogD("loginAsync needn't be called!!!");
	}

	@Override
	public void networkUnReachableNotify() {
		
		Wrapper.postEventToMainThread(new Runnable() {
            @Override
            public void run() {
            	Toast.makeText(Wrapper.getActivity(), R.string.ccxiap_strSimUnavailable, Toast.LENGTH_SHORT).show();
        	}
        });
    }

	@Override
	public void requestProductData(String product) {
		IAPWrapper.didReceivedProducts(product);
	}

	@Override
	public void addPayment(String productIdentifier) {
		LogD("addPayment" + productIdentifier);
		if (null == productIdentifier || null == Wrapper.getActivity()) {
			IAPWrapper.didFailedTransaction(productIdentifier);
			return;
		}
		mProductIdentifier = productIdentifier;
		Wrapper.postEventToMainThread(new Runnable() {
            @Override
            public void run() {
//            	Wrapper.getActivity().startActivity(new Intent(Wrapper.getActivity(), CMGCBillingActivity.class));
        		BillingCallback callback = new GameInterface.BillingCallback() {
        			@Override
        			public void onUserOperCancel() {
        				//用户取消计费流程
        				if (null == mProductIdentifier) return;
        				LogD("onUserOperCancel" + mProductIdentifier);
        				Wrapper.postEventToGLThread(new Runnable() {
							
							@Override
							public void run() {
								// TODO Auto-generated method stub
		        				IAPWrapper.didFailedTransaction(mProductIdentifier);
		        				mProductIdentifier = null;	
							}
						});

        				
        				Toast.makeText(Wrapper.getActivity(), "Callback when user cancel billing.", Toast.LENGTH_SHORT).show();
        			}
        			
        			@Override
        			public void onBillingSuccess() {
        				//计费成功
        				if (null == mProductIdentifier) return;
        				LogD("onBillingSuccess" + mProductIdentifier);
        				Wrapper.postEventToGLThread(new Runnable() {
							
							@Override
							public void run() {
								// TODO Auto-generated method stub
		        				IAPWrapper.didCompleteTransaction(mProductIdentifier);
		        				// 购买成功都用Flurry记录一下
		        			//cjh	Hashtable<String, String> param = new Hashtable<String, String>();
		        			//cjh	param.put(mProductIdentifier, "" + IAPProducts.getProductPrice(mProductIdentifier));
		        			//cjh	FlurryAPIWrapper.logEvent("Payment From CMGC", param);
		        				mProductIdentifier = null;	
							}
						});

        				
        				Toast.makeText(Wrapper.getActivity(), "Callback when billing success.", Toast.LENGTH_SHORT).show();
        			}
        			
        			@Override
        			public void onBillingFail() {
        				if (null == mProductIdentifier) return;
        				LogD("onBillingFail" + mProductIdentifier);
        				Wrapper.postEventToGLThread(new Runnable() {
							
							@Override
							public void run() {
								// TODO Auto-generated method stub
		        				IAPWrapper.didFailedTransaction(mProductIdentifier);
		        				mProductIdentifier = null;
							}
						});
        				
        				Toast.makeText(Wrapper.getActivity(), "Callback when billing fail.", Toast.LENGTH_SHORT).show();
        			}
        		};
        		
        		GameInterface.doBilling(true, true, "000", callback);
            }
        });
		
//		Wrapper.getActivity().startActivity(new Intent(Wrapper.getActivity(), CMGCBillingActivity.class));
		
		LogD("addPayment " + productIdentifier);
	}

	@Override
	public boolean networkReachable() {
		boolean ret = true;
		do {
			if (null == Wrapper.getActivity()) break;
			
			// 检查短信功能是否可用
			TelephonyManager telephonyManager=(TelephonyManager) Wrapper.getActivity().getSystemService(Context.TELEPHONY_SERVICE);
			String imsi = telephonyManager.getSubscriberId();
			if (null != imsi && imsi.length() > 10) break;

			// 都不可用，返回false
			ret = false;
		} while (false);
		return ret;
	}

	@Override
	public String getAdapterName() {
		return "CMGC";
	}
}
