/************************定义结构体数据类型****************************/

/*************************定义结构体数据类型结束***********************/
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
/*******************以下定义的为PC需要接收的CPU发送的数据****************begin**/

#define HAVE_ROUHE

#define TRUE 1
#define FALSE 0
typedef unsigned long DWORD;
typedef long BOOL;


struct PortData  //该结构体主要用于串口原始数据的传递，串口号的传递，数据类型的传递
{
	char sStartType[4];   //用于存储所接受数据的发射系统，如BD或GP等、
	unsigned char sYnanshiData[512]; //用于存储串口接收的原始数据
	int  iDataLen;                  //用于存储接收到的数据的长度
	int ErrType;                  //0表示正常，1表示发射器非“BD”或“GP”，2表示数据类型不属于需要解析的32种，
	                              //其余类型等待扩展
	int DataType;                 //表示当前传输的数据的数据类型，从1到33，其中33为错误类型
	void *pData;                  //所发送的数据的指针，需要在解析时进行根据对应的数据类型进行强制转换
};
struct AAMData
{
	char   cState1;        //状态1
	char   cState2;        //状态2
	double  dRadius;        //到达圈半径
	char   cRadiusUnit;    //半径单位
	char   sLineName[30];  //航路点标识符
};

struct ALFData
{
	double dSentenceCount;           //语句总数
	double dSentenceNum;             //语句号
	char  cStarType[3];             //卫星类别
	int   iStartNum;                //卫星号
	char  sWeekCount[10];           //星期计数 
	char  sStartHealth[10];         //卫星健康状态 
	char  sParty[10];               //偏心率 
	char  sStartNorm[10];           //卫星基准时间 单位：秒 
	char  sOrbitDip[10];            //轨道倾角改正量 单位：PI
	char  sUpPointChange[10];       //升交点经度变化率 单位：PI/秒
	char  sLongAxis[10];            //半长轴平方根   单位：米（1/2）
	char  sperigeeAngle[10];        //近地点幅角    单位：PI
	char  sUpPoint[10];             //升交点经度    单位：PI
	char  sAnomalisticAngle[10];    //平近点角      单位：PI
	char  sClockPar1[10];           //时钟参数0     单位：秒
	char  sClockPar2[10];           //时钟参数1     单位：秒/秒
};
struct ALMData
{
	double dSentenceCount;           //语句总数
	double dSentenceNum;             //语句号
	int   iStartNum;                //卫星号
	char  sWeekCount[10];           //星期计数 
	char  sStartHealth[10];         //卫星健康状态 
	char  sParty[10];               //偏心率 
	char  sStartNorm[10];           //卫星基准时间 单位：秒 
	char  sOrbitDip[10];            //轨道倾角改正量 单位：PI
	char  sEquator[10];             //赤经率 单位：PI/秒
	char  sLongAxis[10];            //半长轴平方根   单位：米（1/2）
	char  sperigeeAngle[10];        //近地点幅角    单位：PI
	char  sUpPoint[10];             //升交点经度    单位：PI
	char  sAnomalisticAngle[10];    //平近点角      单位：PI
	char  sClockPar1[10];           //时钟参数0     单位：秒
	char  sClockPar2[10];           //时钟参数1     单位：秒/秒
	char  sStarType[3];             //卫星类别
};
struct APLData
{
	double dUTCTime;                   //UTC时间
	double  dLevelProtect;             //水平保护门限
	char   cLevelUnit;                //水平保护门限单位
	double  dPlumbProtect;             //垂直保护门限
	char   cPlumbUnit;                //垂直保护门限单位
	double  dSpaceProtect;             //空间保护门限
	char   cSpaceUnit;                //空间保护门限单位
};
struct BSDData
{
	char   sPointName[30];            //标识点名称
	double  dPicNum;                   //图标编号
	double dLat;                      //纬度
	char   cLatDirection;             //纬度方向
	double dLon;                      //经度
	char   cLonDirection;             //经度方向
	double dHigh;                     //高度
	char   cHighUnit;                 //高度单位
};
struct BSQData
{
	char   sPaceName[30];             //标识区名称
	double dLat;                      //中心点纬度
	char   cLatDirection;             //纬度方向
	double dLon;                      //经度
	char   cLonDirection;             //中心点经度方向
	double dHigh;                     //中心点高度
	char   cHighUnit;                 //高度单位
	double dPaceRadius;               //区域半径
	char   cRadiusUint;               //半径单位
};
struct PointData
{
	double dLat;                      //纬度
	char   cLatDirection;             //纬度方向
	double dLon;                      //经度
	char   cLonDirection;             //经度方向
	double dHigh;                     //高度
	char   cHighUnit;                 //高度单位
};
struct BSXData
{
	char   sLineName[100];            //标识线名称
	double  dSentenceCount;            //语句总数
	double  dSentenceNum;              //语句号
	double  dPointCount;               //坐标点数目
	PointData PointSave[20];          //坐标点信息
};
struct DASData
{
	double dAimProtect;               //到达目标提示门限
	char   cAimUnit;                  //到达目标提示门限单位
	double dEndProtect;               //到达终点报警门限
	char   cEndUnit;                  //到达终点报警门限单位
	double dArrivePaceProtect;        //接近标识区域报警门限
	char   cArrivePaceUnit;           //接近标识区域报警门限单位
	double dArrivePointProtect;       //接近标识点提示门限
	char   cArrivePointUnit;          //接近标识点提示门限单位
	double dCrabSpaceProtect;         //偏航距报警门限
	char   cCrabSpaceUnit;            //偏航距报警门限单位
	double dCrabAngle;                //偏航角报警门限  单位：度
	int    iTimeZone;                 //时区设置
	double dTrackTimeSpace;           //航迹记录时间间隔  单位：秒
	double dTurnPoint;                //转向点距离设置  
	char   cTurnPointUnit;            //转向点距离设置单位
	double dTurnPointProtect;         //转向点报警距离设置
	char   cTurnPointProtectUnit;     //转向点报警距离设置单位
	char   cNaviOrder;                //导航顺序
};
struct DHDData
{
	int iInfoType;                    //信息类型
	double dDistance;                 //距离
	char   cDistanceUint;             //单位
};
struct DHPData
{
	double dCourseNum;                //航线序号
	char   sCourseName[25];           //航线名称
	double dFairWayNum;               //航路点序号
	char   sFairWayName[25];          //航路点名称
	double dEndFairWayNum;            //本航线最终航路点序号
	char   sEndFairWayName[25];       //本航线最终航路点名称

};
struct DHRData
{
	int iInfoType;                    //信息类型
	double dAngle;                    //角度 单位：度
};
struct DHSData
{
	char   cNorthNorm;                //北基准
	double dNowDimNum;                //当前目标序号
	char   sDimName[100];             //目标名称
	char   cNaviOrder;                //导航顺序
};
struct DHTData
{
	int   iInfoType;                  //信息类型
	char  sData[10];                  //日期
	char  sTime[10];                  //时间
};
struct DHVData
{
	double   dUTCTime;              //UTC时间
	double   dSpeed;                //速度
	double   dXSpeed;               //X轴速度
	double   dYSpeed;               //Y轴速度
	double   dZSpeed;               //Z轴速度
	double   dEarthSpeed;           //地速
	double   dMaxSpeed;             //最大速度
	double   dAverageSpeed;         //平均速度
	double   dAllAverageSpeed;      //全程平均速度
	double   dValueSpeed;           //有效速度
	char     cSpeedUint;            //速度单位
};
struct DSAData
{
	unsigned long  lUserID;          //用户ID
	char  cHavePlace;                //有无位置
	double dAddZero;                 //附加零值
	char   cOutSet;                  //输出设定
	double dSendFre;                 //频度   单位：秒
	double dLat;                      //纬度
	char   cLatDirection;             //纬度方向
	double dLon;                      //经度
	char   cLonDirection;             //经度方向
	double dHigh;                     //高度
	char   cHighUnit;                 //高度单位
};
struct ECTData
{
	int   iStartNum;                //卫星号
	char  sFrePoint[5];             //频点
	char  cSpurTrack;               //支路
	int   iChannelNum;              //通道号
	char  sNavData[100];            //原始导航信息
};
struct GGAData
{
	double    dUTCTime;             //UTC时间
	double    dLat;                 //纬度
	char      cLatDirection;        //纬度方向
	double    dLon;                 //经度
	char      cLonDirection;        //经度方向
	int       iState;               //状态指示
	int       iViewStarCount;       //视野内卫星数
	double    dHDOP;                //HDOP值
	double    dAntennaHigh;         //天线平均海拔高
	char      cHighUnit;            //海拔高单位
	double    dHighNovel;           //CGS-2000高程异常
	char      cHighNovelUnit;       //高程异常单位
	int       iDataLife;            //差分数据的寿命
	int       iPlatFormID;          //差分站台ID号
	double    dVDOP;                //VDOP值
#ifdef HAVE_ROUHE
	int sYear;						//年
	int sMonth;						//月
	int sDay;						//日
	int sHour;						//时
	int sMinute;					//分
	int sSecond;					//秒
	int sHSecond;					//毫秒

