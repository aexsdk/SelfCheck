#ifndef _JNI_DATA_SWITCH_CLS
#define _JNI_DATA_SWITCH_CLS

#include "memory.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "jni.h"
const int PACK_NAME_MAX_LEN = 40;

class CJniDataSwitchCls
{
private:
	char m_packName[PACK_NAME_MAX_LEN];
	char m_clsName[PACK_NAME_MAX_LEN];
	JNIEnv * m_env;

public:

	CJniDataSwitchCls();
	CJniDataSwitchCls(JNIEnv * env);

	void setPackName(char * packname,int len);

	~CJniDataSwitchCls();

	//bd2
	void OnProcssHunHeTx(char * strContent,int len);
	//bd2
	void OnProcssHanziTx(char * strContent,int len);

	void SendDebugInfo(char * strInfo);

	jclass GetClass(char * clsname);
	jobject CreateClassObject(jclass jcls);
	
	jobjectArray CreateObjectArray(int size);
	void SetObjArrayFieldValue(char *fieldname,void *val,jobjectArray &objary,int counts);

	void SendNotifyMessage(int itype,jobject obj);
	void SendNotifyMessage(int itype,jobject obj,jobjectArray objarry);

	void WriteToDevice(char *buf,int len);
	void WriteToDeviceEx(char *buf,long len);

	bool SetDoubleFieldValue(jclass &clsobj,jobject & obj,char * filedname,double val);
	bool SetIntFieldValue(jclass &clsobj,jobject &obj,char *fieldname,int val);
	bool SetShortFieldValue(jclass &clsobj,jobject &obj,char *filename,short val);
	bool SetLongFieldValue(jclass &clsobj,jobject &obj,char *fieldname,unsigned long val);
	bool SetCharFieldValue(jclass &clsobj,jobject &obj,char *filedname,char val);
	bool SetStringFieldValue(jclass &clsobj,jobject &obj,char *fieldname,char *val);
	bool SetIntArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,int *val,int len);
	bool SetShortArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,short *val,int len);
	bool SetByteArrayFieldValue(jclass &clsobj,jobject & obj,char * filedname,char *val,int len);

};

#endif	//JNI_DATA_SWITCH_CLS