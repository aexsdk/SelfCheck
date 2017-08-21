#include "CDownLoadCpu.h"
#include "memory.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "JniDataSwitchCls.h"

#include <android/log.h>

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define PROTOCOL_MAX_LENGTH	100
#define PROTOCOL_MIN_LENGTH	100

namespace DownLoadCpuNamespace
{
#define PROTOCOL_MAX_LENGTH	100
#define PROTOCOL_MIN_LENGTH	100

	const long max_package_len = 0x10000;

	unsigned char m_pre_tx_buf[max_package_len+12] = {0};
	long m_pre_tx_count = 0;

	CJniDataSwitchCls m_JniDataSwitchCls;
	
	unsigned char rx_buf[100]={0};
	unsigned int rx_count=0;
	unsigned int rx_lenth=PROTOCOL_MAX_LENGTH;
	
	bool cpu_mode = true;
	bool dsp_mode = false;
	bool fpga_mode = false;
	
	unsigned char Comm_status = 0;
	
	unsigned long cpu_tx_count = 0;				// 发送计数
	unsigned long dsp_tx_count = 0;				// 发送计数
	unsigned long fpga_tx_count = 0;			// 发送计数
	
	unsigned char *CPU_TxBuff = NULL;			// 下载缓存
	unsigned char *DSP_TxBuff = NULL;			// 下载缓存
	unsigned char *FPGA_TxBuff = NULL;			// 下载缓存
  
	
	unsigned long cpu_addr = 0x10000;
	unsigned long dsp_addr = 0x1F0000;
	unsigned long fpga_addr = 0x100000;

	FILE * cpu_file = NULL;

}

using namespace DownLoadCpuNamespace;

CDownLoadCpu::CDownLoadCpu(JNIEnv *env)
{
	CJniDataSwitchCls jnicls(env);
	m_JniDataSwitchCls = jnicls;

	m_env = env;
}

void CDownLoadCpu::notifyParentProgress(long curpos)
{
	jclass cls = m_JniDataSwitchCls.GetClass("JniDataPrase");
	if (cls != NULL)
	{
		jmethodID mid = m_env->GetStaticMethodID(cls,"DowndLoadCpuStatusChange","(IJ)V");
		if (mid != NULL)
		{
			m_env->CallStaticVoidMethod(cls,mid,1,(jlong)curpos);
		}
	}
}

void CDownLoadCpu::notifyParentStatusChanged(int statuafterchange)
{
	jclass cls = m_JniDataSwitchCls.GetClass("JniDataPrase");
	if (cls != NULL)
	{
		jmethodID mid = m_env->GetStaticMethodID(cls,"DowndLoadCpuStatusChange","(IJ)V");
		if (mid != NULL)
		{
			m_env->CallStaticVoidMethod(cls,mid,0,(jlong)statuafterchange);
		}
	}
}

void CDownLoadCpu::prepareToBegin()
{
	if (Comm_status == 3)
	{
		Comm_status=0x0;
		cpu_mode =true;
		dsp_mode =false;
        fpga_mode =false;
	}

	notifyParentProgress(0);

	Comm_status=1;
}

long CDownLoadCpu::setCpuFile(char *filename)
{
	if (filename == NULL)
	{
		return 0;
	}
	if (cpu_file != NULL)
	{
		fclose(cpu_file);
		cpu_file = NULL;
	}
	cpu_file = fopen(filename,"r");
	if (cpu_file == NULL)
	{
		return 0;
	}

	fseek(cpu_file,0,SEEK_END);
	
	m_cpu_file_length = ftell(cpu_file);

	fseek(cpu_file,0,SEEK_SET);

	if (m_cpu_file_length == 0)
	{
		return 0;
	}
	if (m_cpu_file_length>0x100000)
	{
		return -1;
	}

	prepareToBegin();

	return m_cpu_file_length;

}

void CDownLoadCpu::WriteToPort(DWORD lengh,unsigned char *buf)
{
	m_JniDataSwitchCls.WriteToDeviceEx((char *)buf,lengh);

	memcpy(m_pre_tx_buf,buf,sizeof(unsigned char)*lengh);
	m_pre_tx_count = lengh;

	notifyParentProgress(m_cpu_tx_count);
}

