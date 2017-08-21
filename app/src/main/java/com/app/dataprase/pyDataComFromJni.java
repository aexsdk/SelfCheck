package com.app.dataprase;

public interface pyDataComFromJni {

	public int CallBackFunction(int iType,Object obj);		//收到Jni来的解析结果后的回调函数
	
	public void WriteDataToDevice(byte[] buf,long len);		//发送数据
}
