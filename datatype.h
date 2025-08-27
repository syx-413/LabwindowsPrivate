#ifndef  __DATATYPE_H__
#define	 __DATATYPE_H__

typedef unsigned char 	u8;
typedef          char 	s8;
typedef	unsigned short  u16;
typedef          short  s16;
typedef unsigned long	u32;
typedef          long	s32;

#pragma pack(1) 
/*------------------------------------------------------
	实时时钟数据
--------------------------------------------------------*/
typedef struct{
	u16 year : 8;							  
	u16 mon  : 8;
	u16 day  : 8;
	u16 hour : 8;
	u16 min  : 8;
	u16 sec  : 8;
}TimeTypeDef;

typedef struct {
    u32 errorCode;     // 
    u16 errorStatus;   // 
    u16 errorCount;    // 
    u16 reserved;      // 
} devErr_t;
/*-----------------------------------------------------
   实时数据
-------------------------------------------------------*/
typedef struct{
	s16 u[3];					//0.三相系统电压
	s16 uz;						//1.零序电流
	s16 unb;					//2.三相电压不平衡度
	s16 thdu[3];				//3.电压总畸变率
	s16 is[4];					//4.系统A/B/C/N电流
	s16 isnb;					//5.系统电流不平衡度
	s16 thdis[4];				//6.系统电流总畸变率
	s16 il[4];					//7.负载A/B/C/N电流
	s16 ilnb;					//8.负载电流不平衡度
	s16 thdil[4];				//9.负载电流总畸变率
	s32 ifb[4];					//10.补偿电流
	s32 ir[4];					//11.指令电流
	s32 ps[4];					//12.系统有功功率
	s32 qs[4];					//13.系统无功功率
	s16 pfs[4];					//14.系统功率因数
	s16 dpfs[4];				//15.位移功率因数
	s32 pl[4];					//16.负载有功功率
	s32 ql[4];					//17.负载无功功率
	s16 pfl[4];					//18.负载功率因数
	s16 dpfl[4];				//19.负载位移功率因数
	s16 temp[4];				//20.IGBT温度
	s16 udc;					//21.单元直流电压
	s16 udc2;					//22.下电容直压
	u16 freq;					//22.频率
	u16 fpgaState[4];			//23.FPGA状态字
	u16 resv;
	u32 devState;				//24.装置状态字
	u16 ioState;				//25.数字量输入和输出
	TimeTypeDef time; 			//26.实时时钟
	u16 runState;				//27.当前运行状态
	u16 debugCmd;				//28.调试命令
	u16 debugDat;				//29.命令数据
	float iqref;				//30.需要补偿的无功电流
	float iqfeed;				//31.实际输出的无功电流
	float inqreal;				//32.负序实部指令电流
	float inqimag;				//33.负序虚部指令电流
	float izqreal;				//34.零序实部指令电流
	float izqimag;				//35.零序虚部指令电流
	
}RTDataType;
/*---------------------------------------------------
	设置参数
-----------------------------------------------------*/
typedef struct{
	s16 k1;						//电压前馈系数
	s16 kp;						//直压控制比例系数
	s16 ki;						//直压控制积分系数
	s16 kp2;					//均压控制比例系数
	s16 ki2;					//均压控制积分系数
	s16 kr;						//电流内环比例系数
	s16 prkr;					//谐振控制比例系数
	s16 prki;					//谐振控制积分系数
	s16	cwp;					//载波相移
	s16	th3;					//3次谐波补偿率
	s16 th5;					//5次谐波补偿率
	s16 th7;					//7次谐波补偿率
	s16 th9;					//9次谐波补偿率
	s16	th11;					//11次谐波补偿率
	s16 th13;					//13次谐波补偿率
	s16 th15;					//15次谐波补偿率
	s16	th17;					//17次谐波补偿率
	s16 th19;					//19次谐波补偿率 
	s16	th21;					//21次谐波补偿率
	s16	th23;					//23次谐波补偿率
	s16 th25;					//25次谐波补偿率
	s16 tha;					//偶次及26~50次谐波补偿率
	s16 cts;					//系统CT变比
	s16 ctl;					//负载CT变比
	s16 uov;					//过压门限
	s16 udn;					//欠压门限
	s16 unb;					//电压不平衡度保护范围
	s16 pir;					//额定电流
	s16 cov;					//过流门限
	s16 csc;					//速断门限
	s16 tov;					//温度门限
	s16 ctrlMode;				//控制模式
	s16 msMode;					//主从模式
	s16 sysId;					//系统ID
	s16 vkp;					//外环比例系数
	s16 vki;					//外环积分系数
	s16 tpf;					//目标功率因数
	s16 vacRef;					//交流参考电压
	s16 k2h;					//谐波电流增益
	s16 iqref;					//无功电流给定
	s16 inqreal;				//负序电流实部给定
	s16 inqimag;				//负序电流虚部给定
	s16 izqreal;				//零序电流实部给定
	s16 izqimag;				//零序电流虚部给定
	s16 punum;					//单元数量
	s16 sampMode;				//采样模式
}PrDataType;
/*------------------------------------------------------------------------
	校准系数
--------------------------------------------------------------------------*/
typedef struct{
	s16 ad1cor[8];
	s16 ad2cor[8]; 
	s16 ad3cor[8]; 
	s16 ad4cor[8];
}CrDataType;

