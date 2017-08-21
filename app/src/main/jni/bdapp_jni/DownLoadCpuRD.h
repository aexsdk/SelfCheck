// DownLoadCpuRD.h: interface for the CDownLoadCpuRD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADCPURD_H__49F3442F_6913_443B_ACFA_37B7661442B5__INCLUDED_)
#define AFX_DOWNLOADCPURD_H__49F3442F_6913_443B_ACFA_37B7661442B5__INCLUDED_

#include "jni.h"

class CDownLoadCpuRD  
{
public:
	CDownLoadCpuRD(JNIEnv *env);
	virtual ~CDownLoadCpuRD();

	//���������ļ��������ļ���С
	long setFile(char *filename);

	//������յ�����
	void onCommunication(char *buf,int len);

	//Flash ѡ��1��2M��0��4M
	void set2Mflag(int flag);

	//���Ѹý���
	void OnResume(int type);
	
private:

	//֪ͨ���������ؽ��ȣ�pos�������ļ���С
	void notifyParentDownloadProgress(long pos);

	//֪ͨ����������״̬
	void notifyParentDownloadStatus(int status);

	//��������
	void writeDataToDevice(char *buf,long len);

	void procssReadyProtocol();

	void procssStarProtocol();

	void procssVerifyProtocol();

	void procssDataProtocol(char * buf,int len);

	int pySleep(int millsecond,int type);


private:

	//�ļ���С
	long m_filesize;

	//�ѷ����ļ���С
	long m_downedSize;

	//��ǰ״̬
	int m_curStatus;

	JNIEnv * m_env;
};

#endif // !defined(AFX_DOWNLOADCPURD_H__49F3442F_6913_443B_ACFA_37B7661442B5__INCLUDED_)
