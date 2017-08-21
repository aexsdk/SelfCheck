package com.app.dataprase;

public class DWRData {
	public short posInfoType;						//定位信息类型 
	public long  userID;							//用户地址(ID号)
	public double dUTCTime;							//UTC时间
	public double lat;								//纬度
	public char   latWay;							//纬度方向  N-北 S-南
	public double lon;								//经度
	public char   lonWay;							//经度方向  E-东 W-西
	public double floorHigh;						//大地高
	public char   floorDW;							//大地高单位
	public double highError;						//高程异常
	public char   highErrorDW;						//高程异常单位
	public short  JDError;							//精度指示
	public char   uposShow;							//紧急定位指示
	public char   DJZShow;							//多解值指示
	public char   highTypeShow;						//高程类型指示
	
	public char cLonDegree;				//经度:度
	public char cLonMinute;				//经度:分
	public double cLonSecond;				//经度:秒
	public char cLonMSecond;				//经度:0.1秒
	public char cLatDegree;				//纬度:度
	public char cLatMinute;				//纬度:分
	public double cLatSecond;				//纬度:秒
	public char cLatMSecond;				//纬度:0.1秒

	public double Latitude;						//纬度度表示
	public double Logitude;						//经度度表示
}
