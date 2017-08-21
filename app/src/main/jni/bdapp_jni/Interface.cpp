
#include "com_app_dataprase_JniDataPrase.h"
#include "BDDataParse.h"
#include "BDAll.h"
#include "CDownLoadCpu.h"
#include "DownLoadCpuRD.h"
#include "string.h"


#include <android/log.h>
#include "JniDataSwitchCls.h"

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define TYPE_SETTING_DEVICE 0

/*
 *g_ProtocolType
 *当前所用协议类型
 *0：4.0数据协议，1：2.1数据协议
 *默认为2.1数据协议
*/
static short g_ProtocolType = 1;

/*
 *g_WorkMode
 *当前工作模式
 *1:设备正常工作模式，2:设置设备模式，3:下载CPU程序RN,4:下载CPU程序RD
*/
static short g_WorkMode = 1;

namespace PY_JNI_PROCSSDATA
{
#define MAX_DATA_LEN	2048
	char g_chCachBuffer[MAX_DATA_LEN+1];
	int g_iBufferBegin;
	int g_iBufferEnd;

	CDownLoadCpu *pDowLoadCpu = NULL;

	CDownLoadCpuRD * pDownLoadCpuRd = NULL;
};

using namespace PY_JNI_PROCSSDATA;

#if 0
void ProcssData2_1(JNIEnv * env,char * chin,int inlen)
{
	if (inlen+g_iBufferBegin >= MAX_DATA_LEN)
	{
		memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN+1));
		g_iBufferBegin = 0;
	}
	memcpy(g_chCachBuffer+g_iBufferBegin,chin,sizeof(char)*inlen);
	g_iBufferEnd = g_iBufferBegin + inlen;

	bool bfindheader = false;
	int ibegin = 0;

	for (int i = ibegin;i < g_iBufferEnd;i++)
	{
		if (g_chCachBuffer[i] == 0x24)		//找到头了
		{
			ibegin = i;
			bfindheader = true;
		}
		if (bfindheader)
		{
			if (g_chCachBuffer[i] == 0x0A && g_chCachBuffer[i-1] == 0x0D)	//完整一包数据
			{
				char chData[512] = {0};
				int len = i-ibegin+1;
				memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*len);

				CBDDataParse dataparse(env);
				dataparse.Parse((unsigned char *)chData,len);

				bfindheader = false;
			}
		}
	}
	if (bfindheader)
	{
		memmove(g_chCachBuffer,g_chCachBuffer+ibegin,sizeof(char)*(g_iBufferEnd - ibegin+1));
		g_iBufferBegin = g_iBufferEnd - ibegin;
	}
	else
	{
		memset(g_chCachBuffer,0,sizeof(char)*MAX_DATA_LEN);
		g_iBufferBegin = 0;
	}
}


void ProcssData4_0(JNIEnv * env,char * chin,int inlen)
{
	if (inlen+g_iBufferBegin >= MAX_DATA_LEN)
	{
		memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN+1));
		g_iBufferBegin = 0;
	}
	memcpy(g_chCachBuffer+g_iBufferBegin,chin,sizeof(char)*inlen);
	g_iBufferEnd = g_iBufferBegin + inlen;

	bool bfindheader = false;
	int ibegin = 0;

	for (int i = ibegin;i < g_iBufferEnd;/*i++*/)
	{
		if (g_chCachBuffer[i] == 0x24)		//找到头了
		{
			ibegin = i;
			bfindheader = true;
			if (g_iBufferEnd - i < 7)
			{
				break;
			}
		}
		else
		{
			i++;
			continue;
		}
		if (bfindheader)
		{
			int len = g_chCachBuffer[ibegin+5] << 8;
			len += g_chCachBuffer[ibegin+6];

			if (g_iBufferEnd - ibegin >= len)
			{

				char chData[512] = {0};
				memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*len);

				CBDAll dataprase(env);
				dataprase.ProcessData(len,(unsigned char *)chData);

				bfindheader = false;
				i = ibegin + len;
			}
			else
			{
				break;
			}
		}
	}
	if (bfindheader)
	{
		memmove(g_chCachBuffer,g_chCachBuffer+ibegin,sizeof(char)*(g_iBufferEnd - ibegin+1));
		g_iBufferBegin = g_iBufferEnd - ibegin;
	}
	else
	{
		memset(g_chCachBuffer,0,sizeof(char)*MAX_DATA_LEN);
		g_iBufferBegin = 0;
	}
}
#endif


