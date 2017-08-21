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

	//设置下载文件，返回文件大小
	long setFile(char *filename);

	//处理接收的数据
	void onCommunication(char *buf,int len);

	//Flash 选择，1：2M，0：4M
	void set2Mflag(int flag);

	//唤醒该进程
	void OnResume(int type);
	
private:

	//通知父窗口下载进度，pos已下载文件大小
	void notifyParentDownloadProgress(long pos);

	//通知父窗口下载状态
	void notifyParentDownloadStatus(int status);

	//发送数据
	void writeDataToDevice(char *buf,long len);

	void procssReadyProtocol();

	void procssStarProtocol();

	void procssVerifyProtocol();

	void procssDataProtocol(char * buf,int len);

	int pySleep(int millsecond,int type);


private:

	//文件大小
	long m_filesize;

	//已发送文件大小
	long m_downedSize;

	//当前状态
	int m_curStatus;

	JNIEnv * m_env;
};

#endif // !defined(AFX_DOWNLOADCPURD_H__49F3442F_6913_443B_ACFA_37B7661442B5__INCLUDED_)
