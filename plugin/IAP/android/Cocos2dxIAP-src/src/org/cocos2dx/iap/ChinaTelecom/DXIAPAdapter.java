package org.cocos2dx.iap.ChinaTelecom;

import org.cocos2dx.iap.IAPProducts;
import org.cocos2dx.iap.IAPWrapper;
import org.cocos2dx.iap.R;
import org.cocos2dx.iap.Wrapper;

import cn.game189.sms.SMSListener;

import com.egamefei.sdk.control.AiDouListener;
import com.egamefei.sdk.control.EGameFei;

import android.widget.Toast;

public class DXIAPAdapter implements org.cocos2dx.iap.IAPWrapper.IAPAdapter {
	
	private static DXIAPAdapter mAdapter = null;
	
	private static String mIMSI;
	private static String mProductIdentifier;

    private static void LogD(String msg) {
		Wrapper.LogD("ChinaTelecom-IAPAdapter", msg);
	}
    
    public static DXIAPAdapter getInstance() {
    	return mAdapter;
    }

    public static void initialize(String cFromer) {
		mAdapter = new DXIAPAdapter();
		
		// 初始化SDK，第二个参数是电信提供的渠道来源 
		EGameFei.init(Wrapper.getActivity(), cFromer); //cjh 渠道来源必须由配置文件中读取
		
		// 爱豆支付的回调
		EGameFei.setAidouListener(new AiDouListener() {
			
			// resultCode结果代码 0成功,1失败,message结果提示语,toolKey道具id
		    @Override
			public void onResult(int resultCode, String message, String toolKey) {		    	
		    	switch (resultCode)
		    	{
		    	case 0:
		    		// 成功 
		    		IAPWrapper.didCompleteTransaction(mProductIdentifier);
		    		break;
		    	case 1:
		    	default:
		    		// 失败 
		    		IAPWrapper.didFailedTransaction(mProductIdentifier);
		    		break;
		    	}
		    }
		});
		
		EGameFei.setSmsListener(new SMSListener() {
			@Override
			public void smsOK(String feeName, String toolKey) {
				//feeName 计费点标识,toolKey 道具id
				IAPWrapper.didCompleteTransaction(mProductIdentifier);
			}
	
			@Override
			public void smsFail(String feeName, int errorCode, String toolKey)  {
				//feeName 计费点标识,errorCode 错误码,toolKey 道具id
				IAPWrapper.didFailedTransaction(mProductIdentifier);
			}
	
			@Override
			public void smsCancel(String feeName, int errorCode, String toolKey)  {
				//feeName 计费点标识,errorCode 错误码,toolKey 道具id
				IAPWrapper.didFailedTransaction(mProductIdentifier);
			}
		});

		mIMSI = Wrapper.getImsiNumber();
	}

	@Override
	public boolean isLogin() {
		return true;
	}

	@Override
	public void loginAsync() {
		// isLogin一直返回true， 不应该调用到这里
		IAPWrapper.didLoginFailed();
	}

	@Override
	public void networkUnReachableNotify() {
		LogD("networkUnReachableNotify");
		Wrapper.postEventToMainThread(new Runnable() {
            @Override
            public void run() {
            	Toast.makeText(Wrapper.getActivity(), R.string.strNetworkUnReachable, Toast.LENGTH_SHORT).show();
        	}
        });
	}

	@Override
	public void requestProductData(String product) {
		LogD("requestProductData : " + product);

		final String productId = product;
		Wrapper.postEventToGLThread(new Runnable() {
            @Override
            public void run() {
        		IAPWrapper.didReceivedProducts(productId);
        	}
        });
	}
 

	@Override
	public void addPayment(String productIdentifier) {
		LogD("addPayment : " + productIdentifier);
		mProductIdentifier = productIdentifier;
//cjh		final String smsKey = IAPProducts.getProductDXSMSKey(mProductIdentifier);
		
//cjh		if (smsKey == null || smsKey.length() == 0) {
//			IAPWrapper.didFailedTransaction(mProductIdentifier);
//			return;
//		}
		
		Wrapper.postEventToMainThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				// 调用电信支付接口 
//cjh				EGameFei.pay(smsKey);
				EGameFei.pay("43484");
			}
		});

	}

	@Override
	public boolean networkReachable() {
		// 判断是否可发短信	
		boolean ret = false;
		do {
			// 检查IMSI
			if (null == mIMSI)
				break;
//cjh			if (! mIMSI.startsWith("46003"))
//				break;
			ret = true;
		} while (false);
		return ret;
	}

	@Override
	public String getAdapterName() {
		return "DianXin";
	}

	
}


