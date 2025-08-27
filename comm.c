#include <userint.h>
#include <utility.h>
#include <cvitdms.h>
#include <ansi_c.h>
#include <udpsupp.h>
#include "comm.h"
#include "datatype.h"
#include "protocol.h"
#include "devData.h"


#define		PORT						50000   // Local Port Client

#define		WAVE_LENGTH		128



unsigned int gUDPChannel;					//UDP通道
IPAddr		 gRemoteAddr;					//远程地址
unsigned int gRemotePort;					//远程端口
int          gRun;							//运行标志

RTDataType   rtdata;						//上传实时数据
PrDataType   prdata;						//设置参数
CrDataType   crdata;						//校准数据
ART_DataType irt_data;						//displayPanel接受的数据


u16          prdataUpdate = 0;				//参数显示更新
/*----------------------------------------------------------------
  process data received
------------------------------------------------------------------*/
unsigned char  rxBuf[512];					//接收缓冲区
unsigned int   rxIndex = 0;					//接收缓冲指针
unsigned short rxLen;						//接收数据长度
unsigned short word;						//功能码
unsigned short addr;						//终端ID
unsigned char  step = 0;  					//处理步骤
/*---------------------------------------------------------------
	缓存1000个工频周波数据
-----------------------------------------------------------------*/
short usWave[3][FIFO_LEN];					//系统电压缓冲区，先进先出结构
short isWave[3][FIFO_LEN];					//系统电流缓冲区，先进先出结构
short ilWave[3][FIFO_LEN];					//负载电流缓冲区, 先进先出结构
short ifWave[3][FIFO_LEN];					//反馈电流缓冲区，先进先出结构
short irWave[3][FIFO_LEN];					//指令电流缓冲区，先进先出结构
short dbWave[3][FIFO_LEN];					//调制波缓冲区，先进先出结构
short dtWave[3][FIFO_LEN];					//误差电流缓冲区，先进先出结构
unsigned long disPlayLen = 256;				//默认显示1024个数据
/*--------------------------------------------------------------
	TDMS文件全部变量
----------------------------------------------------------------*/
TDMSFileHandle 		   gTdmsFileHandle;
TDMSChannelGroupHandle gChannelGroup;
char*                  gChannelGroupName = "实时波形";
TDMSChannelHandle      gChannel[22];
char*                  gChannelName[22] = {"A相系统电压", "B相系统电压", "C相系统电压", "A相系统电流", "B相系统电流", "C相系统电流",
		"A相负载电流", "B相负载电流", "C相负载电流", "A相补偿电流", "B相补偿电流", "C相补偿电流",
		"A相参考电流", "B相参考电流", "C相参考电流", "A相调制波",   "B相调制波",   "C相调制波"  ,
		"A相误差电流", "B相误差电流", "C相误差电流", "参考位移"
										  };
int gRecordFlag = 0;
int gRecordCheck = 0;

int CVICALLBACK UDPCallback(unsigned channel, int eventType, int errCode, void *callbackData);
void processData(unsigned char *buf, unsigned int len ,unsigned int port, IPAddr remoteIp);

void flashRunLed(void);
/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------*/
void udpCreate(unsigned int port)
{
	int errCode;
	char *err;
	errCode = CreateUDPChannelConfig (port, UDP_ANY_ADDRESS, 0, UDPCallback, NULL, &gUDPChannel);
	if(errCode < 0) err = GetUDPErrorString(errCode);
}


/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------*/
void udpDispose(void)
{
	if(gUDPChannel) DisposeUDPChannel(gUDPChannel);
}
/*-------------------------------------------------------
	UDP callback function
---------------------------------------------------------*/
int CVICALLBACK UDPCallback(unsigned channel, int eventType, int errCode, void *callbackData)
{
	int 		  error = 0,
				  size;
	unsigned char *pdat = NULL;

	if(eventType == UDP_DATAREADY)
	{
		// Pass NULL as the input buffer to determine the size of the arrived data.
		size = UDPRead(channel, NULL, 0, UDP_DO_NOT_WAIT, NULL, NULL);
		if(size <= 0) return 0;

		pdat = malloc(size);
		size = UDPRead(channel, pdat, size, UDP_DO_NOT_WAIT, &gRemotePort, gRemoteAddr);
		processData(pdat, size, gRemotePort, gRemoteAddr);
		// 	printf("size",size);

	}
	if(pdat) free(pdat);
	return 0;
}


