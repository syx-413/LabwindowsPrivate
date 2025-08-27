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
	ʵʱʱ������
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
   ʵʱ����
-------------------------------------------------------*/
typedef struct{
	s16 u[3];					//0.����ϵͳ��ѹ
	s16 uz;						//1.�������
	s16 unb;					//2.�����ѹ��ƽ���
	s16 thdu[3];				//3.��ѹ�ܻ�����
	s16 is[4];					//4.ϵͳA/B/C/N����
	s16 isnb;					//5.ϵͳ������ƽ���
	s16 thdis[4];				//6.ϵͳ�����ܻ�����
	s16 il[4];					//7.����A/B/C/N����
	s16 ilnb;					//8.���ص�����ƽ���
	s16 thdil[4];				//9.���ص����ܻ�����
	s32 ifb[4];					//10.��������
	s32 ir[4];					//11.ָ�����
	s32 ps[4];					//12.ϵͳ�й�����
	s32 qs[4];					//13.ϵͳ�޹�����
	s16 pfs[4];					//14.ϵͳ��������
	s16 dpfs[4];				//15.λ�ƹ�������
	s32 pl[4];					//16.�����й�����
	s32 ql[4];					//17.�����޹�����
	s16 pfl[4];					//18.���ع�������
	s16 dpfl[4];				//19.����λ�ƹ�������
	s16 temp[4];				//20.IGBT�¶�
	s16 udc;					//21.��Ԫֱ����ѹ
	s16 udc2;					//22.�µ���ֱѹ
	u16 freq;					//22.Ƶ��
	u16 fpgaState[4];			//23.FPGA״̬��
	u16 resv;
	u32 devState;				//24.װ��״̬��
	u16 ioState;				//25.��������������
	TimeTypeDef time; 			//26.ʵʱʱ��
	u16 runState;				//27.��ǰ����״̬
	u16 debugCmd;				//28.��������
	u16 debugDat;				//29.��������
	float iqref;				//30.��Ҫ�������޹�����
	float iqfeed;				//31.ʵ��������޹�����
	float inqreal;				//32.����ʵ��ָ�����
	float inqimag;				//33.�����鲿ָ�����
	float izqreal;				//34.����ʵ��ָ�����
	float izqimag;				//35.�����鲿ָ�����
	
}RTDataType;
/*---------------------------------------------------
	���ò���
-----------------------------------------------------*/
typedef struct{
	s16 k1;						//��ѹǰ��ϵ��
	s16 kp;						//ֱѹ���Ʊ���ϵ��
	s16 ki;						//ֱѹ���ƻ���ϵ��
	s16 kp2;					//��ѹ���Ʊ���ϵ��
	s16 ki2;					//��ѹ���ƻ���ϵ��
	s16 kr;						//�����ڻ�����ϵ��
	s16 prkr;					//г����Ʊ���ϵ��
	s16 prki;					//г����ƻ���ϵ��
	s16	cwp;					//�ز�����
	s16	th3;					//3��г��������
	s16 th5;					//5��г��������
	s16 th7;					//7��г��������
	s16 th9;					//9��г��������
	s16	th11;					//11��г��������
	s16 th13;					//13��г��������
	s16 th15;					//15��г��������
	s16	th17;					//17��г��������
	s16 th19;					//19��г�������� 
	s16	th21;					//21��г��������
	s16	th23;					//23��г��������
	s16 th25;					//25��г��������
	s16 tha;					//ż�μ�26~50��г��������
	s16 cts;					//ϵͳCT���
	s16 ctl;					//����CT���
	s16 uov;					//��ѹ����
	s16 udn;					//Ƿѹ����
	s16 unb;					//��ѹ��ƽ��ȱ�����Χ
	s16 pir;					//�����
	s16 cov;					//��������
	s16 csc;					//�ٶ�����
	s16 tov;					//�¶�����
	s16 ctrlMode;				//����ģʽ
	s16 msMode;					//����ģʽ
	s16 sysId;					//ϵͳID
	s16 vkp;					//�⻷����ϵ��
	s16 vki;					//�⻷����ϵ��
	s16 tpf;					//Ŀ�깦������
	s16 vacRef;					//�����ο���ѹ
	s16 k2h;					//г����������
	s16 iqref;					//�޹���������
	s16 inqreal;				//�������ʵ������
	s16 inqimag;				//��������鲿����
	s16 izqreal;				//�������ʵ������
	s16 izqimag;				//��������鲿����
	s16 punum;					//��Ԫ����
	s16 sampMode;				//����ģʽ
}PrDataType;
/*------------------------------------------------------------------------
	У׼ϵ��
--------------------------------------------------------------------------*/
typedef struct{
	s16 ad1cor[8];
	s16 ad2cor[8]; 
	s16 ad3cor[8]; 
	s16 ad4cor[8];
}CrDataType;

