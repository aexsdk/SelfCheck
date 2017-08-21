package com.app.dataprase;

public class GGAData
{
	public double    dUTCTime;             //UTC时间
	public double    dLat;                 //纬度
	public char      cLatDirection;        //纬度方向
	public double    dLon;                 //经度
	public char      cLonDirection;        //经度方向
	public int       iState;               //状态指示
	public int       iViewStarCount;       //视野内卫星数
	public double    dHDOP;                //HDOP值
	public double    dAntennaHigh;         //天线平均海拔高
	public char      cHighUnit;            //海拔高单位
	public double    dHighNovel;           //CGS-2000高程异常
	public char      cHighNovelUnit;       //高程异常单位
	public int       iDataLife;            //差分数据的寿命
	public int       iPlatFormID;          //差分站台ID号
	public double    dVDOP;                //VDOP值

	public int sYear;						//年
	public int sMonth;						//月
	public int sDay;						//日
	public int sHour;						//时
	public int sMinute;					//分
	public int sSecond;					//秒
	public int sHSecond;					//毫秒

	public int sWeiduD;					//纬度度
	public int sWeiduF;					//纬度分
	public int sWeiduM;					//纬度秒
	public int sWeiduHM;					//纬度0.1秒
	public int sJingduD;					//经度度
	public int sJingduF;					//经度分
	public int sJingduM;					//经度秒
	public int sJingduHM;					//经度0.1秒
	
	public double Logitude;				//经度用度表示
	public double Latitude;				//纬度用度表示
	
	public String protoSentence;			//协议包
}
