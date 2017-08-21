package com.hthx.tpse01.selfcheck;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.EditText;
import android.widget.TextView;

import com.androidex.plugins.kkserial;
import com.hthx.tpse01.selfcheck.selfrun.SystemRebootService;

public class MainActivity extends Activity {

	//private kkserial serial;
	//private int mFd;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		TextView stableOSVersion = (TextView) findViewById(R.id.StableOSVerion);

		com.androidex.aexlibs.hwService mService = new com.androidex.aexlibs.hwService(this);
		stableOSVersion.setText(mService.getSdkVersion());
		Intent startIntent = new Intent(this, SystemRebootService.class);
		startService(startIntent);
		/*
		serial = new kkserial(this);
		mFd = serial.serial_open(SystemRebootService.mBDPort);
		if(mFd > 0)
			serial.serial_readloop(mFd,100);
		*/
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
