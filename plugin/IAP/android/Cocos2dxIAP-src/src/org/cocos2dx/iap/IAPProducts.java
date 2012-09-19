package org.cocos2dx.iap;

import java.util.Hashtable;

import org.json.JSONObject;

import android.util.Log;

public class IAPProducts {

	private static Hashtable<String, JSONObject> mProducts = null;

	private static JSONObject getProductInfo(String productIdentifier) {
		if (mProducts == null) {
			mProducts = new Hashtable<String, JSONObject>();
		}
		JSONObject ret = mProducts.get(productIdentifier);
		
		if (null == ret) {
			try {
				String strInfo = IAPWrapper.nativeGetProductInfo(productIdentifier);
				ret = new JSONObject(strInfo);
			} catch(Exception e) {
				e.printStackTrace();
			}
			mProducts.put(productIdentifier, ret);
		}
		
		return ret;
	}

	public static float getProductPrice(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		float fRet = 0.0f;
		try {
			String value = productInfo.getString("productPrice");
			fRet = Float.parseFloat(value);
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "productPrice is wrong!");
		}

		return fRet;
	}
	/*
	public static int getProductCoinNum(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		int iRet = 0;
		try {
			String value = productInfo.getString("goodNum");
			iRet = Integer.parseInt(value);
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "goodNum is wrong!");
		}

		return iRet;
	}
	*/
	/*
	public static String getProductGid(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		String strRet = "";
		try {
			strRet = productInfo.getString("gid");
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "gid is wrong!");
		}

		return strRet;
	}
	*/
	public static String getProductDXSMSKey(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		String strRet = "";
		try {
			strRet = productInfo.getString("DXSMSKey");
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "DXSMSKey is wrong!");
		}

		return strRet;
	}
	
	public static String getProductCMGCSMSKey(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		String strRet = "";
		try {
			strRet = productInfo.getString("CMGCSMSKey");
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "CMGCSMSKey is wrong!");
		}

		return strRet;
	}

	public static String getProductInfoByKey(String productIdentifier, String key) {
		JSONObject productInfo = getProductInfo(productIdentifier);
		
		String strRet = "";
		try {
			strRet = productInfo.getString(key);
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "key is not found!");
		}

		return strRet;
	}
	
	public static String getProductName(String productIdentifier) {
		JSONObject productInfo = getProductInfo(productIdentifier);

		String strRet = "";
		try {
			strRet = productInfo.getString("ProductName");
		} catch (Exception e) {
			e.printStackTrace();
			Log.d("IAPProducts", "product " + productIdentifier + "ProductName is wrong!");
		}

		return strRet;
	}
}
