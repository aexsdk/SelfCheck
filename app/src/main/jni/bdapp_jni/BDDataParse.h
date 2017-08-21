
#include "BDSDataType.h"
#include "jni.h"
#include "JniDataSwitchCls.h"

class CBDDataParse 
{

private:
	JNIEnv * m_jniEnv;
	CJniDataSwitchCls m_JniDataSwitchCls;
public:
	CBDDataParse(JNIEnv * env);
	~CBDDataParse();
public:

	bool Parse(const unsigned char * const pBuffers,int nSize);
	bool IsValidData( const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos, int nFieldIndex );
	int SNInteger( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos );
	long SNLong( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos );
	double SNDouble( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos);	
	double SNDoubleEx( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos,  double& dFractions);
	unsigned char SNByte( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos );
	void SNByteString( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos, char *ReturnBuff,int *nLen);

	char sStartType[3];    //用于提取当前数据所属于的卫星类型
	bool ProcessCRC(const unsigned char * const buff,int nLineFeedPos,int iStartPoint);

protected:
	bool ParseAAM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseALM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseAPL(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseBSD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseBSQ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseBSX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDAS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHP(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDHV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDSA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseECT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGGA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGLL(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGSA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGSV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseICS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseIDV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseIHO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParsePCD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParsePLC(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParsePRO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseRAM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseRDW(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseSTX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseSTZ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseVTG(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	//bool ParseZDA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseZDU(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGBS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);   //2011.08.02
	bool ParseGST(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseICM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParsePMU(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseRMC(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseTXT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	//bool ParseVTG(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseZDA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseZTI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseBSI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseDWR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseFKI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseGXM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseICI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseICZ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseKLT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseLZM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseHZR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseWAA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseTXR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseTCS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseRMO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseSCS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParsePKY(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseSBX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	/*bool ParseZHS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);*/
	/*bool ParseSCS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);*/
	bool ParseXSD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool ParseXST(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos);
	bool SendMessage(PortData *pPortData);

/*
	bool SetDoubleFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *filedname,double val);
	bool SetCharFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *filedname,char val);
	bool SetShortFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *filename,short val);
	bool SetIntFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,int val);
	bool SetLongFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,unsigned long val);
	bool SetStringFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,char *val);
	void SetObjArrayFieldValue(JNIEnv *env,char *fieldname,void *val,jobjectArray &objary,int counts);

	void SendNotifyMessage(JNIEnv *env,int itype,jobject obj);
	void SendNotifyMessage(JNIEnv *env,int itype,jobject obj,jobjectArray objarry);
	jclass GetClass(JNIEnv *env,char * clsname);
	jobject CreateClassObject(JNIEnv *env,jclass jcls);
	void SendDebugInfo(JNIEnv * env,char * strInfo);

	void OnProcssHunHeTx(JNIEnv * env,char * strContent,int len);
	void OnProcssHanziTx(JNIEnv * env,char * strContent,int len);
*/	
};
