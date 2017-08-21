#ifndef _SOMESTRUCT_ABOUT_TRIONES_
#define _SOMESTRUCT_ABOUT_TRIONES_

#define TRUE 1
#define FALSE 0

typedef unsigned long DWORD;
typedef long BOOL;
typedef unsigned char BYTE;

const int BASE_TYPE = 0x200;

#define TYPE_GLZK	(BASE_TYPE+1)	//功率状况
#define TYPE_ICXZ	(BASE_TYPE+2)	//IC信息指挥机
#define TYPE_ICXY	(BASE_TYPE+3)	//IC信息用户机
#define TYPE_DWXX	(BASE_TYPE+4)	//定位信息
#define TYPE_WZBG	(BASE_TYPE+5)	//位置报告
#define TYPE_TXXX	(BASE_TYPE+6)	//通信信息
#define TYPE_TXHZ	(BASE_TYPE+7)	//通信回执
#define TYPE_GLXX	(BASE_TYPE+8)	//管理信息
#define TYPE_KLSB	(BASE_TYPE+9)	//口令识别
#define TYPE_ZJXX	(BASE_TYPE+10)	//自检信息
#define TYPE_ZIHUI	(BASE_TYPE+11)	//自毁
#define TYPE_XHXX	(BASE_TYPE+12)	//序号信息
#define TYPE_SJXX	(BASE_TYPE+13)	//授时信息
#define TYPE_FKXX	(BASE_TYPE+14)  //反馈信息
#define TYPE_CRCE	(BASE_TYPE+15)	//校验错误


/*-----------------外包结构体-------------------*/

struct BDIPortData  //该结构体主要用于串口原始数据的传递，串口号的传递，数据类型的传递
{
	unsigned char sYnanshiData[256]; //用于存储串口接收的原始数据
	int  iDataLen;                  //用于存储接收到的数据的长度

	int DataType;                 //表示当前传输的数据的数据类型
	void *pData;                  //所发送的数据的指针，需要在解析时进行根据对应的数据类型进行强制转换
};

struct BDR_GLZK 		 //功率状况
{
	unsigned char cPower1;			//功率1	
	unsigned char cPower2;			//功率2	
	unsigned char cPower3;			//功率3	
	unsigned char cPower4;			//功率4	
	unsigned char cPower5;			//功率5	
	unsigned char cPower6;			//功率6	
};

struct BDR_GLXX 		 //管理信息
{
	unsigned char cManageInfo[32];			
};

struct BDR_CRCE			//CRC错误
{
	DWORD dUserAddr;
	unsigned char cErrorInfo[128];
};

struct BDR_ICXX_ZHJ 	//IC卡信息(指挥机)
{
	DWORD dAppAddr;					//本机地址
	DWORD dBrdcstAddr;				//通播ID
	unsigned char UserCharacter;    //用户特征
	short ServeFrequency;			//服务频度
	unsigned char cCommLevel;		//通信等级
	unsigned char cCnfdFlag;		//加密标志
	short UsersNum;			        //下属用户总数
};

struct BDR_ICXX_YHJ   //IC卡信息(用户机)
{
    DWORD dUnderUserAdd[500];      //下属用户地址
	short sUserCount;				//下属用户个数
};

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
	char TelexContent[256];			//电文内容 ≤120个汉字或420个代码
	BOOL  CRCVerify;                //CRC出错标志
};

struct BDR_TXHZ		//通信回执
{
	DWORD dUserAddr;		        //收信方地址
	short sHzCount;			        //回执数
	short cPostLetterTimeHour1;		//发信时间:时
	short cPostLetterTimeMinute1;	//发信时间:分
	short cHzTimeHour1;		        //回执时间:时
	short cHzTimeMinute1;		    //回执时间:分
	short cPostLetterTimeHour2;		//发信时间:时
	short cPostLetterTimeMinute2;	//发信时间:分
	short cHzTimeHour2;		        //回执时间:时
	short cHzTimeMinute2;		    //回执时间:分
	short cPostLetterTimeHour3;		//发信时间:时
	short cPostLetterTimeMinute3;	//发信时间:分
	short cHzTimeHour3;		        //回执时间:时
	short cHzTimeMinute3;		    //回执时间:分
	short cPostLetterTimeHour4;		//发信时间:时
	short cPostLetterTimeMinute4;	//发信时间:分
	short cHzTimeHour4;		        //回执时间:时
	short cHzTimeMinute4;		    //回执时间:分
	short cPostLetterTimeHour5;		//发信时间:时
	short cPostLetterTimeMinute5;	//发信时间:分
	short cHzTimeHour5;		        //回执时间:时
	short cHzTimeMinute5;		    //回执时间:分
};

struct BDR_ZHQR    //自毁确认
{
	DWORD dUserAdd;	//用户地址
	unsigned char ndata[5];//自毁信息
	short nZHQR;  //1自毁 0非自毁
};

