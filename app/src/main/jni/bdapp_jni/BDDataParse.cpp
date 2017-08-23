
#include "BDDataParse.h"
#include "memory.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "SomeStruct.h"

#include <android/log.h>

#define LOG_TAG	"MYJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


CBDDataParse::CBDDataParse(JNIEnv * env)
{
//	m_JniDataSwitchCls = new CJniDataSwitchCls(env,g_strPackName,strlen(g_strPackName));
	CJniDataSwitchCls jnicls(env);
	m_JniDataSwitchCls = jnicls;
	m_jniEnv = env;
}

CBDDataParse::~CBDDataParse()
{
//	delete m_JniDataSwitchCls;
//	m_JniDataSwitchCls = NULL;

	m_jniEnv = NULL;
}

bool CBDDataParse::IsValidData( const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos, int nCheckFieldIndex )
{
	int nCurFieldIndex = 0;
	int nStartPosOld = nStartPos;

	if (pBuffers[nStartPos+3] == 'I' && pBuffers[nStartPos+3] == 'C' && pBuffers[nStartPos+3] == 'Z')
	{
		while( nStartPos <= nLineFeedPos/* && nCurFieldIndex < nCheckFieldIndex */)
		{
			if( pBuffers[nStartPos] == ',' || pBuffers[nStartPos] == '*' )
			{
				nCurFieldIndex++;
			}
			nStartPos++;
		}
	}
	else
	{
		while( nStartPos <= nLineFeedPos && nCurFieldIndex < nCheckFieldIndex )
		{
			if( pBuffers[nStartPos] == ',' || pBuffers[nStartPos] == '*' )
			{
				nCurFieldIndex++;
			}
			nStartPos++;
		}
	}	
	if( nCurFieldIndex == nCheckFieldIndex )
	{//有效
		return true;
	}
	
	return false;
}
int CBDDataParse::SNInteger( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos )
{
	int nValue = 0;
	
	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',')
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}
		
		if( nCurFieldIndex == nFieldIndex )
		{
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && nCharPos <= nLineFeedPos )
			{
				nCharPos++;
			}
			if( nCharPos - nDataPosStart > 0 )
			{
				unsigned char btNegative = 0;
				for(;nDataPosStart<nCharPos;nDataPosStart++)
				{//整数部分
					if( pBuffers[nDataPosStart] == '*' )
					{
						break;
					}
					else if( pBuffers[nDataPosStart] != ' ' )
					{
						if( pBuffers[nDataPosStart] == '-' )
						{
							btNegative = 1;
						}
						else
						{
							nValue = nValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
						}
					}
				}
				if( btNegative == 1 )
				{
					nValue = 0 - nValue;
				}
			}
			break;
		}
	}
	*nStartPos = nCharPos;
	return nValue;
}
long CBDDataParse::SNLong( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos )
{
	long nValue = 0;
	
	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',' )
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}
		
		if( nCurFieldIndex == nFieldIndex )
		{
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && nCharPos <= nLineFeedPos )
			{
				nCharPos++;
			}
			if( nCharPos - nDataPosStart > 0 )
			{
				unsigned char btNegative = 0;
				for(;nDataPosStart<nCharPos;nDataPosStart++)
				{//整数部分
					if( pBuffers[nDataPosStart] == '*' )
					{
						break;
					}
					else if( pBuffers[nDataPosStart] != ' ' )
					{
						if( pBuffers[nDataPosStart] == '-' )
						{
							btNegative = 1;
						}
						else
						{
							nValue = nValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
						}
					}
				}
				if( btNegative == 1 )
				{
					nValue = 0 - nValue;
				}
			}
			break;
		}
	}
	*nStartPos = nCharPos;
	return nValue;
}
double CBDDataParse::SNDoubleEx( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos, double& dFranctions )
{
	double dValue = 0;

	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',' )
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}

		if( nCurFieldIndex == nFieldIndex )
		{
			int nDotPos = 0;
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && nCharPos <= nLineFeedPos )
			{
				if( pBuffers[nCharPos] == '.' )
				{
					nDotPos = nCharPos;
				}
				nCharPos++;
			}
			
			if( nCharPos - nDataPosStart > 0 )
			{
				unsigned char btNegative = 0;
				if( nDotPos > nDataPosStart )
				{
					for(;nDataPosStart<nDotPos;nDataPosStart++)
					{//整数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							if( pBuffers[nDataPosStart] == '-' )
							{
								btNegative = 1;
							}
							else
							{
								dValue = dValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
							}
						}
					}
					
					double dFraction = 0;
					for(nDataPosStart=nCharPos-1;nDataPosStart>nDotPos;nDataPosStart--)
					{//小数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							dFraction = dFraction * 0.1 + (pBuffers[nDataPosStart] & 0x0f) * 0.1;
						}
					}
					
					dValue += dFraction;

					dFranctions = dFraction;
				}
				else
				{
					for(;nDataPosStart<nCharPos;nDataPosStart++)
					{//整数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							if( pBuffers[nDataPosStart] == '-' )
							{
								btNegative = 1;
							}
							else
							{
								dValue = dValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
							}
						}
					}
				}
				if( btNegative == 1 )
				{
					dValue = 0.0 - dValue;
				}
			}
			break;
		}
	}
	*nStartPos = nCharPos;
	return dValue;
}
double CBDDataParse::SNDouble( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos )
{
	double dValue = 0;

	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',' || pBuffers[nCharPos] == '*' )
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}

		if( nCurFieldIndex == nFieldIndex )
		{
			int nDotPos = 0;
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && pBuffers[nCharPos] != '*' && nCharPos <= nLineFeedPos )
			{
				if( pBuffers[nCharPos] == '.' )
				{
					nDotPos = nCharPos;
				}
				nCharPos++;
			}
			
			if( nCharPos - nDataPosStart > 0 )
			{
				unsigned char btNegative = 0;
				if( nDotPos > nDataPosStart )
				{
					for(;nDataPosStart<nDotPos;nDataPosStart++)
					{//整数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							if( pBuffers[nDataPosStart] == '-' )
							{
								btNegative = 1;
							}
							else
							{
								dValue = dValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
							}
						}
					}
					
					double dFraction = 0;
					for(nDataPosStart=nCharPos-1;nDataPosStart>nDotPos;nDataPosStart--)
					{//小数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							dFraction = dFraction * 0.1 + (pBuffers[nDataPosStart] & 0x0f) * 0.1;
						}
					}
					
					dValue += dFraction;
				}
				else
				{
					for(;nDataPosStart<nCharPos;nDataPosStart++)
					{//整数部分
						if( pBuffers[nDataPosStart] == '*' )
						{
							break;
						}
						else if( pBuffers[nDataPosStart] != ' ' )
						{
							if( pBuffers[nDataPosStart] == '-' )
							{
								btNegative = 1;
							}
							else
							{
								dValue = dValue * 10 + (pBuffers[nDataPosStart] & 0x0f);
							}
						}
					}
				}
				if( btNegative == 1 )
				{
					dValue = 0.0 - dValue;
				}
			}
			break;
		}
	}
	*nStartPos = nCharPos;
	return dValue;
}
unsigned char CBDDataParse::SNByte( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos )
{
	unsigned char btValue = 0;
	
	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',' )
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}
		
		if( nCurFieldIndex == nFieldIndex )
		{
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && nCharPos <= nLineFeedPos )
			{
				nCharPos++;
			}
			if( nCharPos - nDataPosStart > 0 )
			{
				for(;nDataPosStart<nCharPos;nDataPosStart++)
				{//整数部分
					if( pBuffers[nDataPosStart] == '*' )
					{
						break;
					}
					else if( pBuffers[nDataPosStart] != ' ' )
					{
						btValue = pBuffers[nDataPosStart];
						break;
					}
				}
			}
			break;
		}
	}
	*nStartPos = nCharPos;
	return btValue;
}
void CBDDataParse::SNByteString( int nFieldIndex, const unsigned char * const pBuffers, int *nStartPos, int nLineFeedPos,char *ReturnBuff,int *nLen)
{
	int nCurFieldIndex = 0;
	int nDataPosStart = 0;
	int nCharPos = *nStartPos;
	int DataCount = 0;
	while( nCharPos <= nLineFeedPos )
	{
		if( pBuffers[nCharPos] == ',' )
		{
			nCurFieldIndex++;
			nCharPos++;
		}
		else
		{
			nCharPos++;
		}
		
		if( nCurFieldIndex == nFieldIndex )
		{
			nDataPosStart = nCharPos;
			while( pBuffers[nCharPos] != ',' && nCharPos <= nLineFeedPos )
			{
				nCharPos++;
			}
			if( nCharPos - nDataPosStart > 0 )
			{
				DataCount = 0;
				for(;nDataPosStart<nCharPos;nDataPosStart++)
				{
					if( pBuffers[nDataPosStart] == '*' )
					{
						break;
					}
					else /*if( pBuffers[nDataPosStart] != ' ' )//去掉里边存在的空格符号*/
					{
						ReturnBuff[DataCount++] = pBuffers[nDataPosStart];
					}
				}
			}
			break;
		}
	}

	*nStartPos = nCharPos;
	*nLen = DataCount;
}

