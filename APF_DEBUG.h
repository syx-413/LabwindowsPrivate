/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2025. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_TAB                        2       /* control type: tab, callback function: (none) */
#define  PANEL_BUTTON_CLOSE               3       /* control type: command, callback function: exitSysCB */
#define  PANEL_TEXTMSG                    4       /* control type: textMsg, callback function: (none) */
#define  PANEL_LED_ONLINE                 5       /* control type: LED, callback function: (none) */
#define  PANEL_LED_RUN                    6       /* control type: LED, callback function: (none) */
#define  PANEL_LED_3                      7       /* control type: LED, callback function: (none) */
#define  PANEL_LED_2                      8       /* control type: LED, callback function: (none) */
#define  PANEL_LED                        9       /* control type: LED, callback function: (none) */
#define  PANEL_DECORATION                 10      /* control type: deco, callback function: (none) */
#define  PANEL_BINARYSWITCH               11      /* control type: binary, callback function: CB_STARTSTOP */
#define  PANEL_CHECKBOX                   12      /* control type: radioButton, callback function: recordCheckCB */
#define  PANEL_SYSTIMEDISPLAY             13      /* control type: string, callback function: (none) */
#define  PANEL_COMMANDBUTTON              14      /* control type: command, callback function: (none) */
#define  PANEL_TEXTMSG_3                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TMRCHECKCON                16      /* control type: timer, callback function: CheckConnectCB */
#define  PANEL_TIMERGETTIME               17      /* control type: timer, callback function: CBGetSysTime */
#define  PANEL_DEBUGSTATE                 18      /* control type: numeric, callback function: (none) */
#define  PANEL_RING_DEV                   19      /* control type: ring, callback function: SEL_DEVCB */

