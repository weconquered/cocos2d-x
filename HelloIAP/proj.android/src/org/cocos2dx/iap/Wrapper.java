package org.cocos2dx.iap;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map.Entry;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Log;

public class Wrapper {
	private static boolean bDebug = true;//cjh false;
	public static boolean getOutputLogEnable() { return bDebug; }
	public static void setOutputLogEnable(boolean enable) {	bDebug = enable;}
	public static void LogD(String tag, String msg, boolean bFource) { if (bFource || bDebug) Log.d(tag, msg); }
	public static void LogD(String tag, String msg) { if (bDebug) Log.d(tag, msg); }
	public static void LogD(String msg) { LogD("Wrapper", msg); }

	// 是否统计数据	
	static protected boolean mIsStartGame	 = true;
	
	private static Activity mActivity = null;
	public static Activity getActivity() {
		if (null == mActivity) LogD("getActivity null!!!");
		return mActivity;
	}
	
	private static Cocos2dxGLSurfaceView mGLSurfaceView = null;
//	public static Cocos2dxGLSurfaceView getCocos2dxGLSurfaceView() {
//		if (null == mGLSurfaceView) LogD("getCocos2dxGLSurfaceView null!!!");
//		return mGLSurfaceView;
//	}
	
	public static void postEventToGLThread(Runnable r) {
		if (null == mGLSurfaceView) LogD("getCocos2dxGLSurfaceView null!!!");
		mGLSurfaceView.queueEvent(r);
	}
	
	public static void postEventToMainThread(Runnable r) {
		if (null == mHandler) LogD("getUIHandler null !!!");
		mHandler.post(r);
	}
	
	private static Handler mHandler;
//	public static Handler getUIHandler() {
//		if (null == mHandler) LogD("getUIHandler null !!!");
//		return mHandler;
//	}
	
