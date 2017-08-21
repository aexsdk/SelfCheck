package com.app.dataprase;

public interface DataBaseInterface 
{
	
	public static final int TYPE_SETTING_DEVICE = 0;
	/*
	 * 协议号
	 */
	public final int BASE_PRO2_D_TYPE = 0x100;
	
	public static final int TYPE_ERROR = 0 + BASE_PRO2_D_TYPE;
	public static final int TYPE_AAM = 1 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ALM = 2 + BASE_PRO2_D_TYPE;
	public static final int TYPE_APL = 3 + BASE_PRO2_D_TYPE;
	public static final int TYPE_BSD = 4 + BASE_PRO2_D_TYPE;
	public static final int TYPE_BSQ = 5 + BASE_PRO2_D_TYPE;
	public static final int TYPE_BSX = 6 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DAS = 7 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHD = 8 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHP = 9 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHR = 10 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHS = 11 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHT = 12 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DHV = 13 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DSA = 14 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ECT = 15 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GGA = 16 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GLL = 17 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GSA = 18 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GSV = 19 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ICS = 20 + BASE_PRO2_D_TYPE;
	public static final int TYPE_IDV = 21 + BASE_PRO2_D_TYPE;
	public static final int TYPE_IHO = 22 + BASE_PRO2_D_TYPE;
	public static final int TYPE_PCD = 23 + BASE_PRO2_D_TYPE;
	public static final int TYPE_PLC = 24 + BASE_PRO2_D_TYPE;
	public static final int TYPE_PRO = 25 + BASE_PRO2_D_TYPE;
	public static final int TYPE_RAM = 26 + BASE_PRO2_D_TYPE;
	public static final int TYPE_RDW = 27 + BASE_PRO2_D_TYPE;
	public static final int TYPE_STX = 28 + BASE_PRO2_D_TYPE;
	public static final int TYPE_STZ = 29 + BASE_PRO2_D_TYPE;
	public static final int TYPE_VTG = 30 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ZDA = 31 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ZDU = 32 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GBS = 33 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GST = 34 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ICM = 35 + BASE_PRO2_D_TYPE;
	public static final int TYPE_PMU = 36 + BASE_PRO2_D_TYPE;
	public static final int TYPE_RMC = 37 + BASE_PRO2_D_TYPE;
	public static final int TYPE_TXT = 38 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ZTI = 39 + BASE_PRO2_D_TYPE;
	public static final int TYPE_BSI = 40 + BASE_PRO2_D_TYPE;
	public static final int TYPE_DWR = 41 + BASE_PRO2_D_TYPE;
	public static final int TYPE_FKI = 42 + BASE_PRO2_D_TYPE;
	public static final int TYPE_GXM = 43 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ICI = 44 + BASE_PRO2_D_TYPE;
	public static final int TYPE_ICZ = 45 + BASE_PRO2_D_TYPE;
	public static final int TYPE_KLT = 46 + BASE_PRO2_D_TYPE;
	public static final int TYPE_LZM = 47 + BASE_PRO2_D_TYPE;
	public static final int TYPE_HZR = 48 + BASE_PRO2_D_TYPE;
	public static final int TYPE_WAA = 49 + BASE_PRO2_D_TYPE;
	public static final int TYPE_TXR = 50 + BASE_PRO2_D_TYPE;
	public static final int TYPE_TCS = 51 + BASE_PRO2_D_TYPE;
	public static final int TYPE_RMO = 52 + BASE_PRO2_D_TYPE;
	public static final int TYPE_SCS = 53 + BASE_PRO2_D_TYPE;
	public static final int TYPE_PKY = 54 + BASE_PRO2_D_TYPE;
	public static final int TYPE_SBX  =55 + BASE_PRO2_D_TYPE;
	public static final int TYPE_XSD = 56 + BASE_PRO2_D_TYPE;
	public static final int TYPE_XST  =57 + BASE_PRO2_D_TYPE;
	
	
	public final int BASE_PRO4_D_TYPE = 0x200;

	public static final int TYPE_GLZK	=BASE_PRO4_D_TYPE+1;	//功率状况
	public static final int  TYPE_ICXZ	=BASE_PRO4_D_TYPE+2;	//IC信息指挥机
	public static final int  TYPE_ICXY	=BASE_PRO4_D_TYPE+3;	//IC信息用户机
	public static final int  TYPE_DWXX	=BASE_PRO4_D_TYPE+4;	//定位信息
	public static final int  TYPE_WZBG	=BASE_PRO4_D_TYPE+5;	//位置报告
	public static final int  TYPE_TXXX	=BASE_PRO4_D_TYPE+6;	//通信信息
	public static final int  TYPE_TXHZ	=BASE_PRO4_D_TYPE+7;	//通信回执
	public static final int  TYPE_GLXX	=BASE_PRO4_D_TYPE+8;	//管理信息
	public static final int  TYPE_KLSB	=BASE_PRO4_D_TYPE+9;	//口令识别
	public static final int  TYPE_ZJXX	=BASE_PRO4_D_TYPE+10;	//自检信息
	public static final int  TYPE_ZIHUI	=BASE_PRO4_D_TYPE+11;	//自毁
	public static final int  TYPE_XHXX	=BASE_PRO4_D_TYPE+12;	//序号信息
	public static final int  TYPE_SJXX	=BASE_PRO4_D_TYPE+13;	//授时信息
	public static final int  TYPE_FKXX	=BASE_PRO4_D_TYPE+14;  //反馈信息
	public static final int  TYPE_CRCE	=BASE_PRO4_D_TYPE+15;	//校验错误
	
	/*
	 * 结束
	 */
	public boolean SaveToDB();							//将数据保存到数据库
	public boolean ReadFromDB();						//从数据库读取数据
	
	public void packageProtocol();						//打包数据
	public boolean praseProtocol();						//解析数据
}
