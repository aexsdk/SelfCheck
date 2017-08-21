// BDAll.cpp : implementation file
//

/*#include "vxWorks.h"*/
/*#include "StdAfx.h"*/
#include "BDAll.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include <android/log.h>

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


namespace BD_I_PROTOCOL_PROCS
{
CBDAll *pBDAll;

/*LOCAL*/ BDR_ICXX_ZHJ g_ICInfo_ZHJ;         //IC信息(指挥机)
/*LOCAL*/ BDR_ICXX_YHJ g_ICInfo_YHJ;         //IC信息(用户机)
/*LOCAL*/ BDR_GLZK Glzk;                     //最新功率状况
/*LOCAL*/ BDS_DWSQ tempdwsq;                 //定位申请
/*LOCAL*/ BDR_DWXX *pDwxx = new BDR_DWXX;
/*LOCAL*/ BDS_TXSQ Wzbg;                     //位置报告申请

/*LOCAL*/ BOOL bGLJZ = FALSE;                //功率检测标志
/*LOCAL*/ BOOL bXTZJ = FALSE;                //系统自检标志
/*LOCAL*/ BOOL bUs232 = FALSE;               //串口打开标志

/*LOCAL*/ short gICtimes =0;                 //发送IC检测的次数
/*LOCAL*/ short sICJC = 0;                   //IC检测开始停止标志
/*LOCAL*/ short gxiashudegree=0;             //有几百个下属用户
/*LOCAL*/ short count_CheckTime =0;          //时间输入计时
/*LOCAL*/ short count_MsgApply = -1;				//通信申请计数器
/*LOCAL*/ short gflag_PositionType;				//定位方式   1单次  2连续 3紧急
/*LOCAL*/ short count_UrgentPosApply = -1;		//紧急定位申请计数器
/*LOCAL*/ short count_PosApply = -1;

/*LOCAL*/ short gsPosICFrequence;				//定位设置频率
/*LOCAL*/ short count_GSMApply = -1;			//GSM申请计数器
/*LOCAL*/ short gsICFrequence;                //IC卡服务频度(在定位设置中更改后还用该值)
/*LOCAL*/ short gsHeightType;                 //高程类型
/*LOCAL*/ long glHeight;                      //高程
/*LOCAL*/ long glTianXianHeight;              //天线高
/*LOCAL*/ double gdQiYa;                      //气压
/*LOCAL*/ double gdTemperature;               //温度
/*LOCAL*/ BOOL flag_StartSequencePos = FALSE; //连续定位开始停止标志
/*LOCAL*/ char g_zjxx[1024];
/*LOCAL*/ int g_npower;
/*LOCAL*/ int g_YIZHI;
/*LOCAL*/ BOOL bXHDQ = FALSE;
}	

using namespace BD_I_PROTOCOL_PROCS;
	

CBDAll::CBDAll(JNIEnv *env)
{
	pBDAll=this;
	memset(m_BDReceBuf, 0, 1024);

	if (env != NULL)
	{
		m_jniEnv = env;

		CJniDataSwitchCls jnicls(env);
//		jnicls.setPackName("com.app.dataprase.protocol4_0",strlen("com.app.dataprase.protocol4_0"));
		m_JniDataSwitchCls = jnicls;
	}
}
CBDAll::~CBDAll()
{

}