	int sWeiduD;					//纬度度
	int sWeiduF;					//纬度分
	double sWeiduM;					//纬度秒
	//int sWeiduM;
	int sWeiduHM;					//纬度0.1秒
	int sJingduD;					//经度度
	int sJingduF;					//经度分
	double sJingduM;					//经度秒
	//int sJingduM;
	int sJingduHM;					//经度0.1秒
	
	double Logitude;				//经度用度表示
	double Latitude;				//纬度用度表示
#endif
};
struct GLLData
{
	double    dLat;                 //纬度
	char      cLatDirection;        //纬度方向
	double    dLon;                 //经度
	char      cLonDirection;        //经度方向
	double    dUTCTime;             //UTC时间
	char      cDataState;           //数据状态
	int       iModeType;            //模式指示
};
struct GSAData 
{
	char     cModeType;           //模式指示
	int      iModeSel;            //选用模式
	long     lStarNum[12];        //12颗卫星号
	double   dHDOP;               //HDOP值
	double   dVDOP;               //VDOP值
	double   dPDOP;               //PDOP值
	double   dTDOP;               //TDOP值
};
struct StarState
{
	long   lStarNum;             //卫星号
	double dStarElevation;       //卫星仰角
	double dStarAzimuth;         //卫星方位角
	double dSNR;                 //信噪比
};
struct GSVData
{
	double     dSentenceCount;        //GSV语句总数
	double     dSentenceNum;          //当前GSV语句序号
	int       iViewStarCount;       //视野内卫星数
	StarState StarInfo[4];          //卫星信息
};
struct ICSData
{
	char  sFrePoint[5];             //频点
	char  cSpurTrack;               //支路
	short iInfoType;                //信息类型
	int   iStartNum[12];            //卫星号
};
struct DISTURB
{
	char      sDisturbType[3];      //干扰类型  
	double    dDisturbFre;          //干扰频率  单位：GHz
	double    dDisturbWidth;        //干扰带宽  单位：kHz
	double    dDisturbPower;        //干扰功率  单位：dBW
};
struct IDVData
{
	double    dSentenceCount;        //IDV语句总数
	double    dSentenceNum;          //当前IDV语句序号
	int       iDisturbCount;         //干扰数目
	DISTURB   StarInfo[4];          //干扰信息
};
struct IHOData
{
	double    dSentenceCount;        //语句总数
	double    dSentenceNum;          //当前语句序号
	double    dUTCTime;              //UTC时间
	int       iStarNum;              //卫星编号
	double    dStarPlaceX;           //卫星的位置-X
	double    dStarPlaceY;           //卫星的位置-Y
	double    dStarPlaceZ;           //卫星的位置-Z
	double    dStarSpeedX;           //卫星的速度-X
	double    dStarSpeedY;           //卫星的速度-Y
	double    dStarSpeedZ;           //卫星的速度-Z
	double    dDistance;             //伪距测量值
	double    dDistanceSpeed;        //伪距速率测量值
	double    dDistanceEx;           //伪距偏移
	double    dDistanceSpeedEx;      //伪距速率偏移
};
struct PCDData
{
	int     iStarNum;                 //卫星号
	char    sFrePoint[5];             //频点
	int     iChannelNum;              //通道号
	double  dTimeEx;                  //时间偏移  单位：秒
};
struct PLCData
{
	int     iStarNum;                 //卫星号
	char    sFrePoint[5];             //频点
	int     iChannelNum;              //通道号
	char    cAccredit;                //授权指示
};
struct PROData
{
	int     iStarNum;                 //卫星号
	char    sFrePoint[5];             //频点
	int     iChannelNum;              //通道号
	char    cRangeType;               //测距类型
	int     iSenNum;                  //帧号
	char    sSecondCount[20];             //周内秒计数
	char  sRangeValue[20];              //测距值  单位：米
	char  sClockDif[20];                //钟差  单位：秒
};
struct RAMData
{
	double  dViewStarCount;            //可视卫星数
	double  dFailureStarCount;         //故障卫星数
	int     iFailureStarNum[12];       //故障卫星数
};
struct RDWData
{
	char      cInfoType;                //信息类型
	double    dUTCTime;                 //UTC时间
	unsigned long  lUserID;             //用户ID
	double dLat;                       //纬度
	char   cLatDirection;              //纬度方向
	double dLon;                       //经度
	char   cLonDirection;              //经度方向
	char   cKeyDenote;              //密钥指示
	short  iLonDenote;              //经度指示
	char   cExigenceOr;             //紧急定位 
	char   cMoreValue;             //多解值指示
	short  iHighType;              //高程类型
	double dViewStarCount;            //视野内GEO卫星数
	double dHigh;                 //天线平均海拔高
	char   cHighUnit;             //高程单位
	double dHighNovel;           //CGS-2000高程异常
	char   cHighNovelUnit;       //高程异常单位
};
struct STXData 
{
	unsigned long    lSendUserID;     //收信方地址
	unsigned long    lRecUserID;     //发信方地址
	char    cTelType;                //电文形式
	short   icInfoType;              //信息类别
	char    cCRCClue;                //CRC提示
	char    cSecrecyType;            //保密标志
	long    lSendTime;               //发信时间
	char    sTelData[100];           //电文内容
};
struct STZData
{
	unsigned long    lLocalUserID;     //本机地址
	unsigned long    lSearchID;        //查询地址
	short    iReturnCount;             //回执数
	long     lTimeData[5][2];          //绘制信息，lTimeData[i][0]表示发信时间，lTimeData[i][1]表示回执时间
};
struct VTGData
{
	double    dRelNorthCourse;              //对地航向（参考真北） 单位：度
	char      cRelNorth;                    //真北参考
	double    dPoleNorthCourse;             //对地航向（参考磁北） 单位：度
	char      cPoleNorth;                   //磁北参考
	double    dSpeed1;                      //对地速度
	char      cSpeed1Uint;                  //对地速度单位：节
	double    dSpeed2;                      //对地速度
	char      cSpeed2Uint;                  //对地速度单位：km/h
	char      cModeType;                    //模式指示器
};
/*struct ZDAData
{
	double    dUTCTime;                     //UTC时间
	short     iDay;                         //日 0~31
	short     iMonth;                       //月 0~12
	short     iYear;                        //年  >2000
	short     iLocTimeZone;                 //本地时区 +13~-13
	short     iLocClockDif;                 //本时区分钟差  00~59 单位：分钟
};*/
struct ZDUData
{
	double   dUTCTime;                     //UTC时间
	short     iDay;                         //日  0~31
	short     iMonth;                       //月  0~12
	short     iYear;                        //年  >2000
	short     iLocTimeZone;                 //本地时区   +13~-13
	short     iLocClockDif;                 //本时区分钟差  00~59 单位：分钟
};


