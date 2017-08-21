package com.app.dataprase;

public class BDR_TXXX
{
	public long dUserAddr;				//本机用户地址
	public char cInfoSort;		//汉字、代码、混编
	public char Infotype;	    	//信息类别

	public long dPostLetterAddr;			//发信方地址
	public char cPostLetterTimeHour;		//发信时间:时
	public char cPostLetterTimeMinute;		//发信时间:分
	public short sTelexLen;				//电文长度
	public String TelexContent;			//电文内容 ≤120个汉字或420个代码
	public short  CRCVerify;			//CRC标识
}
