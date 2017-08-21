package com.hthx.tpse01.selfcheck.serialport.model;

public class ConstModel {

	public static final String TAG = "SystemSelfCheck";
	
	/**
	 * 当前数据协议类型
	 * 0：4.0协议，1：0183协议
	 */
	public static int m_CurProtocolType = 1;
	
	/**
	 * 串口路径
	 */
	public static final String ttyMxc2 = "/dev/ttymxc2";
	public static final String ttyMxc3 = "/dev/ttymxc3";
	
	/**调试信息输出
	 * LOGDEBUG  true:输出log信息， false:不输出log信息
	 */
	public static final boolean LOGDEBUG = true;          

	/**
	 * ttymxc2串口操作广播
	 */
	public static final String BROADCAST_OPEN_SERIAL_PORT_TTYMXC2 = "com.hthx.tpse01.selfcheck.BROADCAST_OPEN_SERIAL_PORT_TTYMXC2";  //打开串口广播
	
	public static final String BROADCAST_CLOSE_SERIAL_PORT_TTYMXC2 = "com.hthx.tpse01.selfcheck.BROADCAST_CLOSE_SERIAL_PORT_TTYMXC2";  //关闭串口广播
	
	public static final String BROADCAST_WRITE_TO_DEVICE_TTYMXC2 = "com.hthx.tpse01.selfcheck.BROADCAST_WRITE_TO_DEVICE_TTYMXC2";  //向mxc2串口写数据广播
	
	//启动串口mxc2的服务action
	public static final String SERVICE_ACTION_SERIAL_PORT_TTYMXC2 = "com.hthx.tpse01.selfcheck.SERVICE_ACTION_SERIAL_PORT_TTYMXC2";
	
	/**
	 * ttymxc3串口操作广播
	 */
	public static final String BROADCAST_OPEN_SERIAL_PORT_TTYMXC3 = "com.hthx.tpse01.selfcheck.BROADCAST_OPEN_SERIAL_PORT_TTYMXC3";  //打开串口广播
	
	public static final String BROADCAST_CLOSE_SERIAL_PORT_TTYMXC3 = "com.hthx.tpse01.selfcheck.BROADCAST_CLOSE_SERIAL_PORT_TTYMXC3";  //关闭串口广播
	
	public static final String BROADCAST_WRITE_TO_DEVICE_TTYMXC3 = "com.hthx.tpse01.selfcheck.BROADCAST_WRITE_TO_DEVICE_TTYMXC3";  //向mxc3串口写数据广播
	
	//启动串口mxc3的服务action
	public static final String SERVICE_ACTION_SERIAL_PORT_TTYMXC3 = "com.hthx.tpse01.selfcheck.SERVICE_ACTION_SERIAL_PORT_TTYMXC3";

	public static boolean GSMState = false;           //GSM模块状态
	public static boolean GNSSState = false;          //GNSS模块状态
	public static boolean SIMState = false;           //SIM卡状态
	public static boolean TFState = false;            //TF卡状态
	
	public static int CPUCores = 0;                 //CPU核心数
	public static String RAMSize = "";             //运行内存大小
	public static String ROMSize = "";             //机身存储大小
	
	public static String OSVersion = "";             //系统版本
	
	//public static String testContent = "";            //测试用
	
	public static String gTempSwitchStr = "";         //接收数据交换用


	/**
	 * 接收GNSS串口发过啦的数据
	 */
	public static final String BROADCAST_DATA_FROM_GNSS = "com.hthx.tpse01.selfcheck.BROADCAST_DATA_FROM_GNSS";
	
	/**
	 * 发送到28所应用的广播
	 */
	public static final String BRAODCAST_SEND_TO_28APP = "com.hthx.tpse01.selfcheck.BRAODCAST_SEND_TO_28APP";
	
	/**
	 * /dev/ttymxc2 /dev/ttymxc3打开状态
	 */
	public static boolean bMxc2OpenState = false;           //默认未打开
	public static boolean bMxc3OpenState = false;           //默认未打开
	
	/**
	 * GSM同步时间、BD时间
	 */
	public static String sGSMTime = "";
	public static String sGNSSTime = "";
	
	/**
	 * BD、GSM时间获取成功与否
	 */
	public static boolean bGNSSTime = false;
	public static boolean bGSMTime = false;
	
	/**
	 * BD串口数据是否正常
	 */
	public static boolean bGNSSData = false;
	
}