/*--------------------------------------------------------------
	FIFO区域清零
----------------------------------------------------------------*/
void clrFIFO(short *fifoBuf, unsigned long len)
{
	unsigned long i;
	for(i=0; i<len; i++)
	{
		fifoBuf[i] = 0;
	}
}
/*--------------------------------------------------------------
	函数功能: 写FIFO缓冲区
----------------------------------------------------------------*/
void writeFIFO(short *fifoBuf, short *wbuf, unsigned long len)
{
	unsigned long i;
	unsigned long index1, index2;

	index1 = disPlayLen - 1;
	index2 = disPlayLen - len - 1;

	//FIFO数据移动
	for(i=0; i<disPlayLen-len; i++)
	{
		fifoBuf[index1--] = fifoBuf[index2--];
	}
	//写入新的数据
	for(i=0; i<len; i++)
	{
		fifoBuf[i] = wbuf[len-i-1];
	}
}
/*--------------------------------------------------------------
	函数功能: 计算校验和
----------------------------------------------------------------*/
unsigned char checkSum(unsigned char *buf, unsigned int len)
{
	unsigned char sum = 0;
	unsigned int  i;

	for(i=0; i<len; i++)
	{
		sum += buf[i];
	}
	return sum;
}
/*--------------------------------------------------------------
	函数功能：获取波形数据
----------------------------------------------------------------*/
void getWaveData(short *wptr, unsigned char *pbuf)
{
	unsigned int i;

	for(i=0; i<WAVE_LENGTH; i++)
	{
		wptr[i] = pbuf[i*2] + 256*pbuf[i*2+1];
	}
#if(0)
	for(; i<128; i++)
	{
		wptr[i] = wptr[i-100];
	}
#endif
}
/*--------------------------------------------------------------
	函数功能: UDP上传数据处理
----------------------------------------------------------------*/
unsigned long indexBase = 0;


