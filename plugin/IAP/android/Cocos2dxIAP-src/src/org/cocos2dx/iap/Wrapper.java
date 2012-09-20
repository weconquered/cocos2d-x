package org.cocos2dx.iap;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Log;

public class Wrapper {
	private static boolean bDebug = false;
	public static boolean getOutputLogEnable() { return bDebug; }
	public static void setOutputLogEnable(boolean enable) {	bDebug = enable;}
	public static void LogD(String tag, String msg, boolean bFource) { if (bFource || bDebug) Log.d(tag, msg); }
	public static void LogD(String tag, String msg) { if (bDebug) Log.d(tag, msg); }
	public static void LogD(String msg) { LogD("Wrapper", msg); }

	static protected boolean mIsStartGame	 = true;
	
	private static Activity mActivity = null;
	public static Activity getActivity() {
		if (null == mActivity) LogD("getActivity null!!!");
		return mActivity;
	}
	
	private static GLSurfaceView mGLSurfaceView = null;
	
	public static void postEventToGLThread(Runnable r) {
		if (null == mGLSurfaceView) LogD("getCocos2dxGLSurfaceView null!!!");
		mGLSurfaceView.queueEvent(r);
	}
	
	public static void postEventToMainThread(Runnable r) {
		if (null == mHandler) LogD("getUIHandler null !!!");
		mHandler.post(r);
	}
	
	private static Handler mHandler = null;
	
	public static void initialize(Activity activity, GLSurfaceView view) {
		mActivity = activity;
		mGLSurfaceView = view;

		if (null == mHandler) {
			mHandler = new Handler();
		}
		
		if (mIsStartGame) {
			mIsStartGame = false;
		}
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Wrapper functions
	
	public static boolean isNetworkAvailable() {
		if (null == mActivity) {
			LogD("networkAvailable han't find tCocos2dxActivity yet!!!");
			return false;
		}
		try {
			ConnectivityManager conn = (ConnectivityManager)mActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo netInfo = conn.getActiveNetworkInfo();
			if (null != netInfo && netInfo.isAvailable()){
				return true;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		LogD("networkAvailable return false!");
		return false;
	}
	
	public static String getVersionName() {
		String strVerName = null;
		try {
			String packageName = mActivity.getPackageName();
			strVerName = mActivity.getPackageManager().getPackageInfo(packageName, 0).versionName;
		} catch (Exception e) {
		}
		LogD("getVersionName return " + strVerName);
		return strVerName;
	}
	
	public static int getVersionCode() {
		int verCode = 0;
		try {
			String packageName = mActivity.getPackageName();
			verCode = mActivity.getPackageManager().getPackageInfo(packageName, 0).versionCode;
		} catch (Exception e) {
		}
		LogD("getVersionCode return " + verCode);
		return verCode;
	}
	
	public static String getLocalMacAddress() {
		String macAdd = null;
		try {
	        WifiManager wifi = (WifiManager) Wrapper.getActivity().getSystemService(Context.WIFI_SERVICE);   
	        WifiInfo info = wifi.getConnectionInfo();
	        macAdd = info.getMacAddress();
		} catch (Exception e) {
			e.printStackTrace();
		}

        return macAdd;
    }
	
	public static String getPhoneNumber() {
		String phoneNumber = null;
		
		try {
			TelephonyManager phoneMgr = (TelephonyManager) Wrapper.getActivity().getSystemService(Context.TELEPHONY_SERVICE);
			phoneNumber = phoneMgr.getLine1Number();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return phoneNumber;
	}
	
	public static String getImsiNumber() {
		String imsiNumber = null;
		
		try {
			TelephonyManager phoneMgr = (TelephonyManager) Wrapper.getActivity().getSystemService(Context.TELEPHONY_SERVICE);
			imsiNumber = phoneMgr.getSubscriberId();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return imsiNumber;
	}

}
