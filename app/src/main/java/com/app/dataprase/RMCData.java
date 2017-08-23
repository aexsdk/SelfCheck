package com.app.dataprase;

public class RMCData {
	public double dUtcTime;                       //UTC时间
	public char cStatus;						   //定位状态
	public double dLatitude;                      //纬度
	public char cLatitudeDir;                     //纬度方向
	public double dLongitude;                     //经度
	public char cLongitudeDir;                    //经度方向
	public double dSpeed;                         //地面速度
	public double dCourse;                        //地面航向
	public int iDate;                             //日期
	public double dCpj;                           //磁偏角
	public char cCpjDir;                          //磁偏角方向
	public char cMode;                            //模式指示
	
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
	public String protoSentence;
}