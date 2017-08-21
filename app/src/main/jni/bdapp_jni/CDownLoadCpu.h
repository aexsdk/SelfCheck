#include "SomeStruct.h"
#include "jni.h"

#define PROTOCOL_MAX_LENGTH	100
#define PROTOCOL_MIN_LENGTH	100

class CDownLoadCpu
{
public:

	//数据交互过程
	long OnCommunication(unsigned char ch, int len);

	void WriteToPort(DWORD lengh,unsigned char *buf);

	//设置下载文件，返回文件的大小，如果文件不存在，返回0
	long setCpuFile(char *filename);

	//通知父窗口当前已经下载的文件大小
	void notifyParentProgress(long curpos);

	//准备开始下载程序了
	void prepareToBegin();

	//通知父窗口下载状态发生变化
	void notifyParentStatusChanged(int statuafterchange);

	//发送第一包数据
	void sendFirstPackage();

	CDownLoadCpu(JNIEnv *env);

private:
	

	DWORD	m_cpu_file_length;
	DWORD	m_dsp_file_length;
	DWORD	m_fpga_file_length;

	DWORD	m_cpu_tx_count;
	DWORD	m_dsp_tx_count;
	DWORD	m_fpga_tx_count;

	JNIEnv * m_env;

};
