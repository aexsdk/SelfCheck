package com.app.dataprase;

public class DataInterface 
{
	public static final int MESSAGE_DATA_COME	= 1;
	public static final int MESSAGE_DEBUG_INFO	= 2;
}

class PortData
{
	char sStartType[];   			//用于存储所接受数据的发射系统，如BD或GP等、
	char sYnanshiData[]; 			//用于存储串口接收的原始数据
	int  iDataLen;                  //用于存储接收到的数据的长度
	int ErrType;                  	//0表示正常，1表示发射器非“BD”或“GP”，2表示数据类型不属于需要解析的32种，
	                              	//其余类型等待扩展
	int DataType;                 	//表示当前传输的数据的数据类型，从1到33，其中33为错误类型
	Object pData;  
}