#define  PANEL_A                          2
#define  PANEL_A_btn_Autostop             2       /* control type: command, callback function: AutoStop */
#define  PANEL_A_btn_Autostart            3       /* control type: command, callback function: AutoStart */
#define  PANEL_A_BTN_SCOPE                4       /* control type: command, callback function: Scope */
#define  PANEL_A_btn_errorSheet           5       /* control type: command, callback function: EnventLog */
#define  PANEL_A_LED_RUN                  6       /* control type: LED, callback function: (none) */
#define  PANEL_A_BTN_DEBUG                7       /* control type: command, callback function: iDebugPanel */
#define  PANEL_A_BTN_SYSInfo              8       /* control type: command, callback function: DeviceInfo */
#define  PANEL_A_BTN_CONF                 9       /* control type: command, callback function: SysConf */
#define  PANEL_A_COMMANDBUTTON            10      /* control type: command, callback function: quit */
#define  PANEL_A_Num_InPort_IC            11      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_IB            12      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_T             13      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_Q1            14      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_PF1           15      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_PanelID              16      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_P1            17      /* control type: numeric, callback function: (none) */
#define  PANEL_A_SYSTIMEDISPLAY           18      /* control type: string, callback function: (none) */
#define  PANEL_A_Num_InPort_IA            19      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_VC            20      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IOut             21      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IOut             22      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IC_IN            23      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IB_IN            24      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_PF_IN            25      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_Q_IN             26      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_P_IN             27      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IA_IN            28      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IC_IN            29      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IB_IN            30      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_PF_IN            31      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_Q_IN             32      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_P_IN             33      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IA_IN            34      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IC               35      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IB               36      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_PF               37      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_Q                38      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_P                39      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_IA               40      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_Temp             41      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_Temp             42      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_Temp             43      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_PA               44      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_Temp             45      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_PA               46      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_PA               47      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_PB               48      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_PB               49      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_PB               50      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_PB               51      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_PA               52      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_TempC            53      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_Freq             54      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_Udcn             55      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_Udcn             56      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_Udcn             57      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_Udcn             58      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_Udcn             59      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_VBOUT            60      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_VAOUT            61      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_CurBOUT          62      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_CurAOUT          63      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_CurIN            64      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_VBOUT            65      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_VAOUT            66      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_CurBOUT          67      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_CurAOUT          68      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_CurIN            69      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_VBOUT            70      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_VAOUT            71      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_CurBOUT          72      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_CurAOUT          73      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_CurIN            74      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_VBOUT            75      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_VAOUT            76      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC4_Udcp             77      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_CurBOUT          78      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC3_Udcp             79      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_CurAOUT          80      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC2_Udcp             81      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_CurIN            82      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_DC1_Udcp             83      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_Udcp             84      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_TempC            85      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VC_IN            86      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VB_IN            87      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VA_IN            88      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_Freq             89      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_Udcn             90      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_Udcp             91      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_PF               92      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_Q                93      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_P                94      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VC_IN            95      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VB_IN            96      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VA_IN            97      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IC               98      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IB               99      /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_IA               100     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VC               101     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VB               102     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC2_VA               103     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VC               104     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VB               105     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_AC1_VA               106     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_VB            107     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Num_InPort_VA            108     /* control type: numeric, callback function: (none) */
#define  PANEL_A_Led_KV_K1FB              109     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_K2FB             110     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_K2               111     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4OutQF2FB          112     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4OutQF2            113     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4OutQF1FB          114     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4OutQF1            115     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3OutQF1FB          116     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3OutQF1            117     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2OutQF1FB          118     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2OutQF1            119     /* control type: LED, callback function: (none) */
#define  PANEL_A_LED_18                   120     /* control type: LED, callback function: (none) */
#define  PANEL_A_LED_17                   121     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_KV_K2FB              122     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_KV_K2                123     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC2_K1FB             124     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC2_K1               125     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4InQF1FB           126     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4InQF1             127     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3InQF1FB           128     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3InQF1             129     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2InQF1FB           130     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2InQF1             131     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1OutQF1FB          132     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1OutQF1            133     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC2_Error            134     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC2_WORK             135     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4_Error            136     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC4_WORK             137     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3_Error            138     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC3_WORK             139     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2_Error            140     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC2_WORK             141     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1_Error            142     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1_WORK             143     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1InQF1FB           144     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_DC1InQF1             145     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_Error            146     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_WORK             147     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC1_Error            148     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC1_WORK             149     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC1_K1FB             150     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_AC1_K1               151     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_K1FB             152     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_STS_K1               153     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_IN_K2FB              154     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_KV_K1                155     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_IN_K2                156     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_devErr               157     /* control type: LED, callback function: (none) */
#define  PANEL_A_LED_MRun                 158     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_IN_K1                159     /* control type: LED, callback function: (none) */
#define  PANEL_A_Led_IN_K1FB              160     /* control type: LED, callback function: (none) */
#define  PANEL_A_text1                    161     /* control type: textMsg, callback function: (none) */
#define  PANEL_A_TIMERGETTIME             162     /* control type: timer, callback function: STSQFCB */
#define  PANEL_A_TIMERGETTIME_2           163     /* control type: timer, callback function: AC1WorkCB */
#define  PANEL_A_TIMERGETTIME_3           164     /* control type: timer, callback function: AC2WorkCB */
#define  PANEL_A_TIMERGETTIME_4           165     /* control type: timer, callback function: DC1WorkCB */
#define  PANEL_A_TEXTMSG_3                166     /* control type: textMsg, callback function: (none) */
#define  PANEL_A_Led_DC4_OQF2             167     /* control type: picture, callback function: (none) */

