#include "SomeStruct.h"
#include "jni.h"

#define PROTOCOL_MAX_LENGTH	100
#define PROTOCOL_MIN_LENGTH	100

class CDownLoadCpu
{
public:

	//���ݽ�������
	long OnCommunication(unsigned char ch, int len);

	void WriteToPort(DWORD lengh,unsigned char *buf);

	//���������ļ��������ļ��Ĵ�С������ļ������ڣ�����0
	long setCpuFile(char *filename);

	//֪ͨ�����ڵ�ǰ�Ѿ����ص��ļ���С
	void notifyParentProgress(long curpos);

	//׼����ʼ���س�����
	void prepareToBegin();

	//֪ͨ����������״̬�����仯
	void notifyParentStatusChanged(int statuafterchange);

	//���͵�һ������
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