bool CBDDataParse::Parse(const unsigned char * const pBuffers,int nSize)
{
	PortData  ErrorData;

	int nStartPos = 0;
	int nLineFeedPos = nSize-1;

	bool bRet = false;
	if( (pBuffers[ nStartPos] == '$' && pBuffers[ nStartPos+1] == 'G' && pBuffers[ nStartPos+2] == 'P') )
	{
		bRet = true;
		sStartType[0] = 'G';
		sStartType[1] = 'P';
	}
	else if ( (pBuffers[ nStartPos] == '$' && pBuffers[ nStartPos+1] == 'B' && pBuffers[ nStartPos+2] == 'D'))
	{
		bRet = true;
		sStartType[0] = 'B';
		sStartType[1] = 'D';
	}
	else if ( (pBuffers[ nStartPos] == '$' && pBuffers[ nStartPos+1] == 'G' && pBuffers[ nStartPos+2] == 'N'))
	{
		bRet = true;
		sStartType[0] = 'G';
		sStartType[1] = 'N';
	}
	else if ( (pBuffers[ nStartPos] == '$' && pBuffers[ nStartPos+1] == 'G' && pBuffers[ nStartPos+2] == 'L'))   //add by zhs 20170731
	{
		bRet = true;
		sStartType[0] = 'G';
		sStartType[1] = 'L';
	}
	else
	{
		bRet = false;
		memset(&ErrorData,0,sizeof(ErrorData));
		ErrorData.iDataLen = nLineFeedPos-nStartPos + 1;
		memmove(ErrorData.sStartType,pBuffers+nStartPos + 1,2); 
		memmove(ErrorData.sYnanshiData,pBuffers+nStartPos,ErrorData.iDataLen);
		ErrorData.ErrType = 1;
		ErrorData.DataType = TYPE_ERROR;
		
		SendMessage(&ErrorData); //发送信息
	}
	if (bRet)
	{
		if( pBuffers[ nStartPos+3 ] == 'A' &&
			pBuffers[ nStartPos+4 ] == 'A' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseAAM(pBuffers, nStartPos,nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'A' &&
			pBuffers[ nStartPos+4 ] == 'L' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseALM(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'A' &&
			pBuffers[ nStartPos+4 ] == 'P' &&
			pBuffers[ nStartPos+5 ] == 'L' )
		{
			ParseAPL(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'B' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'D' )
		{
			ParseBSD(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'B' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'Q' )
		{
			ParseBSQ(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'B' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'X' )
		{
			ParseBSX(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'A' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseDAS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'D' )
		{
			ParseDHD(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'P' )
		{
			ParseDHP(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'R' )
		{
			ParseDHR(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseDHS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseDHT(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'V' )
		{
			ParseDHV(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'A' )
		{
			ParseDSA(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'E' &&
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseECT(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&
			pBuffers[ nStartPos+4 ] == 'G' &&
			pBuffers[ nStartPos+5 ] == 'A' )
		{
			ParseGGA(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&
			pBuffers[ nStartPos+4 ] == 'L' &&
			pBuffers[ nStartPos+5 ] == 'L' )
		{
			ParseGLL(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'A' )
		{
			ParseGSA(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'V' )
		{
			ParseGSV(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseICS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&
			pBuffers[ nStartPos+4 ] == 'D' &&
			pBuffers[ nStartPos+5 ] == 'V' )
		{
			ParseIDV(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&
			pBuffers[ nStartPos+4 ] == 'H' &&
			pBuffers[ nStartPos+5 ] == 'O' )
		{
			ParseIHO(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'P' &&
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'D' )
		{
			ParsePCD(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'P' &&
			pBuffers[ nStartPos+4 ] == 'L' &&
			pBuffers[ nStartPos+5 ] == 'C' )
		{
			ParsePLC(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'P' &&
			pBuffers[ nStartPos+4 ] == 'R' &&
			pBuffers[ nStartPos+5 ] == 'O' )
		{
			ParsePRO(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'R' &&
			pBuffers[ nStartPos+4 ] == 'A' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseRAM(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'R' &&
			pBuffers[ nStartPos+4 ] == 'D' &&
			pBuffers[ nStartPos+5 ] == 'W' )
		{
			ParseRDW(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'S' &&
			pBuffers[ nStartPos+4 ] == 'T' &&
			pBuffers[ nStartPos+5 ] == 'X' )
		{
			ParseSTX(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'S' &&
			pBuffers[ nStartPos+4 ] == 'T' &&
			pBuffers[ nStartPos+5 ] == 'Z' )
		{
			ParseSTZ(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'V' &&
			pBuffers[ nStartPos+4 ] == 'T' &&
			pBuffers[ nStartPos+5 ] == 'G' )
		{
			ParseVTG(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'Z' &&
			pBuffers[ nStartPos+4 ] == 'D' &&
			pBuffers[ nStartPos+5 ] == 'U' )
		{
			ParseZDU(pBuffers, nStartPos, nLineFeedPos );
		}
		
		else if( pBuffers[ nStartPos+3 ] == 'G' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'B' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseGBS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseGST(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'P' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'M' &&
			pBuffers[ nStartPos+5 ] == 'U' )
		{
			ParsePMU(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseICM(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'T' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'X' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseTXT(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'R' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'M' &&
			pBuffers[ nStartPos+5 ] == 'C' )
		{
			ParseRMC(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'Z' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'D' &&
			pBuffers[ nStartPos+5 ] == 'A' )
		{
			ParseZDA(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'Z' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'T' &&
			pBuffers[ nStartPos+5 ] == 'I' )
		{
			ParseZTI(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'B' &&            //2011-08-09 //
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'I' )
		{
			ParseBSI(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'D' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'W' &&
			pBuffers[ nStartPos+5 ] == 'R' )
		{
			ParseDWR(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'F' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'K' &&
			pBuffers[ nStartPos+5 ] == 'I' )
		{
			ParseFKI(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'G' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'X' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseGXM(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'I' )
		{

			ParseICI(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'I' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'Z' )
		{
			ParseICZ(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'K' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'L' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseKLT(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'L' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'Z' &&
			pBuffers[ nStartPos+5 ] == 'M' )
		{
			ParseLZM(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'H' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'Z' &&
			pBuffers[ nStartPos+5 ] == 'R' )
		{
			ParseHZR(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'W' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'A' &&
			pBuffers[ nStartPos+5 ] == 'A' )
		{
			ParseWAA(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'T' &&           
			pBuffers[ nStartPos+4 ] == 'X' &&
			pBuffers[ nStartPos+5 ] == 'R' )
		{
			ParseTXR(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'T' &&           
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseTCS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'R' &&           
			pBuffers[ nStartPos+4 ] == 'M' &&
			pBuffers[ nStartPos+5 ] == 'O' )
		{
			ParseRMO(pBuffers, nStartPos, nLineFeedPos );
		}
		else if( pBuffers[ nStartPos+3 ] == 'S' &&            //2011-08-09
			pBuffers[ nStartPos+4 ] == 'C' &&
			pBuffers[ nStartPos+5 ] == 'S' )
		{
			ParseSCS(pBuffers, nStartPos, nLineFeedPos );
		}
		else if (pBuffers[ nStartPos+3 ] == 'P' &&            //2012-06-08
			pBuffers[ nStartPos+4 ] == 'K' &&
			pBuffers[ nStartPos+5 ] == 'Y' )
		{
			ParsePKY(pBuffers, nStartPos, nLineFeedPos );
		}
		else if (pBuffers[ nStartPos+3 ] == 'S' &&            //2012-06-08
			pBuffers[ nStartPos+4 ] == 'B' &&
			pBuffers[ nStartPos+5 ] == 'X' )
		{
			ParseSBX(pBuffers, nStartPos, nLineFeedPos );
		}
#if 1
		else if (pBuffers[ nStartPos+3 ] == 'X' &&           
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'D' )
		{
			ParseXSD(pBuffers, nStartPos, nLineFeedPos );
		}
		else if (pBuffers[ nStartPos+3 ] == 'X' &&            
			pBuffers[ nStartPos+4 ] == 'S' &&
			pBuffers[ nStartPos+5 ] == 'T' )
		{
			ParseXST(pBuffers, nStartPos, nLineFeedPos );
		}
#endif
		else
		{
			memset(&ErrorData,0,sizeof(ErrorData));
			ErrorData.iDataLen = nLineFeedPos-nStartPos+1;
			memmove(ErrorData.sStartType,pBuffers+nStartPos + 3,3); 
			memmove(ErrorData.sYnanshiData,pBuffers+nStartPos,ErrorData.iDataLen);
			ErrorData.ErrType = 2;
			ErrorData.DataType = TYPE_ERROR;
			
			SendMessage(&ErrorData); //发送信息
		}
	}
	return true;
}

/****************************计算比对CRC是否正确************************/
bool CBDDataParse::ProcessCRC(const unsigned char * const buff, int nLineFeedPos,int iStartPoint)
{
	unsigned char CRC = 0;
	unsigned char oldCRC = 0;
	int nLen = nLineFeedPos-4;
	for(int i=iStartPoint+1;i<nLen;i++)
	{
		CRC ^= buff[i];
	}
	/******************还原数据中的校验位*******************/
	if(buff[nLineFeedPos-3]>= '0'&&buff[nLineFeedPos-3]<= '9')
	{
		oldCRC = (buff[nLineFeedPos-3]&0x0f)*16;
	}
	else
	{
		oldCRC = (((buff[nLineFeedPos-3]-0x11)&0x0f)+10)*16;
	}
	if(buff[nLineFeedPos-2]>= '0'&&buff[nLineFeedPos-2]<= '9')
	{
		oldCRC += (buff[nLineFeedPos-2]&0x0f);
	}
	else
	{
		oldCRC += (((buff[nLineFeedPos-2]-0x11)&0x0f)+10);
	}
	if(oldCRC == CRC)
		return TRUE;
	else
		return FALSE;
}
/*************************数据解析部分*******************************/
bool CBDDataParse::ParseAAM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--AAM,A(1),A(2),x.x(3),U(4),c--c(5)*hh(6)<CR><LF>"
	AAMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,6))  //判断数据是否有效
		{
			SaveData.cState1 = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cState2 = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dRadius = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cRadiusUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sLineName,&NameLine);

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_AAM;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseALM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--ALM,x.x（1）,x.x（2）,xxx（3）, hh（4）,hhh（5）,hhhhh（6）,hh（7）,hhhh（8）,hhhhh（9）,hhhhhh（10）,hhhhhh（11）,hhhhhh（12）,hhhhhh（13）,hhh（14）,hhh（15）*hh（16）<CR><LF>"
	ALMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效
		{
			SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iStartNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sWeekCount,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sStartHealth,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sParty,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sStartNorm,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sOrbitDip,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sEquator,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sLongAxis,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sperigeeAngle,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sUpPoint,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sAnomalisticAngle,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sClockPar1,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sClockPar2,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sStarType,&NameLine);

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ALM;

			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseAPL(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//$--APL,hhmmss.ss（1）,x.x（2）,U（3）, x.x（4）,U（5）,x.x（6）,U（7）*hh（8）<CR><LF>
	APLData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,8))  //判断数据是否有效
		{
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLevelProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLevelUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dPlumbProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cPlumbUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSpaceProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSpaceUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_APL;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseBSD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--BSD,c--c（1）,x（2）,llll.ll（3）, a（4）,yyyyy.yy（5）,a（6）,x.x（7）,U（8）*hh（9）<CR><LF>"
	BSDData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sPointName,&NameLine);
			SaveData.dPicNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_BSD;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseBSQ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--BSQ,c--c（1）,llll.ll（2）, a（3）,yyyyy.yy（4）,a（5）,x.x（6）,U（7）x.x（8）,U（9）*hh（10）<CR><LF>"
	BSQData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,10))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sPaceName,&NameLine);
			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dPaceRadius = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cRadiusUint = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_BSQ;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseBSX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--BSX,c--c（1）,x.x（2）,x.x（3）,x.x（4）, llll.ll（5）, a（6）,yyyyy.yy（7）,a（8）,x.x（9）,U（10）,……*hh（n）<CR><LF>"
	BSXData SaveData;
	PortData PortData;
	int NameLine = 0;
	int nIntData = 0;
	int StartPoint = nStartPos;
	int i=0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		SaveData.dPointCount = SNDouble(4,pBuffers,&nStartPos,nLineFeedPos); //坐标点数目
		nIntData = (int)SaveData.dPointCount;
		nStartPos = StartPoint;  //将指针放回数据头位置
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,(nIntData*6+5)))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sLineName,&NameLine);
			SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dPointCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos); //坐标点数目
			
			for(i=0;i<nIntData;i++)
			{
				SaveData.PointSave[i].dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.PointSave[i].cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				
				
				SaveData.PointSave[i].dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.PointSave[i].cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);

				SaveData.PointSave[i].dHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.PointSave[i].cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			}

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_BSX;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDAS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DAS,x.x（1）,U（2）,x.x（3）,U（4）,x.x（5）, U（6）,x.x（7）,U（8）,x.x（9）,U（10）,x.x（11）,xx（12）,x.x（13）,x.x（14）,U（15）,x.x（16）,U（17）,a（18）*hh（19）<CR><LF>"
	DASData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,19))  //判断数据是否有效
		{
			SaveData.dAimProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cAimUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dEndProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cEndUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dArrivePaceProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cArrivePaceUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dArrivePointProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cArrivePointUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dCrabSpaceProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cCrabSpaceUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dCrabAngle = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iTimeZone = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTrackTimeSpace = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTurnPoint = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cTurnPointUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTurnPointProtect = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cTurnPointProtectUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cNaviOrder = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DAS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHD,x（1）,x.x（2）, U（3）*hh（4）<CR><LF>"
	DHDData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,4))  //判断数据是否有效
		{
			SaveData.iInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDistance = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cDistanceUint = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHD;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHP(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHP,x.x(1),c-c(2),x.x(3),c-c(4),x.x(5),c-c(6)*hh(7)<CR><LF>"
	DHPData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,7))  //判断数据是否有效
		{
			SaveData.dCourseNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sCourseName,&NameLine);
			SaveData.dFairWayNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFairWayName,&NameLine);
			SaveData.dEndFairWayNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sEndFairWayName,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHP;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHR,x(1),x.x(2)*hh(3)<CR><LF>"
	DHRData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,3))  //判断数据是否有效
		{
			SaveData.iInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dAngle = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHR;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHS,a(1),x.x(2),c--c(3),A(4)*hh(5)<CR><LF>"
	DHSData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,5))  //判断数据是否有效
		{
			SaveData.cNorthNorm = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dNowDimNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sDimName,&NameLine);
			SaveData.cNaviOrder = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHT,x(1),mmddyyyy(2),hhmmss(3)*hh(4)<CR><LF>"
	DHTData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,4))  //判断数据是否有效
		{
			SaveData.iInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sData,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sTime,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDHV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DHV,hhmmss.ss(1),x.x(2),x.x(3),x.x(4),x.x(5),x.x(6),x.x(7),x.x(8),x.x(9),x.x(10),U(11)*hh(12)<CR><LF>"
	DHVData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据

	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,12))  //判断数据是否有效
		{
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dXSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dYSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dZSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dEarthSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dMaxSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dAverageSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dAllAverageSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dValueSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSpeedUint = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DHV;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDSA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--DSA,xxxxxxx（1）,A（2）,x.x（3）,A（4）,x.x（5）,llll.ll（6）,a（7）,yyyyy.yy（8）,a（9）,x.x（10）,U（11）*hh（12）<CR><LF>

	DSAData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,12))  //判断数据是否有效
		{
			SaveData.lUserID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHavePlace = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dAddZero = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cOutSet = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSendFre = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DSA;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseECT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--ECT,xxx(1),c-c(2),xx(3),I(4),aa...a(5)*hh(6)<CR><LF>"
	ECTData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,6))  //判断数据是否有效
		{
			SaveData.iStartNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFrePoint,&NameLine);
			SaveData.iChannelNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSpurTrack = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sNavData,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ECT;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseGGA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--GGA,hhmmss.ss(1),llll.ll(2),a(3),yyyyy.yy(4),a(5),x(6),xx(7),x.x(8),x.x(9),U(10),x.x(11),U(12),xxxx(13),x.x(14),x.x(15)*hh(16)<CR><LF>"

	LOGI("in GGAGGGGGGGGGGGGGGGGGG function;");

	GGAData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if (sStartType[0] == 'G' &&sStartType[1] == 'N' || 
			sStartType[0] == 'g' &&sStartType[1] == 'n')
		{

			//if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效        //2.1协议
			if( IsValidData( pBuffers, nStartPos, nLineFeedPos,15))  //判断数据是否有效               //0183协议
			{
				
				SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iViewStarCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dHDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dAntennaHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dHighNovel = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cHighNovelUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iDataLife = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iPlatFormID = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				//SaveData.dVDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);            //0183协议没有VDOP值
			}
			else 
				return false;
		}
		else
		{
			//if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效        //2.1协议
			if( IsValidData( pBuffers, nStartPos, nLineFeedPos,15))  //判断数据是否有效               //0183协议
			{
				
				SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iViewStarCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dHDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dAntennaHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dHighNovel = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.cHighNovelUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iDataLife = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iPlatFormID = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				//SaveData.dVDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);              //0183协议没有VDOP值
			}
			else
				return false;
		}
// 		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效
// 		{
// 		
// 			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.iState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.iViewStarCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dHDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dAntennaHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dHighNovel = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.cHighNovelUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.iDataLife = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.iPlatFormID = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.dVDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);

			//////////////////////////////////////////////////////////////////////////

/*
#ifdef HAVE_ROUHE
			long BD2Temp = 0;

			BD2Temp = SaveData.dLat*100000;
			SaveData.sWeiduD = BD2Temp/10000000;        //纬度度
			SaveData.sWeiduF = (BD2Temp%10000000)/100000;                 //纬度分
			BD2Temp = (BD2Temp%100000);	                //纬度秒/100000*60
			SaveData.sWeiduM = BD2Temp*60/100000;
			// 			SaveData.sWeiduHM = ((BD2Temp*60)%100000)/10000;                     //纬度角秒
           	SaveData.sWeiduHM = ((BD2Temp*60)%100000)/1000;                     //纬度角秒
			
			BD2Temp = SaveData.dUTCTime * 100;
			SaveData.sHour = (BD2Temp/1000000)%24;        //时
			SaveData.sMinute = (BD2Temp%1000000)/10000;                 //分
			SaveData.sSecond = (BD2Temp%10000)/100;	                //秒
			SaveData.sHSecond = BD2Temp%100;
			
			SaveData.Latitude = int(SaveData.sWeiduD) + 
				(int(SaveData.sWeiduF))/60.0 +
				(int(SaveData.sWeiduM) +int(SaveData.sWeiduHM)/10)/3600.0;
						
			BD2Temp = SaveData.dLon*100000;
			SaveData.sJingduD = BD2Temp/10000000;        //经度度
			SaveData.sJingduF = (BD2Temp%10000000)/100000;                 //经度分
			BD2Temp = (BD2Temp%100000);	                //经度秒/100000*60
			SaveData.sJingduM = BD2Temp*60/100000;
			//			SaveData.sJingduHM = ((BD2Temp*60)%100000)/10000;                     //经度角秒
			SaveData.sJingduHM = ((BD2Temp*60)%100000)/1000;                     //经度角秒
			
			SaveData.Logitude = int(SaveData.sJingduD) + 
				(int(SaveData.sJingduF))/60.0 +
					(int(SaveData.sJingduM) +int(SaveData.sJingduHM)/10)/3600.0;

#endif
	*/
			//////////////////////////////////////////////////////////////////////////
#ifdef HAVE_ROUHE
			long BD2Temp = 0;

			BD2Temp = SaveData.dLat*100000;
			SaveData.sWeiduD = BD2Temp/10000000;        //纬度度
			SaveData.sWeiduF = (BD2Temp%10000000)/100000;                 //纬度分
			BD2Temp = (BD2Temp%100000);	                //纬度秒/100000*60
			SaveData.sWeiduM = BD2Temp/100000.0;
			/* 			SaveData.sWeiduHM = ((BD2Temp*60)%100000)/10000;                     //纬度角秒*/
           	//SaveData.sWeiduHM = ((BD2Temp*60)%100000)/1000;                     //纬度角秒
			
			BD2Temp = SaveData.dUTCTime * 100;
			SaveData.sHour = (BD2Temp/1000000)%24;        //时
			SaveData.sMinute = (BD2Temp%1000000)/10000;                 //分
			SaveData.sSecond = (BD2Temp%10000)/100;	                //秒
			SaveData.sHSecond = BD2Temp%100;

			SaveData.Latitude = int(SaveData.sWeiduD) +
				(int(SaveData.sWeiduF))/60.0 +
				(SaveData.sWeiduM)/60.0;
			//20170303 不是秒，是小数分 by zhs

			BD2Temp = SaveData.dLon*100000;
			SaveData.sJingduD = BD2Temp/10000000;        //经度度
			SaveData.sJingduF = (BD2Temp%10000000)/100000;                 //经度分
			BD2Temp = (BD2Temp%100000);	                //经度秒/100000*60
			SaveData.sJingduM = BD2Temp/100000.0;
			/*			SaveData.sJingduHM = ((BD2Temp*60)%100000)/10000;                     //经度角秒*/
			//SaveData.sJingduHM = ((BD2Temp*60)%100000)/1000;                     //经度角秒

			SaveData.Logitude = int(SaveData.sJingduD) +
				(int(SaveData.sJingduF))/60.0 +
					(SaveData.sJingduM)/60.0;

#endif


			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GGA;

			SendMessage(&PortData); //发送信息
//		}
//		else
//			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseGLL(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--GLL,llll.ll(1),a,yyyyy.yy(2),a(3),hhmmss.ss(4),A(5),x(7)*hh(8)<CR><LF>"
	GLLData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,8))  //判断数据是否有效
		{
			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cDataState = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iModeType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GLL;
			SendMessage(&PortData); //发送信息
			
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseGSA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--GSA,a(1),x(2),xxx(3),…,xxx(14),x.x(15),x.x(16),x.x(17)*hh(18)<CR><LF>"
	GSAData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,18))  //判断数据是否有效
		{
			SaveData.cModeType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iModeSel = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			for(i=0;i<12;i++)
			{
				SaveData.lStarNum[i] = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			SaveData.dPDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dVDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			//SaveData.dTDOP = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);   //0183协议没有TDOP
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GSA;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseGSV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--GSV,x.x(1),x.x(2),xx(3),xxx(4),x.x(5),x.x(6),……*hh<CR><LF>"
	LOGI("in GSVVVVVVVVVVVVVVVVVVVVVV function;");
	GSVData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	//////////////////////////////////////////////////////////////////////////
	int nbagcount = 0;			//语句总数
	int ncurbag = 0;			//当前语句序号
	int nstarcount = 0;			//视野内卫星数
	//////////////////////////////////////////////////////////////////////////
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		nbagcount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
		ncurbag = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
		nstarcount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);

		nStartPos = StartPoint;  //将指针放回数据头位置
		if (ncurbag < nbagcount) //前面几包
		{
			if( IsValidData( pBuffers, nStartPos, nLineFeedPos,(4*4+4)))  //判断数据是否有效
			{
				SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iViewStarCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos); //坐标点数目
				
				for(i=0;i<4;i++)
				{
					SaveData.StarInfo[i].lStarNum = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dStarElevation = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dStarAzimuth = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dSNR = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				}
				
				PortData.iDataLen = nLineFeedPos-StartPoint+1;
				memmove(PortData.sStartType,sStartType,3); 
				memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
				PortData.pData = &SaveData;
				PortData.DataType = TYPE_GSV;
				SendMessage(&PortData); //发送信息
			}
			else
				return false;
		}
		else
		{
			int nstar = nstarcount - (nbagcount - 1)*4;//最后一包卫星颗数
			if( IsValidData( pBuffers, nStartPos, nLineFeedPos,(nstar*4+4)))  //判断数据是否有效
			{
				SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.iViewStarCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos); //坐标点数目
				
				for(i=0;i<nstar;i++)
				{
					SaveData.StarInfo[i].lStarNum = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dStarElevation = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dStarAzimuth = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
					SaveData.StarInfo[i].dSNR = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				}
				
				PortData.iDataLen = nLineFeedPos-StartPoint+1;
				memmove(PortData.sStartType,sStartType,3); 
				memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
				PortData.pData = &SaveData;
				PortData.DataType = TYPE_GSV;
				SendMessage(&PortData); //发送信息
			}
			else
				return false;

		}
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseICS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--ICS,c--c(1),I(2),A(3),xxx(4),xxx(5),xxx(6),xxx(7),xxx(8),xxx(9),xxx(10),xxx(11),xxx(12),xxx(13),xxx(14),xxx(15)*hh(16)<CR><LF>"
	ICSData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFrePoint,&NameLine);
			SaveData.cSpurTrack = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			for(i=0;i<12;i++)
			{
				SaveData.iStartNum[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ICS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseIDV(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--IDV,x.x(1),x.x(2),xx(3),cc(4),x.x(5),x.x(6),x.x(7),……*hh(n)<CR><LF>"
	IDVData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	int nIntData = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		SaveData.iDisturbCount = SNInteger(3,pBuffers,&nStartPos,nLineFeedPos); 
		nIntData = SaveData.iDisturbCount;
// 		if(nIntData > 4)
// 			return false;
		nStartPos = StartPoint;  //将指针放回数据头位置
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,(nIntData*4+4)))  //判断数据是否有效
		{
			SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iDisturbCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos); //坐标点数目
			
			for(i=0;i<nIntData;i++)
			{
				SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.StarInfo[i].sDisturbType,&NameLine);
				SaveData.StarInfo[i].dDisturbFre = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.StarInfo[i].dDisturbWidth = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.StarInfo[i].dDisturbPower = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_IDV;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseIHO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--IHO,x.x（1）,x.x（2）,hhmmss.ss（3）,xxx（4）,x.x（5）,x.x（6）,x.x（7）,x.x（8）,x.x（9）,x.x（10）,x.x（11）,x.x（12）,x.x（13）,x.x（14）*hh（15）<CR><LF>"
	IHOData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,15))  //判断数据是否有效
		{
			SaveData.dSentenceCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSentenceNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iStarNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarPlaceX = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarPlaceY = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarPlaceZ = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarSpeedX = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarSpeedY = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dStarSpeedZ = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDistance = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDistanceSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDistanceEx = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDistanceSpeedEx = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_IHO;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParsePCD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--PCD,xxx(1),c--c(2),xx(3),x.x(4)*hh(5)<CR><LF>"
	PCDData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,5))  //判断数据是否有效
		{
			SaveData.iStarNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFrePoint,&NameLine);
			SaveData.iChannelNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTimeEx = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
	
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_PCD;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParsePLC(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--PLC,xxx(1),c--c(2),xx(3),P(4)*hh(5)<CR><LF>"
	PLCData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,5))  //判断数据是否有效
		{
			SaveData.iStarNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFrePoint,&NameLine);
			SaveData.iChannelNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cAccredit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_PLC;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParsePRO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--PRO,xxx(1),c--c(2),xx(3),P(4),x(5),x.x(6),x.x(7),x.x(8)*hh(9)<CR><LF>"
	PROData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SaveData.iStarNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sFrePoint,&NameLine);
			SaveData.iChannelNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cRangeType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iSenNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
		//	SaveData.lSecondCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
		//	SaveData.dRangeValue = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sSecondCount,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sRangeValue,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sClockDif,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_PRO;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseRAM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--RAM,x.x(1),x.x(2),xxx(3),……,xxx(14)*hh(15)<CR><LF>"
	RAMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,15))  //判断数据是否有效
		{
			SaveData.dViewStarCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dFailureStarCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			for(i=0;i<12;i++)
			{
				SaveData.iFailureStarNum[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_RAM;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseRDW(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--RDW,A(1),hhmmss.ss(2),xxxxxxx(3),llll.ll(4),a(5),yyyyy.yy(6),a(7),a(8),a(9),a(10),a(11),a(12),x(13),x.x(14),U(15),x.x(16),M(17)*hh(18)<CR><LF>"
	RDWData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,18))  //判断数据是否有效
		{
			SaveData.cInfoType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lUserID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLonDirection = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cKeyDenote = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iLonDenote = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cExigenceOr = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cMoreValue = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iHighType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dViewStarCount = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHighUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHighNovel = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cHighNovelUnit = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_RDW;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseSTX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--STX,xxxxxxx(1),xxxxxxx(2),a(3),x(4),a(5),a(6),hhmm(7),c--c(8)*hh<CR><LF>"
	STXData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,8))  //判断数据是否有效
		{
			SaveData.lSendUserID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lRecUserID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cTelType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.icInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cCRCClue = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSecrecyType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lSendTime = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.sTelData,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_STX;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseSTZ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--STZ,xxxxxxx(1),xxxxxxx(2),x.x(3),hhmm(4),hhmm(5),……,hhmm(12),hhmm(13)*hh(14)<CR><LF>"
	STZData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,14))  //判断数据是否有效
		{
			SaveData.lLocalUserID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lSearchID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iReturnCount = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			for(i=0;i<5;i++)
			{
				SaveData.lTimeData[i][0] = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.lTimeData[i][1] = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_STZ;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseVTG(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--VTG,x.x(1),T(2),x.x(3),M(4),x.x(5),N(6),x.x(7),K(8),a(9)*hh(10)<CR><LF>"
	VTGData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,10))  //判断数据是否有效
		{
			SaveData.dRelNorthCourse = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cRelNorth = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dPoleNorthCourse = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cPoleNorth = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSpeed1 = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSpeed1Uint = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSpeed2 = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cSpeed2Uint = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cModeType = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_VTG;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseZDA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	ZDAData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		//if( IsValidData( pBuffers, nStartPos, nLineFeedPos,12))  //判断数据是否有效               //20170813 2.1
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,7))  //判断数据是否有效                     //20170813 0183
		{
			//SaveData.modeShow = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iDay = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iMonth = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iYear = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.localTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.localMinDiss = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			//SaveData.dDsxzTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			//SaveData.xzData = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			//SaveData.iprecision = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);//加的
			//SaveData.spSateLockState = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ZDA;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseZDU(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	//"$--ZDU,hhmmss.ss(1),xx(2),xx(3),xxxx(4),xx(5),xx(6)*hh(7)<CR><LF>"
	ZDUData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,7))  //判断数据是否有效
		{
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iDay = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iMonth = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iYear = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iLocTimeZone = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iLocClockDif = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ZDU;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}


bool CBDDataParse::ParseGBS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	GBSData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLonExpectError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLatExpectError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHighExpectError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iProTroublePRN = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iProTroubleGL = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iProTroublePC = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iEstimateError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GBS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseGST(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	GSTData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dDisErrorJFG = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTYCError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTYDError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dTYWayError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLonError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLatError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dHighError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GST;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseICM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	ICMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,2))  //判断数据是否有效
		{
//			SaveData.startDataTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.endDataTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ICM;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParsePMU(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	PMUData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,4))  //判断数据是否有效
		{
			SaveData.freqDot = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.starDateTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.endDateTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
//			SaveData.endDataTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_PMU;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseRMC(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	LOGI("in RMCRRRRRRRRRRRRRRRRRRRRRR function;");
	RMCData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,13))  //判断数据是否有效
		{
			SaveData.dUtcTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cStatus = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLatitude = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLatitudeDir = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dLongitude = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cLongitudeDir = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dSpeed = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dCourse = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iDate = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dCpj = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cCpjDir = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.cMode = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			//////////////////////////////////////////////////////////////////////////
#ifdef HAVE_ROUHE
//之前的经纬度转换，存在一定问题
//			long RMCTemp = 0;
//
//			RMCTemp = SaveData.dLongitude*100000;
//			SaveData.cLonDegree = RMCTemp/10000000;        //经度度
//			SaveData.cLonMinute = (RMCTemp%10000000)/100000;                 //经度分
//			RMCTemp = (RMCTemp%100000);	                //经度秒/100000*60
//			SaveData.cLonSecond = RMCTemp*60/100000;
//			SaveData.cLonMSecond = ((RMCTemp*60)%100000)/1000;
//
//			SaveData.Logitude = int(SaveData.cLonDegree) +
//				(int(SaveData.cLonMinute))/60.0 +
//				(int(SaveData.cLonSecond) +int(SaveData.cLonMSecond)/10)/3600.0;
//
//			RMCTemp = SaveData.dLatitude*100000;
//			SaveData.cLatDegree = RMCTemp/10000000;        //纬度度
//			SaveData.cLatMinute = (RMCTemp%10000000)/100000;                 //纬度分
//			RMCTemp = (RMCTemp%100000);	                //纬度秒/100000*60
//			SaveData.cLatSecond = RMCTemp*60/100000;
//			SaveData.cLatMSecond = ((RMCTemp*60)%100000)/1000;                     //纬度角秒
//
//			SaveData.Latitude = int(SaveData.cLatDegree) +
//				(int(SaveData.cLatMinute))/60.0 +
//				(int(SaveData.cLatSecond) +int(SaveData.cLatMSecond)/10)/3600.0;
//add by zhs 20161214 经纬度重新计算转换
			long RMCTemp = 0;

			RMCTemp = SaveData.dLongitude*100000;
			SaveData.cLonDegree = RMCTemp/10000000;        //经度度
			SaveData.cLonMinute = (RMCTemp%10000000)/100000;                 //经度分
			RMCTemp = (RMCTemp%100000);	                //经度秒/10000*60
			SaveData.cLonSecond = RMCTemp/100000.0;
			//SaveData.cLonMSecond = ((RMCTemp*60)%10000)/1000;

			SaveData.Logitude = int(SaveData.cLonDegree) +
				(int(SaveData.cLonMinute))/60.0 +
				(SaveData.cLonSecond)/60.0;

			RMCTemp = SaveData.dLatitude*100000;
			SaveData.cLatDegree = RMCTemp/10000000;        //纬度度
			SaveData.cLatMinute = (RMCTemp%10000000)/100000;                 //纬度分
			RMCTemp = (RMCTemp%100000);	                //纬度秒/100000*60
			SaveData.cLatSecond = RMCTemp/100000.0;
			//SaveData.cLatMSecond = ((RMCTemp*60)%100000)/1000;                     //纬度角秒

			SaveData.Latitude = int(SaveData.cLatDegree) +
				(int(SaveData.cLatMinute))/60.0 +
				(SaveData.cLatSecond)/60.0;

#endif
			//////////////////////////////////////////////////////////////////////////

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_RMC;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseTXT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	TXTData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,5))  //判断数据是否有效
		{
			SaveData.sentNumber = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.sentNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.txtSignSymbol = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.txtInfo,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_TXT;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseZTI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	ZTIData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SaveData.PRMModeState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.JJMModeState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.AirState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.doorState = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lwd = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.chargeD = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.remainDL = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.remainDLTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ZTI;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseBSI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	BSIData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,13))  //判断数据是否有效
		{
			SaveData.resPower = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.timePower = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			for (int i = 0;i < 10;i++)
			{
				SaveData.power[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
// 			SaveData.power1 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power2 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power3 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power4 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power5 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power6 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power7 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power8 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power9 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.power10 = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_BSI;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseDWR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	DWRData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效
		{
			SaveData.posInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.userID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.dUTCTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.latWay = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lonWay = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.floorHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.floorDW = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.highError = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.highErrorDW = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.JDError = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.uposShow = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.DJZShow = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.highTypeShow = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);

			//////////////////////////////////////////////////////////////////////////
			/*
#ifdef HAVE_ROUHE

			long DWTemp = 0;

			DWTemp = SaveData.lon*100000;
			SaveData.cLonDegree = DWTemp/10000000;        //经度度
			SaveData.cLonMinute = (DWTemp%10000000)/100000;                 //经度分
			DWTemp = (DWTemp%100000);	                //经度秒/100000*60
			SaveData.cLonSecond = DWTemp*60/100000;
			SaveData.cLonMSecond = ((DWTemp*60)%100000)/1000;
			
			SaveData.Logitude = int(SaveData.cLonDegree) + 
				(int(SaveData.cLonMinute))/60.0 +
				(int(SaveData.cLonSecond) +int(SaveData.cLonMSecond)/10)/3600.0;
			
			DWTemp = SaveData.lat*100000;
			SaveData.cLatDegree = DWTemp/10000000;        //纬度度
			SaveData.cLatMinute = (DWTemp%10000000)/100000;                 //纬度分
			DWTemp = (DWTemp%100000);	                //纬度秒/100000*60
			SaveData.cLatSecond = DWTemp*60/100000;
			SaveData.cLatMSecond = ((DWTemp*60)%100000)/1000;                     //纬度角秒
			
// 			SaveData.Latitude = int(SaveData.cLonDegree) +
// 				(int(SaveData.cLonMinute))/60.0 +
// 				(int(SaveData.cLonSecond) +int(SaveData.cLonMSecond)/10)/3600.0;

			SaveData.Latitude = int(SaveData.cLatDegree) + 
				(int(SaveData.cLatMinute))/60.0 +
				(int(SaveData.cLatSecond) +int(SaveData.cLatMSecond)/10)/3600.0;

#endif
			//////////////////////////////////////////////////////////////////////////
		*/

#ifdef HAVE_ROUHE

			long DWTemp = 0;

			DWTemp = SaveData.lon*100000;
			SaveData.cLonDegree = DWTemp/10000000;        //经度度
			SaveData.cLonMinute = (DWTemp%10000000)/100000;                 //经度分
			DWTemp = (DWTemp%100000);	                //经度秒/100000*60
			SaveData.cLonSecond = DWTemp/100000.0;
			//SaveData.cLonMSecond = ((DWTemp*60)%100000)/1000;

			SaveData.Logitude = int(SaveData.cLonDegree) +
				(int(SaveData.cLonMinute))/60.0 +
				(SaveData.cLonSecond)/60.0;

			DWTemp = SaveData.lat*100000;
			SaveData.cLatDegree = DWTemp/10000000;        //纬度度
			SaveData.cLatMinute = (DWTemp%10000000)/100000;                 //纬度分
			DWTemp = (DWTemp%100000);	                //纬度秒/100000*60
			SaveData.cLatSecond = DWTemp/100000.0;
			//SaveData.cLatMSecond = ((DWTemp*60)%100000)/1000;                     //纬度角秒

			SaveData.Latitude = int(SaveData.cLatDegree) +
				(int(SaveData.cLatMinute))/60.0 +
				(SaveData.cLatSecond)/60.0;

#endif

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_DWR;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseFKI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	FKIData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,6))  //判断数据是否有效
		{
			
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.ZLName,&NameLine);
			SaveData.ZLExcuState = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.FreSetShow = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.sendYZShow = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.waitTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
		
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_FKI;

			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseGXM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	GXMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,3))  //判断数据是否有效
		{
			SaveData.ZLType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.manageInfo,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_GXM;

			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseICI(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	ICIData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,9))  //判断数据是否有效
		{
			SaveData.userID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.XLH = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.TBID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.userTZ = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.serverFre = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.TXOrder = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.JMFlag = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.XSUserNum = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_ICI;

			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

/******************** ICZ中的全局变量 ****************/
static bool isFirstTime = true;//是否是第一包
short PageCount = 0;//总包数
int nUserNum = 0;//总用户数
static bool isEnd = false;//判断是否是最后一包
int nCurNum = 0;//每包逗号数
static short PageNum = 0;//包次
static ICZData SaveICZData;
//char pSendBuffers[1024] = "0";//接收原始数据的缓冲区
static long sendlen = 0;//接收的所有数据的长度
static  short flag = 0;//判断是否是第一次接收总下属用户数的标识

/*****************************************************/

bool CBDDataParse::ParseICZ(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	PortData PortData;	
	int NameLine = 0;
	int StartPoint = nStartPos;
	
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	
	/********************申请临时指针变量以便获得逗号数目**********************/
	
	if (isFirstTime)
	{
		memset(&SaveICZData,0,sizeof(SaveICZData));  //清空结构体内数据
		
		const unsigned char * const pTempBuffers = pBuffers;
		int nTempStartPos = 0;
		nUserNum = SNInteger(1,pTempBuffers,&nTempStartPos,nLineFeedPos);//下属用户总数
		
		PageNum = 0;
		
		if (nUserNum % 40)
		{
			PageCount = nUserNum/40 + 1;
		}
		else
		{
			PageCount = nUserNum/40;
		}
		
		isFirstTime = false;
		isEnd = false;
		flag = 1;
		
	}
	
	nCurNum = 40 > (nUserNum -PageNum * 40) ? (nUserNum -PageNum * 40 +2):42;//逗号数目
	
	int nCurPageUserNum = nCurNum - 2;//当前包所有的用户数
	
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,nCurNum))  //判断数据是否有效
		{	
			SaveICZData.ALLXSUserNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);

			for (int index = 0;index < nCurPageUserNum; index ++)
			{
				SaveICZData.XSUserNum[index] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PageNum++;
			
			if (PageNum == PageCount)
			{
				PageNum = 0;
				isFirstTime = true;
				isEnd = true;
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			
			PortData.pData = &SaveICZData;
			PortData.DataType = TYPE_ICZ;
			isFirstTime = true;
			
			SendMessage(&PortData);
			return true;
		}
		else
		{
			PageNum = 0;
			isFirstTime = true;
			isEnd = false;
			return false;
		}
	}
	else
	{
		PageNum = 0;
		isFirstTime = true;
		isEnd = false;
		return false;
	}
}

bool CBDDataParse::ParseKLT(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	KLTData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,5))  //判断数据是否有效
		{
			SaveData.Flag = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.userID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.DWType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.cont,&NameLine);

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_KLT;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseLZM(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	LZMData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,3))  //判断数据是否有效
		{
			SaveData.manageMode = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.deviceZero = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_LZM;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseHZR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	HZRData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;

	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据

	/********************申请临时指针变量以便获得回执数**********************/
	
	const unsigned char * const pTempBuffers = pBuffers;
	int nTempStartPos = 0;
	int nHzNum = SNInteger(1,pTempBuffers,&nTempStartPos,nLineFeedPos);
	nHzNum = SNInteger(1,pTempBuffers,&nTempStartPos,nLineFeedPos);
	int nCurNum = 3 + 2*nHzNum;
	delete (char*)pTempBuffers;

	SaveData.HZSendTime = new int[nHzNum];
	SaveData.HZHZTime = new int[nHzNum];
	
	/**************************************************************************/

	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		//if( IsValidData( pBuffers, nStartPos, nLineFeedPos,13))  //判断数据是否有效
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,nCurNum))  //判断数据是否有效
		{
			SaveData.userID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.HZNum = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			for (int i=0;i<nHzNum;i++)
			{
				SaveData.HZSendTime[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
				SaveData.HZHZTime[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
// 			SaveData.HZOneSendTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZOneHZTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZTwoSendTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZTwoHZTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZThreeSendTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZThreeHZTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZFourSendTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZFourHZTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZFiveSendTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
// 			SaveData.HZFiveHZTime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_HZR;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool CBDDataParse::ParseWAA(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	WAAData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,11))  //判断数据是否有效
		{
			SaveData.infoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.reportFre = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.userID = SNLong(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.reportTime = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
		//	SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.reportTime,&NameLine);
			SaveData.lat = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.latWay = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lon = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.lonWay = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
		//	SaveData.floorHigh = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
		//	SaveData.floorDW = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.high = SNDouble(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.highDW = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
/*
			//////////////////////////////////////////////////////////////////////////
#ifdef HAVE_ROUHE

			long WATemp = 0;

			WATemp = SaveData.lon*100000;
			SaveData.cLonDegree = WATemp/10000000;        //经度度
			SaveData.cLonMinute = (WATemp%10000000)/100000;                 //经度分
			WATemp = (WATemp%100000);	                //经度秒/100000*60
			SaveData.cLonSecond = WATemp*60/100000;
			SaveData.cLonMSecond = ((WATemp*60)%100000)/1000;

			SaveData.Logitude = int(SaveData.cLonDegree) +
				(int(SaveData.cLonMinute))/60.0 +
				(int(SaveData.cLonSecond) +int(SaveData.cLonMSecond)/10)/3600.0;

			WATemp = SaveData.lat*100000;
			SaveData.cLatDegree = WATemp/10000000;        //纬度度
			SaveData.cLatMinute = (WATemp%10000000)/100000;                 //纬度分
			WATemp = (WATemp%100000);	                //纬度秒/100000*60
			SaveData.cLatSecond = WATemp*60/100000;
			SaveData.cLatMSecond = ((WATemp*60)%100000)/1000;                     //纬度角秒

// 			SaveData.Latitude = int(SaveData.cLonDegree) +
// 				(int(SaveData.cLonMinute))/60.0 +
// 				(int(SaveData.cLonSecond) +int(SaveData.cLonMSecond)/10)/3600.0;

			SaveData.Latitude = int(SaveData.cLatDegree) +
				(int(SaveData.cLatMinute))/60.0 +
				(int(SaveData.cLatSecond) +int(SaveData.cLatMSecond)/10)/3600.0;

#endif
			//////////////////////////////////////////////////////////////////////////
		*/
#ifdef HAVE_ROUHE

			long WATemp = 0;

			WATemp = SaveData.lon*100000;
			SaveData.cLonDegree = WATemp/10000000;        //经度度
			SaveData.cLonMinute = (WATemp%10000000)/100000;                 //经度分
			WATemp = (WATemp%100000);	                //经度秒/100000*60
			SaveData.cLonSecond = WATemp/100000.0;
			//SaveData.cLonMSecond = ((WATemp*60)%100000)/1000;

			SaveData.Logitude = int(SaveData.cLonDegree) +
				(int(SaveData.cLonMinute))/60.0 +
				(SaveData.cLonSecond)/60.0;

			WATemp = SaveData.lat*100000;
			SaveData.cLatDegree = WATemp/10000000;        //纬度度
			SaveData.cLatMinute = (WATemp%10000000)/100000;                 //纬度分
			WATemp = (WATemp%100000);	                //纬度秒/100000*60
			SaveData.cLatSecond = WATemp/100000.0;
			//SaveData.cLatMSecond = ((WATemp*60)%100000)/1000;                     //纬度角秒

			SaveData.Latitude = int(SaveData.cLatDegree) +
				(int(SaveData.cLatMinute))/60.0 +
				(SaveData.cLatSecond)/60.0;

#endif

			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_WAA;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseTXR(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	TXRData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,6))  //判断数据是否有效
		{

			SaveData.ixxleib = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.userID = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.idianwenxs = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.isendtime = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.ccontext,&NameLine);
			
			if (SaveData.idianwenxs == 2)	//混合通信
			{
				int len = strlen(SaveData.ccontext);
				m_JniDataSwitchCls.OnProcssHunHeTx(SaveData.ccontext,len);

			}
			else if (SaveData.idianwenxs == 0 || SaveData.idianwenxs == 1)	//汉子通信,代码
			{
				int len = strlen(SaveData.ccontext);
				m_JniDataSwitchCls.OnProcssHanziTx(SaveData.ccontext,len);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_TXR;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseTCS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	TCSData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,16))  //判断数据是否有效
		{
			
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.cFreNum,&NameLine);
			SaveData.cZhiLuNum = SNByte(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iInfoType = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			for (int i=0;i<12;i++)
			{
				SaveData.iStarNum[i] = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			}
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_TCS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseRMO(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	RMOData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,4))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.cIntence,&NameLine);
			SaveData.iMode = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.iFre = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_RMO;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseSCS(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	SCSData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;

	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,2))  //判断数据是否有效
		{
			SaveData.lShiChaData = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);			
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_SCS;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParsePKY(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	PKYData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;

	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,7))  //判断数据是否有效
		{
			SaveData.nWnzjs = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chMyue,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chKey,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chB1,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chB2,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chB3,&NameLine);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_PKY;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CBDDataParse::ParseSBX(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)
{
	SBXData SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	if(ProcessCRC(pBuffers,nLineFeedPos,nStartPos))   //判断数据校验位是否正确
	{
		if( IsValidData( pBuffers, nStartPos, nLineFeedPos,8))  //判断数据是否有效
		{
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chSbghname,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chSblx,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chBbh,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chCkxybbh,&NameLine);
			SNByteString(1,pBuffers,&nStartPos,nLineFeedPos,SaveData.chIcdxybbh,&NameLine);
			SaveData.nSbxlh = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			SaveData.nID = SNInteger(1,pBuffers,&nStartPos,nLineFeedPos);
			
			PortData.iDataLen = nLineFeedPos-StartPoint+1;
			memmove(PortData.sStartType,sStartType,3); 
			memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
			PortData.pData = &SaveData;
			PortData.DataType = TYPE_SBX;
			SendMessage(&PortData); //发送信息
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

#if 1
bool CBDDataParse::ParseXSD(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)     //下属定位
{	
	BDR_DWXX SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	int i = 0;
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据

	((unsigned char*)(&SaveData.dUserAddr))[0] = pBuffers[nStartPos+10];			//用户地址
	((unsigned char*)(&SaveData.dUserAddr))[1] = pBuffers[nStartPos+9];			//用户地址
	((unsigned char*)(&SaveData.dUserAddr))[2] = pBuffers[nStartPos+8];			//用户地址
	SaveData.cInfoSort = pBuffers[nStartPos+11];							        //信息类别
	SaveData.cPosTimeHour = pBuffers[nStartPos+12];								//定位时刻的小时数据
	SaveData.cPosTimeMinute = pBuffers[nStartPos+13];							//定位时刻的分位数据
	SaveData.cPosTimeSecond = pBuffers[nStartPos+14];							//定位时刻的秒位数据
	SaveData.cPosTimeMSecond = pBuffers[nStartPos+15];							//定位时刻的秒小数数据
	SaveData.cLonDegree = pBuffers[nStartPos+16];								//经度:度
	SaveData.cLonMinute = pBuffers[nStartPos+17];								//     分
	SaveData.cLonSecond = pBuffers[nStartPos+18];								//     秒
	SaveData.cLonMSecond = pBuffers[nStartPos+19];								//  0.1秒
	SaveData.cLatDegree = pBuffers[nStartPos+20];								//纬度:度
	SaveData.cLatMinute = pBuffers[nStartPos+21];								//     分
	SaveData.cLatSecond = pBuffers[nStartPos+22];								//     秒
	SaveData.cLatMSecond = pBuffers[nStartPos+23];								//  0.1秒
	
	if((SaveData.cInfoSort & 0x01) == 0)                          //cInfoSort的最后一位是0则是“普通”
	{
		short height,flag;
		((unsigned char*)(&height))[0] = pBuffers[nStartPos+25];               //高程
		((unsigned char*)(&height))[1] = pBuffers[nStartPos+24];
		flag=height>>14;										//高程的高2位是符号位，后14位为值
		SaveData.dElevation = height&0x3FFF;						//高程数据  0x3FFF == 000011111111111111
		if (flag) SaveData.dElevation = -SaveData.dElevation;		//高程数据
		
		((unsigned char*)(&height))[0] = pBuffers[nStartPos+27];               //高程异常
		((unsigned char*)(&height))[1] = pBuffers[nStartPos+26];
		flag=height>>8;											//高程异常的高8位是符号位，低8位为值
		SaveData.dElvError = height&0x00FF;						//高程异常值
		if (flag) SaveData.dElvError = -SaveData.dElvError;			//高程异常值
	}
	if((SaveData.cInfoSort & 0x01) == 1)                          //cInfoSort的最后一位是1则是“高空”
	{
		((unsigned char*)(&SaveData.dElevation))[0] = pBuffers[nStartPos+26];    //高程数据
		((unsigned char*)(&SaveData.dElevation))[1] = pBuffers[nStartPos+25];    //高程数据
		((unsigned char*)(&SaveData.dElevation))[2] = pBuffers[nStartPos+24];    //高程数据
	}
	SaveData.IsUrgent = BOOL((SaveData.cInfoSort & 0x04)>>2);		 //是否紧急，cInfoSort第6位是紧急标志，1则是紧急
	SaveData.IsMultiRes = BOOL((SaveData.cInfoSort & 0x02)>>1);		 //是否多解，cInfoSort第7位是多解标志，1则是多解
	SaveData.Precision = (SaveData.cInfoSort & 0x08)>>3;	    	 //精度，cInfoSort第5位是精度标志，1则是二档
	SaveData.HeightType = BOOL(SaveData.cInfoSort & 0x01);	    	 //高程指示，cInfoSort第8位是高程标志，1则是高空

	PortData.iDataLen = nLineFeedPos-StartPoint+1;
	memmove(PortData.sStartType,sStartType,3); 
	memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
	PortData.pData = &SaveData;
	PortData.DataType = TYPE_XSD;
	SendMessage(&PortData); //发送信息

	return true;
}

bool CBDDataParse::ParseXST(const unsigned char * const pBuffers, int nStartPos, int nLineFeedPos)     //下属通信
{
	LOGI("Recv xst");
	BDR_TXXX SaveData;
	PortData PortData;
	int NameLine = 0;
	int StartPoint = nStartPos;
	
	memset(&SaveData,0,sizeof(SaveData));  //清空结构体内数据
	memset(&PortData,0,sizeof(PortData));  //清空结构体内数据
	
	((unsigned char*)(&SaveData.dUserAddr))[0] = pBuffers[nStartPos+10];		 //用户地址
	((unsigned char*)(&SaveData.dUserAddr))[1] = pBuffers[nStartPos+9];		 //用户地址
	((unsigned char*)(&SaveData.dUserAddr))[2] = pBuffers[nStartPos+8];		 //用户地址
	SaveData.cInfoSort = pBuffers[11];								 //信息类别
	((unsigned char*)(&SaveData.dPostLetterAddr))[0] = pBuffers[nStartPos+14];//发信方地址
	((unsigned char*)(&SaveData.dPostLetterAddr))[1] = pBuffers[nStartPos+13];//发信方地址
	((unsigned char*)(&SaveData.dPostLetterAddr))[2] = pBuffers[nStartPos+12];//发信方地址
	SaveData.cPostLetterTimeHour = pBuffers[nStartPos+15];					 //发信时间:时
	SaveData.cPostLetterTimeMinute = pBuffers[nStartPos+16];					 //发信时间:分
	((unsigned char*)(&SaveData.sTelexLen))[0] = pBuffers[nStartPos+18];		 //长度
	((unsigned char*)(&SaveData.sTelexLen))[1] = pBuffers[nStartPos+17];		 //长度
	
	short telexlentemp;                                      //电文长度(字节数)
// 	if((SaveData.sTelexLen%8)!=0)//BCD码的情况
// 	{
// 		telexlentemp=SaveData.sTelexLen/8+1;
// 	}
// 	else                                           
// 	{
// 		telexlentemp=SaveData.sTelexLen/8;                //电文长度/8
// 	}
	telexlentemp = SaveData.sTelexLen/8;                //电文长度/8
	memcpy(SaveData.TelexContent,nStartPos+pBuffers+19,telexlentemp);	//电文内容≤120个汉字或420个代码
	
// 	unsigned char cCRC_Flag;
// 	memcpy(&cCRC_Flag,pBuffers+(nStartPos+telexlentemp+19),1);			//得到CRC标志，校验通信信息是否正确
// 	if(cCRC_Flag==0x00)
// 	{
// 		SaveData.CRCVerify=FALSE;//CRC正确
// 	}
// 	else
// 	{
// 		SaveData.CRCVerify=TRUE; //CRC出错
// 		//pBDAll->CreatAlarmWindow("接收短信CRC错误！",1200);
// 	}
	
	SaveData.cInfoSort = (SaveData.cInfoSort & 0x80) >> 7;

	if((unsigned char)(SaveData.TelexContent[0]) == 0xa4)//混编
		SaveData.cInfoSort = 2;
	
//	SaveData.CRCVerify = SaveData.CRCVerify;  //CRC出错标志
	
	if(/*(SaveData.cInfoSort & 0x20)>>5 == 0*/SaveData.cInfoSort == 0) //汉字
	{
		memcpy(SaveData.TelexContent,SaveData.TelexContent,SaveData.sTelexLen/8);

		m_JniDataSwitchCls.OnProcssHanziTx(SaveData.TelexContent,SaveData.sTelexLen/8);
	}
	else if(/*(SaveData.cInfoSort & 0x20)>>5 == 1*/SaveData.cInfoSort == 1)//代码
	{

		char buftemp[250] = {0};
		int lenttemp = 0;
		if((SaveData.sTelexLen%8)!=0)
		{
			for(int i=0;i<=(SaveData.sTelexLen/8);i++)
			{
				lenttemp += sprintf(buftemp+lenttemp,"%02x",SaveData.TelexContent[i]&0xff);
			}
			memset(SaveData.TelexContent,0,sizeof(char)*240);
			
			strncpy(SaveData.TelexContent,buftemp,lenttemp-1);
		}
		else
		{
			for(int i=0;i<(SaveData.sTelexLen/8);i++)
			{
				lenttemp += sprintf(buftemp+lenttemp,"%02x",SaveData.TelexContent[i]&0xff);
			}
			memset(SaveData.TelexContent,0,sizeof(char)*240);
			
			strncpy(SaveData.TelexContent,buftemp,lenttemp);
		}
		//		memcpy(SaveData.TelexContent,SaveData.TelexContent,nStartPos+SaveData.sTelexLen/8);
	}
	else if(SaveData.cInfoSort==2)//混合
	{
		memcpy(SaveData.TelexContent,SaveData.TelexContent+1,SaveData.sTelexLen/8 -1);
		SaveData.TelexContent[SaveData.sTelexLen/8 -1] = 0;

		m_JniDataSwitchCls.OnProcssHanziTx(SaveData.TelexContent,SaveData.sTelexLen/8-1);
	}

	//LOGI(SaveData.TelexContent);
	
	PortData.iDataLen = nLineFeedPos-StartPoint+1;
	memmove(PortData.sStartType,sStartType,3); 
	memmove(PortData.sYnanshiData,pBuffers+StartPoint,PortData.iDataLen);
	PortData.pData = &SaveData;
	PortData.DataType = TYPE_XST ;
	SendMessage(&PortData); //发送信息
	
	return true;
}

#endif

/*
void CBDDataParse::OnProcssHunHeTx(JNIEnv * env,char * strContent,int len)
{
	char ch[3] = "0";
	char *chtemp = new char [len/2];
	memset(chtemp,0,sizeof(char)*(len/2));
	
	int j = 0;
	
	for (int k = 2;k < len;k=k+2)
	{
		ch[0] = strContent[k]&0xff;
		ch[1] = strContent[k+1]&0xff;
		sscanf(ch,"%02X",chtemp+j);
		j++;
	}

	memset(strContent,0,sizeof(char)*len);
	
	jclass jstrcls = env->FindClass("java/lang/String");
	jmethodID jstriniid = env->GetMethodID(jstrcls,"<init>","([BLjava/lang/String;)V");
	
	jbyteArray jba = env->NewByteArray(strlen(chtemp));
	env->SetByteArrayRegion(jba,0,strlen(chtemp),(jbyte *)(chtemp));
	
	jstring jstrendcoding = env->NewStringUTF("gbk");
	
	jstring jstr = (jstring)env->NewObject(jstrcls,jstriniid,jba,jstrendcoding);
	
	char * datap = (char *)env->GetStringUTFChars(jstr,NULL);
	
	strcpy(strContent,datap);

	env->ReleaseStringUTFChars(jstr,datap);

	delete[] chtemp;
	chtemp = NULL;
}

void CBDDataParse::OnProcssHanziTx(JNIEnv * env,char * strContent,int len)
{
	jclass jstrcls = env->FindClass("java/lang/String");
	jmethodID jstriniid = env->GetMethodID(jstrcls,"<init>","([BLjava/lang/String;)V");
	
	jbyteArray jba = env->NewByteArray(strlen(strContent));
	env->SetByteArrayRegion(jba,0,len,(jbyte *)(strContent));
	
	jstring jstrendcoding = env->NewStringUTF("gbk");
	
	jstring jstr = (jstring)env->NewObject(jstrcls,jstriniid,jba,jstrendcoding);
	
	char * datap = (char *)env->GetStringUTFChars(jstr,NULL);
	
	strcpy(strContent,datap);
	
	env->ReleaseStringUTFChars(jstr,datap);

  }
*/

bool CBDDataParse::SendMessage(PortData *pPortData)
{
	int itype = pPortData->DataType;

//	CJniDataSwitchCls m_JniDataSwitchCls(m_jniEnv);

	if (itype == TYPE_ERROR)
	{

		jclass clsobj = m_JniDataSwitchCls.GetClass("ErrorData");
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetByteArrayFieldValue(clsobj,objs,"sYuansiData",(char *)(pPortData->sYnanshiData),pPortData->iDataLen);
		
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	else if (itype == TYPE_GGA)
	{
		LOGI("in GGATOJAVA");
		GGAData data = *(GGAData *)pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("GGAData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUTCTime",data.dUTCTime);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLat",data.dLat);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatDirection",data.cLatDirection);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLon",data.dLon);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonDirection",data.cLonDirection);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iState",data.iState);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iViewStarCount",data.iViewStarCount);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dHDOP",data.dHDOP);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dAntennaHigh",data.dAntennaHigh);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cHighUnit",data.cHighUnit);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dHighNovel",data.dHighNovel);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cHighNovelUnit",data.cHighNovelUnit);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iDataLife",data.iDataLife);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iPlatFormID",data.iPlatFormID);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dVDOP",data.dVDOP);

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Logitude",data.Logitude);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Latitude",data.Latitude);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));
	
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
		LOGI("in GGATOJAVA1");

	}
    /**
     * add by zhs 20161021
     */
	else if (itype == TYPE_WAA)
	{

		WAAData data = *(WAAData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("WAAData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"infoType",data.infoType);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"reportFre",data.reportFre);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"userID",data.userID);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"reportTime",data.reportTime);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"lon",data.lon);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"lonWay",data.lonWay);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"lat",data.lat);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"latWay",data.latWay);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"high",data.high);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"highDW",data.highDW);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonDegree",data.cLonDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMinute",data.cLonMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLonSecond",data.cLonSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMSecond",data.cLonMSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatDegree",data.cLatDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMinute",data.cLatMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLatSecond",data.cLatSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMSecond",data.cLatMSecond);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Logitude",data.Logitude);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Latitude",data.Latitude);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
    /**
     * add by zhs 20170813        0183协议转化为2.1
     */
	else if (itype == TYPE_ZDA)
	{

		ZDAData data = *(ZDAData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("ZDAData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"modeShow",data.modeShow);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUTCTime",data.dUTCTime);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iDay",data.iDay);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iMonth",data.iMonth);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iYear",data.iYear);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"localTime",data.localTime);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"localMinDiss",data.localMinDiss);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dDsxzTime",data.dDsxzTime);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"xzData",data.xzData);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iprecision",data.iprecision);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"spSateLockState",data.spSateLockState);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in ZDATOJAVA1");
	}

    /**
     * add by zhs 20170821        0183协议转化为2.1
     */
	else if (itype == TYPE_DHV)
	{

		DHVData data = *(DHVData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("DHVData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUTCTime",data.dUTCTime);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSpeed",data.dSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dXSpeed",data.dXSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dYSpeed",data.dYSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dZSpeed",data.dZSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dEarthSpeed",data.dEarthSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dMaxSpeed",data.dMaxSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dAverageSpeed",data.dAverageSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dAllAverageSpeed",data.dAllAverageSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dValueSpeed",data.dValueSpeed);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cSpeedUint",data.cSpeedUint);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in DHVTOJAVA1");
	}

    /**
     * add by zhs 20170813        0183协议转化为2.1
     */
	else if (itype == TYPE_GLL)
	{

		GLLData data = *(GLLData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("GLLData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLat",data.dLat);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatDirection",data.cLatDirection);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLon",data.dLon);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonDirection",data.cLonDirection);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUTCTime",data.dUTCTime);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cDataState",data.cDataState);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iModeType",data.iModeType);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in GLLTOJAVA1");
	}
    /**
     * add by zhs 20170813        0183协议转化为2.1
     */
	else if (itype == TYPE_TXT)
	{

		TXTData data = *(TXTData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("TXTData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"sentNumber",data.sentNumber);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"sentNum",data.sentNum);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"txtSignSymbol",data.txtSignSymbol);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"txtInfo",data.txtInfo);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in TXTTOJAVA1");
	}
    /**
     * add by zhs 20161212
     */
	else if (itype == TYPE_RMC)
	{
		LOGI("in RMCTOJAVA");
		RMCData data = *(RMCData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("RMCData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUtcTime",data.dUtcTime);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cStatus",data.cStatus);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLatitude",data.dLatitude);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatitudeDir",data.cLatitudeDir);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dLongitude",data.dLongitude);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLongitudeDir",data.cLongitudeDir);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSpeed",data.dSpeed);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dCourse",data.dCourse);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iDate",data.iDate);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dCpj",data.dCpj);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cCpjDir",data.cCpjDir);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cMode",data.cMode);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonDegree",data.cLonDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMinute",data.cLonMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLonSecond",data.cLonSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMSecond",data.cLonMSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatDegree",data.cLatDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMinute",data.cLatMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLatSecond",data.cLatSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMSecond",data.cLatMSecond);

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Logitude",data.Logitude);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Latitude",data.Latitude);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in RMCTOJAVA1");
	}
    /**
     * add by zhs 20161212
     */
	else if (itype == TYPE_SBX)
	{
		LOGI("in RMCTOJAVA");
		SBXData data = *(SBXData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("SBXData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"chSbghname",data.chSbghname);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"chSblx",data.chSblx);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"chBbh",data.chBbh);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"chCkxybbh",data.chCkxybbh);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"chIcdxybbh",data.chIcdxybbh);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"nSbxlh",data.nSbxlh);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"nID",data.nID);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in SBXTOJAVA1");
	}

    /**
     * add by zhs 20161212
     */
	else if (itype == TYPE_VTG)
	{

		VTGData data = *(VTGData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("VTGData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dRelNorthCourse",data.dRelNorthCourse);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cRelNorth",data.cRelNorth);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dPoleNorthCourse",data.dPoleNorthCourse);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cPoleNorth",data.cPoleNorth);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSpeed1",data.dSpeed1);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cSpeed1Uint",data.cSpeed1Uint);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSpeed2",data.dSpeed2);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cSpeed2Uint",data.cSpeed2Uint);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cModeType",data.cModeType);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	else if (itype == TYPE_TXR)
	{
		TXRData data = *(TXRData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("TXRData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"ixxleib",data.ixxleib);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"userID",data.userID);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"idianwenxs",data.idianwenxs);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"isendtime",data.isendtime);
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"ccontext",data.ccontext);
//		m_JniDataSwitchCls.SetByteArrayFieldValue(clsobj,objs,"contextarray",data.ccontext,strlen(data.ccontext));
		
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

	}
	else if (itype == TYPE_ICI)
	{
		ICIData data = *(ICIData *)pPortData->pData;
		jclass clsobj = m_JniDataSwitchCls.GetClass("ICIData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"userID",data.userID);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"TBID",data.TBID);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"serverFre",data.serverFre);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"TXOrder",data.TXOrder);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"userTZ",data.userTZ);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"XLH",data.XLH);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"XSUserNum",data.XSUserNum);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"JMFlag",data.JMFlag);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
	else if (itype == TYPE_DWR)
	{
		DWRData data = *(DWRData *)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("DWRData");
		if (clsobj == NULL)
		{
			return 0;
		}

		jobject objs= m_JniDataSwitchCls.CreateClassObject(clsobj);
		
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"posInfoType",data.posInfoType);
		m_JniDataSwitchCls.SetLongFieldValue(clsobj,objs,"userID",data.userID);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dUTCTime",data.dUTCTime);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"lat",data.lat);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"latWay",data.latWay);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"lon",data.lon);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"lonWay",data.lonWay);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"floorHigh",data.floorHigh);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"floorDW",data.floorDW);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"highError",data.highError);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"highErrorDW",data.highErrorDW);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"JDError",data.JDError);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"uposShow",data.uposShow);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"DJZShow",data.DJZShow);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"highTypeShow",data.highTypeShow);

		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonDegree",data.cLonDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMinute",data.cLonMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLonSecond",data.cLonSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLonMSecond",data.cLonMSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatDegree",data.cLatDegree);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMinute",data.cLatMinute);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"cLatSecond",data.cLatSecond);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cLatMSecond",data.cLatMSecond);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Logitude",data.Logitude);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"Latitude",data.Latitude);
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

	}
	else if (itype == TYPE_GSV)
	{
		LOGI("in GSVTOJAVA;");
		GSVData data = *(GSVData*)pPortData->pData;

		jclass clsobj = m_JniDataSwitchCls.GetClass("GSVData");
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSentenceCount",data.dSentenceCount);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dSentenceNum",data.dSentenceNum);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iViewStarCount",data.iViewStarCount);

		int size = 0;

		if (data.dSentenceNum < data.dSentenceCount)
		{
			size = 4;
		}
		else
		{
			size = data.iViewStarCount - (data.dSentenceCount-1)*4;
		}

		jobjectArray objarray = m_JniDataSwitchCls.CreateObjectArray(size);
		if (objarray == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetObjArrayFieldValue("StarInfo",data.StarInfo,objarray,size);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs,objarray);
		LOGI("in GSVTOJAVA1;");

	}
	else if (itype == TYPE_BSI)
	{
		BSIData data = *(BSIData *)pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("BSIData");
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs= m_JniDataSwitchCls.CreateClassObject(clsobj);
		
		if (objs == NULL)
		{
			return 0;
		}

		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"resPower",data.resPower);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"timePower",data.timePower);
		m_JniDataSwitchCls.SetShortArrayFieldValue(clsobj,objs,"power",data.power,10);

// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power1",data.power1);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power2",data.power2);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power3",data.power3);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power4",data.power4);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power5",data.power5);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power6",data.power6);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power7",data.power7);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power8",data.power8);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power9",data.power9);
// 		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"power10",data.power10);

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
	else if (itype == TYPE_GSA)
	{
		GSAData data = *(GSAData *)pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("GSAData");
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs= m_JniDataSwitchCls.CreateClassObject(clsobj);
		
		if (objs == NULL)
		{
			return 0;
		}

		
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"cModeType",data.cModeType);

		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"iModeSel",data.iModeSel);
		
		m_JniDataSwitchCls.SetIntArrayFieldValue(clsobj,objs,"lStarNum",(int *)(data.lStarNum),12);

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dHDOP",data.dHDOP);

		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dPDOP",data.dPDOP);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dTDOP",data.dTDOP);
		m_JniDataSwitchCls.SetDoubleFieldValue(clsobj,objs,"dVDOP",data.dVDOP);

		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"protoSentence",(char *)(pPortData->sYnanshiData));

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}
	else if (itype == TYPE_XST)		//下属通信
	{
		LOGI("in xst sendmessage begin");
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
		
		LOGI("in xst sendmessage notify");
		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);

		LOGI("in xst over");
	}
	else if (itype == TYPE_XSD)		//下属定位
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
	else if (itype == TYPE_FKI)		//反馈信息
	{
		FKIData data = *(FKIData *) pPortData->pData;
		
		jclass clsobj = m_JniDataSwitchCls.GetClass("FKIData");
		
		if (clsobj == NULL)
		{
			return 0;
		}
		
		jobject objs = m_JniDataSwitchCls.CreateClassObject(clsobj);
		if (objs == NULL)
		{
			return 0;
		}
		
		m_JniDataSwitchCls.SetStringFieldValue(clsobj,objs,"ZLName",data.ZLName);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"ZLExcuState",data.ZLExcuState);
		m_JniDataSwitchCls.SetCharFieldValue(clsobj,objs,"FreSetShow",data.FreSetShow);
		m_JniDataSwitchCls.SetShortFieldValue(clsobj,objs,"sendYZShow",data.sendYZShow);
		m_JniDataSwitchCls.SetIntFieldValue(clsobj,objs,"waitTime",data.waitTime);	

		m_JniDataSwitchCls.SendNotifyMessage(itype,objs);
	}

	return true;
}

