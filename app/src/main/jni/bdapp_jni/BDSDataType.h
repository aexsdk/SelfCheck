/************************����ṹ����������****************************/

/*************************����ṹ���������ͽ���***********************/
const int BASE_PRO_D_TYPE = 0x100;

#define TYPE_ERROR (0 + BASE_PRO_D_TYPE)
#define TYPE_AAM (1 + BASE_PRO_D_TYPE)
#define TYPE_ALM (2 + BASE_PRO_D_TYPE)
#define TYPE_APL (3 + BASE_PRO_D_TYPE)
#define TYPE_BSD (4 + BASE_PRO_D_TYPE)
#define TYPE_BSQ (5 + BASE_PRO_D_TYPE)
#define TYPE_BSX (6 + BASE_PRO_D_TYPE)
#define TYPE_DAS (7 + BASE_PRO_D_TYPE)
#define TYPE_DHD (8 + BASE_PRO_D_TYPE)
#define TYPE_DHP (9 + BASE_PRO_D_TYPE)
#define TYPE_DHR (10 + BASE_PRO_D_TYPE)
#define TYPE_DHS (11 + BASE_PRO_D_TYPE)
#define TYPE_DHT (12 + BASE_PRO_D_TYPE)
#define TYPE_DHV (13 + BASE_PRO_D_TYPE)
#define TYPE_DSA (14 + BASE_PRO_D_TYPE)
#define TYPE_ECT (15 + BASE_PRO_D_TYPE)
#define TYPE_GGA (16 + BASE_PRO_D_TYPE)
#define TYPE_GLL (17 + BASE_PRO_D_TYPE)
#define TYPE_GSA (18 + BASE_PRO_D_TYPE)
#define TYPE_GSV (19 + BASE_PRO_D_TYPE)
#define TYPE_ICS (20 + BASE_PRO_D_TYPE)
#define TYPE_IDV (21 + BASE_PRO_D_TYPE)
#define TYPE_IHO (22 + BASE_PRO_D_TYPE)
#define TYPE_PCD (23 + BASE_PRO_D_TYPE)
#define TYPE_PLC (24 + BASE_PRO_D_TYPE)
#define TYPE_PRO (25 + BASE_PRO_D_TYPE)
#define TYPE_RAM (26 + BASE_PRO_D_TYPE)
#define TYPE_RDW (27 + BASE_PRO_D_TYPE)
#define TYPE_STX (28 + BASE_PRO_D_TYPE)
#define TYPE_STZ (29 + BASE_PRO_D_TYPE)
#define TYPE_VTG (30 + BASE_PRO_D_TYPE)
#define TYPE_ZDA (31 + BASE_PRO_D_TYPE)
#define TYPE_ZDU (32 + BASE_PRO_D_TYPE)
#define TYPE_GBS (33 + BASE_PRO_D_TYPE)
#define TYPE_GST (34 + BASE_PRO_D_TYPE)
#define TYPE_ICM (35 + BASE_PRO_D_TYPE)
#define TYPE_PMU (36 + BASE_PRO_D_TYPE)
#define TYPE_RMC (37 + BASE_PRO_D_TYPE)
#define TYPE_TXT (38 + BASE_PRO_D_TYPE)
#define TYPE_ZTI (39 + BASE_PRO_D_TYPE)
#define TYPE_BSI (40 + BASE_PRO_D_TYPE)
#define TYPE_DWR (41 + BASE_PRO_D_TYPE)
#define TYPE_FKI (42 + BASE_PRO_D_TYPE)
#define TYPE_GXM (43 + BASE_PRO_D_TYPE)
#define TYPE_ICI (44 + BASE_PRO_D_TYPE)
#define TYPE_ICZ (45 + BASE_PRO_D_TYPE)
#define TYPE_KLT (46 + BASE_PRO_D_TYPE)
#define TYPE_LZM (47 + BASE_PRO_D_TYPE)
#define TYPE_HZR (48 + BASE_PRO_D_TYPE)
#define TYPE_WAA (49 + BASE_PRO_D_TYPE)
#define TYPE_TXR (50 + BASE_PRO_D_TYPE)
#define TYPE_TCS (51 + BASE_PRO_D_TYPE)
#define TYPE_RMO (52 + BASE_PRO_D_TYPE)
#define TYPE_SCS (53 + BASE_PRO_D_TYPE)
#define TYPE_PKY (54 + BASE_PRO_D_TYPE)
#define TYPE_SBX (55 + BASE_PRO_D_TYPE)
#define TYPE_XSD (56 + BASE_PRO_D_TYPE)
#define TYPE_XST (57 + BASE_PRO_D_TYPE)
/*******************���¶����ΪPC��Ҫ���յ�CPU���͵�����****************begin**/