/*-----------------------------------------------------------------------
	曲线数据
-------------------------------------------------------------------------*/
typedef struct{
	double nbil;
	double nbis;
	double nbus;
	double pl;
	double ps;
	double ql;
	double qs;
	double pfl;
	double pfs;
	double thdil[3];
	double thdis[3];
	double thdus[3];
	double ic[3];
	double il[3];
	double is[3];
	double us[3];
}traceDatType;



typedef struct{
	TimeTypeDef time;			//事件发生时刻
	u16 evtType;				//事件类型
	u32 devstate;				//设备整体故障
	u16 fpgaState[4];			//FPGA状态字
	u16 ioState;				//IO状态
}evtTypeStruct;

typedef struct{
	s16 prefval;				//正序电流有效值
	s16 qrefval;				//正序电流功率因数
	s16 nrefreal;				//负序电流实部
	s16 nrefimag;				//负序电流虚部
	s16 zrefreal;				//零序电流实部
	s16 zrefimag;				//零序电流虚部
	s16 downAmp;				//暂降幅值
	u16 downTime;				//暂降时间
	u16 flashType;				//闪变类型
	u16 flashFreq;				//闪变频率
	u16 flashTime;				//闪变时间
	s16 th2real;				//2次谐波实部
	s16 th2imag;				//2次谐波虚部
	s16 th3real;				//3次谐波实部
	s16 th3imag;				//3次谐波虚部
	s16 th4real;				//4次谐波实部
	s16 th4imag;				//4次谐波虚部
	s16 th5real;				//5次谐波实部
	s16 th5imag;				//5次谐波虚部
	s16 th6real;				//6次谐波实部
	s16 th6imag;				//6次谐波虚部
	s16 th7real;				//7次谐波实部
	s16 th7imag;				//7次谐波虚部
	s16 th8real;				//8次谐波实部
	s16 th8imag;				//8次谐波虚部
	s16 th9real;				//9次谐波实部
	s16 th9imag;				//9次谐波虚部
	s16 th10real;				//10次谐波实部
	s16 th10imag;				//10次谐波虚部
	s16 th11real;				//11次谐波实部
	s16 th11imag;				//11次谐波虚部
	s16 th12real;				//12次谐波实部
	s16 th12imag;				//12次谐波虚部
	s16 th13real;				//13次谐波实部
	s16 th13imag;				//13次谐波虚部
	s16 th14real;				//14次谐波实部
	s16 th14imag;				//14次谐波虚部
	s16 th15real;				//15次谐波实部
	s16 th15imag;				//15次谐波虚部
	s16 th16real;				//16次谐波实部
	s16 th16imag;				//16次谐波虚部
	s16 th17real;				//17次谐波实部
	s16 th17imag;				//17次谐波虚部
	s16 th18real;				//18次谐波实部
	s16 th18imag;				//18次谐波虚部
	s16 th19real;				//19次谐波实部
	s16 th19imag;				//19次谐波虚部
	s16 th20real;				//20次谐波实部
	s16 th20imag;				//20次谐波虚部
	s16 th21real;				//21次谐波实部
	s16 th21imag;				//21次谐波虚部
	s16 th22real;				//22次谐波实部
	s16 th22imag;				//22次谐波虚部
	s16 th23real;				//23次谐波实部
	s16 th23imag;				//23次谐波虚部
	s16 th24real;				//24次谐波实部
	s16 th24imag;				//24次谐波虚部
	s16 th25real;				//25次谐波实部
	s16 th25imag;				//25次谐波虚部
}__attribute__ ((aligned (1))) ELTSTDat;					//电子负载试验数据

#define ACDAT_LEN 33
#define DCDAT_LEN 22