void CBDAll::ProcessData(int Len, unsigned char *buf/*,BDIPortData & portData*/)
{
	BDIPortData portData;

	unsigned char Temp[6] = "";
	unsigned char bdTempBuf[1024] = {0};
	memcpy(bdTempBuf, buf, Len);
	memcpy(Temp, bdTempBuf, 5);
	if(strcmp((char *)Temp,"$GLZK") == 0)  // 功率状况
	{
		memset(&Glzk,0,sizeof(BDR_GLZK));

		bGLJZ = TRUE;
		Glzk.cPower1 = buf[10];									//功率1
		Glzk.cPower2 = buf[11];									//功率2
		Glzk.cPower3 = buf[12];									//功率3
		Glzk.cPower4 = buf[13];									//功率4
		Glzk.cPower5 = buf[14];									//功率5
		Glzk.cPower6 = buf[15];									//功率6

		portData.iDataLen = Len;
		portData.DataType = TYPE_GLZK;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &Glzk;

	}
	else if(strcmp((char *)Temp,"$ICXX") == 0)  // IC信息
	{
		if(buf[10] == 0x00) //第0帧
		{
			memset(&g_ICInfo_ZHJ,0,sizeof(BDR_ICXX_ZHJ));

			((unsigned char*)(&(g_ICInfo_ZHJ.dAppAddr)))[0] = buf[9];			//用户地址
			((unsigned char*)(&(g_ICInfo_ZHJ.dAppAddr)))[1] = buf[8];			//用户地址
			((unsigned char*)(&(g_ICInfo_ZHJ.dAppAddr)))[2] = buf[7];			//用户地址
			((unsigned char*)(&(g_ICInfo_ZHJ.dBrdcstAddr)))[0] = buf[13];		//通播ID
			((unsigned char*)(&(g_ICInfo_ZHJ.dBrdcstAddr)))[1] = buf[12];		//通播ID
			((unsigned char*)(&(g_ICInfo_ZHJ.dBrdcstAddr)))[2] = buf[11];		//通播ID
			g_ICInfo_ZHJ.UserCharacter = buf[14];								//用户特征
			((unsigned char*)(&(g_ICInfo_ZHJ.ServeFrequency)))[0] = buf[16];	//服务频度
			((unsigned char*)(&(g_ICInfo_ZHJ.ServeFrequency)))[1] = buf[15];	//服务频度
			g_ICInfo_ZHJ.cCommLevel = buf[17];									//通信等级
			g_ICInfo_ZHJ.cCnfdFlag  = buf[18];									//加密标志
			((unsigned char*)(&(g_ICInfo_ZHJ.UsersNum)))[0] = buf[20];			//下属用户总数
			((unsigned char*)(&(g_ICInfo_ZHJ.UsersNum)))[1] = buf[19];			//下属用户总数
			
			gsICFrequence = g_ICInfo_ZHJ.ServeFrequency;
			
			//以下屏蔽最终要打开，测协议时屏蔽的
			/*			if((g_ICInfo_ZHJ.UserCharacter!=0)&&(g_ICInfo_ZHJ.UserCharacter!=4))//用用户特征来判断插入的是不是指挥机
			{
			sICJC=1;//停止发送检查下属用户机的ICJC,是一般用户机
			//::PostMessage(hwnd_exe,UM_ICXX_SAVE,0,0);    //保存IC信息
			}
			else
			{	*/
			
			if((g_ICInfo_ZHJ.UsersNum%100)!=0)
				gxiashudegree =g_ICInfo_ZHJ.UsersNum/100+1;
			else
				gxiashudegree =g_ICInfo_ZHJ.UsersNum/100;
			gICtimes++;
			//	}

			portData.iDataLen = Len;
			memcpy(portData.sYnanshiData,buf,sizeof(unsigned char )*Len);
			portData.pData = &g_ICInfo_ZHJ;
			portData.DataType = TYPE_ICXZ;

		}
		else              //下属用户信息
		{			
			short sIclen = 0; //用来计算有多少个下属用户
			((unsigned char*)(&(sIclen)))[0] = buf[6];		            	//长度
			((unsigned char*)(&(sIclen)))[1] = buf[5];		            	//长度
			sIclen = sIclen-12;
			g_ICInfo_YHJ.sUserCount = sIclen;

			gICtimes++;

			int gsUserAddCount = 0;

			for(int nUseAdd=0;nUseAdd<sIclen;)
			{
				((unsigned char*)(&(g_ICInfo_YHJ.dUnderUserAdd[gsUserAddCount])))[2] = buf[nUseAdd+11];									
				((unsigned char*)(&(g_ICInfo_YHJ.dUnderUserAdd[gsUserAddCount])))[1] = buf[nUseAdd+12];								
				((unsigned char*)(&(g_ICInfo_YHJ.dUnderUserAdd[gsUserAddCount])))[0] = buf[nUseAdd+13];
				nUseAdd+=3;
				gsUserAddCount++;
			}	
			if(gICtimes>=gxiashudegree)                     //停止发送IC检测
			{
// 				sICJC = 1; 	
// 
// 				portData.iDataLen = Len;
// 				memcpy(portData.sYnanshiData,buf,sizeof(unsigned char )*Len);
// 				portData.pData = &g_ICInfo_YHJ;
// 				portData.DataType = TYPE_ICXY;

// 				MsgData msgdata;
// 				memset(&msgdata,0,sizeof(MsgData));
// 				msgdata.nType = TYPE_ICXY;
// 				msgdata.pData = &g_ICInfo_YHJ;
// 				if (m_SendMsg_ID != NULL)
// 				{
// 					msgQSend(m_SendMsg_ID,(char *)&msgdata,sizeof(msgdata),WAIT_FOREVER,MSG_PRI_NORMAL);
// 				}
			}
		}
	}
	else if(strcmp((char *)Temp,"$CRCE") == 0) //定位信息CRC错误
	{
		DWORD  dAppAddr;
		((unsigned char*)(&(dAppAddr)))[0] = buf[9];			//用户地址
		((unsigned char*)(&(dAppAddr)))[1] = buf[8];			//用户地址
		((unsigned char*)(&(dAppAddr)))[2] = buf[7];			//用户地址

		BDR_CRCE CRCError;
		memset(&CRCError,0,sizeof(BDR_CRCE));

		CRCError.dUserAddr = dAppAddr;
		sprintf((char *)CRCError.cErrorInfo,"用户【%ld】定位CRC出错",dAppAddr);
		
		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.DataType = TYPE_CRCE;
		portData.pData = &CRCError;
		
	}	
	else if(strcmp((char *)Temp,"$XHXX") == 0) //序号信息
	{

		LOGI("in xhxx");

		BDR_XHXX xhxx;
		memset(&xhxx,0,sizeof(BDR_XHXX));

		xhxx.dUserAdd = buf[7] << 16;
		xhxx.dUserAdd += buf[8] << 8;
		xhxx.dUserAdd += buf[9];

		xhxx.xlh = (buf[10]<<24)+(buf[11]<<16)+(buf[12]<<8)+buf[13];	
		bXHDQ = TRUE;

		portData.iDataLen = Len;
		portData.pData = &xhxx;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.DataType = TYPE_XHXX;

	}

	else if(strcmp((char *)Temp,"$SJXX") == 0) //时间信息
	{
		BDR_SJXX bdtime;
		memset(&bdtime,0,sizeof(BDR_SJXX));

// 		((unsigned char*)(&(bdtime.dUserAdd)))[0] = buf[9];			//用户地址
// 		((unsigned char*)(&(bdtime.dUserAdd)))[1] = buf[8];			//用户地址
// 		((unsigned char*)(&(bdtime.dUserAdd)))[2] = buf[7];			//用户地址

 		bdtime.dUserAdd = buf[7] << 16;
		bdtime.dUserAdd += buf[8] << 8;
		bdtime.dUserAdd += buf[9];

// 		((unsigned char*)(&(bdtime.iYear)))[0] = buf[11];
// 		((unsigned char*)(&(bdtime.iYear)))[1] = buf[10];

		bdtime.iYear = buf[10] << 8;
		bdtime.iYear += buf[11];
		
		bdtime.iMonth        = buf[12];
		bdtime.iDay          = buf[13];
		bdtime.iHour         = buf[14];

		bdtime.iMinute       = buf[15];
		bdtime.iSecond       = buf[16];

		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &bdtime;
		portData.DataType = TYPE_SJXX;

	}	

	else if(strcmp((char *)Temp,"$ZHQR") == 0) //自毁确认
	{
		BDR_ZHQR p_zhqr;
		memset(&p_zhqr,0,sizeof(BDR_ZHQR));
		
		((unsigned char*)(&(p_zhqr.dUserAdd)))[0] = buf[9];			//用户地址
		((unsigned char*)(&(p_zhqr.dUserAdd)))[1] = buf[8];			//用户地址
		((unsigned char*)(&(p_zhqr.dUserAdd)))[2] = buf[7];			//用户地址
		
		p_zhqr.ndata[0] = buf[10];
		p_zhqr.ndata[1] = buf[11];
		p_zhqr.ndata[2] = buf[12];
		p_zhqr.ndata[3] = buf[13];

		if (p_zhqr.ndata[0] == 0xaa &&
			p_zhqr.ndata[1] == 0x55 &&
			p_zhqr.ndata[2] == 0x55 &&
			p_zhqr.ndata[3] == 0xaa)
		{
			p_zhqr.nZHQR = 1;
		}
		else
			p_zhqr.nZHQR = 0;

		portData.iDataLen = Len;
		portData.DataType = TYPE_ZIHUI;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &p_zhqr;

	}

	else if(strcmp((char *)Temp,"$GBQR") == 0) //关闭确认
	{	
		;
	}

	else if(strcmp((char *)Temp,"$ZJXX") == 0) //自检信息
	{
		bXTZJ = TRUE;

		BDR_ZJXX zjxx;
		memset(&zjxx,0,sizeof(BDR_ZJXX));
		
		((unsigned char*)(&(zjxx.dUserAdd)))[0] = buf[9];			//用户地址
		((unsigned char*)(&(zjxx.dUserAdd)))[1] = buf[8];			//用户地址
		((unsigned char*)(&(zjxx.dUserAdd)))[2] = buf[7];			//用户地址

		zjxx.Glzk.cPower1 = buf[14];		//波束1							
		zjxx.Glzk.cPower2 = buf[15];		//波束2								
		zjxx.Glzk.cPower3 = buf[16];		//波束3								
		zjxx.Glzk.cPower4 = buf[17];		//波束4
		zjxx.Glzk.cPower5 = buf[18];		//波束5							
		zjxx.Glzk.cPower6 = buf[19];		//波束6


		zjxx.ICType = buf[10];				//ic状态
		zjxx.YjState = buf[11];				//硬件状态
		zjxx.DcDl = buf[12];				//电池电量
		zjxx.RzState = buf[13];				//入站状态
		
		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &zjxx;
		portData.DataType = TYPE_ZJXX;
 		

/*		memset(g_zjxx,0,sizeof(g_zjxx));
		if((buf[10]&0x01)==1) //本机配置智能卡
		{
			if(((buf[10]&0x02)>>1)==1)//智能卡
			{
				sprintf(g_zjxx + strlen(g_zjxx),"智能卡处理异常;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"智能卡处理正常;");
			}
			if(((buf[10]&0x04)>>2)==1)//ID号
			{
				sprintf(g_zjxx + strlen(g_zjxx),"ID号出错;");

			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"ID号正常;");
			}
			if(((buf[10]&0x08)>>3)==1)//校验码
			{
				sprintf(g_zjxx + strlen(g_zjxx),"校验码出错;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"校验码正确;");
			}
			if(((buf[10]&0x10)>>4)==1)//序列号
			{
				sprintf(g_zjxx + strlen(g_zjxx),"序列号出错;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"序列号正常;");
			}
			if(((buf[10]&0x20)>>5)==1)//管理卡
			{
				sprintf(g_zjxx + strlen(g_zjxx),"管理卡;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"用户卡;");
			}
			if(((buf[10]&0x40)>>6)==1)//智能卡数据
			{
				sprintf(g_zjxx + strlen(g_zjxx),"职能卡数据不完整;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"职能卡数据正常;");
			}
			if(((buf[10]&0x80)>>7)==1)//职能卡
			{
				sprintf(g_zjxx + strlen(g_zjxx),"职能卡物理缺损;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"职能卡正常;");
			}
		}
		else                   //本机配置保密模块
		{
			if(((buf[10]&0x02)>>1)==1)//保密模块
			{
				sprintf(g_zjxx + strlen(g_zjxx),"保密模块异常;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"保密模块正常;");
			}
			if(((buf[10]&0x04)>>2)==1)//ID号
			{
				sprintf(g_zjxx + strlen(g_zjxx),"ID号出错;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"ID号正常;");
			}
			if(((buf[10]&0x08)>>3)==1)//IC卡
			{
				sprintf(g_zjxx + strlen(g_zjxx),"无IC卡;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"有IC卡;");
			}
			if(((buf[10]&0x10)>>4)==1)//序列号
			{
				sprintf(g_zjxx + strlen(g_zjxx),"序列号出错;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"序列号正常;");
			}
			if(((buf[10]&0x20)>>5)==1)//双口RAM
			{
				sprintf(g_zjxx + strlen(g_zjxx),"双口RAM出错;");
			}
			else
			{
				sprintf(g_zjxx + strlen(g_zjxx),"双口RAM正常;");

			}	
		}
		if((buf[11]&0x01)==1)     //天线
		{
			sprintf(g_zjxx + strlen(g_zjxx),"天线未连接;");
		}
		else
		{
			sprintf(g_zjxx + strlen(g_zjxx),"天线正常;");
		}
		if(((buf[11]&0x02)>>1)==1)//通道
		{
			sprintf(g_zjxx + strlen(g_zjxx),"通道故障;");

		}
		else
		{
			sprintf(g_zjxx + strlen(g_zjxx),"通道正常;");

		}
		if(((buf[11]&0x04)>>2)==1)//主板
		{
			sprintf(g_zjxx + strlen(g_zjxx),"主板故障;");
		}
		else
		{
			sprintf(g_zjxx + strlen(g_zjxx),"主板正常;");
		}
		if((buf[13]&0x01)==1)     //天线
		{
			sprintf(g_zjxx + strlen(g_zjxx),"抑制;");
		}
		else
		{
			sprintf(g_zjxx + strlen(g_zjxx),"非抑制;");
		}
		if(((buf[13]&0x02)>>1)==1)//通道
		{
			sprintf(g_zjxx + strlen(g_zjxx),"无线电正常;");

		}
		else
		{
			sprintf(g_zjxx + strlen(g_zjxx),"无线电静默;");
		}
		sprintf(g_zjxx + strlen(g_zjxx),"电池电量：%x ％",buf[12]);
		g_npower = int(buf[12]);
		g_YIZHI = buf[13]&0x02;
*/
		
	}
	
	else if (strcmp((char *)Temp,"$DWXX") == 0)	// 定位信息
	{
		memset(pDwxx,0,sizeof(BDR_DWXX));
		
		((unsigned char*)(&pDwxx->dUserAddr))[0] = buf[9];			//用户地址
		((unsigned char*)(&pDwxx->dUserAddr))[1] = buf[8];			//用户地址
		((unsigned char*)(&pDwxx->dUserAddr))[2] = buf[7];			//用户地址
		pDwxx->cInfoSort = buf[10];							        //信息类别
		((unsigned char*)(&pDwxx->dQueryAddr))[0] = buf[13];		//查询地址
		((unsigned char*)(&pDwxx->dQueryAddr))[1] = buf[12];		//查询地址
		((unsigned char*)(&pDwxx->dQueryAddr))[2] = buf[11];		//查询地址
		pDwxx->cPosTimeHour = buf[14];								//定位时刻的小时数据
		pDwxx->cPosTimeMinute = buf[15];							//定位时刻的分位数据
		pDwxx->cPosTimeSecond = buf[16];							//定位时刻的秒位数据
		pDwxx->cPosTimeMSecond = buf[17];							//定位时刻的秒小数数据
		pDwxx->cLonDegree = buf[18];								//经度:度
		pDwxx->cLonMinute = buf[19];								//     分
		pDwxx->cLonSecond = buf[20];								//     秒
		pDwxx->cLonMSecond = buf[21];								//  0.1秒
		pDwxx->cLatDegree = buf[22];								//纬度:度
		pDwxx->cLatMinute = buf[23];								//     分
		pDwxx->cLatSecond = buf[24];								//     秒
		pDwxx->cLatMSecond = buf[25];								//  0.1秒
		
		if((pDwxx->cInfoSort & 0x01) == 0)                          //cInfoSort的最后一位是0则是“普通”
		{
			short height,flag;
			((unsigned char*)(&height))[0] = buf[27];               //高程
			((unsigned char*)(&height))[1] = buf[26];
			flag=height>>14;										//高程的高2位是符号位，后14位为值
			pDwxx->dElevation = height&0x3FFF;						//高程数据  0x3FFF == 000011111111111111
			if (flag) pDwxx->dElevation = -pDwxx->dElevation;		//高程数据
			
			((unsigned char*)(&height))[0] = buf[29];               //高程异常
			((unsigned char*)(&height))[1] = buf[28];
			flag=height>>8;											//高程异常的高8位是符号位，低8位为值
			pDwxx->dElvError = height&0x00FF;						//高程异常值
			if (flag) pDwxx->dElvError = -pDwxx->dElvError;			//高程异常值
		}
		if((pDwxx->cInfoSort & 0x01) == 1)                          //cInfoSort的最后一位是1则是“高空”
		{
			((unsigned char*)(&pDwxx->dElevation))[0] = buf[28];    //高程数据
			((unsigned char*)(&pDwxx->dElevation))[1] = buf[27];    //高程数据
			((unsigned char*)(&pDwxx->dElevation))[2] = buf[26];    //高程数据
		}
		pDwxx->IsUrgent = BOOL((pDwxx->cInfoSort & 0x04)>>2);		 //是否紧急，cInfoSort第6位是紧急标志，1则是紧急
		pDwxx->IsMultiRes = BOOL((pDwxx->cInfoSort & 0x02)>>1);		 //是否多解，cInfoSort第7位是多解标志，1则是多解
		pDwxx->Precision = (pDwxx->cInfoSort & 0x08)>>3;	    	 //精度，cInfoSort第5位是精度标志，1则是二档
		pDwxx->HeightType = BOOL(pDwxx->cInfoSort & 0x01);	    	 //高程指示，cInfoSort第8位是高程标志，1则是高空


		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.DataType = TYPE_DWXX;
		portData.pData = pDwxx;

	}

	else if (strcmp((char *)Temp,"$TXXX") == 0)	// 通信信息
	{
		BDR_TXXX pTxxx;
		memset(&pTxxx,0,sizeof(BDR_TXXX));
		((unsigned char*)(&pTxxx.dUserAddr))[0] = buf[9];		 //用户地址
		((unsigned char*)(&pTxxx.dUserAddr))[1] = buf[8];		 //用户地址
		((unsigned char*)(&pTxxx.dUserAddr))[2] = buf[7];		 //用户地址
		pTxxx.cInfoSort = buf[10];								 //信息类别
		((unsigned char*)(&pTxxx.dPostLetterAddr))[0] = buf[13];//发信方地址
		((unsigned char*)(&pTxxx.dPostLetterAddr))[1] = buf[12];//发信方地址
		((unsigned char*)(&pTxxx.dPostLetterAddr))[2] = buf[11];//发信方地址
		pTxxx.cPostLetterTimeHour = buf[14];					 //发信时间:时
		pTxxx.cPostLetterTimeMinute = buf[15];					 //发信时间:分
		
		((unsigned char*)(&pTxxx.sTelexLen))[0] = buf[17];		 //长度
		((unsigned char*)(&pTxxx.sTelexLen))[1] = buf[16];		 //长度
		
		short telexlentemp;                                      //电文长度(字节数)
		if((pTxxx.sTelexLen%8)!=0)//BCD码的情况
		{
			telexlentemp=pTxxx.sTelexLen/8+1;
		}
		else                                           
		{
			telexlentemp=pTxxx.sTelexLen/8;                //电文长度/8
		}
		memcpy(pTxxx.TelexContent,buf+18,telexlentemp);	//电文内容≤120个汉字或420个代码
		
		unsigned char cCRC_Flag;
		memcpy(&cCRC_Flag,buf+(telexlentemp+18),1);			//得到CRC标志，校验通信信息是否正确
		if(cCRC_Flag==0x00)
		{
			pTxxx.CRCVerify=FALSE;//CRC正确
		}
		else
		{
			pTxxx.CRCVerify=TRUE; //CRC出错
			BDR_CRCE CRCError;
			sprintf((char *)CRCError.cErrorInfo,"接收短信CRC错误");


			portData.iDataLen = Len;
			portData.DataType = TYPE_CRCE;
			memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
			portData.pData = &CRCError;

		}
		if((unsigned char)(pTxxx.TelexContent[0]) == 0xa0
			&& ((pTxxx.cInfoSort & 0x20)>>5) == 1) //位置报告(信息类别是"代码")
		{
			BDR_DWXX pwzbg;
			memset(&pwzbg,0,sizeof(BDR_DWXX));
			pwzbg.dUserAddr = pTxxx.dPostLetterAddr;
			pwzbg.cPosTimeHour = (pTxxx.TelexContent[1]&0xf8)>>3;								//定位时刻的小时数据
			pwzbg.cPosTimeMinute = ((pTxxx.TelexContent[1]&0x07)<<3) | ((pTxxx.TelexContent[2]&0xe0)>>5);							//定位时刻的分位数据
			pwzbg.cPosTimeSecond = ((pTxxx.TelexContent[2]&0x1f)<<1) | ((pTxxx.TelexContent[3]&0x80)>>7);							//定位时刻的秒位数据
			pwzbg.cPosTimeMSecond = pTxxx.TelexContent[3]&0x7f;							    //定位时刻的秒小数数据
			pwzbg.cLonDegree = pTxxx.TelexContent[4];							        	    //经度:度
			pwzbg.cLonMinute = (pTxxx.TelexContent[5]&0xfc)>>2;								//经度:分
			pwzbg.cLonSecond = ((pTxxx.TelexContent[5]&0x03)<<4) | ((pTxxx.TelexContent[6]&0xf0)>>4);								//经度:秒
			pwzbg.cLonMSecond = pTxxx.TelexContent[6]&0x0f;								    //经度:0.1秒
			pwzbg.cLatDegree = (pTxxx.TelexContent[7]&0xfc)>>2;								//纬度:度
			pwzbg.cLatMinute = ((pTxxx.TelexContent[7]&0x03)<<4) | ((pTxxx.TelexContent[8]&0xf0)>>4);								//纬度:分
			pwzbg.cLatSecond = ((pTxxx.TelexContent[8]&0x0f)<<2) | ((pTxxx.TelexContent[9]&0xc0)>>6);								//纬度:秒
			pwzbg.cLatMSecond = (pTxxx.TelexContent[9]&0x3c)>>2;								//纬度:0.1秒
			
			if((pTxxx.TelexContent[9] & 0x03) == 0)//普通
			{
				short height,flag;
				((unsigned char*)(&height))[0] = (((pTxxx.TelexContent[11]&0xfe)>>1)&0x7f)|((pTxxx.TelexContent[10]&0x01)<<7);
				((unsigned char*)(&height))[1] = (pTxxx.TelexContent[10]&0x7f)>>1;
				
				flag=(pTxxx.TelexContent[10]&0x80)>>7;		
				pwzbg.dElevation = height&0x7FFF;					 //高程数据
				if (flag) 
					pwzbg.dElevation = -pwzbg.dElevation;		     //高程数据
				height = pTxxx.TelexContent[12];
				flag = pTxxx.TelexContent[11]&0x01;				
				pwzbg.dElvError = height&0x00FF;					 //高程异常值
				if (flag) 
					pwzbg.dElvError = -pwzbg.dElvError;		     //高程异常值
			}
			if((pTxxx.TelexContent[9] & 0x03) == 1)//高空
			{
				((unsigned char*)(&pwzbg.dElevation))[0] = pTxxx.TelexContent[12];  //高程数据
				((unsigned char*)(&pwzbg.dElevation))[1] = pTxxx.TelexContent[11];  //高程数据
				((unsigned char*)(&pwzbg.dElevation))[2] = pTxxx.TelexContent[10];  //高程数据
			}

			portData.iDataLen = Len;
			memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
			portData.DataType = TYPE_WZBG;
			portData.pData = &pwzbg;

//			::SendMessage(hwnd_exe,UM_WZBG,WPARAM(pwzbg),0);
//			delete pwzbg;
		}
		else //短信
		{
			BDR_TXXX pTxxx_REC;
			memset(&pTxxx_REC,0,sizeof(BDR_TXXX));
						
			pTxxx_REC.dUserAddr = pTxxx.dUserAddr;		          //用户地址
			pTxxx_REC.dPostLetterAddr = pTxxx.dPostLetterAddr;      //发信方地址
			pTxxx_REC.cPostLetterTimeHour = pTxxx.cPostLetterTimeHour;
			pTxxx_REC.cPostLetterTimeMinute = pTxxx.cPostLetterTimeMinute;
			pTxxx_REC.cInfoSort = (pTxxx.cInfoSort & 0x20)>>5;     //信息类别第6位(0代码/1汉字)
			pTxxx_REC.Infotype = pTxxx.cInfoSort;
			if((unsigned char)(pTxxx.TelexContent[0]) == 0xa4)//混编
				pTxxx_REC.cInfoSort = 2;

			pTxxx_REC.CRCVerify = pTxxx.CRCVerify;  //CRC出错标志
			
			if(pTxxx_REC.cInfoSort==0) //汉字
			{
				
				m_JniDataSwitchCls.OnProcssHanziTx(pTxxx.TelexContent,pTxxx.sTelexLen/8);

				memcpy(pTxxx_REC.TelexContent,pTxxx.TelexContent,strlen(pTxxx.TelexContent));

				portData.iDataLen = Len;
				memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
				portData.DataType = TYPE_TXXX;
				portData.pData = &pTxxx_REC;

			}
			else if(pTxxx_REC.cInfoSort==1)//代码
			{
				if((pTxxx.sTelexLen%8)!=0)
				{
					char str[512] = "0";
					for(int i=0;i<=(pTxxx.sTelexLen/8);i++)
					{
						sprintf(str+strlen(str),"%02x",(unsigned char)(pTxxx.TelexContent[i]));
					}
					int str_length=strlen(str);
					memcpy(pTxxx_REC.TelexContent,str,str_length-1);

					portData.iDataLen = Len;
					memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
					portData.DataType = TYPE_TXXX;
					portData.pData = &pTxxx_REC;

				}
				else
				{
					char str[512] = {0};
					for(int i=0;i<(pTxxx.sTelexLen/8);i++)
					{
						sprintf(str+strlen(str),"%02x",(unsigned char)(pTxxx.TelexContent[i]));
					}
					strcpy(pTxxx_REC.TelexContent,str);

					portData.iDataLen = Len;
					memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
					portData.DataType = TYPE_TXXX;
					portData.pData = &pTxxx_REC;

				}
			}
			else if(pTxxx_REC.cInfoSort==2)//混合
			{
				m_JniDataSwitchCls.OnProcssHanziTx(pTxxx.TelexContent+1,pTxxx.sTelexLen/8-1);
				memcpy(pTxxx_REC.TelexContent,pTxxx.TelexContent+1,strlen(pTxxx.TelexContent) -1);
				
		
				portData.iDataLen = Len;
				memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
				portData.DataType = TYPE_TXXX;
				portData.pData = &pTxxx_REC;	
				
			}

		}

	}
	
	else if (strcmp((char *)Temp,"$TXHZ") == 0)	//通信回执
	{
		BDR_TXHZ pTxhz;
		memset(&pTxhz,0,sizeof(BDR_TXHZ));
		
		((unsigned char*)(&pTxhz.dUserAddr))[0] = buf[12];		//用户地址
		((unsigned char*)(&pTxhz.dUserAddr))[1] = buf[11];		//用户地址
		((unsigned char*)(&pTxhz.dUserAddr))[2] = buf[10];		//用户地址
       
		pTxhz.sHzCount = (int)buf[13];
        pTxhz.cPostLetterTimeHour1 = (int)buf[14];
        pTxhz.cPostLetterTimeMinute1 =(int)buf[15];
        pTxhz.cHzTimeHour1 =(int)buf[16];
        pTxhz.cHzTimeMinute1=(int)buf[17];

        pTxhz.cPostLetterTimeHour2 = (int)buf[18];
        pTxhz.cPostLetterTimeMinute2 =(int)buf[19];
        pTxhz.cHzTimeHour2 =(int)buf[20];
        pTxhz.cHzTimeMinute2=(int)buf[21];

		pTxhz.cPostLetterTimeHour3 = (int)buf[22];
        pTxhz.cPostLetterTimeMinute3 =(int)buf[23];
        pTxhz.cHzTimeHour3 =(int)buf[24];
        pTxhz.cHzTimeMinute3=(int)buf[25];

		pTxhz.cPostLetterTimeHour4 = (int)buf[26];
        pTxhz.cPostLetterTimeMinute4 =(int)buf[27];
        pTxhz.cHzTimeHour4 =(int)buf[28];
        pTxhz.cHzTimeMinute4=(int)buf[29];

		pTxhz.cPostLetterTimeHour5 = (int)buf[30];
        pTxhz.cPostLetterTimeMinute5 =(int)buf[31];
        pTxhz.cHzTimeHour5 =(int)buf[32];
        pTxhz.cHzTimeMinute5=(int)buf[33];

		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &pTxhz;
		portData.DataType = TYPE_TXHZ;

	}

	else if (strcmp((char *)Temp,"$GLXX") == 0)	//管理信息
	{
		BDR_GLXX  pGlxx;
		memset(&pGlxx,0,sizeof(BDR_GLXX));

		for(int i=0;i<32;i++)
		{
			pGlxx.cManageInfo[i] = buf[i+10];
		}
		
		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.pData = &pGlxx;
		portData.DataType = TYPE_GLXX;

	}
	else if (strcmp((char *)Temp,"$KLSB") == 0)		//口令识别
	{
		BDR_KLSB klsbData;
		memset(&klsbData,0,sizeof(BDR_KLSB));

// 		((unsigned char*)(&klsbData.dUserAddr))[0] = buf[9];			//用户地址
// 		((unsigned char*)(&klsbData.dUserAddr))[1] = buf[8];			//用户地址
// 		((unsigned char*)(&klsbData.dUserAddr))[2] = buf[7];			//用户地址

		klsbData.dUserAddr = buf[7] << 16;
		klsbData.dUserAddr += buf[8] << 8;
		klsbData.dUserAddr += buf[9];

		klsbData.cInfoType = buf[10];		//信息类别
		klsbData.cklyd = buf[11];			//口令应答
		klsbData.iContentLen = buf[12];		//电文长度

		short telexlentemp;                                      //电文长度(字节数)
		if((klsbData.iContentLen%8)!=0)//BCD码的情况
		{
			telexlentemp=klsbData.iContentLen/8+1;
		}
		else                                           
		{
			telexlentemp=klsbData.iContentLen/8;                //电文长度/8
		}

		short infotype = (klsbData.cInfoType & 0x02) >> 1;
		if (infotype == 0)	//汉子
		{
			memcpy(klsbData.strContent,buf+13,telexlentemp);		//电文内容
		}
		else if (infotype == 1)		//代码
		{
			if((klsbData.iContentLen%8)!=0)
			{
				char str[512] = {0};
				for(int i=0;i<=(klsbData.iContentLen/8);i++)
				{
					sprintf(str+strlen(str),"%02x",(unsigned char)(buf[i+13]));
				}
				int str_length=strlen(str);
				memcpy(klsbData.strContent,str,str_length-1);				
			}
			else
			{
				char str[512] = "0";
				for(int i=0;i<(klsbData.iContentLen/8);i++)
				{
					sprintf(str+strlen(str),"%02x",buf[i+13]);
				}
				strcpy((char *)klsbData.strContent,str);				
			}
		}

		portData.iDataLen = Len;
		memcpy(portData.sYnanshiData,buf,sizeof(unsigned char)*Len);
		portData.DataType = TYPE_KLSB;
		portData.pData = &klsbData;
		
	}
	else if (strcmp((char *)Temp,"$FKXX") == 0)		//反馈信息
	{
		BDR_FKXX g_bdFkxx;
		memset(&g_bdFkxx,0,sizeof(BDR_FKXX));

		g_bdFkxx.dwUserAddr = buf[7]<<16;
		g_bdFkxx.dwUserAddr += buf[8]<<8;
		g_bdFkxx.dwUserAddr += buf[9];

		g_bdFkxx.cFkbz = buf[10];

		memcpy(g_bdFkxx.strFjxx,buf+11,sizeof(unsigned char)*4);

		if (0x00 == g_bdFkxx.cFkbz)		//成功
		{
			memcpy(g_bdFkxx.strcontent,"发送成功",sizeof("发送成功"));
		}
		else if (0x01 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"发送失败",sizeof("发送失败"));
		}
		else if (0x02 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"信号未锁定",sizeof("信号未锁定"));
		}
		else if (0x03 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"电量不足,发射抑制",sizeof("电量不足,发射抑制"));
		}
		else if (0x04 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"服务频度未到",sizeof("服务频度未到"));
		}
		else if (0x05 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"加解密错误",sizeof("加解密错误"));
		}
		else if (0x06 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"CRC错误",sizeof("CRC错误"));
		}
		else if (0x07 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"系统抑制",sizeof("系统抑制"));
		}
		else if (0x08 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"无线电静默",sizeof("无线电静默"));
		}
		else if (0x09 == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"无标识信息",sizeof("无标识信息"));
		}
		else if (0x0a == g_bdFkxx.cFkbz)
		{
			memcpy(g_bdFkxx.strcontent,"无航线信息",sizeof("无航线信息"));
		}

		portData.iDataLen = Len;
		portData.pData = &g_bdFkxx;
		portData.DataType = TYPE_FKXX;
		memcpy(portData.sYnanshiData,buf,Len);
	}
	SendMessage(&portData);
}
/*-------------------------BD SEND----------------------------*/