#define HAVE_ROUHE

#define TRUE 1
#define FALSE 0
typedef unsigned long DWORD;
typedef long BOOL;


struct PortData  //�ýṹ����Ҫ���ڴ���ԭʼ���ݵĴ��ݣ����ںŵĴ��ݣ��������͵Ĵ���
{
	char sStartType[4];   //���ڴ洢���������ݵķ���ϵͳ����BD��GP�ȡ�
	unsigned char sYnanshiData[512]; //���ڴ洢���ڽ��յ�ԭʼ����
	int  iDataLen;                  //���ڴ洢���յ������ݵĳ���
	int ErrType;                  //0��ʾ������1��ʾ�������ǡ�BD����GP����2��ʾ�������Ͳ�������Ҫ������32�֣�
	                              //�������͵ȴ���չ
	int DataType;                 //��ʾ��ǰ��������ݵ��������ͣ���1��33������33Ϊ��������
	void *pData;                  //�����͵����ݵ�ָ�룬��Ҫ�ڽ���ʱ���и��ݶ�Ӧ���������ͽ���ǿ��ת��
};
struct AAMData
{
	char   cState1;        //״̬1
	char   cState2;        //״̬2
	double  dRadius;        //����Ȧ�뾶
	char   cRadiusUnit;    //�뾶��λ
	char   sLineName[30];  //��·���ʶ��
};