typedef  struct {
    s16 havol[3];      /*端口1三相电压*/
    s16 hacur[3];      /*端口1三相电流*/
    s16 hap;           /*端口1有功功率*/
    s16 haq;           /*端口1无功功率*/
    s16 hapf;          /*端口1功率因数*/
    s16 hasw;          /*端口1开关状态*/
	
    s16 lavol[3];      /*端口1低压三相电压*/
    s16 lacur[3];      /*端口1低压三相电流*/
    s16 lap;           /*端口1低压有功功率*/
    s16 laq;           /*端口1低压无功功率*/
    s16 lapf;          /*端口1低压功率因数*/
    s16 lasw;          /*端口1低压侧开关状态*/
    s16 laudcp;        /*端口1上直流*/
    s16 laudcn;        /*端口1下直流*/
    s16 latemp;        /*端口1 IGBT温度*/
	
    s16 lawrksta;      /*端口1工作状态*/
    s16 laErrSta[4];   /*端口1状态字*/
    s16 laBw[4];       /*端口1备用状态*/
    s16 lafreq;        /*端口1频率*/
    s16 ladccur;       /*端口1直流侧电流*/

    s16 hbvol[3];      /*端口2 三相电压*/
    s16 hbcur[3];      /*端口2 三相电流*/
    s16 hbp;           /*端口2 有功功率*/
    s16 hbq;           /*端口2 无功功率*/
    s16 hbpf;          /*端口2 功率因数*/
    s16 hbsw;          /*端口2 开关状态*/
	
    s16 lbvol[3];      /*端口2 低压三相电压*/
    s16 lbcur[3];      /*端口2 低压三相电流*/
    s16 lbp;           /*端口2 低压有功功率*/
    s16 lbq;           /*端口2 低压无功功率*/
    s16 lbpf;          /*端口2 低压侧功率因数*/
    s16 lbsw;          /*端口2 低压侧开关状态*/
    
	s16 lbudcp;        /*端口2 上直流*/
    s16 lbudcn;        /*端口2 下直流*/
    s16 lbtemp;        /*端口2 IGBT温度*/
	
    s16 lbwrksta;      /*端口2 工作状态*/
    s16 lbErrSta[4];   /*端口2 状态字*/
    s16 lbBw[4];       /*端口2 备用状态*/
    s16 lbfreq;        /*端口2 频率*/
    s16 lbdccur;       /*端口2 直流侧电流*/
	
    s16 dc1udcp;       /*DC1 上直流*/
    s16 dc1udcn;       /*DC1 下直流*/
    s16 dc1InCur;      /*DC1 进线电流*/
    s16 dc1outVolA;    /*DC1 输出电压*/
    s16 dc1outCurA;    /*DC1 输出电流*/
    s16 dc1outPA;      /*DC1 输出功率*/
    s16 dc1outVolB;    /*DC1 输出电压*/
    s16 dc1outCurB;    /*DC1 输出电流*/
    s16 dc1outPB;      /*DC1 输出功率*/
    u16 dc1Temp[8];    /*DC1 IGBT温度*/
	
	
    u16 dc1wrkSta;     /*DC1 工作状态*/
    u16 dc1ErrSta[4];  /*DC1 故障状态字*/
	
    s16 dc2udcp;       /*DC2 上直流*/
    s16 dc2udcn;       /*DC2 下直流*/
    s16 dc2InCur;      /*DC2 进线电流*/
    s16 dc2outVolA;    /*DC2 输出电压*/
    s16 dc2outCurA;    /*DC2 输出电流*/
    s16 dc2outPA;      /*DC2 输出功率*/
    s16 dc2outVolB;    /*DC2 输出电压*/
    s16 dc2outCurB;    /*DC2 输出电流*/
    s16 dc2outPB;      /*DC2 输出功率*/
    u16 dc2Temp[8];    /*DC2 IGBT温度*/
	
	
    u16 dc2wrkSta;     /*DC2 工作状态*/
    u16 dc2ErrSta[4];  /*DC2 故障状态字*/
	
    s16 dc3udcp;       /*DC3 上直流*/
    s16 dc3udcn;       /*DC3 下直流*/
    s16 dc3InCur;      /*DC3 进线电流*/
    s16 dc3outVolA;    /*DC3 输出电压*/
    s16 dc3outCurA;    /*DC3 输出电流*/
    s16 dc3outPA;      /*DC3 输出功率*/
    s16 dc3outVolB;    /*DC3 输出电压*/
    s16 dc3outCurB;    /*DC3 输出电流*/
    s16 dc3outPB;      /*DC3 输出功率*/
    u16 dc3Temp[8];    /*DC3 IGBT温度*/
	
	
    u16 dc3wrkSta;     /*DC3 工作状态*/
    u16 dc3ErrSta[4];  /*DC3 故障状态字*/
	
    s16 dc4udcp;       /*DC4 上直流*/
    s16 dc4udcn;       /*DC4 下直流*/
    s16 dc4InCur;      /*DC4 进线电流*/
    s16 dc4outVolA;    /*DC4 输出电压*/
    s16 dc4outCurA;    /*DC4 输出电流*/
    s16 dc4outPA;      /*DC4 输出功率*/
    s16 dc4outVolB;    /*DC4 输出电压*/
    s16 dc4outCurB;    /*DC4 输出电流*/
    s16 dc4outPB;      /*DC4 输出功率*/
    u16 dc4Temp[8];    /*DC4 IGBT温度*/
	
    u16 dc4wrkSta;     /*DC4 工作状态*/
    u16 dc4ErrSta[4];  /*DC4 故障状态字 FB*/
	
	
    s16 stsvol[3];     /*STS 三相电压*/
    s16 stscur[3];     /*STS 三相电流*/
    s16 stsp;          /*STS 有功功率*/
    s16 stsq;          /*STS 无功功率*/
    s16 stspf;         /*STS 功率因数*/
    
	s16 stssw;         /*STS 开关状态*/  
    u16 stsTemp;       /*STS 温度*/
	
    u16 stswrkSta;     /*STS 工作状态 */
    u16 stsErrSta[4];  /*STS 状态字*/
	
    u16 fpgasta[4];    /*FPGA状态*/ 
									
    u16 dc1state[2];   /*DC1状态字*/
    u16 dc2state[2];   /*DC2状态字*/
    u16 dc3state[2];   /*DC3状态字*/
    u16 dc4state[2];   /*DC4状态字*/
    u16 ac1state[2];   /*AC1状态字*/
    u16 ac2state[2];   /*AC2状态字*/
	
    u16 extInA;        /*外部输入A*/
    u16 extInB;        /*外部输入B*/
    u16 extOutA;       /*外部输出A*/
    u16 extOutB;       /*外部输出B*/
	
    u16 dfCount;       /*通信测试字*/
    u16 rsTimer;       /*重启定时器*/
    u16 rsCnt;         /*重启计数器*/
    TimeTypeDef rtcDat;/*当前实时时钟*/
    devErr_t devErr;   /*设备故障*/
    u16 step;          /*运行状态*/
	u16 iostate[6];
	
	// ADD NEW Datatype
	u16 example_n[6];
	u16 example_r1[6];
	s8 example_r2[6];
	
}ART_DataType;