#if 0 
void Pos_Setting(short Frequence,short sHeightType,long lHeight,long lTianXian,double dQiYa,double dTemperature)
{
	gsPosICFrequence = Frequence;
	gsHeightType = sHeightType;
	glHeight = lHeight;
	glTianXianHeight = lTianXian;
	gdQiYa = dQiYa;
	gdTemperature = dTemperature;
}


//发送位置报告
void Send_PosReport(unsigned long lAppAddr,unsigned long lAddress)	
{	
	unsigned char pchar[13];

	memset(pchar,0,13);

	pchar[0] = 0xa0;

	pchar[1] = (((pDwxx->cPosTimeHour)&0x1f)<<3) | (((pDwxx->cPosTimeMinute)>>3)&0x07);
	pchar[2] = (((pDwxx->cPosTimeMinute) & 0x07)<<5) | (((pDwxx->cPosTimeSecond)>>1)&0x1f);
	pchar[3] = (((pDwxx->cPosTimeSecond) & 0x01)<<7) | ((pDwxx->cPosTimeMSecond)&0x7f);

	pchar[4] = pDwxx->cLonDegree;
	pchar[5] = (((pDwxx->cLonMinute) & 0x3f)<<2) | (((pDwxx->cLonSecond)>>4)&0x03);
	pchar[6] = (((pDwxx->cLonSecond) & 0x0f)<<4) | ((pDwxx->cLonMSecond)&0x0f);

	pchar[7] = (((pDwxx->cLatDegree) & 0x3f)<<2) | (((pDwxx->cLatMinute)>>4)&0x03);
	pchar[8] = (((pDwxx->cLatMinute) & 0x0f)<<4) | (((pDwxx->cLatSecond)>>2)&0x0f);
	pchar[9] = (((pDwxx->cLatSecond) & 0x03)<<6) | (((pDwxx->cLatMSecond)&0x0f)<<2) | 0x00;

	int tempElevation = abs(pDwxx->dElevation);
	int tempElvError = abs(pDwxx->dElvError);


	if(pDwxx->dElevation < 0)
	{
		pchar[10] = (0x01<<7) | (((((unsigned char*)(&tempElevation))[1])&0x3f)<<1)
					| (((((unsigned char*)(&tempElevation))[0])&0x80)>>7);
	}
	else
	{
		pchar[10] = (0x00<<7) | (((((unsigned char*)(&tempElevation))[1])&0x3f)<<1)
					| (((((unsigned char*)(&tempElevation))[0])&0x80)>>7);
	}

	if(pDwxx->dElvError < 0)
	{
		pchar[11] = 0x01 | (((((unsigned char*)(&tempElevation))[0])&0x7f)<<1);
	}
	else
	{
		pchar[11] = 0x00 | (((((unsigned char*)(&tempElevation))[0])&0x7f)<<1);
	}
	pchar[12] = tempElvError;//高程异常

	for(int i = 0; i<13; i++)
	{
		Wzbg.TelexContent[i] = pchar[i];
	}
	Wzbg.cInfoSort = 0x46;  //通信
	Wzbg.sTelexLen = 0x68;  //电文长度
	Wzbg.dControlUserAddr = lAppAddr; //本机地址
	Wzbg.dUserAddr = lAddress;               //目标地址

	unsigned char info[500];
	int len;
	memset(info,0,500);
	unsigned char cCheckSum;									//读出的校验和

	short sLength;												//长度
	unsigned char cCommand[5] = {'$','T','X','S','Q'};
	if((Wzbg.sTelexLen%8)!=0)
	{
		sLength = Wzbg.sTelexLen/8+1+18;
	}
	else
	{
		sLength = Wzbg.sTelexLen/8+18;
	}
	memcpy(info,cCommand,5);								//命令字
	info[5] = ((unsigned char*)(&sLength))[1];					//长度
	info[6] = ((unsigned char*)(&sLength))[0];					//长度
	info[7] = ((unsigned char*)(&Wzbg.dControlUserAddr))[2];		//受控用户地址
	info[8] = ((unsigned char*)(&Wzbg.dControlUserAddr))[1];		//受控用户地址
	info[9] = ((unsigned char*)(&Wzbg.dControlUserAddr))[0];		//受控用户地址
	info[10] = Wzbg.cInfoSort;								//信息类别
	info[11] = ((unsigned char*)(&Wzbg.dUserAddr))[2];				//用户地址
	info[12] = ((unsigned char*)(&Wzbg.dUserAddr))[1];				//用户地址
	info[13] = ((unsigned char*)(&Wzbg.dUserAddr))[0];				//用户地址
	info[14] = ((unsigned char*)(&Wzbg.sTelexLen))[1];						//电文长度
	info[15] = ((unsigned char*)(&Wzbg.sTelexLen))[0];						//电文长度
	info[16] = 0x00;										//是否应答
	memcpy(info+17,Wzbg.TelexContent,sLength-18);		//电文内容 ≤580bit通信等级3
	cCheckSum = info[0];
	for(/*int */i=1;i<sLength-1;i++)							//计算校验和
		cCheckSum = cCheckSum^info[i];
	info[sLength-1] = cCheckSum;					//校验和
	len = sLength;

	if (len > 0 && len)
	{
//		g_Serial.Write((char*)info,strlen(info));
	}	
}


