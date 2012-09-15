package org.cocos2dx.iap.ChinaMobile;


import java.util.ArrayList;

import org.cocos2dx.helloiap.R;
import org.cocos2dx.iap.IAPProducts;
import org.cocos2dx.iap.Wrapper;

import cn.emagsoftware.gamebilling.api.GameInterface;
import cn.emagsoftware.gamebilling.api.GameInterface.BillingViewCallBack;
import cn.emagsoftware.gamebilling.util.Const.BillingType;
import android.app.Activity;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;

public class CMGCBillingActivity extends Activity implements BillingViewCallBack{
	
	public static boolean okBtnClicked = false;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		okBtnClicked = false;
		
		// 获取要购买的产品的Id
		String productId = CMGCBillingAdapter.getCurrentProductId();
		if (null == productId || productId.length() <= 0) {
			CMGCBillingAdapter.onUserOperCancel();
			this.finish();
			return;
		}
		
		String sms = IAPProducts.getProductCMGCSMSKey(productId);
		View mMainView = GameInterface.getBillingView(this, BillingType.SMS, this, sms);

		// 找到确定按钮并注册 Touch 事件回调 
		ArrayList<View> children = mMainView.getTouchables();
		for (int i = 0; i < children.size(); i++) {
			Button btn = (Button) (children.get(i));
			if (null != btn) {
				String okStr = Wrapper.getActivity().getResources().getString(R.string.gc_billing_dialog_sure_space);
				CharSequence str = btn.getText();
				if (str.equals(okStr)) {
					btn.setOnTouchListener(new OnTouchListener()
					{

						@Override
						public boolean onTouch(View v, MotionEvent event) {
							switch (event.getActionMasked())
							{
							case MotionEvent.ACTION_UP:
								Rect r = new Rect();
								v.getGlobalVisibleRect(r);
								if (r.contains((int) (event.getRawX()), (int) (event.getRawY()))){
									CMGCBillingActivity.okBtnClicked = true;
			//cjh						FlurryAPIWrapper.logEvent("CMGC button ok clicked!");
									Log.d("CMGCBillingActivity", "CMGC button ok clicked!");
								}
								break;
							}
							return false;
						}
						
					});
				}
			}
		}
		this.setContentView(mMainView);
	} // onCreate
	
	///////////////////////////////////////////////////////////////////////////
	// 把BillingViewCallback 的 回调Wrap到CMGCBillingAdapter中去
	///////////////////////////////////////////////////////////////////////////
	@Override
	public void onBillingFinish() {
		Log.d("CMGCBillingActivity", "onBillingFinish");
		CMGCBillingAdapter.onBillingFinish();
		this.finish();
	}

	@Override
	public void onBillingSuccess() {
		Log.d("CMGCBillingActivity", "onBillingSuccess");
		CMGCBillingAdapter.onBillingSuccess();
	}

	@Override
	public void onUserOperCancel() {
		Log.d("CMGCBillingActivity", "onUserOperCancel");
		CMGCBillingAdapter.onUserOperCancel();
		this.finish();
	}

	@Override
	public void onUserOperError(int arg0) {
		Log.d("CMGCBillingActivity", "onUserOperError");
		CMGCBillingAdapter.onUserOperError(arg0);
		this.finish();
	}
	
	@Override
	protected void onDestroy () {
		Log.d("CMGCBillingActivity", "CMGC pay view closed!");
		CMGCBillingAdapter.onActivityDestroy();
		okBtnClicked = false;
		super.onDestroy();
	}
}