/*

void CBDDataParse::SendDebugInfo(JNIEnv * env,char * strInfo)
{
	jclass cls = env->FindClass("com/project/dataprase/JniDataPrase");
	if (cls == NULL)
	{
		return;
	}

	jmethodID jmid = env->GetStaticMethodID(cls,"ShowDebugInfo","(Ljava/lang/String;)V");
	if (jmid == NULL)
	{
		return;
	}

	jstring jstr = env->NewStringUTF(strInfo);
	env->CallStaticVoidMethod(cls,jmid,jstr);
}

jclass CBDDataParse::GetClass(JNIEnv *env,char * clsname)
{
	char fullname[50] = {0};
	int len = sprintf(fullname,"%s","com/project/dataprase/");
	len += sprintf(fullname+len,"%s",clsname);
	
	jclass cls = env->FindClass(fullname);
	
	return cls;
}

jobject CBDDataParse::CreateClassObject(JNIEnv *env,jclass jcls)
{
	jmethodID mid = env->GetMethodID(jcls,"<init>","()V");
	if (mid == NULL)
	{
		return 0;
	}
	jobject objs = env->NewObject(jcls,mid);
	
	return objs;
}

void CBDDataParse::SendNotifyMessage(JNIEnv *env,int itype,jobject obj)
{	
	jclass clsInterface = env->FindClass("com/project/dataprase/JniDataPrase");
	if (clsInterface == NULL)
	{
		return;
	}
	
	jmethodID jmid = env->GetStaticMethodID(clsInterface,"NotityMain","(ILjava/lang/Object;)I");
	env->CallStaticIntMethod(clsInterface,jmid,itype,obj);
}

void CBDDataParse::SendNotifyMessage(JNIEnv *env,int itype,jobject obj,jobjectArray objarry)
{

	jclass clsInterface = env->FindClass("com/project/dataprase/JniDataPrase");
	if (clsInterface == NULL)
	{
		return;
	}

	jmethodID jmid = env->GetStaticMethodID(clsInterface,"NotityMain","(ILjava/lang/Object;[Ljava/lang/Object;)I");
	if (jmid == NULL)
	{
		return;
	}

	env->CallStaticIntMethod(clsInterface,jmid,itype,obj,objarry);
}

bool CBDDataParse::SetDoubleFieldValue(JNIEnv *env,jclass &clsobj,jobject & obj,char * filedname,double val)
{
	jfieldID jfid = NULL;
	jfid = env->GetFieldID(clsobj,filedname,"D");
	if (jfid == NULL)
	{
		return false;
	}
	env->SetDoubleField(obj,jfid,val);
	return true;
}

bool CBDDataParse::SetCharFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *filedname,char val)
{
	jfieldID jfid = NULL;
	jfid = env->GetFieldID(clsobj,filedname,"C");
	if (jfid == NULL)
	{
		return false;
	}
	env->SetCharField(obj,jfid,val);

	return true;
}

bool CBDDataParse::SetShortFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *filename,short val)
{
	jfieldID jfid = NULL;
	jfid = env->GetFieldID(clsobj,filename,"S");
	if (jfid == NULL)
	{
		return false;
	}
	env->SetShortField(obj,jfid,val);
	return true;
}

bool CBDDataParse::SetIntFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,int val)
{
	jfieldID jfid = NULL;
	jfid = env->GetFieldID(clsobj,fieldname,"I");
	if (jfid == NULL)
	{
		return false;
	}
	env->SetIntField(obj,jfid,val);
	return true;
}

bool CBDDataParse::SetLongFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,unsigned long val)
{
	jfieldID jfid = NULL;
	jfid = env->GetFieldID(clsobj,fieldname,"J");
	if (jfid == NULL)
	{
		return false;
	}
	env->SetLongField(obj,jfid,val);

	return true;
}

bool CBDDataParse::SetStringFieldValue(JNIEnv *env,jclass &clsobj,jobject &obj,char *fieldname,char *val)
{
	jfieldID jfid = NULL;

	jfid = env->GetFieldID(clsobj,fieldname,"Ljava/lang/String;");
	if (jfid == NULL)
	{
		return false;
	}
	
	jstring jstr = env->NewStringUTF(val);
	
	env->SetObjectField(obj,jfid,jstr);
	return true;
}

void CBDDataParse::SetObjArrayFieldValue(JNIEnv *env,char *fieldname,void *val,jobjectArray &objary,int counts)
{

	if (objary == NULL)
	{
		return;
	}

	StarState *data = (StarState *)val;


	jclass objectcls = GetClass(env,"StarInfo");
	if (objectcls == NULL)
	{
		return;
	}
	
	for (int i = 0;i < counts;i++)
	{
		jobject objec = CreateClassObject(env,objectcls);
		if (objec == NULL)
		{
			return;
		}
		SetLongFieldValue(env,objectcls,objec,"lStarNum",data[i].lStarNum);
		SetDoubleFieldValue(env,objectcls,objec,"dStarElevation",data[i].dStarElevation);
		SetDoubleFieldValue(env,objectcls,objec,"dStarAzimuth",data[i].dStarAzimuth);
		SetDoubleFieldValue(env,objectcls,objec,"dSNR",data[i].dSNR);

		env->SetObjectArrayElement(objary,i,objec);
	}
}

*/
