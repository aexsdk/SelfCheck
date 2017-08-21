package com.hthx.tpse01.selfcheck.selfrun;

import com.hthx.tpse01.selfcheck.MainActivity;
import com.hthx.tpse01.selfcheck.serialport.model.ConstModel;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class SystemRebootReceiver extends BroadcastReceiver{
	@Override
	public void onReceive(Context context, Intent intent){
		
		String action = intent.getAction();

		//action.equals("android.intent.action.BOOT_COMPLETED")
		if(intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)){

			if(ConstModel.LOGDEBUG){
				Toast.makeText(context, "SystemSelfCheck_TAG_Test the receiver!", Toast.LENGTH_LONG).show();
			}
			
			//启动service
			//Intent tIntent = new Intent();
			//ComponentName component = new ComponentName("com.hthx.tpse01.selfcheck","com.hthx.tpse01.selfcheck.selfrun.SystemRebootService");
			//tIntent.setComponent(component);
			//context.startService(tIntent);
			
			//启动activity
			Intent startIntent = new Intent(context,MainActivity.class);
			startIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(startIntent);
		}
	}

}
