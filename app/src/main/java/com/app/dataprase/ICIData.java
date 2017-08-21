package com.app.dataprase;

public class ICIData {
	/**
	 * 用户地址(ID号)
	 */
	public long userID; 
	/**
	 * 序列号
	 */
	public long XLH; 
	/**
	 * 通播地址
	 */
	public long TBID;  
	/**
	 * 用户特征值
	 */
	public short userTZ;
	/**
	 * 服务频度
	 */
	public double serverFre; 
	/**
	 * 通信等级
	 */
	public short  TXOrder; 
	/**
	 * 加密标志    E-加密 N-非密
	 */
	public char   JMFlag;  
	/**
	 * 下属用户数
	 */
	public double XSUserNum;     
	/**
	 * 协议包
	 */
	public String protoSentence;
	
	public ICIData()
	{
		userTZ = -1;
	}
}
