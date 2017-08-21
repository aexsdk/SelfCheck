package com.app.dataprase;

public class BDR_DWXX
{

	public long dUserAddr;				//本机用户地址
	public short cInfoSort;				//信息类别
	public long dQueryAddr;		        //查询地址
	public short cPosTimeHour;			//定位时刻的小时数据
	public short cPosTimeMinute;		//定位时刻的分位数据
	public short cPosTimeSecond;   		//定位时刻的秒位数据
	public short cPosTimeMSecond;		//定位时刻的秒小数数据
	public short cLonDegree;			//经度:度
	public short cLonMinute;			//经度:分
	public short cLonSecond;			//经度:秒
	public short cLonMSecond;			//经度:0.1秒
	public short cLatDegree;			//纬度:度
	public short cLatMinute;			//纬度:分
	public short cLatSecond;			//纬度:秒
	public short cLatMSecond;			//纬度:0.1秒
	public int dElevation;			    //高程数据
	public int dElvError;				//高程异常值
	public short IsUrgent;		        //是否紧急
	public short IsMultiRes;		    //是否多解
	public short Precision;	    		//精度
	public short HeightType;	    	//高程指示

}
