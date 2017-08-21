// DownLoadCpuRD.cpp: implementation of the CDownLoadCpuRD class.
//
//////////////////////////////////////////////////////////////////////

#include "DownLoadCpuRD.h"
#include "SomeStruct.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "JniDataSwitchCls.h"

#include <android/log.h>

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace DOWNLOADCPURD_NAMESPACE
{
	//下载的CPU文件
	FILE * m_FILE = NULL;

	CJniDataSwitchCls m_JniDataSwitchCls;

	const int RX_BUF_MAX_SIZE = 512;
	char g_RX_BUF[RX_BUF_MAX_SIZE] = {0};
	
	int g_RX_BUF_RS = 0;

	const long g_package_data_len = 32*1024;		//32k
	
	int g_Tail_byte_counts = 0;

	int g_2M_flag = 1;

	const unsigned char g_tail_buf[] = {
			0xC6,
			0xDF,
			0xC6,
			0xDF,
			0xB6,
			0xFE,
			0xCB,
			0xF9,
			0xB5,
			0xBC,
			0xBA,
			0xBD,
			0xD0,
			0xBE,
			0xC6,
			0xAC,
			0xB2,
			0xBF,
			0x6C,
			0x69,
			0x6A,
			0x75,
			0x6E,
			0x63,
			0x68,
			0x65,
			0x6E,
			0x24
	};

};


using namespace DOWNLOADCPURD_NAMESPACE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CDownLoadCpuRD::pySleep(int millsecond,int type)
{
	jclass cls = m_JniDataSwitchCls.GetClass("JniDataPrase");
	if (cls != NULL)
	{
		jmethodID mid = m_env->GetStaticMethodID(cls,"pyJniSleep","(II)I");
		if (mid != NULL)
		{
			jint res = m_env->CallStaticIntMethod(cls,mid,millsecond,type);
			return res;
		}
	}
	else
	{
		return -2;
	}
}

void CDownLoadCpuRD::OnResume(int type)
{
	if (type == 0)		//recv ready
	{
		writeDataToDevice("Y",1);
	}
	else if (type > 0 && type < 14)
	{
		int index = type - 1;

		if (index == 12)		//下载完毕
		{
			g_Tail_byte_counts = 0;
			
			m_curStatus = 0;
			
			m_downedSize = m_filesize;
			
			notifyParentDownloadProgress(m_downedSize);
			
			notifyParentDownloadStatus(m_curStatus);
			
			if (m_FILE != NULL)
			{
				fclose(m_FILE);
				m_FILE = NULL;
			}
			
			return;
		}
		
		//////////////////////////////////////////////////////////////////////////
		
		char * buf = new char[g_package_data_len+2];
		if (buf == NULL)
		{
			return;
		}
		memset(buf,0,sizeof(char)*(g_package_data_len+2));
		
		fseek(m_FILE,0,SEEK_SET);
		
		{
			if (g_2M_flag)
			{
				fseek(m_FILE,g_package_data_len*(index%8),SEEK_SET);
			}
			else
			{
				fseek(m_FILE,g_package_data_len*index,SEEK_SET);
			}
			
			
			int len = fread(buf,sizeof(char),g_package_data_len,m_FILE);
			
			if (len < g_package_data_len)
			{
				for (int i = len;i < g_package_data_len;i++)
				{
					if (g_Tail_byte_counts < 28)
					{
						buf[i] = g_tail_buf[g_Tail_byte_counts++];
					}
					else{
						buf[i] = 0xff;
					}
					
				}
			}
			
		}
		
		for (int i = 0;i < g_package_data_len;i++)
		{
			buf[g_package_data_len] ^= buf[i];
		}
		
		writeDataToDevice(buf,g_package_data_len+1);
	
//		writeDataToDevice(buf,g_package_data_len);
		m_downedSize = m_filesize*index/12;
		
		notifyParentDownloadProgress(m_downedSize);
		
		delete[] buf;
		buf = NULL;
		
		
		//////////////////////////////////////////////////////////////////////////
	}
}

CDownLoadCpuRD::CDownLoadCpuRD(JNIEnv *env)
{
	this->m_env = env;

	CJniDataSwitchCls jnicls(env);
	
	m_JniDataSwitchCls = jnicls;
}

CDownLoadCpuRD::~CDownLoadCpuRD()
{

}

void CDownLoadCpuRD::set2Mflag(int flag)
{
	g_2M_flag = flag;
}

long CDownLoadCpuRD::setFile(char *filename)
{
	g_Tail_byte_counts = 0;

	g_RX_BUF_RS = 0;

	if (filename != NULL)
	{
		if (m_FILE != NULL)
		{
			fclose(m_FILE);
			m_FILE = NULL;
		}
		
		if ((m_FILE = fopen(filename,"rb")) != NULL)
		{
			fseek(m_FILE,0,SEEK_END);
			long filesize = ftell(m_FILE);
			
			fseek(m_FILE,0,SEEK_SET);
			
			if(filesize%2==0)
			{
				m_filesize=filesize;
				if((m_filesize>393216)||(m_filesize<1))
				{
 					//"bin文件大小超出合法范围!","错误提示";
					fclose(m_FILE);
					m_FILE = NULL;

					return -2;
				}
				
				m_curStatus = 1;		//文件合法
				notifyParentDownloadStatus(m_curStatus); 
				
				return m_filesize;
			}
			else 
			{
 				//"bin文件大小不合理!\n请重新打开文件","错误提示";
				fclose(m_FILE);
				m_FILE = NULL;

				return -1;
			}
		}
		else
		{
			//文件打开失败

			return 0;
		}
	}

	return 0;
}