int Pos_Single() // 单次定位
{
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{	
		return 0;
	}

	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}

	memset(&tempdwsq,0,sizeof(BDS_DWSQ));

	gflag_PositionType = 1;   //单次标志
    count_PosApply = 0;          //定位申请计数器
	pBDAll->PosAccount();
	unsigned char info[500];
	int len;
	
	memset(info,0,500);
	pBDAll->PackStream(info,len,&tempdwsq);
	if (len > 0 && len)
	{
		unsigned char sum=0;
		for(int t=0;t<(len-1);t++)
			sum ^= info[t];
		info[len-1] = sum;
//		g_Serial.Write((char*)info,strlen(info));
		return 2;
	}
	return FALSE;
}


int Pos_Sequence()
{	
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}
	if(flag_StartSequencePos)
	{
		unsigned char info[15] = {'$','J','S','Z','L',0x00,0x0F,0x00,0x00,0x00,'D','W','S','Q',0x00};
		unsigned char sum=0;

		info[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
		info[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
		info[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);

		for(int t=0;t<14;t++)
			sum ^= info[t];
		info[14] = sum;
//		g_Serial.Write((char*)info,15);
		flag_StartSequencePos = FALSE;
		return 3;
	}	

	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}
	memset(&tempdwsq,0,sizeof(BDS_DWSQ));

	count_PosApply = 0;          //定位申请计数器
	
	if(!flag_StartSequencePos)
	{	
		gflag_PositionType = 2;   
        pBDAll->PosAccount();
		tempdwsq.dEntryFrequency = gsPosICFrequence;
		unsigned char info[500];
		int len;
		
		memset(info,0,500);
		pBDAll->PackStream(info,len,&tempdwsq);
		if (len > 0 && len)
		{
			unsigned char sum=0;
			for(int t=0;t<(len-1);t++)
				sum ^= info[t];
			info[len-1] = sum;
//			g_Serial.Write((char*)info,strlen(info));
			flag_StartSequencePos = TRUE;
			return 2;
		}
		return 0;
	}	
	return 0;
}