struct BDR_ZJXX		//自检信息
{
	DWORD dUserAdd;	//用户地址
	unsigned char ICType;	//ic卡状态
	unsigned char YjState;	//硬件状态
	unsigned char DcDl;		//电池电量
	unsigned char RzState;	//入站状态

	BDR_GLZK Glzk;			//功率状况
};

struct BDR_SJXX		//时间信息
{
	DWORD dUserAdd;		//用户地址
	int iYear;		
	short iMonth;
	short iDay;
	short iHour;
	short iMinute;
	short iSecond;
};

struct BDR_XHXX		//序号信息
{
	DWORD dUserAdd;		//用户地址
	long xlh;		//序列号
};

/************************************************************************/
/*							新加接收部分                                */
/************************************************************************/

struct PY_DEF_TIME
{
	int iYear;
	int iMonth;
	int iDay;
	int iHour;
	int iMinute;
	int iSecond;
};

struct PY_DEF_POS
{
	unsigned char cLonDegree;		
	unsigned char cLonMinute;		
	unsigned char cLonSecond;		
	unsigned char cLonMSecond;		
	unsigned char cLatDegree;		
	unsigned char cLatMinute;		
	unsigned char cLatSecond;		
	unsigned char cLatMSecond;	
	int dHeigh;			    	
};



struct BDR_KLSB		//口令识别
{
	DWORD dUserAddr;				//用户地址
	unsigned char cInfoType;		//信息类别
	unsigned char cklyd;			//口令应答
	int iContentLen;				//电文长度
	unsigned char strContent[50];	//电文内容，最长83bit
};

struct BDR_DSJG		//定时结果 
{
	DWORD dUserAddr;				//用户地址
	PY_DEF_TIME struTime;			//时间
	unsigned char cbszk;			//波束状况
	unsigned char cgzzk;			//工况
	unsigned char cType;			//类型
	PY_DEF_POS struPos;				//用户位置
	DWORD dwlz;						//零值
	DWORD dwxzz;					//修正值	
};

struct BDR_ZBSC		//坐标输出
{
	DWORD dUserAddr;				//用户地址
	unsigned char cscType;			//坐标输出方式01H:空间直角，02H高斯平面，03H买卡托平面
	DWORD dwX;						//位置 X 0.1米
	DWORD dwY;						//位置 y 0.1米
	DWORD dwZorH;					//位置 Z 0.1米(空间直角)或者 H 1米(高斯或买卡托)
};

struct BDR_LZXX		//零值信息 
{
	DWORD dwUserAddr;				//用户地址
	DWORD dwjzz;					//校准零值
};

struct BDR_BBXX		//版本信息 
{
	DWORD dwUserAddr;				//用户地址
	unsigned char strBb[50];			//版本号
};

struct BDR_FKXX		//反馈信息
{
	DWORD dwUserAddr;				//用户地址
	unsigned char cFkbz;			//反馈标志
	unsigned char strFjxx[5];		//附加信息
	unsigned char strcontent[120];	//信息说明
};

/************************************************************************/
/*						  新加接收部分结束                              */
/************************************************************************/


/*--------------Send structures-------------*/


struct BDS_DWSQ		//定位申请
{
	DWORD dControlUserAddr;			//用户地址
	unsigned char cInfoSort;		//信息类别
	DWORD dHeight;					//高程和天线高
	DWORD dAirPressure;             //气压数据
	short dEntryFrequency;			//入站频度
};

struct BDS_TXSQ			//通信申请
{
	DWORD dControlUserAddr;			//受控用户地址
	unsigned char cInfoSort;		//信息类别
	DWORD dUserAddr;				//用户地址
	short sTelexLen;				//电文长度
	unsigned char TelexContent[240];//电文内容 ≤580bit通信等级3）
	unsigned char TeleNum[14];
};

#if 0 
extern "C" void Pos_Setting(short Frequence,short sHeightType,long lHeight,long lTianXian,double dQiYa,double dTemperature);
extern "C" void Send_PosReport(unsigned long lAppAddr,unsigned long lAddress);	
extern "C" int Pos_Single();
extern "C" int Pos_Sequence();
extern "C" int Pos_SendUrgent();
extern "C" int Tong_Bo(int nSelleibie,int nSelbianma,char* strMsgContent);
extern "C" void BoShu_Setting(unsigned char chxiangying,unsigned char chtime);  //波束设置
extern "C" int TongXin_Search(int nSearchType,long lAddress); 
extern "C" int OneUser_Comm(int nSelleibie,int nSelbianma,unsigned long lReceiveID,char * strMsgContent);
extern "C" BOOL IC_XLH(long lnumber);  //序列号
extern "C" void Destroy_Self();  //自毁
extern "C" int Pos_Search(int nSearchType,long lAddress);  //定位查询
extern "C" BOOL Read_ManageInfo();  //管信读取
extern "C" BOOL InPut_ManageInfo(char * strMangeInfo);  //管理信息
extern "C" int Position_Calling(long lAddress);

#endif

#endif
