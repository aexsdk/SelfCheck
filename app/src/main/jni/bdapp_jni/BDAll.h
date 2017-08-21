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


	void SendXHDQ(DWORD dAppAddr);				//������Ŷ�ȡ�����������ϢXHXX
	void SendSJSC(int iFre,DWORD dAppAddr);		//����ʱ��������룬����ʱ����ϢSJXX
	void SendICJC();							//����IC��⣬����IC��ϢICXX
	void SendZHSQ(DWORD dAppAddr);				//�����Ի����롣!����
	void SendXTZJ(DWORD dAppAddr,long iFre);	//����ϵͳ�Լ죬�����Լ���ϢZJXX
	void SendGXDQ(DWORD dAppAddr);				//���͹��Ŷ�ȡ���룬���ع�����ϢGLXX
	void SendGXZR(char * strMangeInfo);			//�޸Ĺ�����Ϣ�������޸ĺ�Ĺ�����ϢGLXX
	int SendTXSQ(int nSelleibie,int nSelbianma,unsigned long lReceiveID,char * strMsgContent);		//����ͨ������
	int SendTXSQ(BDS_TXSQ txsqData);			//����ͨ������
	void SendDWSQ(BDS_DWSQ dwsqData);			//���Ͷ�λ���룬���ض�λ��ϢDWXX
	void SendTXCX(int nSearchType,long lAddress,long lLocalAddr);							//����ͨ�Ų�ѯ
	void SendGLJC(DWORD dAppAddr,long iFre);							//���͹��ʼ�⣬����GLZK
	void SendJSZL(DWORD dAppAddr);							//���ͽ���ָ��
	void SendBBDQ(DWORD dAppAddr);							//���Ͱ汾��ȡָ��
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