int Pos_SendUrgent()
{	
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}	
	if(count_UrgentPosApply != -1)
	{
		return 1;
	}

	memset(&tempdwsq,0,sizeof(BDS_DWSQ));

    count_UrgentPosApply = 0;       
	gflag_PositionType = 3;  

	tempdwsq.cInfoSort = 32;		//紧急定位

	pBDAll->PosAccount();
	unsigned char info[500];
	int len;
	
	memset(info,0,500);
	pBDAll->PackStream(info,len,&tempdwsq);
	if (len > 0 && len)
	{
		unsigned char sum=0;
		for(int t=0;t<(len-1);t++)
			sum ^= info[t];
		info[len-1] = sum;
//		g_Serial.Write((char*)info,strlen(info));
		return 2;
	}
	return FALSE;
}


void CBDAll::PosAccount()
{
	
	tempdwsq.dControlUserAddr = g_ICInfo_ZHJ.dAppAddr;//用户地址,从ICXX得到
	
	if (g_ICInfo_ZHJ.cCnfdFlag==0x01)    //加密卡
	{
		tempdwsq.cInfoSort += 2;
	}
	if (gsHeightType == 1)		 //无测高
	{
		tempdwsq.cInfoSort += 4;
	}
	else if (gsHeightType == 2)	 //测高1
	{
		tempdwsq.cInfoSort += 8;
	}
	else if (gsHeightType == 3)	 //测高2
	{
		tempdwsq.cInfoSort += 12;
	}
	if (gsHeightType == 0)
	{		
		if(glHeight<0)          //高程
		{
			tempdwsq.dHeight =  (labs(glHeight))*65536;
			tempdwsq.dHeight += 2147483648;
		}
		else
		{
			tempdwsq.dHeight = glHeight*65536;
		}
	}
	else if(gsHeightType == 1)
	{
		tempdwsq.dHeight =  (glTianXianHeight*10);
	}
	else if(gsHeightType == 2)
	{
		if(gdTemperature < 0)
		{
			tempdwsq.dAirPressure  = (((((int)(gdQiYa*10))&0xFFFFF)<<12) |(((int)(gdTemperature*10))&0xFFF) |0x800);
		}
		else
		{
			tempdwsq.dAirPressure  = (((((int)(gdQiYa*10))&0xFFFFF)<<12) |((int)(gdTemperature*10))&0xFFF);
			tempdwsq.dHeight =  (glTianXianHeight*10);
		}
	}
	else if(gsHeightType == 3)
	{	
		if(gdTemperature < 0)
		{
			tempdwsq.dAirPressure  = (((((int)(gdQiYa*10))&0xFFFFF)<<12) |(((int)(gdTemperature*10))&0xFFF) |0x800);
		}
		else
		{
			tempdwsq.dAirPressure  = (((((int)(gdQiYa*10))&0xFFFFF)<<12) |((int)(gdTemperature*10))&0xFFF);
		}
		if(glHeight<0)
		{
			tempdwsq.dHeight = (labs(glHeight))*65536+(glTianXianHeight*10);
			tempdwsq.dHeight += 2147483648;
		}
		else
		{
			tempdwsq.dHeight = glHeight*65536+glTianXianHeight*10;
		}
	}
}

void CBDAll::PackStream(unsigned char *str, int &len, void *pstrct)
{
	int i = 0;
	short sLength;												//长度
	unsigned char cCheckSum;									//读出的校验和
	
	BDS_DWSQ* p = (BDS_DWSQ*)pstrct;
	unsigned char cCommand[5] = {'$','D','W','S','Q'};
	sLength = 22;
	memcpy(str,cCommand,5);             						//命令字
	str[5] = ((unsigned char*)(&sLength))[1];					//长度
	str[6] = ((unsigned char*)(&sLength))[0];					//长度
	str[7] = ((unsigned char*)(&p->dControlUserAddr))[2];		//受控用户地址
	str[8] = ((unsigned char*)(&p->dControlUserAddr))[1];		//受控用户地址
	str[9] = ((unsigned char*)(&p->dControlUserAddr))[0];		//受控用户地址
	str[10] = p->cInfoSort;							        	//信息类别
	str[11] = ((unsigned char*)(&p->dHeight))[3];				//天线高和已知高程
	str[12] = ((unsigned char*)(&p->dHeight))[2];				//天线高和已知高程
	str[13] = ((unsigned char*)(&p->dHeight))[1];				//天线高和已知高程
	str[14] = ((unsigned char*)(&p->dHeight))[0];				//天线高和已知高程
	str[1signed char*)(&p->dAirPressure))[3];	    	//气压数据
	str[16] = ((unsigned char*)(&p->dAirPressure))[2];	    	//气压数据
	str[17] = ((unsigned char*)(&p->dAirPressure))[1];		    //气压数据
	str[18] = ((unsigned char*)(&p->dAirPressure))[0];		    //气压数据
	str[19] = ((unsigned char*)(&p->dEntryFrequency))[1];		//入站频度
	str[20] = ((unsigned char*)(&p->dEntryFrequency))[0];		//入站频度
	cCheckSum = str[0];
	for(i=1;i<sLength-1;i++)							       //计算校验和5] = ((un
		cCheckSum = cCheckSum^str[i];
	str[21] = cCheckSum;								       //校验和
	len = sLength;
}