struct GBSData
{
	double   dUTCTime;                     //UTC时间
	double   dLonExpectError;				//纬度值预计误差 单位：米
	double   dLatExpectError;               //经度值预计误差 单位：米
	double   dHighExpectError;              //高度值预计误差 单位：米
	short    iProTroublePRN;                //最可能的故障卫星PRN号
	double   iProTroubleGL;                 //对最可能的故障卫星漏检的概率
	double   iProTroublePC;                 //对最可能的故障卫星估计的偏差
	double   iEstimateError;                //偏置估算的标准偏差
};
struct GSTData
{
	double   dUTCTime;                     //UTC时间
	double   dDisErrorJFG;                 //距离标准偏差的均方根
	double   dTYCError;                    //误差椭圆的半长轴标准偏差
	double   dTYDError;                    //误差椭圆的半短轴标准偏差
	double   dTYWayError;                  //误差椭圆的半长轴方向
	double   dLonError;                    //纬度误差的标准偏差
	double   dLatError;                    //精度误差的标准偏差
	double   dHighError;                   //高度误差的标准偏差
};
struct ICMData
{
//	long startDataTime;                  //密钥有效期起始时间 单位：日月年--2.0
	long endDataTime;                    //密钥有效期终止时间 单位：日月年
};
struct PMUData
{
	short freqDot;                         //频点
	long  starDateTime;					   //有效期开始日期--2.1
	long  endDateTime;					   //有效期截止日期--2.1
//	double endDataTime;                    //截止日期--2.0
};
struct RMCData
{
	double dUtcTime;                       //UTC时间
	char cStatus;						   //定位状态
	double dLatitude;                      //纬度
	char cLatitudeDir;                     //纬度方向
	double dLongitude;                     //经度
	char cLongitudeDir;                    //经度方向
	double dSpeed;                         //地面速度
	double dCourse;                        //地面航向
	int iDate;                             //日期
	double dCpj;                           //磁偏角
	char cCpjDir;                          //磁偏角方向
	char cMode;                            //模式指示
#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//经度:度
	unsigned char cLonMinute;				//经度:分
	//unsigned char cLonSecond;				//经度:秒
	double cLonSecond;
	unsigned char cLonMSecond;				//经度:0.1秒
	unsigned char cLatDegree;				//纬度:度
	unsigned char cLatMinute;				//纬度:分
	//unsigned char cLatSecond;				//纬度:秒
	double cLatSecond;
	unsigned char cLatMSecond;				//纬度:0.1秒

