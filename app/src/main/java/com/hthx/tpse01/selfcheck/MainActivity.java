package com.hthx.tpse01.selfcheck;

import android.content.ComponentName;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.app.Activity;
import android.os.RemoteException;
import android.os.UserHandle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.TextView;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import com.androidex.plugins.kkserial;
import com.hthx.tpse01.selfcheck.selfrun.SystemRebootService;

import java.util.ArrayList;
import java.util.List;

import static com.androidex.hwuitls.hwLog.mContext;

public class MainActivity extends Activity {

	public static final int SYSTEM_UI_FLAG_IMMERSIVE_GESTURE_ISOLATED = 0x00002000;
	private boolean mFirstLaunch;
	//private kkserial serial;
	//private int mFd;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		TextView stableOSVersion = (TextView) findViewById(R.id.StableOSVerion);

		//全屏设置，隐藏窗口所有装饰
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);//清除FLAG
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		View v = getWindow().getDecorView();
		v.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE
						| View.SYSTEM_UI_FLAG_FULLSCREEN
						| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
						| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
						| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
						| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
						| SYSTEM_UI_FLAG_IMMERSIVE_GESTURE_ISOLATED);

		com.androidex.aexlibs.hwService mService = new com.androidex.aexlibs.hwService(this);
		stableOSVersion.setText(mService.getSdkVersion());
		mService.EnterFullScreen();
		Intent startIntent = new Intent(this, SystemRebootService.class);
		startService(startIntent);
		/*
		serial = new kkserial(this);
		mFd = serial.serial_open(SystemRebootService.mBDPort);
		if(mFd > 0)
			serial.serial_readloop(mFd,100);
		*/
		Intent intent = new Intent();
		intent.setAction(SystemRebootService.bdStart28App);
		sendBroadcast(intent);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
			case R.id.action_settings:
				system_set();//系统设置
				return true;
			case R.id.action_start28app: {
				Intent intent = new Intent();
				intent.setAction(SystemRebootService.bdStart28App);
				sendBroadcast(intent);
				return true;
			}
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * 系统设置
	 */
	private void system_set() {
		Intent mIntent = new Intent();
		mIntent.setAction(Intent.ACTION_VIEW);
		mIntent.setClassName("com.android.settings", "com.android.settings.Settings");
		mIntent.putExtra("back", true);
		startActivity(mIntent);
	}

}