#define  PANEL_DCF                        3
#define  PANEL_DCF_LED_RUN                2       /* control type: LED, callback function: (none) */
#define  PANEL_DCF_LED_2                  3       /* control type: LED, callback function: (none) */
#define  PANEL_DCF_LED_1                  4       /* control type: LED, callback function: (none) */
#define  PANEL_DCF_CMDWORKCANSEL_3        5       /* control type: command, callback function: CloseSysConf */
#define  PANEL_DCF_CMDWORKCANSEL_6        6       /* control type: command, callback function: SetSysDatCB */
#define  PANEL_DCF_COMMANDBUTTON_2        7       /* control type: command, callback function: GetID */
#define  PANEL_DCF_CMDWORKDONE_4          8       /* control type: command, callback function: GetSysPrDataCB */
#define  PANEL_DCF_CMDWORKCANSEL_7        9       /* control type: command, callback function: InitSysLdDatCB */
#define  PANEL_DCF_DECORATION_2           10      /* control type: deco, callback function: (none) */
#define  PANEL_DCF_NUMERIC_DC4_Cap        11      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_NUMERIC_DC3_Cap        12      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_NUMERIC_AC2_Cap        13      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_NUMERIC_DC2_Cap        14      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_Num_PanelID            15      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_NUMERIC_DC1_Cap        16      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_NUMERIC_AC1_Cap        17      /* control type: numeric, callback function: (none) */
#define  PANEL_DCF_RING_DC4_1             18      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC3_1             19      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_AC2_1             20      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC2_1             21      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC1_1             22      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_AC1_1             23      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_SPLITTER_2             24      /* control type: splitter, callback function: (none) */
#define  PANEL_DCF_SPLITTER_4             25      /* control type: splitter, callback function: (none) */
#define  PANEL_DCF_SPLITTER_3             26      /* control type: splitter, callback function: (none) */
#define  PANEL_DCF_SPLITTER               27      /* control type: splitter, callback function: (none) */
#define  PANEL_DCF_DECORATION             28      /* control type: deco, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_2              29      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_3              30      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_4              31      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_6              32      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_5              33      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_DECORATION_3           34      /* control type: deco, callback function: (none) */
#define  PANEL_DCF_RING_AC2_2             35      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC1_2             36      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_AC1_2             37      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC2_2             38      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_RING_DC4_2             39      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_TEXTMSG                40      /* control type: textMsg, callback function: (none) */
#define  PANEL_DCF_DECORATION_4           41      /* control type: deco, callback function: (none) */
#define  PANEL_DCF_RING_DC3_2             42      /* control type: ring, callback function: (none) */
#define  PANEL_DCF_TEXTMSG_7              43      /* control type: textMsg, callback function: (none) */

#define  PANEL_Scop                       4
#define  PANEL_Scop_RINGCH1SEL_2          2       /* control type: ring, callback function: (none) */
#define  PANEL_Scop_COMMANDBUTTON_2       3       /* control type: command, callback function: iGetID */
#define  PANEL_Scop_COMMANDBUTTON         4       /* control type: command, callback function: QuitWave */
#define  PANEL_Scop_SHIFT_2               5       /* control type: scale, callback function: ShiftFuncy */
#define  PANEL_Scop_LED_RUN               6       /* control type: LED, callback function: (none) */
#define  PANEL_Scop_SHIFT                 7       /* control type: scale, callback function: ShiftFuncx */
#define  PANEL_Scop_CHECKBOXCH_ScopeFD_2  8       /* control type: radioButton, callback function: iscopeDisTypeCB */
#define  PANEL_Scop_Num_PanelID           9       /* control type: numeric, callback function: (none) */
#define  PANEL_Scop_GRAPH                 10      /* control type: graph, callback function: GRAPHCB */
#define  PANEL_Scop_RING_3Down            11      /* control type: ring, callback function: (none) */
#define  PANEL_Scop_RING_CHup             12      /* control type: ring, callback function: (none) */
#define  PANEL_Scop_RINGSLIDE_2           13      /* control type: slide, callback function: Timescales2 */
#define  PANEL_Scop_RINGSLIDE             14      /* control type: slide, callback function: (none) */
#define  PANEL_Scop_COMMANDBUTTON_4       15      /* control type: command, callback function: (none) */
#define  PANEL_Scop_COMMANDBUTTON_3       16      /* control type: command, callback function: RestScopeZ1 */
#define  PANEL_Scop_TEXTMSG               17      /* control type: textMsg, callback function: (none) */
#define  PANEL_Scop_TIMER                 18      /* control type: timer, callback function: (none) */
#define  PANEL_Scop_STRIPCHART            19      /* control type: strip, callback function: (none) */
#define  PANEL_Scop_TIMER_2               20      /* control type: timer, callback function: (none) */
#define  PANEL_Scop_TOGGLEBUTTON          21      /* control type: textButton, callback function: TogglePauseCB */