long CDownLoadCpu::OnCommunication(unsigned char ch, int len)
{
	unsigned char tx_buf[0x10100]={0};
	unsigned long tx_count=0;

	int i = 0;

	rx_buf[rx_count] = ch;
	rx_count++;
	
	if (rx_count == 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (rx_buf[i] == '$')
				break;
		}
		if (i == 0)
		{                                 /* 信息有效性判断 */
			rx_lenth = (rx_buf[5]<<16)|(rx_buf[6]<<8)|rx_buf[7];

			if((rx_lenth< 1) || (rx_lenth > 20))
			{
				rx_count = 0;
				rx_lenth = 0;
//                MessageBox("通信错误！！！","警告");

				LOGI("communicate error!!!");
			}
		}
		else
		{
			for (rx_count = 0; i < 8; rx_count++, i++)
				rx_buf[rx_count]=rx_buf[i];
		}
	}
	
	if (rx_count >= rx_lenth)
	{

		rx_count = 0;
		rx_lenth = PROTOCOL_MAX_LENGTH;
		
        if((rx_buf[0]=='$')&(rx_buf[1]=='Y')&(rx_buf[2]=='D')&(rx_buf[3]=='X')&(rx_buf[4]=='H'))	//应答信号
		{

			Comm_status=2;

			notifyParentStatusChanged(Comm_status);

			//发送首帧

			unsigned char tx_buf[0x10100]={0};
			unsigned long tx_count=0;
			
			cpu_tx_count = 0;
			m_cpu_tx_count = 0;
			
			tx_buf[0]= '$';
			tx_buf[1]= 'W';
			tx_buf[2]= 'J';
			tx_buf[3]= 'C';
			tx_buf[4]= 'S';
			
			if(cpu_mode)
			{
				tx_buf[8]= (unsigned char)(((cpu_addr>>16)+7)>>8);		
				tx_buf[9]= (unsigned char)((cpu_addr>>16)+7);
				
				if(m_cpu_file_length<=max_package_len)
					cpu_tx_count=m_cpu_file_length;
				else
					cpu_tx_count=max_package_len;
				
				if (cpu_file != NULL)
				{
					if (CPU_TxBuff != NULL)
					{
						delete[] CPU_TxBuff;
						CPU_TxBuff = NULL;
					}
					
					CPU_TxBuff = new unsigned char[cpu_tx_count-m_cpu_tx_count+1];
					
					memset(CPU_TxBuff,0,sizeof(unsigned char)*(cpu_tx_count-m_cpu_tx_count+1));
					
					fseek(cpu_file,0,SEEK_SET);
					
					fread(CPU_TxBuff,sizeof(unsigned char),cpu_tx_count - m_cpu_tx_count,cpu_file);
				}
				
				memcpy(&tx_buf[10],CPU_TxBuff,cpu_tx_count - m_cpu_tx_count);
				
				tx_count=cpu_tx_count+10;
				
				Comm_status=3;
				
//		str.Format("在线编程3.0: COM%d,115200bps 正在下载：CPU",m_com+1);
//		SetWindowText(str);
			}
			
			tx_count++;       //校验和
			
			tx_buf[5]= (unsigned char)(tx_count>>16);
			tx_buf[6]= (unsigned char)(tx_count>>8);
			tx_buf[7]= (unsigned char)tx_count;
			
			tx_buf[tx_count-1]=0;
			for(unsigned long j=0;j<tx_count-1;j++)
				tx_buf[tx_count-1]^=tx_buf[j];
			
			WriteToPort(tx_count,tx_buf);

//			SetWindowText(str);
//			GetDlgItem(IDC_BUTTON5)->SetWindowText("中 止 操 作");
			
		}
		else if((rx_buf[0]=='$')&(rx_buf[1]=='S')&(rx_buf[2]=='X')&(rx_buf[3]=='J')&(rx_buf[4]=='D'))	//烧写进度
		{		
			LOGI("$SXJD");
			if(Comm_status==3)
			{
				m_cpu_tx_count=cpu_tx_count;

				if(cpu_mode)
				{
					if(cpu_tx_count>=m_cpu_file_length)
					{
										
//						MessageBox("程序下载完毕!!!","提示");
//						GetDlgItem(IDC_BUTTON5)->SetWindowText("自 动 编 程");

						Comm_status = 0;
						
						LOGI("download over!");
						
						delete[] CPU_TxBuff;
						CPU_TxBuff = NULL;

						if (cpu_file != NULL)
						{
							fseek(cpu_file,0,SEEK_SET);		//置到文件开头
						}
						
						notifyParentProgress(cpu_tx_count);
						notifyParentStatusChanged(Comm_status);

						return 0;
						
					}
					else
					{
						LOGI("SXCPU...");
						if((cpu_tx_count+max_package_len)<=m_cpu_file_length)
							cpu_tx_count+=max_package_len;
						else
							cpu_tx_count=m_cpu_file_length;	
						
						tx_buf[8]= (unsigned char)((((cpu_addr+m_cpu_tx_count)>>16)+7)>>8);
						tx_buf[9]= (unsigned char)(((cpu_addr+m_cpu_tx_count)>>16)+7);

						if (cpu_file != NULL)
						{
							if (CPU_TxBuff != NULL)
							{
								delete[] CPU_TxBuff;
								CPU_TxBuff = NULL;
							}
							
							CPU_TxBuff = new unsigned char[cpu_tx_count-m_cpu_tx_count+1];
							
							memset(CPU_TxBuff,0,sizeof(unsigned char)*(cpu_tx_count-m_cpu_tx_count+1));

							fseek(cpu_file,m_cpu_tx_count,SEEK_SET);
							
							fread(CPU_TxBuff,sizeof(unsigned char),cpu_tx_count - m_cpu_tx_count,cpu_file);
						}
						
						memcpy(&tx_buf[10],CPU_TxBuff,cpu_tx_count-m_cpu_tx_count);
						
						tx_count=cpu_tx_count-m_cpu_tx_count+10;
						
//						str.Format("在线编程3.0: COM%d,115200bps 正在下载：CPU",m_com+1);
					}
				}
			}
		
			tx_count++;       //校验和

			tx_buf[0]= '$';
			tx_buf[1]= 'W';
			tx_buf[2]= 'J';
			tx_buf[3]= 'C';
			tx_buf[4]= 'S';

			tx_buf[5]= (unsigned char)(tx_count>>16);
			tx_buf[6]= (unsigned char)(tx_count>>8);
			tx_buf[7]= (unsigned char)tx_count;
			
			tx_buf[tx_count-1]=0;
			for(unsigned long j=0;j<tx_count-1;j++)
				tx_buf[tx_count-1]^=tx_buf[j];
			
			WriteToPort(tx_count,tx_buf);
			
		}
		else if((rx_buf[0]=='$')&(rx_buf[1]=='C')&(rx_buf[2]=='X')&(rx_buf[3]=='C')&(rx_buf[4]=='S'))	//重新传输
		{
			static int cxcs_count=0;
			if(cxcs_count==3)
			{
				cxcs_count=0;
				Comm_status=0;

				LOGI("download error!!!");

				Comm_status = 4;
				notifyParentStatusChanged(Comm_status);
				return 0;
			}
			else if (cxcs_count < 3)
			{
				WriteToPort(m_pre_tx_count,m_pre_tx_buf);
				cxcs_count++;
			}
			else
			{
				cxcs_count = 4;
			}
			
		}

		notifyParentStatusChanged(Comm_status);
	}

	return 0;
}