	public static void initialize(Activity activity, Cocos2dxGLSurfaceView view) {
		mActivity = activity;
		mGLSurfaceView = view;

		if (null == mHandler) {
			mHandler = new Handler();
		}
		
		if (mIsStartGame) {
			// 是游戏开始	
	        // 检查是否新玩家	
//cjh	        DataStat.checkIsNewUser();
	        
	        // 检查玩家是否有更新	
//cjh	        DataStat.checkIsUpdated();
	        
	        // 上传之前的 session 数据 
//cjh	        DataStat.uploadSessionData();
	        
	        // 更新本地记录的玩家的 session 数据	
	        // 更新玩家当天游戏的次数以及当天首次启动游戏时间	
//cjh	        DataStat.refreshSessionData();

			// 标记为不是游戏开始	
			mIsStartGame = false;
		}
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Wrapper functions
	
	public static boolean networkAvailable() {
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
	
	//////////////////////////////////////////////////////////////////////////
	//UserDefaults
	
	private static final String USER_DEFAULTS_PREFERENCE = "UserDefaultsPreference";
	private static SharedPreferences getUserDefaultPreference() {
		if (null != getActivity()) {
			return mActivity.getSharedPreferences(USER_DEFAULTS_PREFERENCE, 0);
		} else {
			LogD("getUserDefaultPreference return null !");
			return null;
		}
	}

	static final String KEY_GAME_ACTIVATE = "isActivate"; // 特殊的 key，保存游戏是否激活 
	public interface ActivateDataAdapter {
		public void setIsActivated(boolean bValue);
		public boolean getIsActivated();
	}

	static ActivateDataAdapter activateAdapter = null;
	public static void setActivateDataAdapter (ActivateDataAdapter adapter) {
		activateAdapter = adapter;
	}

	// synchronized cache objects
	// cache logic: find object in cache first, if not exist, read from preference and put it in cache.
	// write back data from cache to preference by function saveUserDefault() which called by Cocos2dxRenderer.onDrawFrame
	private static Hashtable<String, Object> userDefaultCache = new Hashtable<String, Object>();
	
	public static void saveUserDefault() {
		LogD("saveUserDefault begin:");
		synchronized (userDefaultCache) {
			SharedPreferences preference = getUserDefaultPreference();
			if (null != preference) {
				SharedPreferences.Editor editor = preference.edit();
				// iterator the userDefaultCache, save all value which modified to preference
				LogD(String.format("userDefaultCache has %d key-value pairs", userDefaultCache.size()));
				int i = 0, j = 0;
				for (Iterator<Entry<String, Object>> it = userDefaultCache.entrySet().iterator(); it.hasNext(); ) {
					++i;
					
					Entry<String, Object> entry = it.next();
					String key = entry.getKey();
					Object obj = entry.getValue();
					if (obj instanceof Boolean) {
						boolean value = ((Boolean)obj).booleanValue();
						if (value != preference.getBoolean(key, false)) {
							editor.putBoolean(key, value); ++j;
						}
					} else
					if (obj instanceof Float) {
						float value = ((Float)obj).floatValue();
						if (value != preference.getFloat(key, 0.0f)) {
							editor.putFloat(key, value); ++j;
						}
					} else
					if (obj instanceof Integer) {
						int value = ((Integer)obj).intValue();
						if (value != preference.getInt(key, 0)) {
							editor.putInt(key, value); ++j;
						}
					} else
					if (obj instanceof String) {
						String value = (String)obj;
						if (! value.equals(preference.getString(key, ""))) {
							editor.putString(key, value); ++j;
						}
					} else {
						LogD("Can't deal with this type:" + obj.getClass().getName());
					}
				}	// for
				LogD(String.format("%d of %d key-value pairs in userDefaultCache has been update to preference.", j, i));
				editor.commit();
			}
		}
		LogD("saveUserDefault end!");
	}
	
	private static Boolean savingLock = new Boolean(false);
	public static void saveUserDefaultAsync() {
		synchronized (savingLock) {
			// if there is no saving task, create one
			if (false == savingLock) {
				savingLock = true;
				LogD("saveUserDefaultAsync");
				Wrapper.postEventToMainThread(new Runnable(){
					@Override
					public void run() {
						new AsyncTask<String, Integer, Long>() {

							@Override
							protected Long doInBackground(String... params) {
								Wrapper.saveUserDefault();
								return new Long(0);
							}
							
							@Override
						    protected void onPostExecute(Long result) {
								savingLock = false;
						        Wrapper.LogD("Success to saveUserDefaultAsync");
						    }
						}.execute();
					}
				});
			}	// if (false == savingLock)
		}	// synchronized
	}
	
	// boolean
	public static boolean getBooleanForKey(String key, boolean defaultValue) {
		if (null != activateAdapter &&
			0 == key.compareTo(KEY_GAME_ACTIVATE))
		{
			return activateAdapter.getIsActivated();
		}
		boolean ret = defaultValue;
		synchronized (userDefaultCache) {
			Object value = userDefaultCache.get(key);
			if (null == value) {
				SharedPreferences preference = getUserDefaultPreference();
				if (null != preference) {
					ret = preference.getBoolean(key, defaultValue);
				}
				userDefaultCache.put(key, Boolean.valueOf(ret));
			} else {
				ret = ((Boolean)value).booleanValue();
			}
		}
		LogD("getBooleanForKey(" + key + "):" + ret);
		return ret;
	}
	
	public static boolean getBooleanForKey(String key) {
		return getBooleanForKey(key, false);
	}
	
	public static void setBooleanForKey(String key, boolean value) {
		if (null != activateAdapter &&
			0 == key.compareTo(KEY_GAME_ACTIVATE))
		{
			activateAdapter.setIsActivated(value);
			return;
		}

		LogD("setBooleanForKey(" + key + "):" + value);
		userDefaultCache.put(key, Boolean.valueOf(value));
	}
	
	// float
	public static float getFloatForKey(String key, float defaultValue) {
		float ret = defaultValue;

		synchronized (userDefaultCache) {
			Object value = userDefaultCache.get(key);
			if (null == value) {
				SharedPreferences preference = getUserDefaultPreference();
				if (null != preference) {
					try {
						ret = preference.getFloat(key, defaultValue);
					} catch (ClassCastException	e) {
						e.printStackTrace();
					}
				}
				userDefaultCache.put(key, Float.valueOf(ret));
			} else {
				ret = ((Float)value).floatValue();
			}
		}
		LogD("getFloatForKey(" + key + "):" + ret);
		return ret;
	}
	
	public static float getFloatForKey(String key) {
		return getFloatForKey(key, 0.0f);
	}
	
	public static void setFloatForKey(String key, float value) {
		LogD("setFloatForKey(" + key + "):" + value);
		userDefaultCache.put(key, Float.valueOf(value));
	}
	
	// int
	public static int getIntegerForKey(String key, int defaultValue) {
		int ret = defaultValue;
		synchronized (userDefaultCache) {
			Object value = userDefaultCache.get(key);
			if (null == value) {
				SharedPreferences preference = getUserDefaultPreference();
				if (null != preference) {
					ret = preference.getInt(key, defaultValue);
				}
				userDefaultCache.put(key, Integer.valueOf(ret));
			} else {
				ret = ((Integer)value).intValue();
			}
		}
		LogD("getIntegerForKey(" + key + "):" + ret);
		return ret;
	}
	
	public static int getIntegerForKey(String key) {
		return getIntegerForKey(key, 0);
	}
	
	public static void setIntegerForKey(String key, int value) {
		LogD("setIntegerForKey(" + key + "):" + value);
		userDefaultCache.put(key, Integer.valueOf(value));
	}
	
    // string
	public static String getStringForKey(String key, String defaultValue) {
		String ret = defaultValue;
		synchronized (userDefaultCache) {
			Object value = userDefaultCache.get(key);
			if (null == value) {
				SharedPreferences preference = getUserDefaultPreference();
				if (null != preference) {
					ret = preference.getString(key, defaultValue);
				}
				userDefaultCache.put(key, ret);
			} else {
				ret = (String)value;
			}
		}
		LogD("getStringForKey(" + key + ")len:" + ret.length());
		return ret;
	}
	
	public static String getStringForKey(String key) {
		return getStringForKey(key, "");
	}
	
	public static void setStringForKey(String key, String value) {
		LogD("setStringForKey(" + key + ")len:" + value.length());
		userDefaultCache.put(key, value);
	}

	public static void dataFlush() {
		saveUserDefault();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get device informations : UDID and MAC
	
	private static final String AUC_PREFERENCE	= "tjcPrefrences";	// Prefs file.
	private static final String PREF_EMULATOR_DEVICE_ID	= "emulatorDeviceId";

	// Unique Id
	private static String mDeviceId = null; 
	@SuppressWarnings("deprecation")
	public static String getUid() {
		if (null != mDeviceId) return mDeviceId;	// 已经取过了，直接返回
		Context context = Wrapper.getActivity();
		try
		{
			TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			
			if (telephonyManager != null)
			{
				mDeviceId = telephonyManager.getDeviceId();
			}

			LogD("deviceID: " + mDeviceId);
			
			boolean invalidDeviceID = false;
			
			//----------------------------------------
			// Is the device ID null or empty?
			//----------------------------------------
			if (mDeviceId == null)
			{
				LogD("Device id is null.");
				invalidDeviceID = true;
			}
			else
			//----------------------------------------
			// Is this an emulator device ID?
			//----------------------------------------
			if (mDeviceId.length() == 0 || mDeviceId.equals("000000000000000") || mDeviceId.equals("0"))
			{
				LogD("Device id is empty or an emulator.");
				invalidDeviceID = true;
			}
			//----------------------------------------
			// Valid device ID.
			//----------------------------------------
			else
			{
				// Lower case the device ID.
				mDeviceId = mDeviceId.toLowerCase();
			}
			
			LogD("ANDROID SDK VERSION: " + android.os.Build.VERSION.SDK);
			
			// Is there no IMEI or MEID?
			// Is this at least Android 2.3+?
			// Then let's get the serial.
			if (invalidDeviceID && Integer.parseInt(android.os.Build.VERSION.SDK) >= 9)
			{
				LogD("TRYING TO GET SERIAL OF 2.3+ DEVICE...");
				
				//cjh mDeviceId = android.os.Build.SERIAL;
				
				LogD("====================");
				LogD("SERIAL: deviceID: [" + mDeviceId + "]");
				LogD("====================");
				
				//----------------------------------------
				// Is the device ID null or empty?
				//----------------------------------------
				if (mDeviceId == null)
				{
					LogD("SERIAL: Device id is null.");
					invalidDeviceID = true;
				}
				else
				//----------------------------------------
				// Is this an emulator device ID?
				//----------------------------------------
				if (mDeviceId.length() == 0 || mDeviceId.equals("000000000000000") || mDeviceId.equals("0") || mDeviceId.equals("unknown"))
				{
					LogD("SERIAL: Device id is empty or an emulator.");
					invalidDeviceID = true;
				}
				//----------------------------------------
				// Valid device ID.
				//----------------------------------------
				else
				{
					// Lower case the device ID.
					mDeviceId = mDeviceId.toLowerCase();
					invalidDeviceID = false;
				}
			}
			
			
			// Is the device ID invalid?  This is probably an emulator or pre-production device.
			if (invalidDeviceID)
			{
				SharedPreferences settings = context.getSharedPreferences(AUC_PREFERENCE, 0);
				
				StringBuffer buff = new StringBuffer();
				buff.append("EMULATOR");
				String deviceId = settings.getString(PREF_EMULATOR_DEVICE_ID, null);
				
				// Do we already have an emulator device id stored for this device?
				if( deviceId != null && !deviceId.equals(""))
				{
					mDeviceId = deviceId;
				}
				// Otherwise generate a deviceID for emulator testing.
				else
				{
					String constantChars = "1234567890abcdefghijklmnopqrstuvw";
					
					for (int i = 0; i < 32; i++)
					{
						int randomChar = (int) ( Math.random()* 100) ;
						int ch = randomChar % 30;
						buff.append(constantChars.charAt(ch));
					}
					
					mDeviceId = buff.toString().toLowerCase();
					
					// Save the emulator device ID in the prefs so we can reuse it.
					SharedPreferences.Editor editor = settings.edit();
					editor.putString(PREF_EMULATOR_DEVICE_ID, mDeviceId);
					editor.commit();
				}
			}
		}
		catch(Exception e)
		{
			LogD("Error getting deviceID. e: " + e.toString());
			mDeviceId = null;
		}

		LogD("getUid:" + mDeviceId);
 		return mDeviceId;
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
	
	public static String getAppID() {
		return "100002";
	}
}
