#include "JniDataSwitchCls.h"
#include "BDSDataType.h"

#include <android/log.h>

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

CJniDataSwitchCls::CJniDataSwitchCls()
{
	memset(m_packName,0,sizeof(char)*PACK_NAME_MAX_LEN);
	memset(m_clsName,0,sizeof(char)*PACK_NAME_MAX_LEN);
}

CJniDataSwitchCls::CJniDataSwitchCls(JNIEnv * env)
{
	CJniDataSwitchCls();
	
	sprintf(m_packName,"%s","com/app/dataprase");
		
	sprintf(m_clsName,"%s","com/app/dataprase/JniDataPrase");
	
	m_env = env;
}

void CJniDataSwitchCls::setPackName(char * packname,int len)
{

	memset(m_packName,0,sizeof(char)*PACK_NAME_MAX_LEN);
	if (len < PACK_NAME_MAX_LEN)
	{
		memcpy(m_packName,packname,sizeof(char)*len);
	}
}

CJniDataSwitchCls::~CJniDataSwitchCls()
{
	m_env = NULL;
}

void CJniDataSwitchCls::OnProcssHunHeTx(char * strContent,int len)
{
	unsigned char ch[2] = {0};
	unsigned char *chtemp = new unsigned char[len/2+2];
	memset(chtemp,0,sizeof(unsigned char)*(len/2+2));
	
	int newlen = 0;
	
	for (int k = 2;k < len-1;k=k+2)
	{
		ch[0] = strContent[k]&0xff;
		ch[1] = strContent[k+1]&0xff;
		newlen += sscanf((char*)ch,"%x",(char*)(chtemp+newlen));
		
	}
	
	memset(strContent,0,sizeof(char)*len);
	
	jclass jstrcls = m_env->FindClass("java/lang/String");
	jmethodID jstriniid = m_env->GetMethodID(jstrcls,"<init>","([BIILjava/lang/String;)V");
	
	jbyteArray jba = m_env->NewByteArray(newlen);
	m_env->SetByteArrayRegion(jba,0,newlen,(jbyte *)(char*)(chtemp));
	
	jstring jstrendcoding = m_env->NewStringUTF("gbk");
	
	jstring jstr = (jstring)m_env->NewObject(jstrcls,jstriniid,jba,0,newlen,jstrendcoding);
	
	char * datap = (char *)m_env->GetStringUTFChars(jstr,NULL);
	
	strcpy(strContent,datap);
	
	m_env->ReleaseStringUTFChars(jstr,datap);
	
	delete[] chtemp;
	chtemp = NULL;
}

void CJniDataSwitchCls::OnProcssHanziTx(char * strContent,int len)
{
	jclass jstrcls = m_env->FindClass("java/lang/String");
	jmethodID jstriniid = m_env->GetMethodID(jstrcls,"<init>","([BLjava/lang/String;)V");
	
	jbyteArray jba = m_env->NewByteArray(strlen(strContent));
	m_env->SetByteArrayRegion(jba,0,len,(jbyte *)(strContent));
	
	jstring jstrendcoding = m_env->NewStringUTF("gbk");
	
	jstring jstr = (jstring)m_env->NewObject(jstrcls,jstriniid,jba,jstrendcoding);
	
	char * datap = (char *)m_env->GetStringUTFChars(jstr,NULL);
	
	strcpy(strContent,datap);
	
	m_env->ReleaseStringUTFChars(jstr,datap);
	
}

void CJniDataSwitchCls::SendDebugInfo(char * strInfo)
{
	jclass cls = m_env->FindClass(m_clsName);
	if (cls == NULL)
	{
		return;
	}

	jmethodID jmid = m_env->GetStaticMethodID(cls,"ShowDebugInfo","(Ljava/lang/String;)V");
	if (jmid == NULL)
	{
		return;
	}

	jstring jstr = m_env->NewStringUTF(strInfo);
	m_env->CallStaticVoidMethod(cls,jmid,jstr);
}

jclass CJniDataSwitchCls::GetClass(char * clsname)
{
	char fullname[50] = {0};
	int len = sprintf(fullname,"%s/",m_packName);
	len += sprintf(fullname+len,"%s",clsname);

	jclass cls = m_env->FindClass(fullname);
	
	return cls;
}

jobject CJniDataSwitchCls::CreateClassObject(jclass jcls)
{
	jmethodID mid = m_env->GetMethodID(jcls,"<init>","()V");
	if (mid == NULL)
	{
		return 0;
	}

	jobject objs = m_env->NewObject(jcls,mid);
	
	return objs;
}

jobjectArray CJniDataSwitchCls::CreateObjectArray(int size)
{
	jclass objcls = m_env->FindClass("java/lang/Object");
	
	if (objcls == NULL)
	{
		return NULL;
	}
	
	jobjectArray objarray = m_env->NewObjectArray(size,objcls,0);

	return objarray;
}

void CJniDataSwitchCls::SendNotifyMessage(int itype,jobject obj)
{	

	jclass clsInterface = m_env->FindClass(m_clsName);

	if (clsInterface == NULL)
	{
		return;
	}
	
	jmethodID jmid = m_env->GetStaticMethodID(clsInterface,"NotityMain","(ILjava/lang/Object;)I");

	m_env->CallStaticIntMethod(clsInterface,jmid,itype,obj);

}