struct ALFData
{
	double dSentenceCount;           //�������
	double dSentenceNum;             //����
	char  cStarType[3];             //�������
	int   iStartNum;                //���Ǻ�
	char  sWeekCount[10];           //���ڼ��� 
	char  sStartHealth[10];         //���ǽ���״̬ 
	char  sParty[10];               //ƫ���� 
	char  sStartNorm[10];           //���ǻ�׼ʱ�� ��λ���� 
	char  sOrbitDip[10];            //�����Ǹ����� ��λ��PI
	char  sUpPointChange[10];       //�����㾭�ȱ仯�� ��λ��PI/��
	char  sLongAxis[10];            //�볤��ƽ����   ��λ���ף�1/2��
	char  sperigeeAngle[10];        //���ص����    ��λ��PI
	char  sUpPoint[10];             //�����㾭��    ��λ��PI
	char  sAnomalisticAngle[10];    //ƽ�����      ��λ��PI
	char  sClockPar1[10];           //ʱ�Ӳ���0     ��λ����
	char  sClockPar2[10];           //ʱ�Ӳ���1     ��λ����/��
};
struct ALMData
{
	double dSentenceCount;           //�������
	double dSentenceNum;             //����
	int   iStartNum;                //���Ǻ�
	char  sWeekCount[10];           //���ڼ��� 
	char  sStartHealth[10];         //���ǽ���״̬ 
	char  sParty[10];               //ƫ���� 
	char  sStartNorm[10];           //���ǻ�׼ʱ�� ��λ���� 
	char  sOrbitDip[10];            //�����Ǹ����� ��λ��PI
	char  sEquator[10];             //�ྭ�� ��λ��PI/��
	char  sLongAxis[10];            //�볤��ƽ����   ��λ���ף�1/2��
	char  sperigeeAngle[10];        //���ص����    ��λ��PI
	char  sUpPoint[10];             //�����㾭��    ��λ��PI
	char  sAnomalisticAngle[10];    //ƽ�����      ��λ��PI
	char  sClockPar1[10];           //ʱ�Ӳ���0     ��λ����
	char  sClockPar2[10];           //ʱ�Ӳ���1     ��λ����/��
	char  sStarType[3];             //�������
};
struct APLData
{
	double dUTCTime;                   //UTCʱ��
	double  dLevelProtect;             //ˮƽ��������
	char   cLevelUnit;                //ˮƽ�������޵�λ
	double  dPlumbProtect;             //��ֱ��������
	char   cPlumbUnit;                //��ֱ�������޵�λ
	double  dSpaceProtect;             //�ռ䱣������
	char   cSpaceUnit;                //�ռ䱣�����޵�λ
};
struct BSDData
{
	char   sPointName[30];            //��ʶ������
	double  dPicNum;                   //ͼ����
	double dLat;                      //γ��
	char   cLatDirection;             //γ�ȷ���
	double dLon;                      //����
	char   cLonDirection;             //���ȷ���
	double dHigh;                     //�߶�
	char   cHighUnit;                 //�߶ȵ�λ
};
struct BSQData
{
	char   sPaceName[30];             //��ʶ������
	double dLat;                      //���ĵ�γ��
	char   cLatDirection;             //γ�ȷ���
	double dLon;                      //����
	char   cLonDirection;             //���ĵ㾭�ȷ���
	double dHigh;                     //���ĵ�߶�
	char   cHighUnit;                 //�߶ȵ�λ
	double dPaceRadius;               //����뾶
	char   cRadiusUint;               //�뾶��λ
};
struct PointData
{
	double dLat;                      //γ��
	char   cLatDirection;             //γ�ȷ���
	double dLon;                      //����
	char   cLonDirection;             //���ȷ���
	double dHigh;                     //�߶�
	char   cHighUnit;                 //�߶ȵ�λ
};
struct BSXData
{
	char   sLineName[100];            //��ʶ������
	double  dSentenceCount;            //�������
	double  dSentenceNum;              //����
	double  dPointCount;               //�������Ŀ
	PointData PointSave[20];          //�������Ϣ
};
struct DASData
{
	double dAimProtect;               //����Ŀ����ʾ����
	char   cAimUnit;                  //����Ŀ����ʾ���޵�λ
	double dEndProtect;               //�����յ㱨������
	char   cEndUnit;                  //�����յ㱨�����޵�λ
	double dArrivePaceProtect;        //�ӽ���ʶ���򱨾�����
	char   cArrivePaceUnit;           //�ӽ���ʶ���򱨾����޵�λ
	double dArrivePointProtect;       //�ӽ���ʶ����ʾ����
	char   cArrivePointUnit;          //�ӽ���ʶ����ʾ���޵�λ
	double dCrabSpaceProtect;         //ƫ���౨������
	char   cCrabSpaceUnit;            //ƫ���౨�����޵�λ
	double dCrabAngle;                //ƫ���Ǳ�������  ��λ����
	int    iTimeZone;                 //ʱ������
	double dTrackTimeSpace;           //������¼ʱ����  ��λ����
	double dTurnPoint;                //ת����������  
	char   cTurnPointUnit;            //ת���������õ�λ
	double dTurnPointProtect;         //ת��㱨����������
	char   cTurnPointProtectUnit;     //ת��㱨���������õ�λ
	char   cNaviOrder;                //����˳��
};
struct DHDData
{
	int iInfoType;                    //��Ϣ����
	double dDistance;                 //����
	char   cDistanceUint;             //��λ
};
struct DHPData
{
	double dCourseNum;                //�������
	char   sCourseName[25];           //��������
	double dFairWayNum;               //��·�����
	char   sFairWayName[25];          //��·������
	double dEndFairWayNum;            //���������պ�·�����
	char   sEndFairWayName[25];       //���������պ�·������

};
struct DHRData
{
	int iInfoType;                    //��Ϣ����
	double dAngle;                    //�Ƕ� ��λ����
};
struct DHSData
{
	char   cNorthNorm;                //����׼
	double dNowDimNum;                //��ǰĿ�����
	char   sDimName[100];             //Ŀ������
	char   cNaviOrder;                //����˳��
};
struct DHTData
{
	int   iInfoType;                  //��Ϣ����
	char  sData[10];                  //����
	char  sTime[10];                  //ʱ��
};
struct DHVData
{
	double   dUTCTime;              //UTCʱ��
	double   dSpeed;                //�ٶ�
	double   dXSpeed;               //X���ٶ�
	double   dYSpeed;               //Y���ٶ�
	double   dZSpeed;               //Z���ٶ�
	double   dEarthSpeed;           //����
	double   dMaxSpeed;             //����ٶ�
	double   dAverageSpeed;         //ƽ���ٶ�
	double   dAllAverageSpeed;      //ȫ��ƽ���ٶ�
	double   dValueSpeed;           //��Ч�ٶ�
	char     cSpeedUint;            //�ٶȵ�λ
};
struct DSAData
{
	unsigned long  lUserID;          //�û�ID
	char  cHavePlace;                //����λ��
	double dAddZero;                 //������ֵ
	char   cOutSet;                  //����趨
	double dSendFre;                 //Ƶ��   ��λ����
	double dLat;                      //γ��
	char   cLatDirection;             //γ�ȷ���
	double dLon;                      //����
	char   cLonDirection;             //���ȷ���
	double dHigh;                     //�߶�
	char   cHighUnit;                 //�߶ȵ�λ
};
struct ECTData
{
	int   iStartNum;                //���Ǻ�
	char  sFrePoint[5];             //Ƶ��
	char  cSpurTrack;               //֧·
	int   iChannelNum;              //ͨ����
	char  sNavData[100];            //ԭʼ������Ϣ
};
struct GGAData
{
	double    dUTCTime;             //UTCʱ��
	double    dLat;                 //γ��
	char      cLatDirection;        //γ�ȷ���
	double    dLon;                 //����
	char      cLonDirection;        //���ȷ���
	int       iState;               //״ָ̬ʾ
	int       iViewStarCount;       //��Ұ��������
	double    dHDOP;                //HDOPֵ
	double    dAntennaHigh;         //����ƽ�����θ�
	char      cHighUnit;            //���θߵ�λ
	double    dHighNovel;           //CGS-2000�߳��쳣
	char      cHighNovelUnit;       //�߳��쳣��λ
	int       iDataLife;            //������ݵ�����
	int       iPlatFormID;          //���վ̨ID��
	double    dVDOP;                //VDOPֵ
#ifdef HAVE_ROUHE
	int sYear;						//��
	int sMonth;						//��
	int sDay;						//��
	int sHour;						//ʱ
	int sMinute;					//��
	int sSecond;					//��
	int sHSecond;					//����

	int sWeiduD;					//γ�ȶ�
	int sWeiduF;					//γ�ȷ�
	double sWeiduM;					//γ����
	//int sWeiduM;
	int sWeiduHM;					//γ��0.1��
	int sJingduD;					//���ȶ�
	int sJingduF;					//���ȷ�
	double sJingduM;					//������
	//int sJingduM;
	int sJingduHM;					//����0.1��
	
	double Logitude;				//�����öȱ�ʾ
	double Latitude;				//γ���öȱ�ʾ
#endif
};
struct GLLData
{
	double    dLat;                 //γ��
	char      cLatDirection;        //γ�ȷ���
	double    dLon;                 //����
	char      cLonDirection;        //���ȷ���
	double    dUTCTime;             //UTCʱ��
	char      cDataState;           //����״̬
	int       iModeType;            //ģʽָʾ
};
struct GSAData 
{
	char     cModeType;           //ģʽָʾ
	int      iModeSel;            //ѡ��ģʽ
	long     lStarNum[12];        //12�����Ǻ�
	double   dHDOP;               //HDOPֵ
	double   dVDOP;               //VDOPֵ
	double   dPDOP;               //PDOPֵ
	double   dTDOP;               //TDOPֵ
};
struct StarState
{
	long   lStarNum;             //���Ǻ�
	double dStarElevation;       //��������
	double dStarAzimuth;         //���Ƿ�λ��
	double dSNR;                 //�����
};
struct GSVData
{
	double     dSentenceCount;        //GSV�������
	double     dSentenceNum;          //��ǰGSV������
	int       iViewStarCount;       //��Ұ��������
	StarState StarInfo[4];          //������Ϣ
};
struct ICSData
{
	char  sFrePoint[5];             //Ƶ��
	char  cSpurTrack;               //֧·
	short iInfoType;                //��Ϣ����
	int   iStartNum[12];            //���Ǻ�
};
struct DISTURB
{
	char      sDisturbType[3];      //��������  
	double    dDisturbFre;          //����Ƶ��  ��λ��GHz
	double    dDisturbWidth;        //���Ŵ���  ��λ��kHz
	double    dDisturbPower;        //���Ź���  ��λ��dBW
};
struct IDVData
{
	double    dSentenceCount;        //IDV�������
	double    dSentenceNum;          //��ǰIDV������
	int       iDisturbCount;         //������Ŀ
	DISTURB   StarInfo[4];          //������Ϣ
};
struct IHOData
{
	double    dSentenceCount;        //�������
	double    dSentenceNum;          //��ǰ������
	double    dUTCTime;              //UTCʱ��
	int       iStarNum;              //���Ǳ��
	double    dStarPlaceX;           //���ǵ�λ��-X
	double    dStarPlaceY;           //���ǵ�λ��-Y
	double    dStarPlaceZ;           //���ǵ�λ��-Z
	double    dStarSpeedX;           //���ǵ��ٶ�-X
	double    dStarSpeedY;           //���ǵ��ٶ�-Y
	double    dStarSpeedZ;           //���ǵ��ٶ�-Z
	double    dDistance;             //α�����ֵ
	double    dDistanceSpeed;        //α�����ʲ���ֵ
	double    dDistanceEx;           //α��ƫ��
	double    dDistanceSpeedEx;      //α������ƫ��
};
struct PCDData
{
	int     iStarNum;                 //���Ǻ�
	char    sFrePoint[5];             //Ƶ��
	int     iChannelNum;              //ͨ����
	double  dTimeEx;                  //ʱ��ƫ��  ��λ����
};
struct PLCData
{
	int     iStarNum;                 //���Ǻ�
	char    sFrePoint[5];             //Ƶ��
	int     iChannelNum;              //ͨ����
	char    cAccredit;                //��Ȩָʾ
};
struct PROData
{
	int     iStarNum;                 //���Ǻ�
	char    sFrePoint[5];             //Ƶ��
	int     iChannelNum;              //ͨ����
	char    cRangeType;               //�������
	int     iSenNum;                  //֡��
	char    sSecondCount[20];             //���������
	char  sRangeValue[20];              //���ֵ  ��λ����
	char  sClockDif[20];                //�Ӳ�  ��λ����
};
struct RAMData
{
	double  dViewStarCount;            //����������
	double  dFailureStarCount;         //����������
	int     iFailureStarNum[12];       //����������
};
struct RDWData
{
	char      cInfoType;                //��Ϣ����
	double    dUTCTime;                 //UTCʱ��
	unsigned long  lUserID;             //�û�ID
	double dLat;                       //γ��
	char   cLatDirection;              //γ�ȷ���
	double dLon;                       //����
	char   cLonDirection;              //���ȷ���
	char   cKeyDenote;              //��Կָʾ
	short  iLonDenote;              //����ָʾ
	char   cExigenceOr;             //������λ 
	char   cMoreValue;             //���ֵָʾ
	short  iHighType;              //�߳�����
	double dViewStarCount;            //��Ұ��GEO������
	double dHigh;                 //����ƽ�����θ�
	char   cHighUnit;             //�̵߳�λ
	double dHighNovel;           //CGS-2000�߳��쳣
	char   cHighNovelUnit;       //�߳��쳣��λ
};
struct STXData 
{
	unsigned long    lSendUserID;     //���ŷ���ַ
	unsigned long    lRecUserID;     //���ŷ���ַ
	char    cTelType;                //������ʽ
	short   icInfoType;              //��Ϣ���
	char    cCRCClue;                //CRC��ʾ
	char    cSecrecyType;            //���ܱ�־
	long    lSendTime;               //����ʱ��
	char    sTelData[100];           //��������
};
struct STZData
{
	unsigned long    lLocalUserID;     //������ַ
	unsigned long    lSearchID;        //��ѯ��ַ
	short    iReturnCount;             //��ִ��
	long     lTimeData[5][2];          //������Ϣ��lTimeData[i][0]��ʾ����ʱ�䣬lTimeData[i][1]��ʾ��ִʱ��
};
struct VTGData
{
	double    dRelNorthCourse;              //�Եغ��򣨲ο��汱�� ��λ����
	char      cRelNorth;                    //�汱�ο�
	double    dPoleNorthCourse;             //�Եغ��򣨲ο��ű��� ��λ����
	char      cPoleNorth;                   //�ű��ο�
	double    dSpeed1;                      //�Ե��ٶ�
	char      cSpeed1Uint;                  //�Ե��ٶȵ�λ����
	double    dSpeed2;                      //�Ե��ٶ�
	char      cSpeed2Uint;                  //�Ե��ٶȵ�λ��km/h
	char      cModeType;                    //ģʽָʾ��
};
/*struct ZDAData
{
	double    dUTCTime;                     //UTCʱ��
	short     iDay;                         //�� 0~31
	short     iMonth;                       //�� 0~12
	short     iYear;                        //��  >2000
	short     iLocTimeZone;                 //����ʱ�� +13~-13
	short     iLocClockDif;                 //��ʱ�����Ӳ�  00~59 ��λ������
};*/
struct ZDUData
{
	double   dUTCTime;                     //UTCʱ��
	short     iDay;                         //��  0~31
	short     iMonth;                       //��  0~12
	short     iYear;                        //��  >2000
	short     iLocTimeZone;                 //����ʱ��   +13~-13
	short     iLocClockDif;                 //��ʱ�����Ӳ�  00~59 ��λ������
};


