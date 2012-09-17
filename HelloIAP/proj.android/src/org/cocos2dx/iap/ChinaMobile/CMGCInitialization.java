package org.cocos2dx.iap.ChinaMobile;

import org.cocos2dx.helloiap.HelloIAP;

import cn.emagsoftware.gamebilling.api.GameInterface.AnimationCompleteCallback;
import cn.emagsoftware.gamebilling.view.OpeningAnimation;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

/** 游戏第一个展现屏幕显示的Activity界面 */
public class CMGCInitialization extends Activity {

	// 游戏启动画面View.
	private OpeningAnimation mOpeningAnimation;
   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 展现开机动画界面，需要开发人员实现动画完成后的逻辑处理：即加载游戏主界面
        // 并选择是否开启游戏背景音乐
       
        mOpeningAnimation = new OpeningAnimation(this, new AnimationCompleteCallback() {
			@Override
			public void onAnimationCompleted(boolean isMusicEnabled) {
				if(isMusicEnabled){
					// 游戏开发者需要实现的代码：即开启游戏背景音乐
					//cjh FishingJoyWrapper.setInitalBackgroundMusicVolume(1.0f);
				}
				else {
					//cjh FishingJoyWrapper.setInitalBackgroundMusicVolume(0);
				}
				// 游戏开发者需要在此处：实现游戏主界面的展现代码
				CMGCInitialization.this.startActivity(new Intent(CMGCInitialization.this, HelloIAP.class));
				CMGCInitialization.this.finish();
			}
		});
        setContentView(mOpeningAnimation);
    }

}