	double Latitude;						//纬度度表示
	double Logitude;						//经度度表示
#endif

};
struct TXTData
{
	short sentNumber;                      //语句总数
	short sentNum;                         //语句号
	short txtSignSymbol;                   //文本标识符
	char  txtInfo[100];                    //文本信息
};
struct ZDAData
{
	int   modeShow;                      //模式指示
	double dUTCTime;                     //UTC时间
	int    iDay;                         //日
	int    iMonth;                       //月
	int    iYear;                        //年
	int    localTime;                    //本地时区  00~+/-13 
	int    localMinDiss;                 //本地区分钟差  00 ~ 59 单位：分钟
	double dDsxzTime;                    //定时修正时刻
	int    xzData;                    //修正值  单位：ns  0~3ns
	int    iprecision;					 //精度指示
	char   spSateLockState;              //卫星信号锁定状态
};
struct ZTIData
{
	short PRMModeState;                  //PRM模块状态指示    0-正常 1-异常
	short JJMModeState;                  //加解密模块状态指示 0-正常 1-异常
	short AirState;                      //天线状态           0-正常 1-异常 
	short doorState;                     //通道状态           0-正常 1-异常
	short lwd;                           //内外电使用         0-使用内置电池 1-使用外置电池
	short chargeD;                       //充电指示           0-充电状态 1-非充电状态
	double remainDL;                     //剩余电量    
	int   remainDLTime;                  //剩余电量可用时间   只有时分
};
struct BSIData
{
	short resPower;                      //响应波束号  0-10
	short timePower;                     //时差波束号  0-10
	short power[10];					//1-10号波束信号功率
// 	short power1;                       //1号波束信号功率
// 	short power2;
// 	short power3;
// 	short power4;
// 	short power5;
// 	short power6;
// 	short power7;
// 	short power8;
// 	short power9;
// 	short power10;                       //10号波束信号功率
};
struct DWRData
{
	short posInfoType;						//定位信息类型 
	unsigned long userID;					//用户地址(ID号)
	double dUTCTime;						//UTC时间
	double lat;								//纬度
	char   latWay;							//纬度方向  N-北 S-南
	double lon;								//经度
	char   lonWay;							//经度方向  E-东 W-西
	double floorHigh;						//大地高
	char   floorDW;							//大地高单位
	double highError;						//高程异常
	char   highErrorDW;						//高程异常单位
	short  JDError;							//精度指示
	char   uposShow;						//紧急定位指示
	char   DJZShow;							//多解值指示
	char   highTypeShow;					//高程类型指示
#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//经度:度
	unsigned char cLonMinute;				//经度:分
	//unsigned char cLonSecond;				//经度:秒
	double cLonSecond;
	unsigned char cLonMSecond;				//经度:0.1秒
	unsigned char cLatDegree;				//纬度:度
	unsigned char cLatMinute;				//纬度:分
	//unsigned char cLatSecond;				//纬度:秒
	double cLatSecond;
	unsigned char cLatMSecond;				//纬度:0.1秒