struct GBSData
{
	double   dUTCTime;                     //UTCʱ��
	double   dLonExpectError;				//γ��ֵԤ����� ��λ����
	double   dLatExpectError;               //����ֵԤ����� ��λ����
	double   dHighExpectError;              //�߶�ֵԤ����� ��λ����
	short    iProTroublePRN;                //����ܵĹ�������PRN��
	double   iProTroubleGL;                 //������ܵĹ�������©��ĸ���
	double   iProTroublePC;                 //������ܵĹ������ǹ��Ƶ�ƫ��
	double   iEstimateError;                //ƫ�ù���ı�׼ƫ��
};
struct GSTData
{
	double   dUTCTime;                     //UTCʱ��
	double   dDisErrorJFG;                 //�����׼ƫ��ľ�����
	double   dTYCError;                    //�����Բ�İ볤���׼ƫ��
	double   dTYDError;                    //�����Բ�İ�����׼ƫ��
	double   dTYWayError;                  //�����Բ�İ볤�᷽��
	double   dLonError;                    //γ�����ı�׼ƫ��
	double   dLatError;                    //�������ı�׼ƫ��
	double   dHighError;                   //�߶����ı�׼ƫ��
};
struct ICMData
{
//	long startDataTime;                  //��Կ��Ч����ʼʱ�� ��λ��������--2.0
	long endDataTime;                    //��Կ��Ч����ֹʱ�� ��λ��������
};
struct PMUData
{
	short freqDot;                         //Ƶ��
	long  starDateTime;					   //��Ч�ڿ�ʼ����--2.1
	long  endDateTime;					   //��Ч�ڽ�ֹ����--2.1
//	double endDataTime;                    //��ֹ����--2.0
};
struct RMCData
{
	double dUtcTime;                       //UTCʱ��
	char cStatus;						   //��λ״̬
	double dLatitude;                      //γ��
	char cLatitudeDir;                     //γ�ȷ���
	double dLongitude;                     //����
	char cLongitudeDir;                    //���ȷ���
	double dSpeed;                         //�����ٶ�
	double dCourse;                        //���溽��
	int iDate;                             //����
	double dCpj;                           //��ƫ��
	char cCpjDir;                          //��ƫ�Ƿ���
	char cMode;                            //ģʽָʾ
#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//����:��
	unsigned char cLonMinute;				//����:��
	//unsigned char cLonSecond;				//����:��
	double cLonSecond;
	unsigned char cLonMSecond;				//����:0.1��
	unsigned char cLatDegree;				//γ��:��
	unsigned char cLatMinute;				//γ��:��
	//unsigned char cLatSecond;				//γ��:��
	double cLatSecond;
	unsigned char cLatMSecond;				//γ��:0.1��

	double Latitude;						//γ�ȶȱ�ʾ
	double Logitude;						//���ȶȱ�ʾ
#endif

};
struct TXTData
{
	short sentNumber;                      //�������
	short sentNum;                         //����
	short txtSignSymbol;                   //�ı���ʶ��
	char  txtInfo[100];                    //�ı���Ϣ
};
struct ZDAData
{
	int   modeShow;                      //ģʽָʾ
	double dUTCTime;                     //UTCʱ��
	int    iDay;                         //��
	int    iMonth;                       //��
	int    iYear;                        //��
	int    localTime;                    //����ʱ��  00~+/-13 
	int    localMinDiss;                 //���������Ӳ�  00 ~ 59 ��λ������
	double dDsxzTime;                    //��ʱ����ʱ��
	int    xzData;                    //����ֵ  ��λ��ns  0~3ns
	int    iprecision;					 //����ָʾ
	char   spSateLockState;              //�����ź�����״̬
};
struct ZTIData
{
	short PRMModeState;                  //PRMģ��״ָ̬ʾ    0-���� 1-�쳣
	short JJMModeState;                  //�ӽ���ģ��״ָ̬ʾ 0-���� 1-�쳣
	short AirState;                      //����״̬           0-���� 1-�쳣 
	short doorState;                     //ͨ��״̬           0-���� 1-�쳣
	short lwd;                           //�����ʹ��         0-ʹ�����õ�� 1-ʹ�����õ��
	short chargeD;                       //���ָʾ           0-���״̬ 1-�ǳ��״̬
	double remainDL;                     //ʣ�����    
	int   remainDLTime;                  //ʣ���������ʱ��   ֻ��ʱ��
};
struct BSIData
{
	short resPower;                      //��Ӧ������  0-10
	short timePower;                     //ʱ�����  0-10
	short power[10];					//1-10�Ų����źŹ���
// 	short power1;                       //1�Ų����źŹ���
// 	short power2;
// 	short power3;
// 	short power4;
// 	short power5;
// 	short power6;
// 	short power7;
// 	short power8;
// 	short power9;
// 	short power10;                       //10�Ų����źŹ���
};
struct DWRData
{
	short posInfoType;						//��λ��Ϣ���� 
	unsigned long userID;					//�û���ַ(ID��)
	double dUTCTime;						//UTCʱ��
	double lat;								//γ��
	char   latWay;							//γ�ȷ���  N-�� S-��
	double lon;								//����
	char   lonWay;							//���ȷ���  E-�� W-��
	double floorHigh;						//��ظ�
	char   floorDW;							//��ظߵ�λ
	double highError;						//�߳��쳣
	char   highErrorDW;						//�߳��쳣��λ
	short  JDError;							//����ָʾ
	char   uposShow;						//������λָʾ
	char   DJZShow;							//���ֵָʾ
	char   highTypeShow;					//�߳�����ָʾ
#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//����:��
	unsigned char cLonMinute;				//����:��
	//unsigned char cLonSecond;				//����:��
	double cLonSecond;
	unsigned char cLonMSecond;				//����:0.1��
	unsigned char cLatDegree;				//γ��:��
	unsigned char cLatMinute;				//γ��:��
	//unsigned char cLatSecond;				//γ��:��
	double cLatSecond;
	unsigned char cLatMSecond;				//γ��:0.1��

	double Latitude;						//γ�ȶȱ�ʾ
	double Logitude;						//���ȶȱ�ʾ
#endif
};
struct FKIData
{
	char ZLName[4];                       //ָ������
	char ZLExcuState;                     //ָ��ִ�����
	char FreSetShow;                      //Ƶ������ָʾ
	short sendYZShow;                     //��������ָʾ
	int  waitTime;                        //�ȴ�ʱ��
};
struct GXMData
{
	short ZLType;                         //ָ������
	char manageInfo[32];                  //������Ϣ
};
struct ICIData
{
	unsigned long userID;                 //�û���ַ(ID��)
	unsigned long XLH;                    //���к�
	unsigned long TBID;                   //ͨ����ַ
	short userTZ;                         //�û�����ֵ
	double serverFre;                     //����Ƶ��
	short  TXOrder;                       //ͨ�ŵȼ�
	char   JMFlag;                        //���ܱ�־    E-���� N-����
	double XSUserNum;                     //�����û���
};
struct ICZData     //CPU
{
	int ALLXSUserNum;                     //�������û���
	unsigned long XSUserNum[40];		  //�����û�ID�ţ������һ������⣬����ÿ�����봫��40���û���Ϣ
};
struct KLTData
{
	char Flag;                            //��ʶ  P/Z
	unsigned long userID;                 //�û���ַ(ID��)
	short DWType;                         //��������
	char cont[1024];                       //��������
};
struct LZMData
{
	short manageMode;                     //����ģʽ  1-��ȡ�豸��ֵ 2-�����豸��ֵ 3-�����豸��ֵ
	int   deviceZero;                     //�豸��ֵ  ��λ��NS
};