void ProcssData(JNIEnv * env,char * chin,int inlen)
{
	LOGI(" in ProcssData function");
	if (inlen+g_iBufferBegin >= MAX_DATA_LEN)
	{
		memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN+1));
		g_iBufferBegin = 0;
	}
	memcpy(g_chCachBuffer+g_iBufferBegin,chin,sizeof(char)*inlen);
	g_iBufferEnd = g_iBufferBegin + inlen;

	bool bfindheader = false;
	int ibegin = 0;

	short curproto = -1;		//1:2.1,0:4.0

	//LOGI(g_chCachBuffer);
	char testLen[20] = {0};
	sprintf(testLen,"len is %d",strlen(g_chCachBuffer));
	//LOGI(testLen);

	for (int i = ibegin;i < g_iBufferEnd;)
	{
		if (!bfindheader)
		{
			if (g_chCachBuffer[i] == 0x24)		//找到头了
			{
				ibegin = i;
				bfindheader = true;
			}
			else
			{
				i++;
				continue;
			}
		}

		if (bfindheader)
		{
			i++;

			if (curproto == -1)
			{
				if (g_iBufferEnd - ibegin > 5)
				{
					if ((g_chCachBuffer[ibegin+1] == 'B' && g_chCachBuffer[ibegin+2] == 'D') ||
						(g_chCachBuffer[ibegin+1] == 'G' && g_chCachBuffer[ibegin+2] == 'P') ||
						(g_chCachBuffer[ibegin+1] == 'G' && g_chCachBuffer[ibegin+2] == 'N') ||
						(g_chCachBuffer[ibegin+1] == 'G' && g_chCachBuffer[ibegin+2] == 'L') ||
						(g_chCachBuffer[ibegin+1] == 'P' && g_chCachBuffer[ibegin+2] == 'H' && g_chCachBuffer[ibegin+3] == 'X' && g_chCachBuffer[ibegin+4] == 'M' && g_chCachBuffer[ibegin+5] == 'T') ||
						g_chCachBuffer[ibegin+6] == ',')
					{
						curproto = 1;
					}
					else
					{
						curproto = 0;
						LOGI("4.0");
					}
				}
				else
				{
					break;
				}
			}

			if (curproto == 0)		//4.0协议
			{
				if (g_iBufferEnd - ibegin > 7)
				{
					long len = 0;
					//下载CPU RN时
					if ((g_chCachBuffer[ibegin] == '$' && g_chCachBuffer[ibegin+1] == 'Y' && g_chCachBuffer[ibegin+2] == 'D' && g_chCachBuffer[ibegin+3] == 'X' && g_chCachBuffer[ibegin+4] == 'H') ||
						(g_chCachBuffer[ibegin] == '$' && g_chCachBuffer[ibegin+1] == 'S' && g_chCachBuffer[ibegin+2] == 'X' && g_chCachBuffer[ibegin+3] == 'J' && g_chCachBuffer[ibegin+4] == 'D') ||
						(g_chCachBuffer[ibegin] == '$' && g_chCachBuffer[ibegin+1] == 'C' && g_chCachBuffer[ibegin+2] == 'X' && g_chCachBuffer[ibegin+3] == 'C' && g_chCachBuffer[ibegin+4] == 'S')
							)
					{
						len = g_chCachBuffer[ibegin+5] << 16;
						len += g_chCachBuffer[ibegin+6] << 8;
						len += g_chCachBuffer[ibegin+7];

						char buftemp[20] = {0};
						sprintf(buftemp,"len is %d",len);
						//LOGI(buftemp);

						if (g_iBufferEnd - ibegin >= len)
						{
							bfindheader = false;
							i = ibegin + len;
							curproto = -1;
						}
						else
						{
							bfindheader = false;
							break;
						}

					}
					else
					{
						len = g_chCachBuffer[ibegin+5] << 8;
						len += g_chCachBuffer[ibegin+6];

						if (g_iBufferEnd - ibegin >= len)
						{
							char chData[512] = {0};
							memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*len);

							CBDAll dataprase(env);
							dataprase.ProcessData(len,(unsigned char *)chData);

							bfindheader = false;
							i = ibegin + len;
							curproto = -1;
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
			else if (curproto == 1)		//2.1协议
			{
				bool xsflag = false;

				if (i - ibegin > 8)
				{
					if (g_chCachBuffer[ibegin+3] == 'X' && g_chCachBuffer[ibegin+4] == 'S' && g_chCachBuffer[ibegin+5] == 'T' ||
						g_chCachBuffer[ibegin+3] == 'X' && g_chCachBuffer[ibegin+4] == 'S' && g_chCachBuffer[ibegin+5] == 'D' )
					{
						int length = g_chCachBuffer[6]<<8;
						length += g_chCachBuffer[7];

						xsflag = true;

						if (g_iBufferEnd - ibegin >= length)
						{
							char chData[512] = {0};

							memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*length);

							CBDDataParse dataparse(env);
							dataparse.Parse((unsigned char *)chData,length);

							curproto = -1;
							bfindheader = false;
						}
					}
				}
				if (!xsflag)
				{
					if (g_chCachBuffer[i] == 0x0A && g_chCachBuffer[i-1] == 0x0D)	//完整一包数据
					{
						char chData[512] = {0};
						int len = i-ibegin+1;
						memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*len);

						CBDDataParse dataparse(env);
						dataparse.Parse((unsigned char *)chData,len);

						curproto = -1;
						bfindheader = false;
					}
				}
			}

		}
	}
	if (bfindheader)         //add by zhs 20170613，找到了协议头，但是还没有协议尾，因此此条协议暂时不处理，保留到协议尾出现再处理。
	{
		memmove(g_chCachBuffer,g_chCachBuffer+ibegin,sizeof(char)*(g_iBufferEnd - ibegin+1));
		g_iBufferBegin = g_iBufferEnd - ibegin;
	}
	else                     //add by zhs 20170613，没有找到协议头，那么清空缓冲数组g_chCachBuffer
	{
		memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN + 1));
		g_iBufferBegin = 0;
	}
}


