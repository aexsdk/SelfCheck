#if !defined(AFX_BDALL_H__D99E2286_DCF4_4FBB_85B7_D60355983A2B__INCLUDED_)
#define AFX_BDALL_H__D99E2286_DCF4_4FBB_85B7_D60355983A2B__INCLUDED_

// #if _MSC_VER > 1000
// #pragma once
// #endif // _MSC_VER > 1000
// BDAll.h : header file
//

#include "JniDataSwitchCls.h"
#include "jni.h"
#include "SomeStruct.h"

// #include "vxWorks.h"
// #include "msgQLib.h"
// #include "Serial.h"
// #include "wdLib.h"
// #include "taskLib.h"
// 
// 
// LOCAL CSerial g_Serial;

/////////////////////////////////////////////////////////////////////////////
// CBDAll dialog

class CBDAll/* : public CDialog*/
{
// Construction
public:
//	BOOL InitSerialPort(int nport,int nbaud);
//	void CloseSerialPort();

	CBDAll(JNIEnv *env);
	~CBDAll();
    void ProcessData(int Len, unsigned char *buf/*,BDIPortData & portData*/);
//	void Close_Window();
//  void CreatAlarmWindow(CString str,int nDelay);
    void PosAccount();
    void PackStream(unsigned char *str, int &len, void *pstrct);
	int String2Int(char* str,int length,int radix); 
	int Qexp(int a,int b);
	int ConvertHexChar(int ch);


	void SendXHDQ(DWORD dAppAddr);				//发送序号读取，返回序号信息XHXX
	void SendSJSC(int iFre,DWORD dAppAddr);		//发送时间输出申请，返回时间信息SJXX
	void SendICJC();							//发送IC检测，返回IC信息ICXX
	void SendZHSQ(DWORD dAppAddr);				//发送自毁申请。!慎用
	void SendXTZJ(DWORD dAppAddr,long iFre);	//发送系统自检，返回自检信息ZJXX
	void SendGXDQ(DWORD dAppAddr);				//发送管信读取申请，返回管理信息GLXX
	void SendGXZR(char * strMangeInfo);			//修改管理信息，返回修改后的管理信息GLXX
	int SendTXSQ(int nSelleibie,int nSelbianma,unsigned long lReceiveID,char * strMsgContent);		//发送通信申请
	int SendTXSQ(BDS_TXSQ txsqData);			//发送通信申请
	void SendDWSQ(BDS_DWSQ dwsqData);			//发送定位申请，返回定位信息DWXX
	void SendTXCX(int nSearchType,long lAddress,long lLocalAddr);							//发送通信查询
	void SendGLJC(DWORD dAppAddr,long iFre);							//发送功率检测，返回GLZK
	void SendJSZL(DWORD dAppAddr);							//发送结束指令
	void SendBBDQ(DWORD dAppAddr);							//发送版本读取指令
	char * GetStrAt(char * resource,int npos,int len);
//	void SetSendMsgID(MSG_Q_ID msgid);

	unsigned char m_BDReceBuf[1024];
private:
//	MSG_Q_ID 	m_SendMsg_ID;

	JNIEnv * m_jniEnv;
	CJniDataSwitchCls m_JniDataSwitchCls;

	bool SendMessage(BDIPortData *pPortData);

};

#endif // !defined(AFX_BDALL_H__D99E2286_DCF4_4FBB_85B7_D60355983A2B__INCLUDED_)
