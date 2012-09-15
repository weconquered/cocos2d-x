package org.cocos2dx.iap.ChinaMobile;

import java.util.Hashtable;

import org.cocos2dx.helloiap.R;
import org.cocos2dx.iap.IAPProducts;
import org.cocos2dx.iap.IAPWrapper;
import org.cocos2dx.iap.IAPWrapper.IAPAdapter;
import org.cocos2dx.iap.Wrapper;

import cn.emagsoftware.gamebilling.api.GameInterface;
import cn.emagsoftware.gamebilling.view.BillingView;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
//import android.telephony.PhoneStateListener;
//import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.Toast;

//class SignalStrengthListener extends PhoneStateListener{  
//    @Override  
//    public void onSignalStrengthsChanged(SignalStrength signalStrength){  
//    	CMGCBillingAdapter.setSingleStrength(signalStrength.getGsmSignalStrength());
//    	super.onSignalStrengthsChanged(signalStrength);  
//    }  
//}

public class CMGCBillingAdapter implements IAPAdapter{

    
	private static CMGCBillingAdapter mAdapter = null;
	
//	private static SignalStrengthListener signalStrengthListener = null;
//	private static int singleStrength = 0;
	
//	public static void setSingleStrength(int singleStrengthValue){
//		singleStrength = singleStrengthValue;
//	}
	
	private static final boolean mDebug = false;
	private static void LogD(String msg) {
		if (mDebug) Log.d("CMGCBillingAdapter", msg);
	}
	
	private static String mProductIdentifier;
	public static String getCurrentProductId() { return mProductIdentifier; }
	
	public static CMGCBillingAdapter getInstance() {
    	return mAdapter;
    }
	
	public static void initialize() {
		mAdapter = new CMGCBillingAdapter();
        
		// cjh replace company name
		try {
			GameInterface.initializeApp(Wrapper.getActivity(), Wrapper.getActivity().getResources().getString(R.string.app_name), "Please replace me", "000-0000000");
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
		if(null == Wrapper.getUIHandler()) return;
		
		Wrapper.getCocos2dxGLSurfaceView().post(new Runnable() {
            @Override
            public void run() {
            	Toast.makeText(Wrapper.getActivity(), R.string.strSimUnavailable, Toast.LENGTH_SHORT).show();
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
		Wrapper.getCocos2dxGLSurfaceView().post(new Runnable() {
            @Override
            public void run() {
            	Wrapper.getActivity().startActivity(new Intent(Wrapper.getActivity(), CMGCBillingActivity.class));
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

	///////////////////////////////////////////////////////////////////////////
	// 实现  由 CMGCBillingActivity 回调的接口
	///////////////////////////////////////////////////////////////////////////

	static public boolean bHandled = false;
	static public void onBillingFinish() {
		//计费流程结束
		// mProductIdentifier = null;
		LogD("onBillingFinish" + mProductIdentifier);
	}

	static public void onBillingSuccess() {
		//计费成功
		if (null == mProductIdentifier) return;
		LogD("onBillingSuccess" + mProductIdentifier);
		IAPWrapper.didCompleteTransaction(mProductIdentifier);
		// 购买成功都用Flurry记录一下
		Hashtable<String, String> param = new Hashtable<String, String>();
		param.put(mProductIdentifier, "" + IAPProducts.getProductPrice(mProductIdentifier));
	//cjh	FlurryAPIWrapper.logEvent("Payment From CMGC", param);
		mProductIdentifier = null;
		bHandled = true;
	}

	static public void onUserOperCancel() {
		//用户取消计费流程
		if (null == mProductIdentifier) return;
		LogD("onUserOperCancel" + mProductIdentifier);
		IAPWrapper.didFailedTransaction(mProductIdentifier);
		mProductIdentifier = null;
		bHandled = true;
	}

	static public void onUserOperError(int errCode) {
		Hashtable<String, String> param = new Hashtable<String, String>();
		param.put(mProductIdentifier, "" + IAPProducts.getProductPrice(mProductIdentifier));

		//需要实现错误捕捉，具体errCode请参考文档
		switch(errCode){
		case BillingView.ERROR_SMS_SEND_FAILURE:
			bHandled = false;
			break;
		case BillingView.ERROR_WEB_NETWORK_ERROR:
			bHandled = true;
			IAPWrapper.didFailedTransaction(mProductIdentifier);
			mProductIdentifier = null;
			Toast.makeText(Wrapper.getActivity(), R.string.strNetworkUnReachable, Toast.LENGTH_SHORT).show();
			
			param.put("reason", "ERROR_WEB_NETWORK_ERROR");
			break;
		case BillingView.ERROR_BILLING_FAILURE:
			bHandled = true;
			IAPWrapper.didFailedTransaction(mProductIdentifier);
			mProductIdentifier = null;
			Toast.makeText(Wrapper.getActivity(), R.string.strSendConfirmSMSFailed, Toast.LENGTH_SHORT).show();
			
			param.put("reason", "ERROR_BILLING_FAILURE");
			break;
		default:
			bHandled = false;
			
			param.put("reason", "default case");
			break;
		}
		
		LogD("onUserOperError" + mProductIdentifier + errCode);
	//cjh	FlurryAPIWrapper.logEvent("onUserOperError", param);
	}
	
	static public void onActivityDestroy() {
		LogD("onActivityDestroy" + bHandled + CMGCBillingActivity.okBtnClicked);
		if (! bHandled){
			if (CMGCBillingActivity.okBtnClicked) {
				handleCheck();
			} else
			if (mProductIdentifier != null)
			{
				IAPWrapper.didFailedTransaction(mProductIdentifier);
			}
		}
		
		// reset
		bHandled = false;
	}
	
	static public void handleCheck() {
		if (mProductIdentifier == null) {
			return;
		}
		
		// 发送短信失败，如果没有明显的作弊嫌疑，当作成功
		ContentResolver cr = Wrapper.getActivity().getContentResolver();
		if ((! mProductIdentifier.equals(IAPWrapper.PRODUCT_ACTIVATE)) ||
			Settings.System.getInt(cr, Settings.System.AIRPLANE_MODE_ON, 0) == 1/* ||
			singleStrength > 40*/) {
			Toast.makeText(Wrapper.getActivity(), R.string.strSendSMSFailed, Toast.LENGTH_SHORT).show();
			IAPWrapper.didFailedTransaction(mProductIdentifier);
			mProductIdentifier = null;
		}
		else
		{
			LogD("handleCheck" + "bill Success!");
			onBillingSuccess();
		
			// 这个情况要记录一下，以便查询
			Hashtable<String, String> param = new Hashtable<String, String>();
			param.put(mProductIdentifier, "" + IAPProducts.getProductPrice(mProductIdentifier));
//			param.put("singleStrength", "" + singleStrength);
//cjh			FlurryAPIWrapper.logEvent("payment sucessfull though sms failed", param);
		}
		
		return;
	}

	@Override
	public String getAdapterName() {
		return "CMGC";
	}
}
