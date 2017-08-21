package com.app.dataprase;

public class ClassGGA extends GGAData implements DataBaseInterface,pyDataComFromJni
{
	boolean bIsUpdate = false;			//当通过制定语句进行解析时的更新标志
	
	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.pyDataComFromJni#CallBackFunction(int, java.lang.Object)
	 * 
	 * 接收到jni消息通知的回调函数
	 */
	public int CallBackFunction(int iType, Object obj) {
		// TODO Auto-generated method stub
		if (iType == TYPE_GGA) {
			fillData(obj);
			bIsUpdate = true;
		}
		return 0;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.DataBaseInterface#SaveToDB()
	 * 
	 * 将数据保存到数据库
	 */
	public boolean SaveToDB() {
		// TODO Auto-generated method stub
		return false;
	}

	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.DataBaseInterface#ReadFromDB()
	 * 
	 * 从数据库读取数据
	 */
	public boolean ReadFromDB() {
		// TODO Auto-generated method stub
		return false;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.DataBaseInterface#packageProtocol()
	 * 
	 * 协议数据打包
	 */
	public void packageProtocol() {
		// TODO Auto-generated method stub
		
		String str = "$BDGGA,";
		str += this.dUTCTime + ",";
		str += this.dLat + ",N,";
		str += this.dLon + ",E,";
		str += this.iState + ",";
		str += this.iViewStarCount + ",";
		str += this.dHDOP + ",";
		str += this.dAntennaHigh + ",";
		str += this.cHighUnit + ",";
		str += this.dHighNovel + ",";
		str += this.cHighNovelUnit + ",";
		str += this.iDataLife + ",";
		str += this.iPlatFormID + ",";
		str += this.dVDOP + "*";
		
		byte crc = 0;
		
		for (int i = 1; i < str.length()-1; i++) {
			crc ^= str.charAt(i);
		}

		String strcrc = String.format("%02X", crc);
		str += strcrc + "\r\n";
			
		protoSentence = str;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.DataBaseInterface#praseProtocol()
	 * 
	 * 数据包解析,运用jni技术，调用本地C++写的协议数据解析功能模块
	 */
	public boolean praseProtocol() {
		// TODO Auto-generated method stub
		JniDataPrase jni = new JniDataPrase(this);
		
		if (jni.ProcssData(protoSentence.getBytes(), protoSentence.length()) == 0) {
			return true;
		}	
		return false;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.project.dataprase.DataBaseInterface#fillData(java.lang.Object)
	 * 
	 * 填充数据
	 */
	private void fillData(Object obj)
	{
		GGAData ggaData = (GGAData)obj;
		
		this.dUTCTime = ggaData.dUTCTime;
		this.dLat = ggaData.dLat;
		this.cLatDirection = ggaData.cLatDirection;
		this.dLon = ggaData.dLon;
		this.cLonDirection = ggaData.cLonDirection;
		this.iState = ggaData.iState;
		this.iViewStarCount = ggaData.iViewStarCount;
		this.dHDOP = ggaData.dHDOP;
		this.dAntennaHigh = ggaData.dAntennaHigh;
		this.cHighUnit = ggaData.cHighUnit;
		this.dHighNovel = ggaData.dHighNovel;
		this.cHighNovelUnit = ggaData.cHighNovelUnit;
		this.iDataLife = ggaData.iDataLife;
		this.iDataLife = ggaData.iDataLife;
		this.dVDOP = ggaData.dVDOP;
		
		//...其他值的相同方式进行赋值
		
	}
	
	public ClassGGA()
	{
		super();
	}
	
	public ClassGGA(GGAData data)
	{
		super();
		fillData(data);
	}
	
	public ClassGGA(String sentence)
	{
		super();
		bIsUpdate = false;
		this.protoSentence = sentence;			
		praseProtocol();
	}

	
	
	public void setdUTCTime(double data)
	{
		this.dUTCTime = data;
	}
	
	public double getdUTCTime()
	{
		return this.dUTCTime;
	}
	
	public void setdLat(double data)
	{
		this.dLat = data;
	}
	
	public double getdLat()
	{
		return this.dLat;
	}
	
	public String getProtocolString() {
		packageProtocol();
		return this.protoSentence;
	}

	@Override
	public void WriteDataToDevice(byte[] buf, long len) {
		// TODO Auto-generated method stub
		
	}
}
