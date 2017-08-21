/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_project_dataprase_JniDataPrase */

#ifndef _Included_com_app_dataprase_JniDataPrase
#define _Included_com_app_dataprase_JniDataPrase
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    ProcssData
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_app_dataprase_JniDataPrase_ProcssData__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    ProcssData
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_com_app_dataprase_JniDataPrase_ProcssData___3BI
  (JNIEnv *, jobject, jbyteArray, jint);


/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SetProtocolType
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_com_app_dataprase_JniDataPrase_SetProtocolType
  (JNIEnv *,jobject,jint);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendTXSQ
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendTXSQ
  (JNIEnv *,jobject,jint,jint,jlong,jbyteArray);


/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendICJC
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendICJC
  (JNIEnv *,jobject);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendXHDQ
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendXHDQ
  (JNIEnv *,jobject,jlong);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendXTZJ
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendXTZJ
  (JNIEnv *,jobject,jlong);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendJSZL
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendJSZL
  (JNIEnv *,jobject,jlong);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendGLJC
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendGLJC
  (JNIEnv *,jobject,jlong,jint);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendBBDQ
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendBBDQ
  (JNIEnv *,jobject,jlong);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SendDWSQ
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SendDWSQ
  (JNIEnv *,jobject,jobject);


/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SetWorkMode
 * Signature: ([BI)I
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SetWorkMode
  (JNIEnv *,jobject,jint);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SetDownLoadCPUFile
 * Signature: 
 */
JNIEXPORT jlong JNICALL Java_com_app_dataprase_JniDataPrase_SetDownLoadCPUFile
  (JNIEnv*,jobject,jstring);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    SetDownLoadCPUFlashMode
 * Signature: 
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_SetDownLoadCPUFlashMode
  (JNIEnv*,jobject,jint);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    DownLoadRNSendFirstPackage
 * Signature: 
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_DownLoadRNSendFirstPackage
  (JNIEnv*,jobject);

/*
 * Class:     com_project_dataprase_JniDataPrase
 * Method:    ResumeJni
 * Signature: 
 */
JNIEXPORT void JNICALL Java_com_app_dataprase_JniDataPrase_ResumeJni
  (JNIEnv*,jobject,jint);

#ifdef __cplusplus
}
#endif
#endif