int Tong_Bo(int nSelleibie,int nSelbianma,char* strMsgContent)
{	
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}

	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}

	count_MsgApply =0;


	int length=18; //除电文内容外的全部长度
	int lenSMS=0;  //电文长度
	int i,j;
	unsigned char chMsgType;
	lenSMS = strlen(strMsgContent);

	unsigned char buf[500]={'$','T','X','S','Q'};
	buf[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	buf[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	buf[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);

	chMsgType = 64;	
	if (nSelleibie == 0) //普通
	{
		chMsgType += 4;
	}
	if (nSelbianma == 0 || nSelbianma == 2)	//混合或代码
	{
		chMsgType += 2;
	}
	buf[10] = chMsgType;
	buf[11] = (unsigned char)((g_ICInfo_ZHJ.dBrdcstAddr) >> 16);
	buf[12] = (unsigned char)((g_ICInfo_ZHJ.dBrdcstAddr) >> 8);
	buf[13] = (unsigned char)(g_ICInfo_ZHJ.dBrdcstAddr);
	
	if(nSelbianma == 0)	     //混发
	{
		lenSMS += 1;      //0xA4长度
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		buf[17] = 0xA4;
		memcpy(buf+18,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 1) //汉字
	{
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		memcpy(buf+17,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 2) //代码
	{
		length += ceil(lenSMS/2.0);
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = ((lenSMS*8)/2) >> 8;
		buf[15] = ((lenSMS*8)/2);
		/*if (lenSMS%2 == 0)
		{
			strMsgContent += "F";
		}*/
		if(lenSMS%2 != 0)
		{
			strcat(strMsgContent,"0");
		}
		for(i=0,j=0; i<lenSMS; i+=2,j++)		//报文信息
			sscanf(pBDAll->GetStrAt(strMsgContent,i,2),"%02X",&buf[17+j]);
	}
	for(int k=0; k<(length-1); k++)
		buf[length-1] ^= buf[k];
//	g_Serial.Write((char*)buf,length);
	return 2;
}

void BoShu_Setting(unsigned char chxiangying,unsigned char chtime)  //波束设置
{
	unsigned char info[20] = {'$','B','S','S','Z',0x00,0x0D,0x00,0x00,0x00};

	info[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	info[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	info[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);

	info[10] = chxiangying;
	info[11] = chtime;
	for(int i=0;i<12;i++)
		info[12] ^= info[i];

//	g_Serial.Write((char*)info,13);
}

//通信查询
int TongXin_Search(int nSearchType,long lAddress) 
{
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}
	
	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}

	count_MsgApply =0;
	
	unsigned char TxData[100];
	unsigned char Sum = 0,Count = 15;
	
	TxData[0] = '$';
	TxData[1] = 'T';
	TxData[2] = 'X';
	TxData[3] = 'S';
	TxData[4] = 'Q';	
	TxData[5] = Count >> 8;
	TxData[6] = Count;
	TxData[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	TxData[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);  
	TxData[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);  
	
	nSearchType = nSearchType + 1;
	if(nSearchType == 1)    //回执
	{
		TxData[10]=0x78;
	}
	/*else if(nSearchType==2) //发信地址
	{
		TxData[10]=0x74;
	}*/
	else if(nSearchType==2)	//最新电文
	{
		TxData[10]=0x70;
	}
	TxData[11] = (char)(lAddress >> 16);
	TxData[12] = (char)(lAddress >> 8);
	TxData[13] = (char)lAddress;
	
	for(int i = 0;i < Count - 1;i++)
	{
		Sum ^= TxData[i];
	}
	TxData[Count - 1] = Sum;
//	g_Serial.Write((char*)TxData,Count);
	return 2;
}

//点对点用户通信
int OneUser_Comm(int nSelleibie,int nSelbianma,unsigned long lReceiveID,char * strMsgContent)
{
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}

	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}
	count_MsgApply =0;
	
	unsigned char buf[500]={'$','T','X','S','Q'};
	int length=18; //除电文内容外的全部长度
	int lenSMS=0;  //电文长度
	int i,j;
	unsigned char chMsgType;
	lenSMS = strlen(strMsgContent);
	
	buf[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	buf[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	buf[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	chMsgType = 64;	
	if (nSelleibie == 0) //普通
	{
		chMsgType += 4;
	}
	if (nSelbianma == 0 || nSelbianma == 2)	//混合或代码
	{
		chMsgType += 2;
	}
	buf[10] = chMsgType;
	buf[11] = (unsigned char)(lReceiveID >> 16);
	buf[12] = (unsigned char)(lReceiveID >> 8);
	buf[13] = (unsigned char)lReceiveID;
	
	if(nSelbianma == 0)	     //混发
	{
		lenSMS += 1;      //0xA4长度
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		buf[17] = 0xA4;
		memcpy(buf+18,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 1) //汉字
	{
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		memcpy(buf+17,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 2) //代码
	{
		length += ceil(lenSMS/2.0);
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = ((lenSMS*8)/2) >> 8;
		buf[15] = ((lenSMS*8)/2);
		if (lenSMS%2 != 0)
		{
			strcat(strMsgContent,"0");
		}
		for(i=0,j=0; i<lenSMS; i+=2,j++)		//报文信息
			sscanf(pBDAll->GetStrAt(strMsgContent,i,2),"%02X",&buf[17+j]);
	}
	for(int k=0; k<(length-1); k++)
		buf[length-1] ^= buf[k];
//	g_Serial.Write((char*)buf,length);
	return 2;
}

BOOL IC_XLH(long lnumber)  //序列号
{
	BYTE buf[20]={'$','X','G','X','L',0,0x0F};
	int length = 15;

	buf[7] = 0;
	buf[8] = 0;
	buf[9] = 0;

	buf[10] = (BYTE)(lnumber >> 24);
	buf[11] = (BYTE)(lnumber >> 16);
	buf[12] = (BYTE)(lnumber >> 8);
	buf[13] = (BYTE)lnumber;
	
	for(int i = 0; i < (length-1); i++)
		buf[length-1] ^= buf[i];
	
//	g_Serial.Write((char*)buf,length);
	return TRUE;
}

void Destroy_Self()  //自毁
{
	for(int i=0;i<3;i++)
	{
		unsigned char info[15] = {'$','Z','H','S','Q',0x00,0x0F,0x00,0x00,0x00
			,0x55,0xAA,0xAA,0x55,0x00};

		info[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
		info[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
		info[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
		
		unsigned char sum=0;
		for(int t=0;t<(14);t++)
			sum ^= info[t];
		info[14] = sum;
//		g_Serial.Write((char*)info,15);
	}
}

int Pos_Search(int nSearchType,long lAddress)  //定位查询
{	
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}


	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}	
	if(count_GSMApply != -1)
	{
		return 1;
	}
	count_MsgApply =0;
	
	unsigned char TxData[100];
	unsigned char Sum = 0,Count = 15;
	
	TxData[0] = '$';
	TxData[1] = 'T';
	TxData[2] = 'X';
	TxData[3] = 'S';
	TxData[4] = 'Q';	
	TxData[5] = Count >> 8;
	TxData[6] = Count;
	TxData[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	TxData[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	TxData[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	nSearchType = nSearchType + 1;
	if(nSearchType == 1)    //1次
	{
		TxData[10]=0x60;
	}
	else if(nSearchType==2) //2次
	{
		TxData[10]=0x64;
	}
	else if(nSearchType==3)	//3次
	{
		TxData[10]=0x68;
	}
	TxData[11] = (char)(lAddress >> 16);
	TxData[12] = (char)(lAddress >> 8);
	TxData[13] = (char)lAddress;
	
	for(int i = 0;i < Count - 1;i++)
	{
		Sum ^= TxData[i];
	}
	TxData[Count - 1] = Sum;
//	g_Serial.Write((char*)TxData,Count);
	return 2;
}

BOOL Read_ManageInfo()  //管信读取
{
	unsigned char Sum = 0;
	unsigned char INFO[11] = {'$','G','X','D','Q',0x00,0x0B,0x00,0x00,0x00};
	
	INFO[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	INFO[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	INFO[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	for(int i = 0;i < 10;i++)
	{
		Sum ^= INFO[i];
	}	
	INFO[10] = Sum;

//	g_Serial.Write((char*)INFO,11);
	return TRUE;
}

BOOL InPut_ManageInfo(char * strMangeInfo)  //管理信息
{
	unsigned char INFO[43];
	memset(INFO,0,43);
	unsigned char Command[10] = {'$','G','X','Z','R',0x00,0x2b,0x00,0x00,0x00};
	memcpy(INFO,Command,10);

	for (int i = 0;i< 32;i++)
	{
		INFO[10+i] = pBDAll->String2Int(pBDAll->GetStrAt(strMangeInfo,i*3,2),2,16);
	}

	int parity = 0;

	for(int k = 0;k<30;k++)
	{
		for(int j = 0; j < 8; j++)
		{
			if((INFO[10+k] << (j + 8) ^ parity) & 0x8000)
				parity = (parity << 1) ^ 0x8005;
			else
				parity <<= 1;
		}
	}
	parity &= 0xFFFF;
	if (parity != ((INFO[40]<<8)|INFO[41]))
	{
		return FALSE;
	}

	int sum=0;
	for(int t=0;t<42;t++)
		sum ^= INFO[t];
	INFO[42] = sum;
//	g_Serial.Write((char*)INFO,43);
	return TRUE;
}


int Position_Calling(long lAddress)
{
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{	
		
		return 0;
	}

	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}
	count_PosApply = 0;          //定位申请计数器

	unsigned char sendbuf[70];
	unsigned char Sum = 0;
	
	memset(sendbuf,0,70);
	
	sendbuf[0] = '$';
	sendbuf[1] = 'T';
	sendbuf[2] = 'X';
	sendbuf[3] = 'S';
	sendbuf[4] = 'Q';
	sendbuf[5] = 0x00;
	sendbuf[6] = 0x17;
	
	sendbuf[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	sendbuf[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	sendbuf[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	sendbuf[10] = 0x46;
	
	sendbuf[11] = (char)(lAddress >> 16);   //收信方地址
	sendbuf[12] = (char)(lAddress >> 8);
	sendbuf[13] = (char)(lAddress);
	
	sendbuf[14] = 0x00;
	sendbuf[15] = 0x28;
	sendbuf[16] = 0x00;
	sendbuf[17] = 0xA2;
	
	sendbuf[18] = (char)((lAddress*8) >> 24);
	sendbuf[19] = (char)((lAddress*8) >> 16); 
	sendbuf[20] = (char)((lAddress*8) >> 8);
	sendbuf[21] = (char)(lAddress*8);
	
	for(int i = 0;i < 23;i++)
	{
		Sum ^= sendbuf[i];
	}
	
	sendbuf[22] = Sum;
//	g_Serial.Write((char*)sendbuf,23);
	
	return 2;
}

int neirong(char * strPhoneContent,char * p_strMidContent)
{
	if(g_YIZHI == 1)
	{	
		return 0;
	}
	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
	{
		return 0;
	}
	
	if(count_UrgentPosApply != -1)
	{
		return 1;
	}
	if(count_PosApply != -1)
	{
		return 1;
	}
	if(count_MsgApply != -1)
	{
		return 1;
	}
	if(count_GSMApply != -1)
	{
		return 1;
	}
	count_GSMApply =0;
	
	int length=18; //除电文内容外的全部长度
	int lenSMS=0;  //电文长度
	long iMidContent = atol(p_strMidContent);
	unsigned char chMsgType;
	lenSMS = strlen(strPhoneContent);
	
	unsigned char buf[500]={'$','T','X','S','Q'};
	buf[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	buf[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	buf[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	chMsgType = 70;	
	/*if (nSelleibie == 0) //普通
	{
	chMsgType += 4;
	}
	if (nSelbianma == 0 || nSelbianma == 2)	//混合或代码
	{
	chMsgType += 2;
}*/
	buf[10] = chMsgType;
	buf[11] = (unsigned char)((iMidContent) >> 16);
	buf[12] = (unsigned char)((iMidContent) >> 8);
	buf[13] = (unsigned char)(iMidContent);
	
	lenSMS += 1;      //0xA4长度
	length += lenSMS;
	buf[5] = length>>8;
	buf[6] = length;
	buf[14] = (lenSMS * 8) >> 8;
	buf[15] = lenSMS * 8;
	buf[17] = 0xA4;
	memcpy(buf+18,strPhoneContent,lenSMS);
	
	for(int k=0; k<(length-1); k++)
		buf[length-1] ^= buf[k];
//	g_Serial.Write((char*)buf,length);
	return 2;
}

/*--------------------------END------------------------------*/

#endif 



//字符转换为相应的十六进制值的函数
int CBDAll::ConvertHexChar(int ch)
{
	if((ch>=0x30)&&(ch<=0x39))
		return ch-0x30;
	else if((ch>=0x41)&&(ch<=0x46))
		return ch-0x41+10;
	else if((ch>=0x61)&&(ch<=0x66))
		return ch-0x61+10;
	else return (-1);
}

//指数函数
int CBDAll::Qexp(int a,int b)
{
	int result;
	if(b==0)
	{
		result=1;
	}
	else if(b==1)
	{
		result=a;
	}
	else 
	{
		result=a;
		for(int i=1;i<b;i++)
		{
			result=result*a;
		}
	}
	return result;
}


//字符串转换为各种进制的整数
char * CBDAll::GetStrAt(char * resource,int npos,int len)
{
	char * temp = (char*)malloc(len*sizeof(char));
	memset(temp,0,sizeof(temp));
	memcpy(temp,resource+npos,len);
	return temp;
	free(temp);
}

int CBDAll::String2Int(char* str,int length,int radix)  
{
	int hexdata=0;
	int temp[30];

	for(int i=0;i<length;i++)
	{
		temp[i]=ConvertHexChar(str[i]);
		hexdata+=temp[i]*Qexp(radix,length-1-i);
	}
	return hexdata;
}


void CBDAll::SendGXDQ(DWORD dAppAddr)		//管信读取
{
	unsigned char Sum = 0;
	unsigned char INFO[11] = {'$','G','X','D','Q',0x00,0x0B,0x00,0x00,0x00};
	
	INFO[7] = (unsigned char)((dAppAddr)>>16);   //本机地址
	INFO[8] = (unsigned char)((dAppAddr)>>8);
	INFO[9] = (unsigned char)(dAppAddr);
	
	for(int i = 0;i < 10;i++)
	{
		Sum ^= INFO[i];
	}	
	INFO[10] = Sum;

	m_JniDataSwitchCls.WriteToDevice((char*)INFO,11);
}

void CBDAll::SendICJC()		//IC检测
{
	unsigned char info[12] = {'$','I','C','J','C',0x00,0x0C,0x00,0x00,0x00,0x00,0x2B};

	m_JniDataSwitchCls.WriteToDevice((char*)info,12);
}

void CBDAll::SendGLJC(DWORD dAppAddr,long iFre)		//GL检测
{
	unsigned char info[12] = {0x24,0x47,0x4C,0x4A,0x43,0x00,0x0C,0x00,0x00,0x00,0x01,0x00};

	info[7] = (unsigned char)(dAppAddr>>16);
	info[8] = (unsigned char)(dAppAddr>>8);
	info[9] = (unsigned char)dAppAddr;

	info[10] = iFre;

	for(int i=0; i<11; i++)
		info[11] ^= info[i];

	m_JniDataSwitchCls.WriteToDevice((char*)info,12);
}

void CBDAll::SendXTZJ(DWORD dAppAddr,long iFre) //系统自检
{
	unsigned char into[13] ={'$','X','T','Z','J',0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00};//2秒
	into[7] = (unsigned char)(dAppAddr>>16);
	into[8] = (unsigned char)(dAppAddr>>8);
	into[9] = (unsigned char)dAppAddr;

	into[10] = iFre >> 8;
	into[11] = iFre;

	for(int i=0; i<12; i++)
		into[12] ^= into[i];
	m_JniDataSwitchCls.WriteToDevice((char*)into,13);
}
void CBDAll::SendSJSC(int iFre,DWORD dAppAddr)		//时间输出
{
	unsigned char info[13] = {'$','S','J','S','C',0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00}; //0表示单次输出
	
	info[7] = (unsigned char)(dAppAddr>>16);
	info[8] = (unsigned char)(dAppAddr>>8);
	info[9] = (unsigned char)dAppAddr;
	
	info[10] = iFre >> 8;
	info[11] = iFre;

	unsigned char sum=0;
	for(int t=0;t<(12);t++)
		sum ^= info[t];
	info[12] = sum;

	m_JniDataSwitchCls.WriteToDevice((char*)info,13);
}

void CBDAll::SendXHDQ(DWORD dAppAddr)	 //序号读取	
{
	BYTE buf[11]={'$','X','H','D','Q',0,0x0B,0,0,0};
	int length = 11;
	
	buf[7] = (unsigned char)(dAppAddr>>16);
	buf[8] = (unsigned char)(dAppAddr>>8);
	buf[9] = (unsigned char)dAppAddr;						

	buf[length-1] = 0;			
	for(int i = 0; i < (length-1); i++)
		buf[length-1] ^= buf[i];
	
	m_JniDataSwitchCls.WriteToDevice((char*)buf,12);
//	g_Serial.Write((char*)buf,12);
}

void CBDAll::SendBBDQ(DWORD dAppAddr)
{
	BYTE buf[11]={'$','B','B','D','Q',0,0x0B,0,0,0};
	int length = 11;
	
	buf[7] = (unsigned char)(dAppAddr>>16);
	buf[8] = (unsigned char)(dAppAddr>>8);
	buf[9] = (unsigned char)dAppAddr;						
	
	buf[length-1] = 0;			
	for(int i = 0; i < (length-1); i++)
		buf[length-1] ^= buf[i];
	
	m_JniDataSwitchCls.WriteToDevice((char*)buf,12);
}

void CBDAll::SendZHSQ(DWORD dAppAddr)	//自毁
{
	unsigned char info[15] = {'$','Z','H','S','Q',0x00,0x0F,0x00,0x00,0x00
		,0x55,0xAA,0xAA,0x55,0x00};
	
	info[7] = (unsigned char)((dAppAddr)>>16);   //本机地址
	info[8] = (unsigned char)((dAppAddr)>>8);
	info[9] = (unsigned char)(dAppAddr);
	
	unsigned char sum=0;
	for(int t=0;t<(14);t++)
		sum ^= info[t];
	info[14] = sum;

	m_JniDataSwitchCls.WriteToDevice((char*)info,15);
	
}

void CBDAll::SendGXZR(char * strMangeInfo)	//管信注入
{
	unsigned char INFO[43];
	memset(INFO,0,43);
	unsigned char Command[10] = {'$','G','X','Z','R',0x00,0x2b,0x00,0x00,0x00};
	memcpy(INFO,Command,10);
	
	for (int i = 0;i< 32;i++)
	{
		INFO[10+i] = pBDAll->String2Int(pBDAll->GetStrAt(strMangeInfo,i*3,2),2,16);
	}
	
// 	int parity = 0;
// 	
// 	for(int k = 0;k<30;k++)
// 	{
// 		for(int j = 0; j < 8; j++)
// 		{
// 			if((INFO[10+k] << (j + 8) ^ parity) & 0x8000)
// 				parity = (parity << 1) ^ 0x8005;
// 			else
// 				parity <<= 1;
// 		}
// 	}
// 	parity &= 0xFFFF;
// 	if (parity != ((INFO[40]<<8)|INFO[41]))
// 	{
// 		return FALSE;
// 	}
	
	int sum=0;
	for(int t=0;t<42;t++)
		sum ^= INFO[t];
	INFO[42] = sum;
	//	g_Serial.Write((char*)INFO,43);

	m_JniDataSwitchCls.WriteToDevice((char*)INFO,43);

}



/************************************************************************/
/* 参数说明
nSelleibie:信息类别(0:特快，1普通)
nSelbianma:编码方式(0:汉子，1代码，2混编)
lReceiveID:收信方地址
strMsgContent:电文内容                                                                    */
/************************************************************************/
int CBDAll::SendTXSQ(int nSelleibie,int nSelbianma,unsigned long lReceiveID,char * strMsgContent)	//发送通信申请
{
// 	if(g_YIZHI == 1)
// 	{	
// 		return 0;
// 	}
// 	if((UINT)Glzk.cPower1 == 0 && (UINT)Glzk.cPower2 == 0 && (UINT)Glzk.cPower3 == 0 &&
// 		(UINT)Glzk.cPower4 == 0 && (UINT)Glzk.cPower5 == 0 && (UINT)Glzk.cPower6 == 0)
// 	{
// 		return 0;
// 	}
// 	
// 	if(count_UrgentPosApply != -1)
// 	{
// 		return 1;
// 	}
// 	if(count_PosApply != -1)
// 	{
// 		return 1;
// 	}
// 	if(count_MsgApply != -1)
// 	{
// 		return 1;
// 	}
// 	if(count_GSMApply != -1)
// 	{
// 		return 1;
// 	}

//	count_MsgApply =0;
	
	unsigned char buf[500]={'$','T','X','S','Q'};
	int length=18; //除电文内容外的全部长度
	int lenSMS=0;  //电文长度
	int i,j;
	unsigned char chMsgType;
	lenSMS = strlen(strMsgContent);
	
	buf[7] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>16);   //本机地址
	buf[8] = (unsigned char)((g_ICInfo_ZHJ.dAppAddr)>>8);
	buf[9] = (unsigned char)(g_ICInfo_ZHJ.dAppAddr);
	
	chMsgType = 64;	
	if (nSelleibie == 1) //普通
	{
		chMsgType += 4;
	}
	if (nSelbianma == 1 || nSelbianma == 2)	//混合或代码
	{
		chMsgType += 2;
	}
	buf[10] = chMsgType;
	buf[11] = (unsigned char)(lReceiveID >> 16);
	buf[12] = (unsigned char)(lReceiveID >> 8);
	buf[13] = (unsigned char)lReceiveID;
	
	if(nSelbianma == 2)	     //混发
	{
		lenSMS += 1;      //0xA4长度
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		buf[17] = 0xA4;
		memcpy(buf+18,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 0) //汉字
	{
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		memcpy(buf+17,strMsgContent,lenSMS);
	}
	else if(nSelbianma == 1) //代码
	{
		length += ceil(lenSMS/2.0);
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = ((lenSMS*8)/2) >> 8;
		buf[15] = ((lenSMS*8)/2);
		if (lenSMS%2 != 0)
		{
			strcat(strMsgContent,"0");
		}
		for(i=0,j=0; i<lenSMS; i+=2,j++)		//报文信息
			sscanf(pBDAll->GetStrAt(strMsgContent,i,2),"%02X",&buf[17+j]);
	}
	for(int k=0; k<(length-1); k++)
		buf[length-1] ^= buf[k];
	//	g_Serial.Write((char*)buf,length);

	m_JniDataSwitchCls.WriteToDevice((char*)buf,length);

	return 2;
}

/************************************************************************/
/* txsqData:通信申请结构体数据                                                                     */
/************************************************************************/
int CBDAll::SendTXSQ(BDS_TXSQ txsqData)		//发送通信申请
{
	unsigned char buf[500]={'$','T','X','S','Q'};
	int length=18; //除电文内容外的全部长度
	int lenSMS=0;  //电文长度
	int i,j;
	unsigned char chMsgType;
	lenSMS = txsqData.sTelexLen;
	
	buf[7] = (unsigned char)((txsqData.dControlUserAddr)>>16);   //本机地址
	buf[8] = (unsigned char)((txsqData.dControlUserAddr)>>8);
	buf[9] = (unsigned char)(txsqData.dControlUserAddr);
	
	buf[10] = txsqData.cInfoSort;

	buf[11] = (unsigned char)((txsqData.dUserAddr) >> 16);		//收信方地址
	buf[12] = (unsigned char)((txsqData.dUserAddr) >> 8);
	buf[13] = (unsigned char)(txsqData.dUserAddr);
	
	int nSelbianma = (txsqData.cInfoSort>>1) & 0x01;

// 	if(nSelbianma == 0)	     //混发
// 	{
// 		lenSMS += 1;      //0xA4长度
// 		length += lenSMS;
// 		buf[5] = length>>8;
// 		buf[6] = length;
// 		buf[14] = (lenSMS * 8) >> 8;
// 		buf[15] = lenSMS * 8;
// 		buf[17] = 0xA4;
// 		memcpy(buf+18,txsqData.TelexContent,lenSMS);
// 	}
// 	else 
	if(nSelbianma == 0) //汉字
	{
		length += lenSMS;
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = (lenSMS * 8) >> 8;
		buf[15] = lenSMS * 8;
		memcpy(buf+17,txsqData.TelexContent,lenSMS);
	}
	else if(nSelbianma == 1) //代码
	{
		length += ceil(lenSMS/2.0);
		buf[5] = length>>8;
		buf[6] = length;
		buf[14] = ((lenSMS*8)/2) >> 8;
		buf[15] = ((lenSMS*8)/2);
		if (lenSMS%2 != 0)
		{
			strcat((char *)(txsqData.TelexContent),"0");
		}
		for(i=0,j=0; i<lenSMS; i+=2,j++)		//报文信息
			sscanf(pBDAll->GetStrAt((char *)(txsqData.TelexContent),i,2),"%02X",&buf[17+j]);
	}
	for(int k=0; k<(length-1); k++)
		buf[length-1] ^= buf[k];
	//	g_Serial.Write((char*)buf,length);
	m_JniDataSwitchCls.WriteToDevice((char*)buf,length);
	return 2;
}

void CBDAll::SendDWSQ(BDS_DWSQ dwsqData)	//定位申请
{
	char str[22] = {0};
	unsigned char cCommand[5] = {'$','D','W','S','Q'};
	int sLength = 22;
	memcpy(str,cCommand,5);             						//命令字
	str[5] = ((unsigned char*)(&sLength))[1];					//长度
	str[6] = ((unsigned char*)(&sLength))[0];					//长度
	str[7] = ((unsigned char*)(&dwsqData.dControlUserAddr))[2];		//受控用户地址
	str[8] = ((unsigned char*)(&dwsqData.dControlUserAddr))[1];		//受控用户地址
	str[9] = ((unsigned char*)(&dwsqData.dControlUserAddr))[0];		//受控用户地址
	str[10] = dwsqData.cInfoSort;							        	//信息类别
	str[11] = ((unsigned char*)(&dwsqData.dHeight))[3];				//天线高和已知高程
	str[12] = ((unsigned char*)(&dwsqData.dHeight))[2];				//天线高和已知高程
	str[13] = ((unsigned char*)(&dwsqData.dHeight))[1];				//天线高和已知高程
	str[14] = ((unsigned char*)(&dwsqData.dHeight))[0];				//天线高和已知高程
	str[15] = ((unsigned char*)(&dwsqData.dAirPressure))[3];	    	//气压数据
	str[16] = ((unsigned char*)(&dwsqData.dAirPressure))[2];	    	//气压数据
	str[17] = ((unsigned char*)(&dwsqData.dAirPressure))[1];		    //气压数据
	str[18] = ((unsigned char*)(&dwsqData.dAirPressure))[0];		    //气压数据
	str[19] = ((unsigned char*)(&dwsqData.dEntryFrequency))[1];		//入站频度
	str[20] = ((unsigned char*)(&dwsqData.dEntryFrequency))[0];		//入站频度
	unsigned char cCheckSum = str[0];
	for(int i=1;i<sLength-1;i++)							       //计算校验和
		cCheckSum = cCheckSum^str[i];
	str[21] = cCheckSum;

	m_JniDataSwitchCls.WriteToDevice((char*)str,22);
}

/************************************************************************/
/* 通信查询
nSearchType:查询方式 1:回执查询，2:按最新电文查询
lAddress:查询地址                                                          
lLocalAddr:本机地址
*/
/************************************************************************/
void CBDAll::SendTXCX(int nSearchType,long lAddress,long lLocalAddr)
{
	unsigned char TxData[17];
	unsigned char Sum = 0,Count = 15;
	
	TxData[0] = '$';
	TxData[1] = 'T';
	TxData[2] = 'X';
	TxData[3] = 'S';
	TxData[4] = 'Q';	
	TxData[5] = Count >> 8;
	TxData[6] = Count;
	TxData[7] = (unsigned char)(lLocalAddr>>16);   //本机地址
	TxData[8] = (unsigned char)(lLocalAddr>>8);  
	TxData[9] = (unsigned char)lLocalAddr;  
	
	nSearchType = nSearchType + 1;
	if(nSearchType == 1)    //回执
	{
		TxData[10]=0x78;
	}
// 	else if(nSearchType==2) //发信地址
// 	{
// 		TxData[10]=0x74;
// 	}
	else if(nSearchType==2)	//最新电文
	{
		TxData[10]=0x70;
	}
	TxData[11] = (char)(lAddress >> 16);
	TxData[12] = (char)(lAddress >> 8);
	TxData[13] = (char)lAddress;
	
	for(int i = 0;i < Count - 1;i++)
	{
		Sum ^= TxData[i];
	}
	TxData[Count - 1] = Sum;

	m_JniDataSwitchCls.WriteToDevice((char*)TxData,Count);
}

void CBDAll::SendJSZL(DWORD dAppAddr)
{
	BYTE buf[14]={0x24,0x4A,0x53,0x5A,0x4C,0x00,0x0E,0x00,0x00,0x00,0x55,0x55,0x55,0x00};
	int length = 14;
	
	buf[7] = (unsigned char)(dAppAddr>>16);
	buf[8] = (unsigned char)(dAppAddr>>8);
	buf[9] = (unsigned char)dAppAddr;						
	
	buf[length-1] = 0;			
	for(int i = 0; i < (length-1); i++)
		buf[length-1] ^= buf[i];
	
	m_JniDataSwitchCls.WriteToDevice((char*)buf,14);
}


bool CBDAll::SendMessage(BDIPortData *pPortData)
{
	if (pPortData == NULL)
	{
		return false;
	}

	int itype = pPortData->DataType;


	if (itype == TYPE_TXXX)
	{
		BDR_TXXX data = *(BDR_TXXX *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_TXXX");


		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cInfoSort",data.cInfoSort);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cPostLetterTimeHour",data.cPostLetterTimeHour);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cPostLetterTimeMinute",data.cPostLetterTimeMinute);

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"CRCVerify",data.CRCVerify);

		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dPostLetterAddr",data.dPostLetterAddr);



		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dUserAddr",data.dUserAddr);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"Infotype",data.Infotype);


		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"sTelexLen",data.sTelexLen);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"TelexContent",data.TelexContent);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
	else if (itype == TYPE_DWXX || itype == TYPE_WZBG)
	{
		BDR_DWXX data = *(BDR_DWXX *) pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_DWXX");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cInfoSort",data.cInfoSort);

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLatDegree",data.cLatDegree);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLatMinute",data.cLatMinute);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLatSecond",data.cLatSecond);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLatMSecond",data.cLatMSecond);

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLonDegree",data.cLonDegree);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLonMinute",data.cLonMinute);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLonSecond",data.cLonSecond);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cLonMSecond",data.cLonMSecond);

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cPosTimeHour",data.cPosTimeHour);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cPosTimeMinute",data.cPosTimeMinute);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cPosTimeSecond",data.cPosTimeSecond);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"cPosTimeMSecond",data.cPosTimeMSecond);


		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"dElevation",data.dElevation);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"dElvError",data.dElvError);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dQueryAddr",data.dQueryAddr);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dUserAddr",data.dUserAddr);

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"HeightType",data.HeightType);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"IsMultiRes",data.IsMultiRes);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"IsUrgent",data.IsUrgent);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"Precision",data.Precision);

		
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

	}

	else if (itype == TYPE_ICXZ)
	{		
		BDR_ICXX_ZHJ data = *(BDR_ICXX_ZHJ *)pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_ICXX_ZHJ");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cCnfdFlag",data.cCnfdFlag);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cCommLevel",data.cCommLevel);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dAppAddr",data.dAppAddr);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dBrdcstAddr",data.dBrdcstAddr);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"ServeFrequency",data.ServeFrequency);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"UserCharacter",data.UserCharacter);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"UsersNum",data.UsersNum);
		
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
	else if (itype == TYPE_GLZK)
	{
		BDR_GLZK data = *(BDR_GLZK *) pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_GLZK");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		short powers[6] = {0};
		powers[0] = data.cPower1;
		powers[1] = data.cPower2;
		powers[2] = data.cPower3;
		powers[3] = data.cPower4;
		powers[4] = data.cPower5;
		powers[5] = data.cPower6;

		m_JniDataSwitchCls.SetShortArrayFieldValue(clsobj,objs,"cPowers",powers,6);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	else if (itype == TYPE_ZJXX)
	{
		BDR_ZJXX data = *(BDR_ZJXX*) pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_ZJXX");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		short powers[6] = {0};
		powers[0] = data.Glzk.cPower1;
		powers[1] = data.Glzk.cPower2;
		powers[2] = data.Glzk.cPower3;
		powers[3] = data.Glzk.cPower4;
		powers[4] = data.Glzk.cPower5;
		powers[5] = data.Glzk.cPower6;

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"DcDl",data.DcDl);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dUserAdd",data.dUserAdd);
		m_JniDataSwitchCls.SetShortArrayFieldValue(clsobj,objs,"cPowers",powers,6);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"ICType",data.ICType);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"RzState",data.RzState);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"YjState",data.YjState);
		
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	else if (itype == TYPE_XHXX)
	{
		BDR_XHXX data = *(BDR_XHXX *) pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_XHXX");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dUserAdd",data.dUserAdd);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"xlh",data.xlh);
			
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	else if (itype == TYPE_FKXX)
	{
		BDR_FKXX data = *(BDR_FKXX *) pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BDR_FKXX");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"dwUserAddr",data.dwUserAddr);

		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"cFkbz",data.cFkbz);

		m_JniDataSwitchCls.SetByteArrayFieldValue(clsobj,objs,"strFjxx",(char*)data.strFjxx,4);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

	}
	
	return true;
}