	double Latitude;						//纬度度表示
	double Logitude;						//经度度表示
#endif
};
struct FKIData
{
	char ZLName[4];                       //指令名称
	char ZLExcuState;                     //指令执行情况
	char FreSetShow;                      //频度设置指示
	short sendYZShow;                     //发射抑制指示
	int  waitTime;                        //等待时间
};
struct GXMData
{
	short ZLType;                         //指令类型
	char manageInfo[32];                  //管理信息
};
struct ICIData
{
	unsigned long userID;                 //用户地址(ID号)
	unsigned long XLH;                    //序列号
	unsigned long TBID;                   //通播地址
	short userTZ;                         //用户特征值
	double serverFre;                     //服务频度
	short  TXOrder;                       //通信等级
	char   JMFlag;                        //加密标志    E-加密 N-非密
	double XSUserNum;                     //下属用户数
};
struct ICZData     //CPU
{
	int ALLXSUserNum;                     //总下属用户数
	unsigned long XSUserNum[40];		  //下属用户ID号，除最后一条语句外，其余每条必须传满40个用户信息
};
struct KLTData
{
	char Flag;                            //标识  P/Z
	unsigned long userID;                 //用户地址(ID号)
	short DWType;                         //电文类型
	char cont[1024];                       //电文内容
};
struct LZMData
{
	short manageMode;                     //管理模式  1-读取设备零值 2-设置设备零值 3-返回设备零值
	int   deviceZero;                     //设备零值  单位：NS
};

