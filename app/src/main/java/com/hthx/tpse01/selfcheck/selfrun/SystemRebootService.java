package com.hthx.tpse01.selfcheck.selfrun;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;
import java.util.Timer;
import java.util.TimerTask;

import com.androidex.plugins.OnCallback;
import com.androidex.plugins.kkserial;
import com.app.dataprase.DataBaseInterface;
import com.app.dataprase.GGAData;
import com.app.dataprase.RMCData;
import com.app.dataprase.pyDataComFromJni;

import com.hthx.tpse01.selfcheck.hardware.HardwareCheck;
import com.hthx.tpse01.selfcheck.serialport.model.ConstModel;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import static android.app.AlarmManager.ELAPSED_REALTIME_WAKEUP;

public class SystemRebootService extends Service implements pyDataComFromJni,OnCallback {

	private com.app.dataprase.JniDataPrase mJniDataPraseMode = null;
	public static final String mGSMPort = "/dev/ttymxc3,115200,N,1,8";
	public static final String mBDPortOrige = "/dev/ttymxc2,9600,N,1,8";
	public static final String mBDPort = "/dev/ttymxc2,115200,N,1,8";
	public static final String actionRunReboot           = "com.androidex.REBOOT";
	public static final String bdSerialReciveData        = "bd.serial.recivedata";
	public static final String bdSerialReciveLog         = "bd.serial.recivelog";
	public static final String bdSerialSendData         = "bd.serial.senddata";
	private NotifyReceiver mReceiver;
	public kkserial serial;
	public int mGnssFd = 0;
	private Timer rebootTimer;
	private RebootTimerTask rebootTask;

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		
		return null;
	}
	@Override
	public void onCreate(){
		super.onCreate();
		serial = new kkserial(this);
		mJniDataPraseMode = new com.app.dataprase.JniDataPrase(this);
		mJniDataPraseMode.SetProtocolType(ConstModel.m_CurProtocolType);

		/////////////////////////////////////////////////////////首次默认写入本地时间
		if(!new File("StoreLocalTime.txt").exists()){
			writeFile("2017-08-25 14:10:18");
		}

		/////////////////////////////////////////////////////////初始化协议数据解析模块(jni模块)
		mReceiver = new NotifyReceiver();
		IntentFilter filter = new IntentFilter();
		filter.addAction(bdSerialReciveData);
		filter.addAction(bdSerialReciveLog);
		filter.addAction(actionRunReboot);
		registerReceiver(mReceiver, filter);

		/////////////////////////////////////////////////////////检测TF卡状态、获取系统信息
		HardwareCheck hardwareCheck = new HardwareCheck();
		ConstModel.TFState = hardwareCheck.getTFCardState();
		ConstModel.CPUCores = hardwareCheck.getNumberOfCPUCores();
		ConstModel.RAMSize = hardwareCheck.getRAMSize();
		ConstModel.ROMSize = hardwareCheck.getROMSize();
		ConstModel.OSVersion = hardwareCheck.getOSVersion();

		gsm_selfcheck();

		String bdData = bd_selfcheck(mBDPort);
		if(bdData == null){
			bdData = bd_selfcheck(mBDPortOrige);
			if(bdData != null) {
				//9600监测正确
				int fd = serial.serial_open(mBDPortOrige);
				String cmd = "$PCAS01,5*19\r\n";
				serial.serial_write(fd, cmd.getBytes(), cmd.length());
				serial.serial_close(fd);
			}
		}
		ConstModel.GNSSState = true;
		//北斗已经变为115200，以后按照115200检测，下面检查北斗返回的内容设置时间
		if(bdData != null && bdData != "")
			mJniDataPraseMode.ProcssData(bdData.getBytes(),bdData.length());

		///////////////////////////////////////////////////////// 启动二个串口的服务
		/*
		Intent intentMXC2 = new Intent(ConstModel.SERVICE_ACTION_SERIAL_PORT_TTYMXC2);
		startService(intentMXC2);
		
		Intent intentMXC3 = new Intent(ConstModel.SERVICE_ACTION_SERIAL_PORT_TTYMXC3);
		startService(intentMXC3);
		gnss_runnable();
		*/
		serial.serial_readloop(mBDPort,100,10*1000000);
		run28app("com.androidex.apps.aexserial", "com.androidex.apps.aexserial.MainActivity");

		/*
		AlarmManager am = (AlarmManager) getApplicationContext().getSystemService(Context.ALARM_SERVICE);
		am.set(ELAPSED_REALTIME_WAKEUP, System.currentTimeMillis() + 480000,   //从现在起30s
				PendingIntent.getBroadcast(getApplicationContext(), 100, new Intent(actionRunReboot), PendingIntent.FLAG_UPDATE_CURRENT));
		*/
		rebootTimer = new Timer();
		rebootTask = new RebootTimerTask();
		rebootTimer.schedule(rebootTask,60000);
	}

	public String CheckGsmCommand(int lGsmFd,String cmd) {
		serial.serial_write(lGsmFd,cmd.getBytes(),cmd.length());
		byte[] rdata = serial.serial_read(lGsmFd,50,2000);
		if(rdata != null){
			try {
				return new String(rdata, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}
		return "";
	}

	public boolean gsm_selfcheck()
	{
		boolean r = false;
		int lGsmFd = 0;
		lGsmFd = serial.serial_open(mGSMPort);
		//监测GSM模块状态，发AT指令
		if(CheckGsmCommand(lGsmFd,"AT\r").indexOf("OK",0) != -1){
			//监测SIM卡状态
			ConstModel.GSMState = true;
			if(CheckGsmCommand(lGsmFd,"AT+CIMI\r").indexOf("OK",0) != -1){
				ConstModel.SIMState = true;
				String rdata = CheckGsmCommand(lGsmFd,"AT+QNITZ=1\rAT+QLTS=1\r");
				String rtime = parseGsmTime(rdata);
				if(rtime != "") {
					ConstModel.bGSMTime = true;
					setSystemTime(UTC2LocalTime(rtime));
				}
				r = true;
			}else{
				ConstModel.SIMState = false;
			}
		}else{
			ConstModel.GSMState = false;
		}
		serial.serial_close(lGsmFd);
		return r;
	}

	public String bd_selfcheck(String port)
	{
		String r = null;
		int lBdFd = 0;
		lBdFd = serial.serial_open(port);
		byte[] rdata = serial.serial_read(lBdFd,256,3000);
		if(rdata != null){
			try {
				String sdata = new String(rdata,"UTF-8");
				if((sdata.indexOf("$BD",0) != -1) ||
						(sdata.indexOf("$GP",0) != -1) ||
						(sdata.indexOf("$GL",0) != -1) ||
						(sdata.indexOf("$GN",0) != -1)){
					r = sdata;
				}
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}

		serial.serial_close(lBdFd);
		return r;
	}

	private void run28app(String app,String mainpage) {
		Intent intentTo28App = new Intent(Intent.ACTION_MAIN);  //打包intent数据
		intentTo28App.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intentTo28App.addCategory(Intent.CATEGORY_LAUNCHER);
		intentTo28App.putExtra("GSM_State", ConstModel.GSMState);
		intentTo28App.putExtra("GNSS_State", ConstModel.GNSSState);
		intentTo28App.putExtra("SIM_State", ConstModel.SIMState);
		intentTo28App.putExtra("TF_State", ConstModel.TFState);

		intentTo28App.putExtra("CPU_Cores", ConstModel.CPUCores);
		intentTo28App.putExtra("RAM_Size", ConstModel.RAMSize);
		intentTo28App.putExtra("ROM_Size", ConstModel.ROMSize);
		intentTo28App.putExtra("OS_Version", ConstModel.OSVersion);

		if(ConstModel.LOGDEBUG){
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_GSM-" + ConstModel.GSMState);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_SIM-" + ConstModel.SIMState);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_GNSS-" + ConstModel.GNSSState);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_TF-" + ConstModel.TFState);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_CPU_Cores-" + ConstModel.CPUCores);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_RAM_Size-" + ConstModel.RAMSize);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_ROM_Size-" + ConstModel.ROMSize);
			Log.e(ConstModel.TAG, "SystemSelfCheck_TAG_OS_Version-" + ConstModel.OSVersion);
		}

		//ComponentName component = new ComponentName("com.example.testbroadcastintentotherapp2","com.example.testbroadcastintentotherapp2.MainActivity");
		ComponentName component = new ComponentName(app,mainpage);//"com.example.mywork","com.example.smsproject.Login");
		//ComponentName component = new ComponentName("com.example.testserialportextenal","com.example.testserialportextenal.MainActivity");

		intentTo28App.setComponent(component);
		try{
			startActivity(intentTo28App);
		}catch(ActivityNotFoundException e){
			e.printStackTrace();
		}finally{

		}
	}

	private String parseGsmTime(String data) {
		int firstIndex = 0, lastIndex = 0;
		firstIndex = data.indexOf("\"");
		lastIndex = data.lastIndexOf("\"");

		if(lastIndex - firstIndex > 1){

            String strDateTime = data.substring(firstIndex + 1, lastIndex);

            String[] strTemp1 = strDateTime.split(",");

            if(strTemp1.length >= 2){
                String[] strTemp2 = strTemp1[0].split("/");
                String[] strTemp3 = strTemp1[1].split(":");

                String year = "17";                  //年          eg:2017
                String month = "08";                 //月          eg:07
                String day = "08";                   //日          eg:31
                String hour = "14";                  //时          eg:04
                String minute = "10";                //分           eg:05
                String second = "18";                //秒           eg:03

                if(strTemp2.length == 3 && strTemp3.length == 3){       //格式正确就用正确的值，否则就用默认值
                    year = strTemp2[0];
                    month = strTemp2[1];
                    day = strTemp2[2];

                    hour = strTemp3[0];
                    minute = strTemp3[1];
                    second = strTemp3[2].substring(0, 2);
                }

                return String.format("20%s-%s-%s %s:%s:%s", year, month, day, hour, minute, second);
            }
        }
        return "";
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId){
		return START_STICKY;
	}
	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		if(mGnssFd > 0)
			serial.serial_close(mGnssFd);
		unregisterReceiver(mReceiver);
		super.onDestroy();
	}
	
	@Override
	public int CallBackFunction(int iType, Object obj) {
		// TODO Auto-generated method stub
		String oneTag = "Unkown\n";	//单个协议包
		switch (iType) {
		case com.app.dataprase.DataBaseInterface.TYPE_RMC: 
		{
			ConstModel.bGNSSData = true;
			if(ConstModel.LOGDEBUG){
				Log.i(ConstModel.TAG, "SystemSelfCheck_TAG_in the java_rmc");
			}
			RMCData data = (RMCData) obj;
			oneTag = data.toString();
			if(data.iDate != 0 && data.dUtcTime != 0.0){                         //若协议中无数据，那么就不处理
				String strDate = "";               //日期字符串
				String strUTCTime = "";            //时间字符串
				String strUTCTimeInteger = "";            //时间字符串整数部分
				String strUTCTimeDecimal = "";            //时间字符串小数部分
				String year = "17";                  //年          eg:2017
				String month = "08";                 //月          eg:07
				String day = "08";                   //日          eg:31
				String hour = "18";                  //时          eg:04
				String minute = "18";                //分           eg:05
				String second = "18";                //秒           eg:03
				String millisecond = "888";           //毫秒       eg:333
				
				strDate = String.format("%06d", data.iDate);
				strUTCTime = String.format("%f", data.dUtcTime);
				String [] temStr = strUTCTime.split("\\.");
				strUTCTimeDecimal = temStr[1];
				strUTCTimeInteger = String.format("%06d", Integer.parseInt(temStr[0]));
				day = strDate.substring(0, 2);
				month = strDate.substring(2, 4);
				year = strDate.substring(4, 6);
				hour = strUTCTimeInteger.substring(0, 2);
				minute = strUTCTimeInteger.substring(2, 4);
				second = strUTCTimeInteger.substring(4, 6);
				millisecond = strUTCTimeDecimal;
				//ConstModel.sGNSSTime = String.format("20%s-%s-%s %s:%s:%s.%s", year, month, day,hour, minute, second, millisecond);
				ConstModel.sGNSSTime = String.format("20%s-%s-%s %s:%s:%s", year, month, day, hour, minute, second);  //不处理毫秒
				ConstModel.bGNSSTime = true;
				if(ConstModel.LOGDEBUG){
					Log.i(ConstModel.TAG, "SystemSelfCheck_TAG_rmc gnssTime---" + ConstModel.sGNSSTime);
				}
				String strLocalTime = UTC2LocalTime(ConstModel.sGNSSTime);           //将UTC时间转换为本地时间来设置系统时间以及存储到txt中。
				writeFile(strLocalTime);               //存储本地时间到本地txt，作为下一次使用
				// ②在此处设置系统时间      ///////////////////////////////////////////////////////////////////////////////////////////////GNSS时间
				//setSystemTime(Integer.parseInt(year20), Integer.parseInt(month), Integer.parseInt(day), Integer.parseInt(hour), Integer.parseInt(minute), Integer.parseInt(second));
				setSystemTime(strLocalTime);
			}
			break;
		}
		case DataBaseInterface.TYPE_GGA:
		{
			GGAData data = (GGAData) obj;
			oneTag = data.protoSentence+"\n";
		}
			break;
		default:
			break;
		}
		oneTag = String.format("%s\n",obj.toString());
		Intent ds_intent = new Intent();
		ds_intent.setAction(bdSerialReciveData);
		ds_intent.putExtra("fd",mGnssFd);
		ds_intent.putExtra("data",oneTag.getBytes());
		sendBroadcast(ds_intent);

		return 0;
	}
	
	@Override
	public void WriteDataToDevice(byte[] buf, long len) {
		// TODO Auto-generated method stub
		
	}

	public String readFile(){
		
		try {
			//FileInputStream fis = new FileInputStream("StoreLocalTime.txt");
			FileInputStream fis = openFileInput("StoreLocalTime.txt");
			byte[] buf = new byte[100];
			int hasRead = 0;
			String content = "";
			while((hasRead = fis.read(buf)) > 0){
				content = new String(buf, 0 ,hasRead);
			}
			fis.close();
			
			if(ConstModel.LOGDEBUG){
				Log.i(ConstModel.TAG, "SystemSelfCheck_TAG_read StoreLocalTime.txt finish");
			}
			
			return content;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;
	}
	
	public void writeFile(String content){
		
		try {
			//FileOutputStream fos = new  FileOutputStream("StoreLocalTime.txt");     //会提示只读文件系统  java中提供的api
			FileOutputStream fos = openFileOutput("StoreLocalTime.txt",MODE_PRIVATE);  //android中提供的api
			PrintStream ps = new PrintStream(fos);
			ps.print(content);
			ps.close();
			
			if(ConstModel.LOGDEBUG){
				Log.i(ConstModel.TAG, "SystemSelfCheck_TAG_write StoreLocalTime.txt finish");
			}
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	//public void setSystemTime(int year, int month, int day, int hour, int minute, int second){
	public boolean setSystemTime(String sLocalTime){
		
		//有日期时间字符串中获取年月日时分秒数据
		if(sLocalTime == null || sLocalTime.length() < 19){        //判断字符串格式
			return false;
		}
		String[] tempStr = sLocalTime.split(" ");
		String[] tempStr1 = tempStr[0].split("-");
		String[] tempStr2 = tempStr[1].split(":");
		//设置时间
		Calendar c = Calendar.getInstance();
		
		c.set(Calendar.YEAR, Integer.parseInt(tempStr1[0]));
		c.set(Calendar.MONTH, Integer.parseInt(tempStr1[1]) - 1);                //设置月份需要将数值减1才行，获取月份需要将获取的值加上1才是正确的月份
		c.set(Calendar.DAY_OF_MONTH, Integer.parseInt(tempStr1[2]));
		c.set(Calendar.HOUR_OF_DAY, Integer.parseInt(tempStr2[0]));
		c.set(Calendar.MINUTE, Integer.parseInt(tempStr2[1]));
		c.set(Calendar.SECOND, Integer.parseInt(tempStr2[2]));
		
		AlarmManager am = (AlarmManager)SystemRebootService.this.getSystemService(Context.ALARM_SERVICE);
		am.setTime(c.getTimeInMillis());
		return true;
	}
	
	public String UTC2LocalTime(String utcTime){
		//String utcTime = "2017-07-31 18:37:48";
		String utcTimePatten = "yyyy-MM-dd HH:mm:ss";
		String localTimePatten = "yyyy-MM-dd HH:mm:ss";
		SimpleDateFormat utcFormater = new SimpleDateFormat(utcTimePatten);
		utcFormater.setTimeZone(TimeZone.getTimeZone("UTC"));
		//Toast.makeText(this, TimeZone.getDefault().getDisplayName(), Toast.LENGTH_LONG).show();
		Date gpsUTCDate = null;
		try {
			gpsUTCDate = utcFormater.parse(utcTime);
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		SimpleDateFormat localFormater = new SimpleDateFormat(localTimePatten);
		localFormater.setTimeZone(TimeZone.getDefault());
		String localTime = localFormater.format(gpsUTCDate.getTime());
		//Toast.makeText(this, localTime, Toast.LENGTH_LONG).show();
		return localTime;
	}

	@Override
	public void onLogEvent(int fd, String msg) {
		//收到的日志数据
		//log(String.format("fd=%d:%s", fd, msg));
		android.util.Log.i("SELFCHECK", String.format("OnLogEvent: fd=%d,msg=%s\n",fd,msg));
		Intent ds_intent = new Intent();
		ds_intent.setAction(bdSerialReciveLog);
		ds_intent.putExtra("fd",mGnssFd);
		ds_intent.putExtra("data",msg);
		sendBroadcast(ds_intent);
	}

	@Override
	public void onDataRecive(int fd, byte[] data, int len) {
		//收到的串口数据
		//logData(data, len);
		android.util.Log.i("SELFCHECK", String.format("Rev from fd=%d(%d):%s\n",fd,len,serial.byteToString(data)));
		if(data != null)
			mJniDataPraseMode.ProcssData(data,data.length);
		//下面先发广播数据，以后调好拼包解析代码则可以去掉
		/*
		Intent ds_intent = new Intent();
		ds_intent.setAction(bdSerialReciveData);
		ds_intent.putExtra("data",data);
		sendBroadcast(ds_intent);
		*/
	}

	public void runReboot() {
		//首先要关闭程序提供的服务,以免关机时程序还需要写入数据,导致存属区损坏
		onLogEvent(0,"Will reboot now.");
		Intent intent = new Intent();
		intent.setAction("com.androidex.action.reboot");
		sendBroadcast(intent);
	}

	public class NotifyReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			if (intent.getAction().equals(bdSerialReciveLog)){
				//接收到北斗模块的串口操作日志
				String data = intent.getStringExtra("data");
				//log(data);
			}else if(intent.getAction().equals(bdSerialReciveData)){
				//接收到北斗模块的串口数据
				byte[] data = intent.getByteArrayExtra("data");
				//logData(data,data.length);
			}else if(intent.getAction().equals(bdSerialSendData)){
				//发送北斗模块指令
				byte[] data = intent.getByteArrayExtra("data");
				//logData(data,data.length);
				if(mGnssFd > 0)
					serial.serial_write(mGnssFd,data,data.length);
			}else if(intent.getAction().equals(actionRunReboot)){
				runReboot();
			}
		}
	}

	public class RebootTimerTask  extends TimerTask{
		@Override
		public void run() {
			Intent ds_intent = new Intent();
			ds_intent.setAction(actionRunReboot);
			sendBroadcast(ds_intent);
		}
	}
}
