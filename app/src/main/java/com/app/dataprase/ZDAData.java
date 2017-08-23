package com.app.dataprase;

public class ZDAData {

	public int    modeShow;                       //模式指示
	public double dUTCTime;                     //UTC时间
	public int    iDay;                         //日
	public int    iMonth;                       //月
	public int    iYear;                        //年
	public int    localTime;                    //本地时区  00~+/-13 
	public int    localMinDiss;                 //本地区分钟差  00 ~ 59 单位：分钟
	public double dDsxzTime;                    //定时修正时刻
	public int    xzData;                       //修正值  单位：ns  0~3ns
	public int    iprecision;					//精度指示
	public char   spSateLockState;              //卫星信号锁定状态
	
	public String protoSentence;			//协议包
}