extern RTDataType rtdata;
extern PrDataType prdata;
extern CrDataType crdata;
extern traceDatType traceDat;
extern evtTypeStruct evtData; 

extern ELTSTDat elTstVal;
extern ART_DataType irt_data;

#define		EVT_START			0x01			/*启动事件*/
#define		EVT_STOP			0x02			/*停止事件*/
#define		EVT_EMRON			0x03			/*急停有效事件*/
#define		EVT_EMROFF			0x04			/*急停松开事件*/
#define		EVT_FAULT			0x05			/*故障事件*/

#define		CMD_READPRDATA		0x0001
#define		CMD_WRITEPRDATA		0x0002
#define		CMD_READCRDATA		0x0003
#define		CMD_WRITECRDATA		0x0004
#define		CMD_CLRFDONE		0x0005
#define		CMD_CLRFCANSEL		0x0006
#define		CMD_SRON			0x0007
#define		CMD_SROFF			0x0008
#define		CMD_SMON			0x0009
#define		CMD_SMOFF			0x000A
#define		CMD_WORKDONE		0x000B
#define		CMD_WORKCANSEL		0x000C
#define		CMD_EMRDONE			0x000D
#define		CMD_EMRCANSEL		0x000E
#define		CMD_CLREVENT		0x000F
#define		CMD_READEVENT		0x0010
#define		CMD_CHECKTIME		0x0011
#define		CMD_SELCTSYS		0x0012
#define		CMD_SELCTLOAD		0x0013
#define		CMD_QF3ON			0x0014
#define		CMD_QF3OFF			0x0015


#define		CMD_STSQF1ON		0x0016
#define		CMD_STSQF1OFF		0x0017
#define		CMD_STSQF2ON		0x001A
#define		CMD_STSQF2OFF		0x001B
#define		CMD_STSCUTON		0x001C
#define		CMD_STSCUTOFF		0x001D
#define		CMD_STSKM1ON		0x001E
#define		CMD_STSKM1OFF		0x001F


#define 	CMD_GETADATA		0x0030 // New DCF READ DATA 
#define 	CMD_SENDADATA		0x0031 // New DCF SEND DATA


//心跳指令
#define		CMD_HB				0x0100

#define		CMD_RECORDSTART		0x1000
#define		CMD_RECORDEND		0x1001

#define		CMD_AUTOSTART		0x0018
#define		CMD_AUTOSTOP		0x0019

#define		CMD_READREF			0x0022
#define		CMD_WRITEREF		0x0023

#endif
