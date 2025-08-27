#ifndef 	__UDPCOMM_H__
#define		__UDPCOMM_H__

#define		FIFO_LEN					128000		/*FIFO深度*/    

#define		STATE_SENDRTDAT				0x00		
#define		STATE_SENDPRDAT				0x01			
#define		STATE_SENDCRDAT				0x02		
#define		STATE_SENDSCOPEUSA			0x03		
#define		STATE_SENDSCOPEUSB			0x04		
#define		STATE_SENDSCOPEUSC			0x05		
#define		STATE_SENDSCOPEISA			0x06			
#define		STATE_SENDSCOPEISB			0x07		
#define		STATE_SENDSCOPEISC			0x08			
#define		STATE_SENDSCOPEILA			0x09	
#define		STATE_SENDSCOPEILB			0x0A			
#define		STATE_SENDSCOPEILC			0x0B	
#define		STATE_SENDSCOPEIFA			0x0C		
#define		STATE_SENDSCOPEIFB			0x0D			
#define		STATE_SENDSCOPEIFC			0x0E	
#define		STATE_SENDSCOPEIRA			0x0F		
#define		STATE_SENDSCOPEIRB			0x10	
#define		STATE_SENDSCOPEIRC			0x11		
#define		STATE_SENDSCOPEDBA			0x12	
#define		STATE_SENDSCOPEDBB			0x13		
#define		STATE_SENDSCOPEDBC			0x14
#define		STATE_SENDSCOPEDTA			0x15	
#define		STATE_SENDSCOPEDTB			0x16		
#define		STATE_SENDSCOPEDTC			0x17
#define		STATE_SENDEVTDAT			0x20			/*发送事件数据*/
#define		STATE_SENDTSTDAT			0x21			/*发送试验数据*/

#define 	STATE_RxAPDATA    			0x80		 //通信箱rword

#define		TX_RDPRDATA					0x0101		
#define		TX_WRPRDATA					0x0102		
#define		TX_RDCRDATA					0x0103
#define		TX_WRCRDATA					0x0104
#define		TX_CLRDONE					0x0105
#define		TX_CLRCANSEL				0x0106
#define		TX_SRON						0x0107
#define		TX_SROFF					0x0108
#define		TX_SMON						0x0109
#define		TX_SMOFF					0x010A
#define		TX_WORKDONE					0x010B
#define		TX_WORKCANSEL				0x010C
#define		TX_EMRDONE					0x010D
#define		TX_EMRCANSEL				0x010E
#define		TX_CLREVENT					0x010F
#define		TX_READEVENT				0x0110
#define		TX_CHECKTIME				0x0111
#define		TX_SELCTSYS					0x0112		/*选择采系统电流*/
#define		TX_SELCTLOAD				0x0113		/*选择采负载电流，默认状态*/

#define		TX_QF3ON					0x0114		/*出线断路器合*/
#define		TX_QF3OFF					0x0115		/*出线断路器分*/

#define		TX_AUTOSTART				0x0118		/*自动运行*/
#define		TX_AUTOSTOP					0x0119		/*自动停止*/

#define		TX_READREF					0x0121		/*读取参考电流*/
#define		TX_WRITEREF					0x0120		/*写参考电流*/

#define		TX_HB						0x0200		/*心跳命令*/

#define		TX_STSQF1ON					0x0201		/*STSQF1ON命令*/   
#define		TX_STSQF1OFF				0x0202		/*STSQF1OFF命令*/   
#define		TX_STSQF2ON					0x0203		/*STSQF2ON命令*/   
#define		TX_STSQF2OFF				0x0204		/*STSQF2OFF命令*/   
#define		TX_STSCUTON					0x0205		/*STSCUTON命令*/   
#define		TX_STSCUTOFF				0x0206		/*STSCUTOFF命令*/   
#define		TX_STSKM1ON					0x0207		/*STSKM1ON命令*/   
#define		TX_STSKM1OFF				0x0208		/*STSKM1OFF命令*/


extern short usWave[3][FIFO_LEN];		//系统电压缓冲区，先进先出结构								
extern short isWave[3][FIFO_LEN];		//系统电流缓冲区，先进先出结构 
extern short ilWave[3][FIFO_LEN];		//负载电流缓冲区, 先进先出结构
extern short ifWave[3][FIFO_LEN];		//反馈电流缓冲区，先进先出结构
extern short irWave[3][FIFO_LEN];		//指令电流缓冲区，先进先出结构
extern short dbWave[3][FIFO_LEN];		//调制波缓冲区，先进先出结构
extern short dtWave[3][FIFO_LEN];
extern unsigned long disPlayLen;		//默认显示1024个数据

extern unsigned int gUDPChannel;  
extern void udpCreate(unsigned int port);
extern void udpDispose(void);

extern void tdmsFileOpen(void);
extern void tdmsFileClose(void);

extern int gRun;							//运行标志    
extern CmtTSQHandle tsqRxId, tsqTxId;  
extern int CVICALLBACK udpThread (void *functionData);

#endif