#define  PANEL_SW                         5
#define  PANEL_SW_CMDSTSKM1_OFF           2       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_CMDSTSKM1_ON            3       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_TEXTMSG_10              4       /* control type: textMsg, callback function: (none) */
#define  PANEL_SW_CMDSTSCUT_OFF           5       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_CMDSTSCUT_ON            6       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_TEXTMSG_9               7       /* control type: textMsg, callback function: (none) */
#define  PANEL_SW_CMDSTSQF2_OFF           8       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_CMDSTSQF2_ON            9       /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_TEXTMSG_8               10      /* control type: textMsg, callback function: (none) */
#define  PANEL_SW_CMDSTSQF1_OFF           11      /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_CMDSTSQF1_ON            12      /* control type: command, callback function: STSSW_CB */
#define  PANEL_SW_TEXTMSG_7               13      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  CurRef_TH25ImagRef               2       /* control type: numeric, callback function: (none) */
#define  CurRef_TH25RealRef               3       /* control type: numeric, callback function: (none) */
#define  CurRef_TH24ImagRef               4       /* control type: numeric, callback function: (none) */
#define  CurRef_TH24RealRef               5       /* control type: numeric, callback function: (none) */
#define  CurRef_TH23ImagRef               6       /* control type: numeric, callback function: (none) */
#define  CurRef_TH23RealRef               7       /* control type: numeric, callback function: (none) */
#define  CurRef_TH22ImagRef               8       /* control type: numeric, callback function: (none) */
#define  CurRef_TH22RealRef               9       /* control type: numeric, callback function: (none) */
#define  CurRef_TH21ImagRef               10      /* control type: numeric, callback function: (none) */
#define  CurRef_TH21RealRef               11      /* control type: numeric, callback function: (none) */
#define  CurRef_TH20ImagRef               12      /* control type: numeric, callback function: (none) */
#define  CurRef_TH20RealRef               13      /* control type: numeric, callback function: (none) */
#define  CurRef_TH19ImagRef               14      /* control type: numeric, callback function: (none) */
#define  CurRef_TH19RealRef               15      /* control type: numeric, callback function: (none) */
#define  CurRef_TH18ImagRef               16      /* control type: numeric, callback function: (none) */
#define  CurRef_TH18RealRef               17      /* control type: numeric, callback function: (none) */
#define  CurRef_TH17ImagRef               18      /* control type: numeric, callback function: (none) */
#define  CurRef_TH17RealRef               19      /* control type: numeric, callback function: (none) */
#define  CurRef_TH16ImagRef               20      /* control type: numeric, callback function: (none) */
#define  CurRef_TH16RealRef               21      /* control type: numeric, callback function: (none) */
#define  CurRef_TH15ImagRef               22      /* control type: numeric, callback function: (none) */
#define  CurRef_TH15RealRef               23      /* control type: numeric, callback function: (none) */
#define  CurRef_TH14ImagRef               24      /* control type: numeric, callback function: (none) */
#define  CurRef_TH14RealRef               25      /* control type: numeric, callback function: (none) */
#define  CurRef_TH13ImagRef               26      /* control type: numeric, callback function: (none) */
#define  CurRef_TH13RealRef               27      /* control type: numeric, callback function: (none) */
#define  CurRef_TH12ImagRef               28      /* control type: numeric, callback function: (none) */
#define  CurRef_TH12RealRef               29      /* control type: numeric, callback function: (none) */
#define  CurRef_TH11ImagRef               30      /* control type: numeric, callback function: (none) */
#define  CurRef_TH11RealRef               31      /* control type: numeric, callback function: (none) */
#define  CurRef_TH10ImagRef               32      /* control type: numeric, callback function: (none) */
#define  CurRef_TH10RealRef               33      /* control type: numeric, callback function: (none) */
#define  CurRef_TH9ImagRef                34      /* control type: numeric, callback function: (none) */
#define  CurRef_TH9RealRef                35      /* control type: numeric, callback function: (none) */
#define  CurRef_TH8ImagRef                36      /* control type: numeric, callback function: (none) */
#define  CurRef_TH8RealRef                37      /* control type: numeric, callback function: (none) */
#define  CurRef_TH7ImagRef                38      /* control type: numeric, callback function: (none) */
#define  CurRef_TH7RealRef                39      /* control type: numeric, callback function: (none) */
#define  CurRef_TH6ImagRef                40      /* control type: numeric, callback function: (none) */
#define  CurRef_TH6RealRef                41      /* control type: numeric, callback function: (none) */
#define  CurRef_TH5ImagRef                42      /* control type: numeric, callback function: (none) */
#define  CurRef_TH5RealRef                43      /* control type: numeric, callback function: (none) */
#define  CurRef_TH4ImagRef                44      /* control type: numeric, callback function: (none) */
#define  CurRef_TH4RealRef                45      /* control type: numeric, callback function: (none) */
#define  CurRef_TH3ImagRef                46      /* control type: numeric, callback function: (none) */
#define  CurRef_TH3RealRef                47      /* control type: numeric, callback function: (none) */
#define  CurRef_TH2ImagRef                48      /* control type: numeric, callback function: (none) */
#define  CurRef_TH2RealRef                49      /* control type: numeric, callback function: (none) */
#define  CurRef_IZImagRef                 50      /* control type: numeric, callback function: (none) */
#define  CurRef_IZRealRef                 51      /* control type: numeric, callback function: (none) */
#define  CurRef_INImagRef                 52      /* control type: numeric, callback function: (none) */
#define  CurRef_INRealRef                 53      /* control type: numeric, callback function: (none) */
#define  CurRef_IBasePfRef                54      /* control type: numeric, callback function: (none) */
#define  CurRef_TH25ImagCur               55      /* control type: numeric, callback function: (none) */
#define  CurRef_TH25RealCur               56      /* control type: numeric, callback function: (none) */
#define  CurRef_TH24ImagCur               57      /* control type: numeric, callback function: (none) */
#define  CurRef_TH24RealCur               58      /* control type: numeric, callback function: (none) */
#define  CurRef_TH23ImagCur               59      /* control type: numeric, callback function: (none) */
#define  CurRef_TH23RealCur               60      /* control type: numeric, callback function: (none) */
#define  CurRef_TH22ImagCur               61      /* control type: numeric, callback function: (none) */
#define  CurRef_TH22RealCur               62      /* control type: numeric, callback function: (none) */
#define  CurRef_TH21ImagCur               63      /* control type: numeric, callback function: (none) */
#define  CurRef_TH21RealCur               64      /* control type: numeric, callback function: (none) */
#define  CurRef_TH20ImagCur               65      /* control type: numeric, callback function: (none) */
#define  CurRef_TH20RealCur               66      /* control type: numeric, callback function: (none) */
#define  CurRef_TH19ImagCur               67      /* control type: numeric, callback function: (none) */
#define  CurRef_TH19RealCur               68      /* control type: numeric, callback function: (none) */
#define  CurRef_TH18ImagCur               69      /* control type: numeric, callback function: (none) */
#define  CurRef_TH18RealCur               70      /* control type: numeric, callback function: (none) */
#define  CurRef_TH17ImagCur               71      /* control type: numeric, callback function: (none) */
#define  CurRef_TH17RealCur               72      /* control type: numeric, callback function: (none) */
#define  CurRef_IZImagCur                 73      /* control type: numeric, callback function: (none) */
#define  CurRef_IZRealCur                 74      /* control type: numeric, callback function: (none) */
#define  CurRef_TH16ImagCur               75      /* control type: numeric, callback function: (none) */
#define  CurRef_TH16RealCur               76      /* control type: numeric, callback function: (none) */
#define  CurRef_TH15ImagCur               77      /* control type: numeric, callback function: (none) */
#define  CurRef_TH15RealCur               78      /* control type: numeric, callback function: (none) */
#define  CurRef_TH14ImagCur               79      /* control type: numeric, callback function: (none) */
#define  CurRef_TH14RealCur               80      /* control type: numeric, callback function: (none) */
#define  CurRef_TH13ImagCur               81      /* control type: numeric, callback function: (none) */
#define  CurRef_TH13RealCur               82      /* control type: numeric, callback function: (none) */
#define  CurRef_TH12ImagCur               83      /* control type: numeric, callback function: (none) */
#define  CurRef_TH12RealCur               84      /* control type: numeric, callback function: (none) */
#define  CurRef_TH11ImagCur               85      /* control type: numeric, callback function: (none) */
#define  CurRef_TH11RealCur               86      /* control type: numeric, callback function: (none) */
#define  CurRef_TH10ImagCur               87      /* control type: numeric, callback function: (none) */
#define  CurRef_TH10RealCur               88      /* control type: numeric, callback function: (none) */
#define  CurRef_TH9ImagCur                89      /* control type: numeric, callback function: (none) */
#define  CurRef_TH9RealCur                90      /* control type: numeric, callback function: (none) */
#define  CurRef_INImagCur                 91      /* control type: numeric, callback function: (none) */
#define  CurRef_INRealCur                 92      /* control type: numeric, callback function: (none) */
#define  CurRef_TH8ImagCur                93      /* control type: numeric, callback function: (none) */
#define  CurRef_TH8RealCur                94      /* control type: numeric, callback function: (none) */
#define  CurRef_TH7ImagCur                95      /* control type: numeric, callback function: (none) */
#define  CurRef_TH7RealCur                96      /* control type: numeric, callback function: (none) */
#define  CurRef_TH6ImagCur                97      /* control type: numeric, callback function: (none) */
#define  CurRef_TH6RealCur                98      /* control type: numeric, callback function: (none) */
#define  CurRef_TH5ImagCur                99      /* control type: numeric, callback function: (none) */
#define  CurRef_TH5RealCur                100     /* control type: numeric, callback function: (none) */
#define  CurRef_TH4ImagCur                101     /* control type: numeric, callback function: (none) */
#define  CurRef_TH4RealCur                102     /* control type: numeric, callback function: (none) */
#define  CurRef_TH3ImagCur                103     /* control type: numeric, callback function: (none) */
#define  CurRef_TH3RealCur                104     /* control type: numeric, callback function: (none) */
#define  CurRef_TH2ImagCur                105     /* control type: numeric, callback function: (none) */
#define  CurRef_TH2RealCur                106     /* control type: numeric, callback function: (none) */
#define  CurRef_IBasePfCur                107     /* control type: numeric, callback function: (none) */
#define  CurRef_IBaseAmpCur               108     /* control type: numeric, callback function: (none) */
#define  CurRef_IBaseAmpRef               109     /* control type: numeric, callback function: (none) */
#define  CurRef_SPLITTER_6                110     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_5                111     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_3                112     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER                  113     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_8                114     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_10               115     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_9                116     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_7                117     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_4                118     /* control type: splitter, callback function: (none) */
#define  CurRef_SPLITTER_2                119     /* control type: splitter, callback function: (none) */
#define  CurRef_TEXTMSG_5                 120     /* control type: textMsg, callback function: (none) */
#define  CurRef_TEXTMSG_3                 121     /* control type: textMsg, callback function: (none) */
#define  CurRef_TEXTMSG                   122     /* control type: textMsg, callback function: (none) */
#define  CurRef_TEXTMSG_2                 123     /* control type: textMsg, callback function: (none) */
#define  CurRef_TEXTMSG_4                 124     /* control type: textMsg, callback function: (none) */
#define  CurRef_CMB_REFWrite              125     /* control type: command, callback function: CMB_REFWriteCB */
#define  CurRef_CMB_REFRead               126     /* control type: command, callback function: CMB_REFReadCB */
#define  CurRef_TEXTMSG_6                 127     /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_TABLEGRID               2       /* control type: table, callback function: (none) */
#define  TABPANEL_TABLEAPF                3       /* control type: table, callback function: (none) */
#define  TABPANEL_TABLEBIT                4       /* control type: table, callback function: (none) */
#define  TABPANEL_CMDEMRCANSEL_2          5       /* control type: command, callback function: CmdSelCanselCB */
#define  TABPANEL_CMDEMRCANSEL            6       /* control type: command, callback function: CmdEmrCanselCB */
#define  TABPANEL_CMDEMRDONE_4            7       /* control type: command, callback function: CmdQF3CanselCB */
#define  TABPANEL_CMDEMRDONE_3            8       /* control type: command, callback function: CmdQF3DoneCB */
#define  TABPANEL_CMDEMRDONE_2            9       /* control type: command, callback function: CmdSelDoneCB */
#define  TABPANEL_CMDEMRDONE              10      /* control type: command, callback function: CmdEmrDoneCB */
#define  TABPANEL_CMDWORKCANSEL           11      /* control type: command, callback function: CmdWorkCanselCB */
#define  TABPANEL_CMDWORKDONE             12      /* control type: command, callback function: CmdWorkDoneCB */
#define  TABPANEL_CMDSMCANSEL             13      /* control type: command, callback function: CmdSMCanselCB */
#define  TABPANEL_CMDSMDONE               14      /* control type: command, callback function: CmdSMDoneCB */
#define  TABPANEL_CMDSRCANSEL             15      /* control type: command, callback function: CmdSrCanselCB */
#define  TABPANEL_CMDSRDONE               16      /* control type: command, callback function: CmdSRDoneCB */
#define  TABPANEL_CMDCLRCANSEL            17      /* control type: command, callback function: CmdClrCanselCB */
#define  TABPANEL_CMDCLRDONE              18      /* control type: command, callback function: CmdClrDoneCB */
#define  TABPANEL_TEXTMSG_2               19      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_3               20      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG                 21      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_4               22      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_7               23      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_5               24      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_6               25      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_2_TABLECR               2       /* control type: table, callback function: (none) */
#define  TABPANEL_2_TABLEPR               3       /* control type: table, callback function: (none) */
#define  TABPANEL_2_CBTLCR                4       /* control type: command, callback function: ldDfCrDatCB */
#define  TABPANEL_2_CBTSETCR              5       /* control type: command, callback function: SetCrCB */
#define  TABPANEL_2_CBTGETCR              6       /* control type: command, callback function: GetCrCB */
#define  TABPANEL_2_CBTLPRD               7       /* control type: command, callback function: ldPrDataCB */
#define  TABPANEL_2_CBTSETPR              8       /* control type: command, callback function: SetPrDataCB */
#define  TABPANEL_2_CBTGETPR              9       /* control type: command, callback function: GetPrDataCB */

     /* tab page panel controls */
