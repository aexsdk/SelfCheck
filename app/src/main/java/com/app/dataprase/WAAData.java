package com.app.dataprase;

public class WAAData {

	public short infoType;                       //信息类型  1/0
	public int   reportFre;                      //报告频度  单位：秒   0-单次位置报告
	public long userID;                 //用户地址(ID号)
	public double  reportTime;                   //位置报告时间
	public double lon;                           //纬度
	public char   lonWay;                        //纬度方向  N-北 S-南
	public double lat;                           //经度
	public char   latWay;                        //经度方向  E-东 W-西
	public double high;                          //高程值
	public char   highDW;                        //高程单位
	
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