void dataAnalyse(unsigned char *buf, unsigned int len, unsigned int rword, unsigned int raddr)
{
	static int c = 0;
	int i, j;
	short wave[128];
	u16 dataType;
	u8  *pdat;
	u8  *padat;
	u8  *sdat;
	u8  *adat;
	u16 devIndex;
	switch(rword)
	{
			//系统电压A
		case STATE_SENDSCOPEUSA:
			if((gRecordCheck == 1)&&(gRecordFlag == 0))
			{
				indexBase = 0;
				gRecordFlag = 1;
			}
			else if((gRecordCheck == 0)&&(gRecordFlag == 1))
			{
				gRecordFlag = 0;
				tdmsFileClose();
			}

			getWaveData(wave, buf);
			 /*
			 printf("--- Generating Source Byte Data (pbuf) ---\n");
			 for (i = 0; i < WAVE_LENGTH * 2; i++)
			 {
				 buf[i] = i % 256; // ?? 0, 1, 2, ..., 255, 0, 1, ...
			 }
			 printf("Source data generated.\n\n");
			 printf("--- Converted Wave Data (wptr) ---\n");
			 for (i = 0; i < WAVE_LENGTH; i++)
			 {

				 printf("wptr[%3d] = %6d\n", i, wave[i]);
			 }
			 */
			if(gRecordFlag) TDMS_AppendDataValues(gChannel[0], wave, WAVE_LENGTH, 1);
			writeFIFO(&usWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区

			if(gRecordFlag)
			{
				for(i=0; i<128; i++)
				{
					wave[i] = indexBase++;
				}
				TDMS_AppendDataValues(gChannel[21], wave, WAVE_LENGTH, 1);
			}
			break;
		case STATE_SENDSCOPEUSB:
			getWaveData(wave, buf);
			if(gRecordFlag) TDMS_AppendDataValues(gChannel[1], wave, WAVE_LENGTH, 1);
			writeFIFO(&usWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEUSC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[2], wave, WAVE_LENGTH, 1);
			writeFIFO(&usWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//系统电流IA
		case STATE_SENDSCOPEISA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[3], wave, WAVE_LENGTH, 1);
			writeFIFO(&isWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEISB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[4], wave, WAVE_LENGTH, 1);
			writeFIFO(&isWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEISC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[5], wave, WAVE_LENGTH, 1);
			writeFIFO(&isWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//负载电流ILA
		case STATE_SENDSCOPEILA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[6], wave, WAVE_LENGTH, 1);
			writeFIFO(&ilWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEILB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[7], wave, WAVE_LENGTH, 1);
			writeFIFO(&ilWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEILC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[8], wave, WAVE_LENGTH, 1);
			writeFIFO(&ilWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//反馈电流FBA
		case STATE_SENDSCOPEIFA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[9], wave, WAVE_LENGTH, 1);
			writeFIFO(&ifWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEIFB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[10], wave, WAVE_LENGTH, 1);
			writeFIFO(&ifWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEIFC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[11], wave, WAVE_LENGTH, 1);
			writeFIFO(&ifWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//指令电流
		case STATE_SENDSCOPEIRA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[12], wave, WAVE_LENGTH, 1);
			writeFIFO(&irWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEIRB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[13], wave, WAVE_LENGTH, 1);
			writeFIFO(&irWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEIRC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[14], wave, WAVE_LENGTH, 1);
			writeFIFO(&irWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//调制波
		case STATE_SENDSCOPEDBA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[15], wave, WAVE_LENGTH, 1);
			writeFIFO(&dbWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEDBB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[16], wave, WAVE_LENGTH, 1);
			writeFIFO(&dbWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEDBC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[17], wave, WAVE_LENGTH, 1);
			writeFIFO(&dbWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//误差电流
		case STATE_SENDSCOPEDTA:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[18], wave, WAVE_LENGTH, 1);
			writeFIFO(&dtWave[0][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEDTB:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[19], wave, WAVE_LENGTH, 1);
			writeFIFO(&dtWave[1][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
		case STATE_SENDSCOPEDTC:
			getWaveData(wave, buf);

			if(gRecordFlag) TDMS_AppendDataValues(gChannel[20], wave, WAVE_LENGTH, 1);
			writeFIFO(&dtWave[2][0], wave, WAVE_LENGTH);		//写入缓冲区
			break;
			//--------------------------------------------------------
		case STATE_SENDRTDAT:									//接收实时数据
			pdat = (u8*)&rtdata;
			for(i=0; i<sizeof(rtdata); i++)
			{
				pdat[i] = buf[i];
			}
			if(++c > 10)
			{
				c = 0;
				dataType = 1;
				if(getDevIndexFromId(raddr, &devIndex))
				{
					if(devInfo[devIndex].state == DEV_OFFLINE)
					{
						devInfo[devIndex].state = DEV_ONLINE;
						strcpy(devInfo[devIndex].rIP, gRemoteAddr);
						devInfo[devIndex].rport = gRemotePort;
					}
					devInfo[devIndex].wdtCnt = 0;
					if(devIndex == debugCurDev) CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					sdat = (u8*)&devRtdata[devIndex];
					for(i=0; i<sizeof(rtdata); i++)
					{
						sdat[i] = pdat[i];
					}
				}
			}
			break;
		case STATE_RxAPDATA:  // Newline This
			padat = (u8*)&irt_data;
			for(i = 0; i < sizeof(irt_data); i++)
			{
				padat[i] = buf[i];
			}
			if(++c > 10)
			{
				c = 0;
				dataType = 1;
				if(getDevIndexFromId(raddr, &devIndex))
				{
					if(devInfo[devIndex].state == DEV_OFFLINE)
					{
						devInfo[devIndex].state = DEV_ONLINE;
						strcpy(devInfo[devIndex].rIP, gRemoteAddr);
						devInfo[devIndex].rport = gRemotePort;
					}
					devInfo[devIndex].wdtCnt = 0;
					if(devIndex == debugCurDev) CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					sdat = (u8*)&devRtdata[devIndex];
					for(i=0; i<sizeof(irt_data); i++)
					{
						sdat[i] = padat[i];
					}
				}
			}
			break;
		case STATE_SENDPRDAT:
			dataType = 2;
			pdat = (u8*)&prdata;
			for(i=0; i<sizeof(prdata); i++)
			{
				pdat[i] = buf[i];
			}
			if(getDevIndexFromId(raddr, &devIndex))
			{
				if(devIndex == debugCurDev) prdataUpdate = 2;
			}
			//CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);

			break;
		case STATE_SENDCRDAT:
			dataType = 3;
			pdat = (u8*)&crdata;
			for(i=0; i<sizeof(crdata); i++)
			{
				pdat[i] = buf[i];
			}
			if(getDevIndexFromId(raddr, &devIndex))
			{
				if(devIndex == debugCurDev) prdataUpdate = 3;
			}
			//CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
		case STATE_SENDEVTDAT:		/*接收到事件数据*/
			dataType = 4;
			pdat = (u8*)&evtData;
			for(i=0; i<sizeof(evtData); i++)
			{
				pdat[i] = buf[i+1];
			}
			CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
		case STATE_SENDTSTDAT:
			dataType = 5;
			pdat = (u8*)&elTstVal;
			for(i=0; i<sizeof(elTstVal); i++)
			{
				pdat[i] = buf[i];
			}
			CmtWriteTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
		default:
			break;
	}
}
/*--------------------------------------------------------------
	函数功能: 上传数据包解析
----------------------------------------------------------------*/
void processData(unsigned char *buf, unsigned int len, unsigned int port, IPAddr remoteIp)
{
	unsigned int i;
	// For loop To Get the Full Data char
	for(i=0; i<len; i++)
	{
		switch(step)
		{
			case 0:
				//Header
				if(buf[i] == 0x68)
				{
					rxIndex = 0; // buffIndex
					step=1;		  // Next step
				}
				break;
			case 1:
				// Address High   unsigned short
				addr = ((buf[i]<<8)&0xFF00);
				step=2;
				break;
			case 2:
				// Address Low
				addr |= (buf[i]&0xFF);
				step=3;
				break;
			case 3:
				// Recive the Data Length Low
				rxLen = buf[i];
				step=4;
				break;
			case 4:
				// Recive the Data Length High
				rxLen |= ((buf[i]<<8)&0xFF00);
				step=5;
				break;
			case 5:
				//Re justify the Header
				if(buf[i] != 0x68) step = 0;
				else step=6;
				break;
			case 6:
				//Store Low word
				word = buf[i];
				step=7;
				break;
			case 7:
				// Gather Word
				word |= ((buf[i]<<8)&0xFF00);
				step=8;
				break;
			case 8:
				// 先赋值 再++
				rxBuf[rxIndex++] = buf[i];
				if(rxIndex >= rxLen)
				{
					step=9;
				}
				break;
			case 9:
				if(checkSum(rxBuf, rxIndex) != buf[i])
				{
					step = 0;
				}
				else step=10;
				break;
			case 10:
				// End Send the Data Index Word And adress
				if(buf[i] == 0x16)
					dataAnalyse(rxBuf, rxIndex, word, addr);
				step = 0;
				break;
			default:
				step = 0;
				break;
		}
	}
}


/*--------------------------------------------------------------------------------------------
	创建TDMS文件
----------------------------------------------------------------------------------------------*/
void createTDMSFile(void)
{
	short i;
	char  filename[64];
	int   year, mon, day, hour, min, sec;
	GetSystemDate(&mon, &day, &year);
	GetSystemTime(&hour, &min, &sec);

	sprintf(filename, "apfDat%d%d%d%d%d%d.tdms", year, mon, day, hour, min, sec);
	TDMS_CreateFile(filename, TDMS_Streaming2_0, "apfDebug Data", "APF Debug Data", "APF调试数据", "wuhankeliyuan", &gTdmsFileHandle);
	TDMS_AddChannelGroup(gTdmsFileHandle, gChannelGroupName, "APF DATA Group", &gChannelGroup);
	for(i=0; i<22; i++)
	{
		TDMS_AddChannel (gChannelGroup, TDMS_Int16, gChannelName[i], "波形", 0, &gChannel[i]);
	}
}
/*--------------------------------------------------------------------------------------------
	打开TDMS文件
----------------------------------------------------------------------------------------------*/
void tdmsFileOpen(void)
{
#if(0)
	int err = -1;
	int i;
	err = TDMS_OpenFile("apfData.tdms", 0, &gTdmsFileHandle);

	if(err < 0)
	{
		createTDMSFile();
	}
	else
	{
		TDMS_GetChannelGroupByName(gTdmsFileHandle, gChannelGroupName, &gChannelGroup);
		for(i=0; i<18; i++)
		{
			TDMS_GetChannelByName(gChannelGroup, gChannelName[i], &gChannel[i]);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------------
	关闭TDMS文件
---------------------------------------------------------------------------------------------*/
void tdmsFileClose(void)
{
	TDMS_CloseFile(gTdmsFileHandle);
}
/*--------------------------------------------------------------------------------------------
	主线程发过来的消息
----------------------------------------------------------------------------------------------*/
void CVICALLBACK tsqTxCallback(CmtTSQHandle queueHandle, unsigned int event, int value, void *callbackData)
{
	u16 dataType;
	u8  sendBuf[512];
	// u8  sendBuf[256];
	u8  isendBuf[512];
	u16 slen = 0;

	switch(event)
	{
		case EVENT_TSQ_ITEMS_IN_QUEUE:
			CmtReadTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, 0);
			switch(dataType)
			{
				case CMD_READPRDATA:			//读参数
					makeReadPrDataPacket(devInfo[debugCurDev].id, sendBuf, &slen);

					break;
				case CMD_WRITEPRDATA:			//写参数
					makeWritePrDataPacket(devInfo[debugCurDev].id, sendBuf, &slen);

				case CMD_READCRDATA:			//读校准数据
					makeReadCrDataPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_WRITECRDATA:			//写校准数据
					makeWriteCrDataPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_CLRFDONE:				//清故障执行
					makeClrfDonePacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_CLRFCANSEL:			//清故障撤销
					makeClrfCanselPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_SRON:					//投电阻
					makeSRONPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_SROFF:					//切电阻
					makeSROFFPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_SMON:					//投主开关
					makeSMONPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_SMOFF:					//切主开关
					makeSMOFFPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_WORKDONE:				//工作执行
					makeWorkDonePacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_WORKCANSEL:			//工作撤销
					makeWorkCanselPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_EMRDONE:				//急停执行
					makeEMRDonePacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_EMRCANSEL:				//急停撤销
					makeEMRCanselPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_CLREVENT:				//清除事件记录
					makeClearEventPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_READEVENT:				//读事件记录
					makeReadEventPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_CHECKTIME:				//对时
					makeCheckTimePacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_RECORDSTART:			//录波开始
					createTDMSFile();
					gRecordCheck = 1;
					break;
				case CMD_RECORDEND:				//录波结束
					gRecordCheck = 0;
					//	tdmsFileClose();
					break;
				case CMD_SELCTSYS:				//选择系统电流采样
					makeSELCTSYSPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_SELCTLOAD:				//选择负载电流采样
					makeSELCTLOADPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_QF3ON:					//合QF3
					makeQF3ONPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_QF3OFF:				//分QF3
					makeQF3OFFPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;

				case CMD_AUTOSTART:				//自动启动
					makeAutoStartPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_AUTOSTOP:				//自动停止
					makeAutoStopPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_READREF:				//读参考电流命令
					makeReadRefPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;
				case CMD_WRITEREF:				//写参考电流命令
					makeWriteRefPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;

				case CMD_HB:
					makeHBPacket(devInfo[debugCurDev].id, sendBuf, &slen);
					break;

				case CMD_STSQF1ON:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSQF1ON, sendBuf, &slen);
					break;

				case CMD_STSQF1OFF:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSQF1OFF, sendBuf, &slen);
					break;

				case CMD_STSQF2ON:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSQF2ON, sendBuf, &slen);
					break;

				case CMD_STSQF2OFF:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSQF2OFF, sendBuf, &slen);
					break;

				case CMD_STSCUTON:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSCUTON, sendBuf, &slen);
					break;

				case CMD_STSCUTOFF:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSCUTOFF, sendBuf, &slen);
					break;

				case CMD_STSKM1ON:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSKM1ON, sendBuf, &slen);
					break;

				case CMD_STSKM1OFF:
					makeSTSPacket(devInfo[debugCurDev].id, TX_STSKM1OFF, sendBuf, &slen);
					break;
				case CMD_GETADATA:
					rtDataGet(sendBuf,  &slen);
					break;
				case CMD_SENDADATA:
					rtDataMaker(sendBuf,  &slen);
					break;
				default:
					break;
			}
			if(slen)
			{
				if(devInfo[debugCurDev].state == DEV_ONLINE)
				{
					UDPWrite(gUDPChannel, devInfo[debugCurDev].rport, devInfo[debugCurDev].rIP, sendBuf, slen);
				}
			}

			break;
	}
}
/*--------------------------------------------------------------------------------------------
	UDP处理线程 Main Fuction
----------------------------------------------------------------------------------------------*/
int CVICALLBACK udpThread (void *functionData)
{
	//创建UDP通CVICALLBACK信端口
	udpCreate(PORT);
	//创建数据发送队列
	CmtNewTSQ(1, sizeof(u16), OPT_TSQ_DYNAMIC_SIZE, &tsqTxId);
	CmtInstallTSQCallback(tsqTxId, EVENT_TSQ_ITEMS_IN_QUEUE, 1, tsqTxCallback, NULL, CmtGetCurrentThreadID(), NULL);

	while(gRun)
	{
		ProcessSystemEvents();
	}
	//销毁UDP端口
	udpDispose();
	//销毁队列
	CmtDiscardTSQ(tsqTxId);
	//关闭TDMS文件
	if(gRecordFlag)
	{
		gRecordFlag = 0;
		tdmsFileClose();
	}
	return 0;
}