struct HZRData
{
	unsigned long userID;                 //�û���ַ(ID��)
	short  HZNum;                         //��ִ��
	int *   HZSendTime;					  //��ִ����ʱ��
 	int *  HZHZTime;                      //��ִ��ִʱ��
};
struct WAAData
{
	short infoType;                       //��Ϣ����  1/0
	int   reportFre;                      //����Ƶ��  ��λ����   0-����λ�ñ���
	unsigned long userID;                 //�û���ַ(ID��)
	double  reportTime;                   //λ�ñ���ʱ��
	double lon;                           //γ��
	char   lonWay;                        //γ�ȷ���  N-�� S-��
	double lat;                           //����
	char   latWay;                        //���ȷ���  E-�� W-��
//	double floorHigh;                     //��ظ�
//	char   floorDW;                       //��ظߵ�λ
	double high;                          //�߳�ֵ
	char   highDW;                        //�̵߳�λ

#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//����:��
	unsigned char cLonMinute;				//����:��
	//unsigned char cLonSecond;				//����:��
	double cLonSecond;
	unsigned char cLonMSecond;				//����:0.1��
	unsigned char cLatDegree;				//γ��:��
	unsigned char cLatMinute;				//γ��:��
	//unsigned char cLatSecond;				//γ��:��
	double cLatSecond;
	unsigned char cLatMSecond;				//γ��:0.1��

	double Latitude;						//γ�ȶȱ�ʾ
	double Logitude;						//���ȶȱ�ʾ
#endif
};
struct TXRData
{
	short ixxleib;							//��Ϣ���
	unsigned long userID;					//�û���ַ
	short idianwenxs;						//������ʽ
	int isendtime;							//����ʱ��
	char ccontext[256];						//ͨ�ŵ�������
};
struct TCSData
{
	char cFreNum[3];                      //��Ƶ���
	char cZhiLuNum;                       //֧·��
	short iInfoType;                      //��Ϣ����
	int iStarNum[12];                     //���Ǻ�
};
struct RMOData
{
	char cIntence[4];						//Ŀ����䡢����־��
	short iMode;							//ģʽ
	int iFre;								//Ŀ��������Ƶ��
};
struct SCSData
{	
	long lShiChaData;					//ʱ������
};
struct PKYData
{
	int nWnzjs;							//WN�ܼ���
	char chMyue[7];						//��Կ
	char chKey[129];					//KEY
	char chB1[25];						//B1��������(24�ֽ�)
	char chB2[25];						//B2��������(24�ֽ�)
	char chB3[25];						//B3��������(24�ֽ�)
};
struct SBXData
{
	char chSbghname[10];				//�豸����������
	char chSblx[10];					//�豸����
	char chBbh[10];						//����汾��
	char chCkxybbh[8];					//����Э��汾��
	char chIcdxybbh[10];				//ICDЭ��汾��
	int nSbxlh;							//�豸���к�
	long nID;							//ID��
};

/*
struct BDR_DWXX		 //BD��λ��Ϣ
{
	DWORD dUserAddr;		        //�����û���ַ
	unsigned char cInfoSort;		//��Ϣ���
	DWORD dQueryAddr;		        //��ѯ��ַ
	unsigned char cPosTimeHour;		//��λʱ�̵�Сʱ����
	unsigned char cPosTimeMinute;	//��λʱ�̵ķ�λ����
	unsigned char cPosTimeSecond;   //��λʱ�̵���λ����
	unsigned char cPosTimeMSecond;	//��λʱ�̵���С������
	unsigned char cLonDegree;		//����:��
	unsigned char cLonMinute;		//����:��
	unsigned char cLonSecond;		//����:��
	unsigned char cLonMSecond;		//����:0.1��
	unsigned char cLatDegree;		//γ��:��
	unsigned char cLatMinute;		//γ��:��
	unsigned char cLatSecond;		//γ��:��
	unsigned char cLatMSecond;		//γ��:0.1��
	int dElevation;			    	//�߳�����
	int dElvError;				    //�߳��쳣ֵ
    BOOL IsUrgent;		            //�Ƿ����
	BOOL IsMultiRes;		        //�Ƿ���
	unsigned char Precision;	    //����
	BOOL HeightType;	    	    //�߳�ָʾ
	
	double Latitude;
	double Logitude;
};

struct BDR_TXXX			//ͨ����Ϣ
{
	DWORD dUserAddr;				//�����û���ַ
	unsigned char cInfoSort;		//���֡����롢���
	unsigned char Infotype;	    	//��Ϣ���
	
	DWORD dPostLetterAddr;			//���ŷ���ַ
	char cPostLetterTimeHour;		//����ʱ��:ʱ
	char cPostLetterTimeMinute;		//����ʱ��:��
	short sTelexLen;				//���ĳ���
	char TelexContent[240];			//�������� ��120�����ֻ�420������
	BOOL  CRCVerify;                //CRC�����־
};
*/

/************************����������ݽṹ�����*********************end************/