/*-----------------------------------------------------------------------
	��������
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
	TimeTypeDef time;			//�¼�����ʱ��
	u16 evtType;				//�¼�����
	u32 devstate;				//�豸�������
	u16 fpgaState[4];			//FPGA״̬��
	u16 ioState;				//IO״̬
}evtTypeStruct;

typedef struct{
	s16 prefval;				//���������Чֵ
	s16 qrefval;				//���������������
	s16 nrefreal;				//�������ʵ��
	s16 nrefimag;				//��������鲿
	s16 zrefreal;				//�������ʵ��
	s16 zrefimag;				//��������鲿
	s16 downAmp;				//�ݽ���ֵ
	u16 downTime;				//�ݽ�ʱ��
	u16 flashType;				//��������
	u16 flashFreq;				//����Ƶ��
	u16 flashTime;				//����ʱ��
	s16 th2real;				//2��г��ʵ��
	s16 th2imag;				//2��г���鲿
	s16 th3real;				//3��г��ʵ��
	s16 th3imag;				//3��г���鲿
	s16 th4real;				//4��г��ʵ��
	s16 th4imag;				//4��г���鲿
	s16 th5real;				//5��г��ʵ��
	s16 th5imag;				//5��г���鲿
	s16 th6real;				//6��г��ʵ��
	s16 th6imag;				//6��г���鲿
	s16 th7real;				//7��г��ʵ��
	s16 th7imag;				//7��г���鲿
	s16 th8real;				//8��г��ʵ��
	s16 th8imag;				//8��г���鲿
	s16 th9real;				//9��г��ʵ��
	s16 th9imag;				//9��г���鲿
	s16 th10real;				//10��г��ʵ��
	s16 th10imag;				//10��г���鲿
	s16 th11real;				//11��г��ʵ��
	s16 th11imag;				//11��г���鲿
	s16 th12real;				//12��г��ʵ��
	s16 th12imag;				//12��г���鲿
	s16 th13real;				//13��г��ʵ��
	s16 th13imag;				//13��г���鲿
	s16 th14real;				//14��г��ʵ��
	s16 th14imag;				//14��г���鲿
	s16 th15real;				//15��г��ʵ��
	s16 th15imag;				//15��г���鲿
	s16 th16real;				//16��г��ʵ��
	s16 th16imag;				//16��г���鲿
	s16 th17real;				//17��г��ʵ��
	s16 th17imag;				//17��г���鲿
	s16 th18real;				//18��г��ʵ��
	s16 th18imag;				//18��г���鲿
	s16 th19real;				//19��г��ʵ��
	s16 th19imag;				//19��г���鲿
	s16 th20real;				//20��г��ʵ��
	s16 th20imag;				//20��г���鲿
	s16 th21real;				//21��г��ʵ��
	s16 th21imag;				//21��г���鲿
	s16 th22real;				//22��г��ʵ��
	s16 th22imag;				//22��г���鲿
	s16 th23real;				//23��г��ʵ��
	s16 th23imag;				//23��г���鲿
	s16 th24real;				//24��г��ʵ��
	s16 th24imag;				//24��г���鲿
	s16 th25real;				//25��г��ʵ��
	s16 th25imag;				//25��г���鲿
}__attribute__ ((aligned (1))) ELTSTDat;					//���Ӹ�����������

#define ACDAT_LEN 33
#define DCDAT_LEN 22

typedef  struct {
    s16 havol[3];      /*�˿�1�����ѹ*/
    s16 hacur[3];      /*�˿�1�������*/
    s16 hap;           /*�˿�1�й�����*/
    s16 haq;           /*�˿�1�޹�����*/
    s16 hapf;          /*�˿�1��������*/
    s16 hasw;          /*�˿�1����״̬*/
	
    s16 lavol[3];      /*�˿�1��ѹ�����ѹ*/
    s16 lacur[3];      /*�˿�1��ѹ�������*/
    s16 lap;           /*�˿�1��ѹ�й�����*/
    s16 laq;           /*�˿�1��ѹ�޹�����*/
    s16 lapf;          /*�˿�1��ѹ��������*/
    s16 lasw;          /*�˿�1��ѹ�࿪��״̬*/
    s16 laudcp;        /*�˿�1��ֱ��*/
    s16 laudcn;        /*�˿�1��ֱ��*/
    s16 latemp;        /*�˿�1 IGBT�¶�*/
	
    s16 lawrksta;      /*�˿�1����״̬*/
    s16 laErrSta[4];   /*�˿�1״̬��*/
    s16 laBw[4];       /*�˿�1����״̬*/
    s16 lafreq;        /*�˿�1Ƶ��*/
    s16 ladccur;       /*�˿�1ֱ�������*/

    s16 hbvol[3];      /*�˿�2 �����ѹ*/
    s16 hbcur[3];      /*�˿�2 �������*/
    s16 hbp;           /*�˿�2 �й�����*/
    s16 hbq;           /*�˿�2 �޹�����*/
    s16 hbpf;          /*�˿�2 ��������*/
    s16 hbsw;          /*�˿�2 ����״̬*/
	
    s16 lbvol[3];      /*�˿�2 ��ѹ�����ѹ*/
    s16 lbcur[3];      /*�˿�2 ��ѹ�������*/
    s16 lbp;           /*�˿�2 ��ѹ�й�����*/
    s16 lbq;           /*�˿�2 ��ѹ�޹�����*/
    s16 lbpf;          /*�˿�2 ��ѹ�๦������*/
    s16 lbsw;          /*�˿�2 ��ѹ�࿪��״̬*/
    
	s16 lbudcp;        /*�˿�2 ��ֱ��*/
    s16 lbudcn;        /*�˿�2 ��ֱ��*/
    s16 lbtemp;        /*�˿�2 IGBT�¶�*/
	
    s16 lbwrksta;      /*�˿�2 ����״̬*/
    s16 lbErrSta[4];   /*�˿�2 ״̬��*/
    s16 lbBw[4];       /*�˿�2 ����״̬*/
    s16 lbfreq;        /*�˿�2 Ƶ��*/
    s16 lbdccur;       /*�˿�2 ֱ�������*/
	
    s16 dc1udcp;       /*DC1 ��ֱ��*/
    s16 dc1udcn;       /*DC1 ��ֱ��*/
    s16 dc1InCur;      /*DC1 ���ߵ���*/
    s16 dc1outVolA;    /*DC1 �����ѹ*/
    s16 dc1outCurA;    /*DC1 �������*/
    s16 dc1outPA;      /*DC1 �������*/
    s16 dc1outVolB;    /*DC1 �����ѹ*/
    s16 dc1outCurB;    /*DC1 �������*/
    s16 dc1outPB;      /*DC1 �������*/
    u16 dc1Temp[8];    /*DC1 IGBT�¶�*/
	
	
    u16 dc1wrkSta;     /*DC1 ����״̬*/
    u16 dc1ErrSta[4];  /*DC1 ����״̬��*/
	
    s16 dc2udcp;       /*DC2 ��ֱ��*/
    s16 dc2udcn;       /*DC2 ��ֱ��*/
    s16 dc2InCur;      /*DC2 ���ߵ���*/
    s16 dc2outVolA;    /*DC2 �����ѹ*/
    s16 dc2outCurA;    /*DC2 �������*/
    s16 dc2outPA;      /*DC2 �������*/
    s16 dc2outVolB;    /*DC2 �����ѹ*/
    s16 dc2outCurB;    /*DC2 �������*/
    s16 dc2outPB;      /*DC2 �������*/
    u16 dc2Temp[8];    /*DC2 IGBT�¶�*/
	
	
    u16 dc2wrkSta;     /*DC2 ����״̬*/
    u16 dc2ErrSta[4];  /*DC2 ����״̬��*/
	
    s16 dc3udcp;       /*DC3 ��ֱ��*/
    s16 dc3udcn;       /*DC3 ��ֱ��*/
    s16 dc3InCur;      /*DC3 ���ߵ���*/
    s16 dc3outVolA;    /*DC3 �����ѹ*/
    s16 dc3outCurA;    /*DC3 �������*/
    s16 dc3outPA;      /*DC3 �������*/
    s16 dc3outVolB;    /*DC3 �����ѹ*/
    s16 dc3outCurB;    /*DC3 �������*/
    s16 dc3outPB;      /*DC3 �������*/
    u16 dc3Temp[8];    /*DC3 IGBT�¶�*/
	
	
    u16 dc3wrkSta;     /*DC3 ����״̬*/
    u16 dc3ErrSta[4];  /*DC3 ����״̬��*/
	
    s16 dc4udcp;       /*DC4 ��ֱ��*/
    s16 dc4udcn;       /*DC4 ��ֱ��*/
    s16 dc4InCur;      /*DC4 ���ߵ���*/
    s16 dc4outVolA;    /*DC4 �����ѹ*/
    s16 dc4outCurA;    /*DC4 �������*/
    s16 dc4outPA;      /*DC4 �������*/
    s16 dc4outVolB;    /*DC4 �����ѹ*/
    s16 dc4outCurB;    /*DC4 �������*/
    s16 dc4outPB;      /*DC4 �������*/
    u16 dc4Temp[8];    /*DC4 IGBT�¶�*/
	
    u16 dc4wrkSta;     /*DC4 ����״̬*/
    u16 dc4ErrSta[4];  /*DC4 ����״̬�� FB*/
	
	
    s16 stsvol[3];     /*STS �����ѹ*/
    s16 stscur[3];     /*STS �������*/
    s16 stsp;          /*STS �й�����*/
    s16 stsq;          /*STS �޹�����*/
    s16 stspf;         /*STS ��������*/
    
	s16 stssw;         /*STS ����״̬*/  
    u16 stsTemp;       /*STS �¶�*/
	
    u16 stswrkSta;     /*STS ����״̬ */
    u16 stsErrSta[4];  /*STS ״̬��*/
	
    u16 fpgasta[4];    /*FPGA״̬*/ 
									
    u16 dc1state[2];   /*DC1״̬��*/
    u16 dc2state[2];   /*DC2״̬��*/
    u16 dc3state[2];   /*DC3״̬��*/
    u16 dc4state[2];   /*DC4״̬��*/
    u16 ac1state[2];   /*AC1״̬��*/
    u16 ac2state[2];   /*AC2״̬��*/
	
    u16 extInA;        /*�ⲿ����A*/
    u16 extInB;        /*�ⲿ����B*/
    u16 extOutA;       /*�ⲿ���A*/
    u16 extOutB;       /*�ⲿ���B*/
	
    u16 dfCount;       /*ͨ�Ų�����*/
    u16 rsTimer;       /*������ʱ��*/
    u16 rsCnt;         /*����������*/
    TimeTypeDef rtcDat;/*��ǰʵʱʱ��*/
    devErr_t devErr;   /*�豸����*/
    u16 step;          /*����״̬*/
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

#define		EVT_START			0x01			/*�����¼�*/
#define		EVT_STOP			0x02			/*ֹͣ�¼�*/
#define		EVT_EMRON			0x03			/*��ͣ��Ч�¼�*/
#define		EVT_EMROFF			0x04			/*��ͣ�ɿ��¼�*/
#define		EVT_FAULT			0x05			/*�����¼�*/

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


//����ָ��
#define		CMD_HB				0x0100

#define		CMD_RECORDSTART		0x1000
#define		CMD_RECORDEND		0x1001

#define		CMD_AUTOSTART		0x0018
#define		CMD_AUTOSTOP		0x0019

#define		CMD_READREF			0x0022
#define		CMD_WRITEREF		0x0023

#endif
