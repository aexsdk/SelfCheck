package com.app.dataprase;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class JniDataPrase {

	static Handler mHandler;
	
	static pyDataComFromJni mInterface;	
	
	public JniDataPrase(Handler handler)
	{
		mHandler = handler;
	}
	
	public JniDataPrase(pyDataComFromJni interf)
	{
		mInterface = interf;
	}
	
	//public native int ProcssData(String data);
	
	public native int ProcssData(byte[] data,int len);
	
	public native void SetProtocolType(int type);
	
	public static native long SetDownLoadCPUFile(String cpufilename);
	
	/**
	 * 设置CPU flash RD下载程序用
	 * @param mod 1:2M,0:4M
	 */
	public static native void SetDownLoadCPUFlashMode(int mod);
	
	/**
	 * 下载RN程序时开始发送第一包数据
	 */
	public static native void DownLoadRNSendFirstPackage();
	
	public static native void SendTXSQ(int leib,int bianm,long add,byte[] msgcontent);
	public static native void SendICJC();
	public static native void SendXHDQ(long addr);
	public static native void SendXTZJ(long addr);
	public static native void SendJSZL(long addr);
	public static native void SendGLJC(long addr,int fre);
	public static native void SendBBDQ(long addr);
	
	
	public static native void ResumeJni(int type);
	
	public static int pyJniSleep(int millsecond,int type)
	{			
		new Thread(new pyResumeThread(mHandler2, 500, type)).start();
		
		return 0;
		
	}
	
	private static class pyResumeThread implements Runnable
	{
		private Handler handler;
		private int sleeptime;
		private int type;
		public pyResumeThread(Handler handler,int sleeptime,int type)
		{
			this.handler = handler;
			this.sleeptime = sleeptime;
			this.type = type;
		}
		
		@Override
		public void run()
		{
			// TODO Auto-generated method stub
			try {
				
				Thread.sleep(sleeptime);
				
				if (this.handler != null)
				{
					Message msg = this.handler.obtainMessage();
					msg.what = 0x1001;
					msg.arg1 = type;
					
					msg.sendToTarget();
				}

			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	private static Handler mHandler2 = new Handler()
	{
		public void handleMessage(android.os.Message msg)
		{
			if (msg.what == 0x1001)
			{
				ResumeJni(msg.arg1);
			}
		}
	};
	
	/**
	 * 1:设备正常工作模式，2:设置设备模式，3:下载CPU程序
	 * @param type
	 */
	public static native void SetWorkMode(int type);
	
	public static native void SendDWSQ(BDS_DWSQ data);
	
	static int NotityMain(int itype,Object data)
	{	
//		PortData pData = (PortData) data;
//		Log.i("aaa", "recv data " + new String(pData.sYnanshiData, 0, pData.iDataLen));
		
		mInterface.CallBackFunction(itype,data);
				
		return 0;
	}
	
	static int NotityMain(int itype,Object data,Object[] infos)
	{
		if (itype == DataBaseInterface.TYPE_GSV) {
			int len = infos.length;
			
			GSVData gsvData = (GSVData) data;
			gsvData.starinfos = new StarInfo[len];
			for (int i = 0; i < len; i++) {
				gsvData.starinfos[i] = (StarInfo)infos[i];
			}
			
			mInterface.CallBackFunction(itype,gsvData);
		}

		return 0;
	}
	
	public static void sendDataToDevice(int len,byte[] data)
	{
		mInterface.WriteDataToDevice(data, len);
	}
	
	public static void sendDataToDeviceEx(long len,byte[] data)
	{	
		mInterface.WriteDataToDevice(data, len);
	}
	public void setHandle(Handler handler)
	{
		mHandler = handler;
	}
	
	static void ShowDebugInfo(String strinfo)
	{		
		if (mHandler != null) {
			mHandler.obtainMessage(0x102, strinfo).sendToTarget();
		}	
	}
	
	static
	{
		System.loadLibrary("appSerialLibs");
	}
}