void CDownLoadCpu::sendFirstPackage()
{
	unsigned char tx_buf[0x10100]={0};
	unsigned long tx_count=0;

	cpu_tx_count = 0;
	m_cpu_tx_count = 0;
	
	tx_buf[0]= '$';
	tx_buf[1]= 'W';
	tx_buf[2]= 'J';
	tx_buf[3]= 'C';
	tx_buf[4]= 'S';
	
	if(cpu_mode)
	{
		tx_buf[8]= (unsigned char)(((cpu_addr>>16)+7)>>8);		
		tx_buf[9]= (unsigned char)((cpu_addr>>16)+7);
		
		if(m_cpu_file_length<=max_package_len)
			cpu_tx_count=m_cpu_file_length;
		else
			cpu_tx_count=max_package_len;
		
		if (cpu_file != NULL)
		{
			if (CPU_TxBuff != NULL)
			{
				delete[] CPU_TxBuff;
				CPU_TxBuff = NULL;
			}
			
			CPU_TxBuff = new unsigned char[cpu_tx_count-m_cpu_tx_count+1];
			
			memset(CPU_TxBuff,0,sizeof(unsigned char)*(cpu_tx_count-m_cpu_tx_count+1));
			
			fseek(cpu_file,0,SEEK_SET);
			
			fread(CPU_TxBuff,sizeof(unsigned char),cpu_tx_count - m_cpu_tx_count,cpu_file);
		}
		
		memcpy(&tx_buf[10],CPU_TxBuff,cpu_tx_count - m_cpu_tx_count);
		
		tx_count=cpu_tx_count+10;
		
		Comm_status=3;
		
//		str.Format("在线编程3.0: COM%d,115200bps 正在下载：CPU",m_com+1);
//		SetWindowText(str);
	}
	
	tx_count++;       //校验和
	
	tx_buf[5]= (unsigned char)(tx_count>>16);
	tx_buf[6]= (unsigned char)(tx_count>>8);
	tx_buf[7]= (unsigned char)tx_count;
	
	tx_buf[tx_count-1]=0;
	for(unsigned long j=0;j<tx_count-1;j++)
		tx_buf[tx_count-1]^=tx_buf[j];
	
	WriteToPort(tx_count,tx_buf);
}