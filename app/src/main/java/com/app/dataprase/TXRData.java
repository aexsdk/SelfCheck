package com.app.dataprase;

public class TXRData
{
	public short ixxleib;							//信息类别
	public long userID;								//用户地址
	public short idianwenxs;						//电文形式
	public int isendtime;							//发信时间
	public String ccontext;							//通信电文内容
	
	public int commupassway;						//通信通道;0：北斗，1:SIM
	
	public TXRData()
	{
		commupassway = 0;		//默认为北斗
	}

}