void CJniDataSwitchCls::SendNotifyMessage(int itype,jobject obj,jobjectArray objarry)
{

	jclass clsInterface = m_env->FindClass(m_clsName);
	if (clsInterface == NULL)
	{
		return;
	}

	jmethodID jmid = m_env->GetStaticMethodID(clsInterface,"NotityMain","(ILjava/lang/Object;[Ljava/lang/Object;)I");
	if (jmid == NULL)
	{
		return;
	}

	m_env->CallStaticIntMethod(clsInterface,jmid,itype,obj,objarry);
}

bool CJniDataSwitchCls::SetIntArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,int *val,int len)
{
	jintArray jia = m_env->NewIntArray(len);
	m_env->SetIntArrayRegion(jia,0,len,(jint *)val);
	
	
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,filedname,"[I");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetObjectField(obj,jfid,jia);
	
	return true;
}

bool CJniDataSwitchCls::SetShortArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,short *val,int len)
{
	jshortArray jia = m_env->NewShortArray(len);
	m_env->SetShortArrayRegion(jia,0,len,(jshort *)val);
	
	
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,filedname,"[S");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetObjectField(obj,jfid,jia);
	
	return true;
}

bool CJniDataSwitchCls::SetByteArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,char *val,int len)
{
	jbyteArray jba = m_env->NewByteArray(len);
	if (jba == NULL)
	{
		return false;
	}
	m_env->SetByteArrayRegion(jba,0,len,(jbyte*)val);

	jfieldID jfid = m_env->GetFieldID(clsobj,filedname,"[B");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetObjectField(obj,jfid,jba);

	return true;
}

bool CJniDataSwitchCls::SetDoubleFieldValue(jclass &clsobj,jobject & obj,char * filedname,double val)
{
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,filedname,"D");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetDoubleField(obj,jfid,val);
	return true;
}

bool CJniDataSwitchCls::SetCharFieldValue(jclass &clsobj,jobject &obj,char *filedname,char val)
{
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,filedname,"C");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetCharField(obj,jfid,val);

	return true;
}

bool CJniDataSwitchCls::SetShortFieldValue(jclass &clsobj,jobject &obj,char *filename,short val)
{
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,filename,"S");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetShortField(obj,jfid,val);
	return true;
}

bool CJniDataSwitchCls::SetIntFieldValue(jclass &clsobj,jobject &obj,char *fieldname,int val)
{
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,fieldname,"I");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetIntField(obj,jfid,val);
	return true;
}

bool CJniDataSwitchCls::SetLongFieldValue(jclass &clsobj,jobject &obj,char *fieldname,unsigned long val)
{
	jfieldID jfid = NULL;
	jfid = m_env->GetFieldID(clsobj,fieldname,"J");
	if (jfid == NULL)
	{
		return false;
	}
	m_env->SetLongField(obj,jfid,val);

	return true;
}

bool CJniDataSwitchCls::SetStringFieldValue(jclass &clsobj,jobject &obj,char *fieldname,char *val)
{
	jfieldID jfid = NULL;

	jfid = m_env->GetFieldID(clsobj,fieldname,"Ljava/lang/String;");
	if (jfid == NULL)
	{
		return false;
	}
	
	jstring jstr = m_env->NewStringUTF(val);
	
	m_env->SetObjectField(obj,jfid,jstr);
	return true;
}

void CJniDataSwitchCls::SetObjArrayFieldValue(char *fieldname,void *val,jobjectArray &objary,int counts)
{
	
	if (objary == NULL)
	{
		return;
	}
	
	StarState *data = (StarState *)val;
	
	
	jclass objectcls = GetClass("StarInfo");
	if (objectcls == NULL)
	{
		return;
	}
	
	for (int i = 0;i < counts;i++)
	{
		jobject objec = CreateClassObject(objectcls);
		if (objec == NULL)
		{
			return;
		}
		SetLongFieldValue(objectcls,objec,"lStarNum",data[i].lStarNum);
		SetDoubleFieldValue(objectcls,objec,"dStarElevation",data[i].dStarElevation);
		SetDoubleFieldValue(objectcls,objec,"dStarAzimuth",data[i].dStarAzimuth);
		SetDoubleFieldValue(objectcls,objec,"dSNR",data[i].dSNR);
		
		m_env->SetObjectArrayElement(objary,i,objec);
	}
}

void CJniDataSwitchCls::WriteToDevice(char *buf,int len)
{
	jclass clsInterface = m_env->FindClass(m_clsName);
	if (clsInterface == NULL)
	{
		return;
	}
	
	jmethodID jmid = m_env->GetStaticMethodID(clsInterface,"sendDataToDevice","(I[B)V");
	if (jmid == NULL)
	{
		return;
	}

	jbyteArray jba = m_env->NewByteArray(len);
	m_env->SetByteArrayRegion(jba,0,len,(jbyte *)(buf));

	m_env->CallStaticVoidMethod(clsInterface,jmid,len,jba);
}

void CJniDataSwitchCls::WriteToDeviceEx(char *buf,long len)
{
	jclass clsInterface = m_env->FindClass(m_clsName);
	if (clsInterface == NULL)
	{
		return;
	}
	
	jmethodID jmid = m_env->GetStaticMethodID(clsInterface,"sendDataToDeviceEx","(J[B)V");
	if (jmid == NULL)
	{
		return;
	}
	
	jbyteArray jba = m_env->NewByteArray(len);
	m_env->SetByteArrayRegion(jba,0,len,(jbyte *)(buf));
	
	m_env->CallStaticVoidMethod(clsInterface,jmid,(jlong)len,jba);
}