#define  TABPANEL_3_STRIPCHARTCH1         2       /* control type: strip, callback function: (none) */
#define  TABPANEL_3_STRIPCHARTCH2         3       /* control type: strip, callback function: (none) */
#define  TABPANEL_3_CHECKBOX54            4       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX53            5       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX52            6       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX51            7       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX50            8       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX49            9       /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX60            10      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX59            11      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX58            12      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX57            13      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX56            14      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX55            15      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX48            16      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX47            17      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX46            18      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX45            19      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX44            20      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX43            21      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX42            22      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX41            23      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX40            24      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX39            25      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX38            26      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX37            27      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX36            28      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX35            29      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX34            30      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX33            31      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX32            32      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX31            33      /* control type: radioButton, callback function: traceCB2 */
#define  TABPANEL_3_CHECKBOX24            34      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX23            35      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX22            36      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX21            37      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX20            38      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX19            39      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX30            40      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX29            41      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX28            42      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX27            43      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX26            44      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX25            45      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX18            46      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX17            47      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX16            48      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX15            49      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX14            50      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX13            51      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX12            52      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX11            53      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX10            54      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX9             55      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX8             56      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX7             57      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX6             58      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX5             59      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX4             60      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX3             61      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX2             62      /* control type: radioButton, callback function: traceCB */
#define  TABPANEL_3_CHECKBOX1             63      /* control type: radioButton, callback function: traceCB */

     /* tab page panel controls */