struct HZRData
{
	unsigned long userID;                 //用户地址(ID号)
	short  HZNum;                         //回执数
	int *   HZSendTime;					  //回执发信时间
 	int *  HZHZTime;                      //回执回执时间
};
struct WAAData
{
	short infoType;                       //信息类型  1/0
	int   reportFre;                      //报告频度  单位：秒   0-单次位置报告
	unsigned long userID;                 //用户地址(ID号)
	double  reportTime;                   //位置报告时间
	double lon;                           //纬度
	char   lonWay;                        //纬度方向  N-北 S-南
	double lat;                           //经度
	char   latWay;                        //经度方向  E-东 W-西
//	double floorHigh;                     //大地高
//	char   floorDW;                       //大地高单位
	double high;                          //高程值
	char   highDW;                        //高程单位

#ifdef HAVE_ROUHE
	unsigned char cLonDegree;				//经度:度
	unsigned char cLonMinute;				//经度:分
	//unsigned char cLonSecond;				//经度:秒
	double cLonSecond;
	unsigned char cLonMSecond;				//经度:0.1秒
	unsigned char cLatDegree;				//纬度:度
	unsigned char cLatMinute;				//纬度:分
	//unsigned char cLatSecond;				//纬度:秒
	double cLatSecond;
	unsigned char cLatMSecond;				//纬度:0.1秒