void ProcssSettingData(JNIEnv * env,char * chin,int inlen)
{
	if (inlen+g_iBufferBegin >= MAX_DATA_LEN)
	{
		memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN+1));
		g_iBufferBegin = 0;
	}
	memcpy(g_chCachBuffer+g_iBufferBegin,chin,sizeof(char)*inlen);
	g_iBufferEnd = g_iBufferBegin + inlen;

	bool bfindheader = false;
	int ibegin = 0;

	for (int i = ibegin;i < g_iBufferEnd;)
	{
		if (!bfindheader)
		{
			if (g_chCachBuffer[i] == 'A' && g_chCachBuffer[i+1] == 'T')		//找到头了
			{
				ibegin = i;
				bfindheader = true;
			}
			else
			{
				i++;
				continue;
			}
		}

		if (bfindheader)
		{
			i++;
			if (g_iBufferEnd - ibegin > 7)
			{
				if (i - ibegin > 7)
				{
					if (g_chCachBuffer[i] == 0x0A &&
						g_chCachBuffer[i-1] == 0x0D &&
						g_chCachBuffer[i-2] == 0x0A &&
						g_chCachBuffer[i-3] == 0x0D)
					{
						char chData[512] = {0};
						int len = i-ibegin+1;
						memcpy(chData,g_chCachBuffer+ibegin,sizeof(char)*len);

						jclass jstrcls = env->FindClass("java/lang/String");
						if (jstrcls == NULL)
						{
							break;
						}
						jmethodID jstriniid = env->GetMethodID(jstrcls,"<init>","([B)V");
						{
							if (jstriniid == NULL)
							{
								break;
							}
						}

						jbyteArray jba = env->NewByteArray(len);
						env->SetByteArrayRegion(jba,0,len,(jbyte *)(chData));

						jstring jstr = (jstring)env->NewObject(jstrcls,jstriniid,jba);
						if (jstr == NULL)
						{
							break;
						}

						CJniDataSwitchCls cls(env);
						cls.SendNotifyMessage(TYPE_SETTING_DEVICE,jstr);

					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SetWorkMode
		(JNIEnv *env,jobject obj,jint type)
{
	if (type > 0 && type < 5)
	{
		g_WorkMode = type;
	}
	else
	{
		g_WorkMode = 1;
	}

	memset(g_chCachBuffer,0,sizeof(char)*(MAX_DATA_LEN+1));
}

/* modify by zhs 20161222此函数会导致应用闪退，程序运行一段时间后会调用此函数，导致stack corruption，程序运行一开始是调用下面的同名函数
jint JNICALL Java_com_app_dataprase_JniDataPrase_ProcssData
  (JNIEnv *env, jobject obj, jstring jstr)
{

	char * buf = (char *)env->GetStringUTFChars(jstr,0);
	int len = strlen(buf);

	CBDDataParse dataparse(env);
	if (dataparse.Parse((unsigned char *)buf,len))
	{

	}

	return 0;
}
*/

jint JNICALL Java_com_app_dataprase_JniDataPrase_ProcssData___3BI
		(JNIEnv *env, jobject obj, jbyteArray bytes, jint len)
{
	//20170105 modify by zhs 将之前的数组缓存大小512改为MAX_DATA_LEN（2048），512太小，会造成溢出，拷机闪退，改后正常无闪退。
	jbyte datas[MAX_DATA_LEN];
	memset(datas,0,MAX_DATA_LEN);

	env->GetByteArrayRegion(bytes,0,len,datas);

	if (g_WorkMode == 1)
	{
		ProcssData(env,(char*)datas,len);
	}
	else if (g_WorkMode == 2)
	{
		ProcssSettingData(env,(char*)datas,len);
	}
	else if (g_WorkMode == 3)
	{
		if (pDowLoadCpu == NULL)
		{
			pDowLoadCpu = new CDownLoadCpu(env);
		}
		for (int i = 0;i < len;i++)
		{
			pDowLoadCpu->OnCommunication((unsigned char)datas[i],1);
		}
	}
	else if (g_WorkMode == 4)
	{
		if (pDownLoadCpuRd == NULL)
		{
			pDownLoadCpuRd = new CDownLoadCpuRD(env);
		}
		pDownLoadCpuRd->onCommunication((char*)datas,len);
	}

	return 0;
}

jint JNICALL Java_com_app_dataprase_JniDataPrase_SetProtocolType
		(JNIEnv *env,jobject obj,jint type)
{
	if (type < 0 || type > 1)
	{
		type = 1;
	}

	g_ProtocolType = type;

	memset(g_chCachBuffer,0,sizeof(MAX_DATA_LEN+1)*sizeof(char));
	g_iBufferBegin = 0;
	g_iBufferEnd = 0;

	return 0;
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendTXSQ(JNIEnv *env,jobject obj,jint leib,jint bianm,jlong add,jbyteArray msgcontent)
{
	jbyte datas[512];
	memset(datas,0,512);

	int len = env->GetArrayLength(msgcontent);

	env->GetByteArrayRegion(msgcontent,0,len,datas);

	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendTXSQ(leib,bianm,add,(char *)datas);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendICJC(JNIEnv *env,jobject obj)
{

	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendICJC();
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendXHDQ(JNIEnv *env,jobject obj,jlong addr)
{
	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendXHDQ((DWORD)addr);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendBBDQ(JNIEnv *env,jobject obj,jlong addr)
{
	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendBBDQ((DWORD)addr);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}


void JNICALL Java_com_app_dataprase_JniDataPrase_SendXTZJ(JNIEnv *env,jobject obj,jlong addr)
{
	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendXTZJ((DWORD) addr,5);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendJSZL(JNIEnv *env,jobject obj,jlong addr)
{
	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendJSZL((DWORD) addr);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendGLJC(JNIEnv *env,jobject obj,jlong addr,jint fre)
{
	if (0 == g_ProtocolType)	//4.0协议
	{
		CBDAll dataprase(env);
		dataprase.SendGLJC(addr,fre);
	}
	else if (1 == g_ProtocolType)	//2.1协议
	{
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SendDWSQ(JNIEnv *env,jobject obj,jobject data)
{
	if (g_ProtocolType == 0)	//4.0协议
	{
		CBDAll dataprase(env);

		BDS_DWSQ dwsqdata;

		CJniDataSwitchCls jniswitchcls(env);

		jclass cls = jniswitchcls.GetClass("BDS_DWSQ");
		if (cls == NULL)
		{
			return;
		}

		jfieldID jid = env->GetFieldID(cls,"dControlUserAddr","J");
		dwsqdata.dControlUserAddr = (DWORD)(env->GetLongField(data,jid));

		jid = env->GetFieldID(cls,"cInfoSort","S");
		dwsqdata.cInfoSort = (unsigned char)(env->GetShortField(data,jid));

		jid = env->GetFieldID(cls,"dAirPressure","J");
		dwsqdata.dAirPressure = (DWORD)(env->GetLongField(data,jid));

		jid = env->GetFieldID(cls,"dEntryFrequency","S");
		dwsqdata.dEntryFrequency = (short)(env->GetShortField(data,jid));

		jid = env->GetFieldID(cls,"dHeight","J");
		dwsqdata.dHeight = (long)(env->GetLongField(data,jid));

		dataprase.SendDWSQ(dwsqdata);
	}
	else if (g_ProtocolType == 1)	//2.1协议
	{
	}
}


jlong JNICALL Java_com_app_dataprase_JniDataPrase_SetDownLoadCPUFile(JNIEnv *env,jobject obj,jstring jstr)
{
	if (g_WorkMode == 3)		//下载RN
	{
		if (pDowLoadCpu == NULL)
		{
			pDowLoadCpu = new CDownLoadCpu(env);
		}

		char * filename = (char *)env->GetStringUTFChars(jstr,0);

		long filelength = pDowLoadCpu->setCpuFile(filename);

		return (jlong)filelength;

	}
	else if (g_WorkMode == 4)	//下载RD
	{
		if (pDownLoadCpuRd == NULL)
		{
			pDownLoadCpuRd = new CDownLoadCpuRD(env);
		}

		char * filename = (char *)env->GetStringUTFChars(jstr,0);

		long filelength = pDownLoadCpuRd->setFile(filename);

		return (jlong)filelength;
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_SetDownLoadCPUFlashMode(JNIEnv* env,jobject obj,jint mod)
{
	if (pDownLoadCpuRd == NULL)
	{
		pDownLoadCpuRd = new CDownLoadCpuRD(env);
	}

	pDownLoadCpuRd->set2Mflag(mod);
}

void JNICALL Java_com_app_dataprase_JniDataPrase_DownLoadRNSendFirstPackage(JNIEnv* env,jobject obj)
{
	if (g_WorkMode == 3)
	{
		if (pDowLoadCpu != NULL)
		{
			pDowLoadCpu->sendFirstPackage();
		}
	}
}

void JNICALL Java_com_app_dataprase_JniDataPrase_ResumeJni(JNIEnv* env,jobject obj,jint type)
{
	if (g_WorkMode == 3)
	{
	}
	else if (g_WorkMode == 4)
	{
		if (pDownLoadCpuRd != NULL)
		{
			pDownLoadCpuRd->OnResume(type);
		}
	}
}