#define  TABPANEL_4_TABLE                 2       /* control type: table, callback function: evtTableSelCB */
#define  TABPANEL_4_CBTREADEVENT          3       /* control type: command, callback function: readEventCB */
#define  TABPANEL_4_TABLEBIT              4       /* control type: table, callback function: (none) */
#define  TABPANEL_4_CBTCLREVENT           5       /* control type: command, callback function: clrEventCB */
#define  TABPANEL_4_CBTCHECKTIME          6       /* control type: command, callback function: checkTimeCB */
#define  TABPANEL_4_DEV_DATETIME          7       /* control type: string, callback function: (none) */
#define  TABPANEL_4_PROGRESSBAR           8       /* control type: scale, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_5_TABLE                 2       /* control type: table, callback function: (none) */

     /* tab page panel controls */
#define  TABWAVE_GRAPHCH2                 2       /* control type: graph, callback function: (none) */
#define  TABWAVE_GRAPHCH1                 3       /* control type: graph, callback function: (none) */
#define  TABWAVE_RINGCH2SEL               4       /* control type: ring, callback function: (none) */
#define  TABWAVE_RINGCH1SEL               5       /* control type: ring, callback function: (none) */
#define  TABWAVE_CHECKBOXCH2_FD           6       /* control type: radioButton, callback function: scopeDisTypeCB2 */
#define  TABWAVE_CHECKBOXCH1_FD           7       /* control type: radioButton, callback function: scopeDisTypeCB */
#define  TABWAVE_NUMERICTHD1              8       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1
#define  CTRLARRAY_2                      2

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AC1WorkCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AC2WorkCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AutoStart(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AutoStop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_STARTSTOP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CBGetSysTime(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CheckConnectCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK checkTimeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseSysConf(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK clrEventCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CMB_REFReadCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CMB_REFWriteCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdClrCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdClrDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdEmrCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdEmrDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdQF3CanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdQF3DoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSelCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSelDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSMCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSMDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSrCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdSRDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdWorkCanselCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CmdWorkDoneCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DC1WorkCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DeviceInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EnventLog(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK evtTableSelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitSysCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetCrCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetID(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetPrDataCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetSysPrDataCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GRAPHCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK iDebugPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK iGetID(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InitSysLdDatCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK iscopeDisTypeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ldDfCrDatCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ldPrDataCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitWave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK readEventCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK recordCheckCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RestScopeZ1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Scope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK scopeDisTypeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK scopeDisTypeCB2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SEL_DEVCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetCrCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetPrDataCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetSysDatCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ShiftFuncx(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ShiftFuncy(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK STSQFCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK STSSW_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SysConf(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Timescales2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TogglePauseCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK traceCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK traceCB2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