void CDownLoadCpuRD::onCommunication(char *buf,int len)
{
#if 0
	if (g_RX_BUF_RS + len > RX_BUF_MAX_SIZE)
	{
		g_RX_BUF_RS = 0;
	}

	if (len > RX_BUF_MAX_SIZE)
	{
		return;
	}

	memcpy(g_RX_BUF+g_RX_BUF_RS,buf,sizeof(char)*len);
	int counts = g_RX_BUF_RS + len;

	bool findheader = false;
	int beginpos = 0;

	for (int i = 0;i < counts;i++)
	{
		if (!findheader)
		{
			if (g_RX_BUF[i] == 'r' ||
				g_RX_BUF[i] == 's' ||
				g_RX_BUF[i] == 'v' ||
				g_RX_BUF[i] == 'd')
			{
				findheader = true;
				beginpos = i;
			}
		}
		if (findheader)
		{
			if (counts - beginpos >= 8)
			{
				if (strncmp(g_RX_BUF+beginpos,"ready?\r\n",8) == 0)		//ready?\r\n
				{
					procssReadyProtocol();
					i += 8;
					findheader = false;
				}
				else if (strncmp(g_RX_BUF+beginpos,"start!\r\n",8) == 0)	//start!\r\n
				{
					procssStarProtocol();
					i += 8;
					findheader = false;
				}
				else if (strncmp(g_RX_BUF+beginpos,"verify",6) == 0)	//verify_failed\r\n
				{
					if (counts - beginpos >= 15)
					{
						procssVerifyProtocol();
						i += 15;
						findheader = false;
					}
					else
					{
						break;
					}
				}
				else if (strncmp(g_RX_BUF+beginpos,"data",4) == 0)	//data_er_1\r\n | data_ok_1\r\n ...
				{
					if (counts - beginpos >= 11)
					{
						procssDataProtocol(g_RX_BUF+beginpos,11);
						i += 11;
						findheader = false;
					}
					else
					{
						break;
					}
				}
				else
				{
					i+= 8;
					findheader = false;
				}
			}
			else
			{
				break;
			}
		}
		
	}

	if (findheader)
	{
		int rs = counts - beginpos;

		memmove(g_RX_BUF,g_RX_BUF+beginpos,sizeof(char)*rs);

		g_RX_BUF_RS = rs;
	}
	else
	{
		g_RX_BUF_RS = 0;
	}
#if 0
	if (g_RX_BUF_RS + len > RX_BUF_MAX_SIZE)
	{
		g_RX_BUF_RS = 0;
	}

	LOGI(buf);
	
	if (len > RX_BUF_MAX_SIZE)
	{
		return;
	}
	
	memcpy(g_RX_BUF+g_RX_BUF_RS,buf,sizeof(char)*len);
	int counts = g_RX_BUF_RS + len;
	
	bool findheader = false;
	int beginpos = 0;
	int endpos = 0;
	
	for (int i = 0;i < counts;i++)
	{
		if (!findheader)
		{
			if (g_RX_BUF[i] == 0x0A)
			{
				if (g_RX_BUF[i-1] == 0x0D)
				{
					findheader = true;
					endpos = i;
				}	
			}
		}
		if (findheader)
		{
			if (endpos - beginpos >= 7)
			{
				if (strncmp(&g_RX_BUF[endpos-7],"ready?\r\n",8) == 0)		//ready?\r\n
				{
					procssReadyProtocol();

					findheader = false;

					beginpos = endpos + 1;
				}
				else if (strncmp(&g_RX_BUF[endpos-7],"start!\r\n",8) == 0)	//start!\r\n
				{
					procssStarProtocol();

					findheader = false;

					beginpos = endpos + 1;
				}
				else
				{
					if (endpos - beginpos >= 10)
					{
						if (strncmp(&g_RX_BUF[endpos-10],"data_",5) == 0)
						{
							procssDataProtocol(&g_RX_BUF[endpos-10],11);
							findheader = false;

							beginpos = endpos + 1;
						}
						else
						{
							;
						}
					}
					if (endpos - beginpos >= 14)
					{
						if (strncmp(&g_RX_BUF[endpos-14],"verify_failed\r\n",15) == 0)
						{
							procssVerifyProtocol();
							findheader = false;

							beginpos = endpos + 1;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
		
	}
	
	if (!findheader)
	{
		int rs = counts - beginpos;

		if (rs != 0)
		{
			memmove(g_RX_BUF,g_RX_BUF+beginpos,sizeof(char)*rs);
		}
		
		g_RX_BUF_RS = rs;
	}
	else
	{
		g_RX_BUF_RS = 0;
	}
#endif
	//////////////////////////////////////////////////////////////////////////
#else
	if (g_RX_BUF_RS + len > RX_BUF_MAX_SIZE)
	{
		g_RX_BUF_RS = 0;
	}

	//LOGI(buf);
	
	if (len > RX_BUF_MAX_SIZE)
	{
		return;
	}
	
	memcpy(g_RX_BUF+g_RX_BUF_RS,buf,sizeof(char)*len);
	int counts = g_RX_BUF_RS + len;

	g_RX_BUF_RS = counts;
	
	bool findtail = false;
	int endpos = 0;
	
	for (int i = counts-1;i >= 0;i--)
	{
		if (!findtail)
		{
			if (g_RX_BUF[i] == 0x0A)
			{
				if (g_RX_BUF[i-1] == 0x0D)
				{
					findtail = true;
					endpos = i;
				}	
			}
		}
		if (findtail)
		{
			if (endpos >= 7)
			{
				if (strncmp(&g_RX_BUF[endpos-7],"ready?\r\n",8) == 0)		//ready?\r\n
				{
					procssReadyProtocol();

					break;
				}
				else if (strncmp(&g_RX_BUF[endpos-7],"start!\r\n",8) == 0)	//start!\r\n
				{
					procssStarProtocol();

					break;
				}
				else
				{
					if (endpos >= 14)
					{
						if (strncmp(&g_RX_BUF[endpos-14],"verify_failed\r\n",15) == 0)
						{
							procssVerifyProtocol();	
						}
						break;
					}

					if (endpos >= 10)
					{
						if (strncmp(&g_RX_BUF[endpos-10],"data_",5) == 0)
						{
							procssDataProtocol(&g_RX_BUF[endpos-10],11);
						}
						break;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
		
	}
	
	if (findtail)
	{
		g_RX_BUF_RS = 0;
	}

	//////////////////////////////////////////////////////////////////////////
#endif

}

void CDownLoadCpuRD::procssReadyProtocol()		//收到ready?\r\n，返回"Y"
{
	g_Tail_byte_counts = 0;

	m_curStatus = 2;
	notifyParentDownloadStatus(m_curStatus);

	pySleep(100,0);
}

void CDownLoadCpuRD::procssStarProtocol()		//准备第一块数据，并发送
{
	m_curStatus = 3;
	notifyParentDownloadStatus(m_curStatus);

	pySleep(200,1);
}

void CDownLoadCpuRD::procssVerifyProtocol()
{

	m_curStatus = 5;
	notifyParentDownloadStatus(m_curStatus);

	if (m_FILE != NULL)
	{
		fclose(m_FILE);
		m_FILE = NULL;
	}

	LOGI("verify_failed!");
	//校验和错误
}

void CDownLoadCpuRD::procssDataProtocol(char * buf,int len)
{
	if (strncmp(buf+5,"er",2) == 0)
	{
		m_curStatus = 4;
		notifyParentDownloadStatus(m_curStatus);

		if (m_FILE != NULL)
		{
			fclose(m_FILE);
			m_FILE = NULL;
		}
	}

	else if (strncmp(buf+5,"ok",2) == 0)
	{
		char ci = buf[8];
		int index = 0;
		if (ci > '0' && ci <= '9')
		{
			index = ci - '0';
		}
		else if (ci >= 'a' && ci <= 'f')
		{
			index = ci - 'a' + 10;
		}
		else if (ci >= 'A' && ci <= 'F')
		{
			index = ci - 'A' + 10;
		}

		index += 1;

		m_curStatus = 3;
		notifyParentDownloadStatus(m_curStatus);

		pySleep(10,index);
	}
}

void CDownLoadCpuRD::notifyParentDownloadProgress(long pos)
{
	jclass cls = m_JniDataSwitchCls.GetClass("JniDataPrase");
	if (cls != NULL)
	{
		jmethodID mid = m_env->GetStaticMethodID(cls,"DowndLoadCpuStatusChange","(IJ)V");
		if (mid != NULL)
		{
			m_env->CallStaticVoidMethod(cls,mid,1,(jlong)pos);
		}
	}
}

void CDownLoadCpuRD::notifyParentDownloadStatus(int status)
{
	jclass cls = m_JniDataSwitchCls.GetClass("JniDataPrase");
	if (cls != NULL)
	{
		jmethodID mid = m_env->GetStaticMethodID(cls,"DowndLoadCpuStatusChange","(IJ)V");
		if (mid != NULL)
		{
			m_env->CallStaticVoidMethod(cls,mid,0,(jlong)status);
		}
	}
}

void CDownLoadCpuRD::writeDataToDevice(char *buf,long len)
{
	m_JniDataSwitchCls.WriteToDeviceEx((char *)buf,len);
	
// 	memcpy(m_pre_tx_buf,buf,sizeof(unsigned char)*lengh);
// 	m_pre_tx_count = lengh;
// 	
// 	notifyParentProgress(m_cpu_tx_count);
}