	double Latitude;						//纬度度表示
	double Logitude;						//经度度表示
#endif
};
struct TXRData
{
	short ixxleib;							//信息类别
	unsigned long userID;					//用户地址
	short idianwenxs;						//电文形式
	int isendtime;							//发信时间
	char ccontext[256];						//通信电文内容
};
struct TCSData
{
	char cFreNum[3];                      //信频点号
	char cZhiLuNum;                       //支路号
	short iInfoType;                      //信息类型
	int iStarNum[12];                     //卫星号
};
struct RMOData
{
	char cIntence[4];						//目标语句、语句标志符
	short iMode;							//模式
	int iFre;								//目标语句输出频度
};
struct SCSData
{	
	long lShiChaData;					//时差数据
};
struct PKYData
{
	int nWnzjs;							//WN周计数
	char chMyue[7];						//密钥
	char chKey[129];					//KEY
	char chB1[25];						//B1工作参数(24字节)
	char chB2[25];						//B2工作参数(24字节)
	char chB3[25];						//B3工作参数(24字节)
};
struct SBXData
{
	char chSbghname[10];				//设备供货商名称
	char chSblx[10];					//设备类型
	char chBbh[10];						//程序版本号
	char chCkxybbh[8];					//串口协议版本号
	char chIcdxybbh[10];				//ICD协议版本号
	int nSbxlh;							//设备序列号
	long nID;							//ID号
};

/*
struct BDR_DWXX		 //BD定位信息
{
	DWORD dUserAddr;		        //本机用户地址
	unsigned char cInfoSort;		//信息类别
	DWORD dQueryAddr;		        //查询地址
	unsigned char cPosTimeHour;		//定位时刻的小时数据
	unsigned char cPosTimeMinute;	//定位时刻的分位数据
	unsigned char cPosTimeSecond;   //定位时刻的秒位数据
	unsigned char cPosTimeMSecond;	//定位时刻的秒小数数据
	unsigned char cLonDegree;		//经度:度
	unsigned char cLonMinute;		//经度:分
	unsigned char cLonSecond;		//经度:秒
	unsigned char cLonMSecond;		//经度:0.1秒
	unsigned char cLatDegree;		//纬度:度
	unsigned char cLatMinute;		//纬度:分
	unsigned char cLatSecond;		//纬度:秒
	unsigned char cLatMSecond;		//纬度:0.1秒
	int dElevation;			    	//高程数据
	int dElvError;				    //高程异常值
    BOOL IsUrgent;		            //是否紧急
	BOOL IsMultiRes;		        //是否多解
	unsigned char Precision;	    //精度
	BOOL HeightType;	    	    //高程指示
	
	double Latitude;
	double Logitude;
};

struct BDR_TXXX			//通信信息
{
	DWORD dUserAddr;				//本机用户地址
	unsigned char cInfoSort;		//汉字、代码、混编
	unsigned char Infotype;	    	//信息类别
	
	DWORD dPostLetterAddr;			//发信方地址
	char cPostLetterTimeHour;		//发信时间:时
	char cPostLetterTimeMinute;		//发信时间:分
	short sTelexLen;				//电文长度
	char TelexContent[240];			//电文内容 ≤120个汉字或420个代码
	BOOL  CRCVerify;                //CRC出错标志
};
*/

/************************定义接收数据结构体结束*********************end************/
