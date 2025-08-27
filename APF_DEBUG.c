//==============================================================================
//
// Title:       APF_DEBUG
// Purpose:     A short description of the application.
//
// Created on:  2012/11/13 at 15:20:08 by Î¢ÈíÓÃ»§.
// Copyright:   Î¢ÈíÖÐ¹ú. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <udpsupp.h>
#include "APF_DEBUG.h"
#include "toolbox.h"
#include "comm.h"
#include "datatype.h"
#include "iniFile.h"
#include "devData.h"

// Marco Define Area

#define LED_FLASH_PERIOD_TICKS 4
#define LED_ON_OFFSET_TICKS    2
#define Radius			4
#define WAVE_LENGTH 128
#define SAMPLES_PER_UPDATE 128
#define NUM_CHANNELS 6
//==============================================================================
// Constants

// Static global variables

int panelHandle;
int swPanelHandle;
int disPanelHandle,debugHandle;
int portHandle,scopePanelHandle,disPanelHandle;
static int plotHandle;
static int maxIterations;
static ColorMapEntry colorArray[255];
static int autoFollowEnabled = 1; //¸ú×ÙÇúÏß

ART_DataType irt_data;

//==============================================================================
// Static functions		for Default Setting
//==============================================================================
// Global variables
CmtTSQHandle tsqRxId, tsqTxId;

int conCount = 0;
unsigned long trace1Conf = 0;
unsigned long trace2Conf = 0;
traceDatType traceDat;

unsigned char evtStart = 0;
unsigned int  evtIndex = 0;
unsigned char evtReady = 1;
evtTypeStruct evtData;

unsigned int  comTst = 0;
//==============================================================================
// Types

//==============================================================================

void ClearARTdata(int disPanelHandle)
{
	// Set Data Zreo
	//SetCtrlVal(disPanelHandle, PANEL_A_Num_Upper_V1, 1);
	//Set LED off
	//return 0;
}



void updateGraphDisplay(double *data)
{
	// ?????
	DeleteGraphPlot(scopePanelHandle, PANEL_Scop_GRAPH, -1, VAL_DELAYED_DRAW);

	// ??6??????
	int colors[NUM_CHANNELS] = {VAL_RED, VAL_GREEN, VAL_BLUE, VAL_BLACK, VAL_CYAN, VAL_MAGENTA};

	// ?????????
	for (int ch = 0; ch < NUM_CHANNELS; ch++)
	{
		// &data[ch * SAMPLES_PER_UPDATE] ?????????????
		PlotY(scopePanelHandle, PANEL_Scop_GRAPH,
			  &data[ch * SAMPLES_PER_UPDATE], SAMPLES_PER_UPDATE,
			  VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, colors[ch]);
	}

	// ????
	RefreshGraph(scopePanelHandle, PANEL_Scop_GRAPH);
}

void updateStripChartDisplay(double *data)
{
	// ???????Strip Chart
	static int isConfigured = 0;
	if (!isConfigured)
	{
		SetCtrlAttribute(scopePanelHandle, PANEL_Scop_STRIPCHART, ATTR_NUM_TRACES, NUM_CHANNELS);

		int colors[NUM_CHANNELS] = {VAL_RED, VAL_GREEN, VAL_BLUE, VAL_BLACK, VAL_CYAN, VAL_MAGENTA};
		for (int i = 0; i < NUM_CHANNELS; i++)
		{
			SetTraceAttribute(scopePanelHandle, PANEL_Scop_STRIPCHART, i + 1, ATTR_TRACE_COLOR, colors[i]);
		}
		isConfigured = 1;
	}

	// ??Strip Chart??????? (????)
	// C1P1, C2P1, ..., C6P1, C1P2, C2P2, ...
	double stripData[NUM_CHANNELS * SAMPLES_PER_UPDATE];
	for (int i = 0; i < SAMPLES_PER_UPDATE; i++)
	{
		for (int ch = 0; ch < NUM_CHANNELS; ch++)
		{
			stripData[i * NUM_CHANNELS + ch] = data[ch * SAMPLES_PER_UPDATE + i];
		}
	}

	PlotStripChart(scopePanelHandle, PANEL_Scop_STRIPCHART,
				   stripData, NUM_CHANNELS * SAMPLES_PER_UPDATE, 0, 0, VAL_DOUBLE);
}

// ??updateC??
void updateC(void)
{
	static int isFirstUpdate = 1;
	// ????? 6 * 128
	static double scaledData[NUM_CHANNELS * SAMPLES_PER_UPDATE];

	if (isFirstUpdate)
	{
		// initializeStripChart(); // ???????
		isFirstUpdate = 0;
	}

	// ????????????
	int phase_shift_per_channel = SAMPLES_PER_UPDATE / NUM_CHANNELS; // 128 / 6 ˜ 21??

	// ?6?????????????
	for (int ch = 0; ch < NUM_CHANNELS; ch++)
	{
		int shift = ch * phase_shift_per_channel;

		for (int i = 0; i < SAMPLES_PER_UPDATE; i++)
		{
			// ??????(%)??????,????????
			int source_index = (i + shift) % SAMPLES_PER_UPDATE;

			// ??????????
			int dest_index = ch * SAMPLES_PER_UPDATE + i;

			// ?usWave[0]?????????
			scaledData[dest_index] = usWave[0][source_index] * 300.0;
		}
	}

	// ??????
	updateGraphDisplay(scaledData);
	updateStripChartDisplay(scaledData);
}
//==============================================================================


//==============================================================================
// Global functions
void updateWaveDisplay(void);
void flashRunLed(void);
void updateRTDisplay(RTDataType *pdat);
void updatePRDataToTable(PrDataType *pdat);
void updateCRDataToTable(CrDataType *pdat);
void updateHarmDisplay(void);
void refCurDisplay(ELTSTDat *val);
extern void updateARTdisplay(ART_DataType *irt_data, int disPanelHandle);
/*------------------------------------------------------------------------------
	¸üÐÂÇúÏßÊý¾Ý
--------------------------------------------------------------------------------*/
void updateTraceGraph(void)
{
	double yarray[30];
	double   *p = (double*)&traceDat;
	int i;
	int n = 0;
	int tabHandle;

	traceDat.us[2] = (double)rtdata.u[0]/10.0;
	traceDat.us[1] = (double)rtdata.u[1]/10.0;
	traceDat.us[0] = (double)rtdata.u[2]/10.0;

	traceDat.is[2] = (double)rtdata.is[0];
	traceDat.is[1] = (double)rtdata.is[1];
	traceDat.is[0] = (double)rtdata.is[2];

	traceDat.il[2] = (double)rtdata.il[0];
	traceDat.il[1] = (double)rtdata.il[1];
	traceDat.il[0] = (double)rtdata.il[2];

	traceDat.ic[2] = (double)rtdata.ifb[0];
	traceDat.ic[1] = (double)rtdata.ifb[1];
	traceDat.ic[0] = (double)rtdata.ifb[2];

	traceDat.thdus[2] = (double)rtdata.thdu[0];
	traceDat.thdus[1] = (double)rtdata.thdu[1];
	traceDat.thdus[0] = (double)rtdata.thdu[2];

	traceDat.thdis[2] = (double)rtdata.thdis[0];
	traceDat.thdis[1] = (double)rtdata.thdis[1];
	traceDat.thdis[0] = (double)rtdata.thdis[2];

	traceDat.thdil[2] = (double)rtdata.thdil[0];

	traceDat.thdil[1] = (double)rtdata.thdil[1];
	traceDat.thdil[0] = (double)rtdata.thdil[2];

	traceDat.nbus     = (double)rtdata.unb;
	traceDat.nbil     = (double)rtdata.ilnb;
	traceDat.nbis     = (double)rtdata.isnb;

	traceDat.pfs      = (double)rtdata.pfs[3]/1000.0;
	traceDat.pfl      = (double)rtdata.pfl[3]/1000.0;
	traceDat.ps       = (double)rtdata.ps[3];
	traceDat.qs       = (double)rtdata.qs[3];
	traceDat.pl       = (double)rtdata.pl[3];
	traceDat.ql       = (double)rtdata.ql[3];

	//¸üÐÂÇúÏß1ÏÔÊ¾
	if(trace1Conf)
	{
		for(i=0; i<30; i++)
		{
			if(trace1Conf & (0x01ul<<i))
			{
				yarray[n] = p[i];
				n++;
			}
		}
		GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 2, &tabHandle);
		PlotStripChart(tabHandle, TABPANEL_3_STRIPCHARTCH1, (void*)yarray, n, 0, 0, VAL_DOUBLE);
	}

	if(trace2Conf)
	{
		for(i=0; i<30; i++)
		{
			if(trace2Conf & (0x01ul<<i))
			{
				yarray[n] = p[i];
				n++;
			}
		}
		GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 2, &tabHandle);
		PlotStripChart(tabHandle, TABPANEL_3_STRIPCHARTCH2, (void*)yarray, n, 0, 0, VAL_DOUBLE);
	}

}
/*------------------------------------------------------------------------------
	Çå³ýÊÂ¼þÏÔÊ¾
--------------------------------------------------------------------------------*/
void clearEventDisplay(void)
{
	int tabHandle;
	int i;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &tabHandle);
	SetCtrlAttribute(tabHandle, TABPANEL_4_TABLE, ATTR_VISIBLE, 0);
	GetNumTableRows(tabHandle, TABPANEL_4_TABLE, &i);
	if(i < 2) return;
	while(i > 1)
	{
		DeleteTableRows(tabHandle, TABPANEL_4_TABLE, i, 1);
		i--;
	}
	SetCtrlAttribute(tabHandle, TABPANEL_4_TABLE, ATTR_VISIBLE, 1);
}
/*------------------------------------------------------------------------------
	¸üÐÂÊÂ¼þÏÔÊ¾
--------------------------------------------------------------------------------*/
void updateEventDisplay(void)
{
	int tabHandle;
	int i;
	int j;
	u16 year, mon, day;
	u16 hour, min, sec;
	char buf[64];

	if((evtData.time.year > 0x99)||(evtData.time.year < 0x10)) return;
//	if((evtData.time.mon  > 0x12)||(evtData.time.mon < 0x01))  return;
	if((evtData.time.day  > 0x31)||(evtData.time.day < 0x01))  return;
	if(evtData.time.hour > 0x23) return;
	if(evtData.time.min  > 0x59) return;
	if(evtData.time.sec  > 0x59) return;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &tabHandle);

	SetCtrlAttribute(tabHandle, TABPANEL_4_TABLE, ATTR_VISIBLE, 0);

	GetNumTableRows(tabHandle, TABPANEL_4_TABLE, &i);
	GetNumTableColumns(tabHandle, TABPANEL_4_TABLE, &j);
	i++;
	InsertTableRows(tabHandle, TABPANEL_4_TABLE, i, 1, VAL_USE_MASTER_CELL_TYPE);			//²åÈëÐÂµÄÐÐ£¬°´ÕÕÉèÖÃºÃµÄ¸ñÊ½
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(1, i), evtIndex); //i-1);
	//ÏÔÊ¾ÊÂ¼þÊ±¿Ì
	year = ((evtData.time.year>>4)&0x0F)*10 + (evtData.time.year&0x0F) + 2000;
	mon  = ((evtData.time.mon>>4)&0x0F)*10 + (evtData.time.mon&0x0F);
	day  = ((evtData.time.day>>4)&0x0F)*10 + (evtData.time.day&0x0F);
	hour = ((evtData.time.hour>>4)&0x0F)*10 + (evtData.time.hour&0x0F);
	min  = ((evtData.time.min>>4)&0x0F)*10 + (evtData.time.min&0x0F);
	sec  = ((evtData.time.sec>>4)&0x0F)*10 + (evtData.time.sec&0x0F);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(2, i), buf);
	//ÏÔÊ¾ÊÂ¼þÀàÐÍ
	switch(evtData.evtType)
	{
		case EVT_START:
			SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(3, i), "×Ô¶¯¿ªÊ¼");
			break;
		case EVT_STOP:
			SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(3, i), "×Ô¶¯½áÊø");
			break;
		case EVT_EMRON:
			SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(3, i), "¼±Í£°´ÏÂ");
			break;
		case EVT_EMROFF:
			SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(3, i), "¼±Í£ËÉ¿ª");
			break;
		case EVT_FAULT:
			SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(3, i), "Éè±¸¹ÊÕÏ");
			break;
		default:
			break;
	}
	//ÏÔÊ¾IO×´Ì¬
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(4, i), evtData.ioState);
	//ÏÔÊ¾FPGA×´Ì¬×Ö
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(5, i), evtData.fpgaState[0]);
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(6, i), evtData.fpgaState[1]);
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(7, i), evtData.fpgaState[2]);
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(8, i), evtData.fpgaState[3]);
	//ÏÔÊ¾×°ÖÃ¹ÊÕÏ×Ö
	SetTableCellVal(tabHandle, TABPANEL_4_TABLE, MakePoint(9, i), evtData.devstate);

	SetCtrlAttribute(tabHandle, TABPANEL_4_TABLE, ATTR_VISIBLE, 1);
}
/*------------------------------------------------------------------------------
	¸üÐÂÉè±¸Ê±¼ä
--------------------------------------------------------------------------------*/
void updateDevTime(void)
{
	u16 year, mon, day;
	u16 hour, min, sec;
	char buf[32];
	int tabHandle;

	year = ((rtdata.time.year>>4)&0x0F)*10 + (rtdata.time.year&0x0F) + 2000;
	mon  = ((rtdata.time.mon>>4)&0x0F)*10 + (rtdata.time.mon&0x0F);
	day  = ((rtdata.time.day>>4)&0x0F)*10 + (rtdata.time.day&0x0F);
	hour = ((rtdata.time.hour>>4)&0x0F)*10 + (rtdata.time.hour&0x0F);
	min  = ((rtdata.time.min>>4)&0x0F)*10 + (rtdata.time.min&0x0F);
	sec  = ((rtdata.time.sec>>4)&0x0F)*10 + (rtdata.time.sec&0x0F);

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &tabHandle);

	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	SetCtrlVal(tabHandle, TABPANEL_4_DEV_DATETIME, buf);

}
/*------------------------------------------------------------------------------
	ÊÕµ½Êý¾Ýµ÷ÓÃ
--------------------------------------------------------------------------------*/
extern u16 prdataUpdate;

void CVICALLBACK tsqRxCallback(CmtTSQHandle queueHandle, unsigned int event, int value, void *callbackData)
{
	s16 dataType;
	int curTabIndex;
	float temp;

	switch(event)
	{
		case EVENT_TSQ_ITEMS_IN_QUEUE:
			CmtReadTSQData(tsqRxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, 0);
			//¸üÐÂÊý¾Ý
			conCount = 0;

			if(prdataUpdate)
			{
				updatePRDataToTable(&prdata);
				updateCRDataToTable(&crdata);

				prdataUpdate = 0;
			}
			// activePanel = GetActivePanel();
			// printf("activePanel %d\n",activePanel);

			switch(dataType)
			{
				case 1:
					flashRunLed();
					//±êÊ¶ÔÚÏß
					SetCtrlVal(panelHandle, PANEL_LED_ONLINE, 1);
					GetActiveTabPage(panelHandle, PANEL_TAB, &curTabIndex);
					if(curTabIndex == 1)      updateWaveDisplay();
					else if(curTabIndex == 0) updateRTDisplay(&rtdata);
					else if(curTabIndex == 4) updateDevTime();				//¸üÐÂÉè±¸Ê±¼ä
					else if(curTabIndex == 5) updateHarmDisplay();
					updateARTdisplay(&irt_data,disPanelHandle); 
					//¸üÐÂÇúÏßÊý¾Ý
					updateTraceGraph();
					updateC();
					SetCtrlVal(panelHandle, PANEL_DEBUGSTATE, rtdata.runState);
					
					break;
				case 2:					//¶ÁÈ¡²ÎÊý
					break;
				case 3:					//¶ÁÈ¡Ð£×¼Êý¾Ý;
					break;
				case 4:					//¶ÁÈ¡ÊÂ¼þÊý¾Ý
					updateEventDisplay();
					evtReady = 1;
					evtIndex++;
					break;
				case 5:					//¶ÁÈ¡²Î¿¼µçÁ÷
					refCurDisplay(&elTstVal);
					break;
				default:
					break;
			}
			break;
	}
}
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	int udpThreadId = -1;

	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (swPanelHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL_SW));
	errChk (panelHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL));
	errChk (disPanelHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL_A));
	errChk (portHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL_DCF));
	errChk (scopePanelHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL_Scop));

	// errChk (disPanelHandle = LoadPanel (0, "APF_DEBUG.uir", PANEL_2));


	SetPanelPos(disPanelHandle, 100, 100);
	// errChk(SetPanelAttribute(panelHandle, ATTR_ACTIVATE_WHEN_CLICKED_ON, 0));
	devInfoInit();
	//
	/* ´´½¨UDP´¦ÀíÏß³Ì  */
	gRun = 1;
	CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, udpThread, NULL, &udpThreadId);
	/* ´´½¨Êý¾Ý½»»»¶ÓÁÐ */
	CmtNewTSQ(1, sizeof(u16), OPT_TSQ_DYNAMIC_SIZE, &tsqRxId);
	// detect DataRecive tsqRxCallback Thread
	CmtInstallTSQCallback(tsqRxId, EVENT_TSQ_ITEMS_IN_QUEUE, 1, tsqRxCallback, NULL, CmtGetCurrentThreadID(), NULL);
	errChk (DisplayPanel(disPanelHandle));
	errChk (DisplayPanel(scopePanelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	//µÈ´ýÏß³Ì½áÊø
	gRun = 0;
	CmtWaitForThreadPoolFunctionCompletion(DEFAULT_THREAD_POOL_HANDLE, udpThreadId, 0);
	//Ïú»Ù¶ÓÁÐ
	CmtDiscardTSQ(tsqRxId);

	if (panelHandle > 0) DiscardPanel (panelHandle);
	if (swPanelHandle > 0) DiscardPanel (swPanelHandle);

	//	udpDispose();
	//	tdmsFileClose();
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)QuitUserInterface (0);
	return 0;
}

/*-----------------------------------------------------------------------------
	update display
-------------------------------------------------------------------------------*/
static int scopeCH1IsFd;
static int scopeCH2IsFd;

void updateWaveDisplay(void)
{
	int tabHandle;
	int chsel;
	double x[128], y[128];
	double t;
	int i;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 1, &tabHandle);

	GetCtrlVal(tabHandle, TABWAVE_RINGCH1SEL, &chsel);

	DeleteGraphPlot(tabHandle, TABWAVE_GRAPHCH1,-1, VAL_NO_DRAW);

	PlotWaveform (scopePanelHandle, PANEL_Scop_GRAPH, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
	switch(chsel)
	{
		case 0:				/*ÏµÍ³µçÑ¹ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = usWave[0][i]/10.0;
					y[i] = 0.0;
				}
				FFT(x, y, 128);
				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);

				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				//ÏÈ×öµ¥Ïà£¬ÈýÏàÏÔÊ¾ÎÊÌâ»¹ÐèÒªÏë°ì·¨½â¾ö
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &usWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &usWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &usWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 1:				/*ÏµÍ³µçÁ÷ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = isWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);

				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);

				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &isWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &isWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &isWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 2:				/*¸ºÔØµçÁ÷ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = ilWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);

				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);

				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ilWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ilWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ilWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 3:				/*²¹³¥µçÁ÷ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = ifWave[0][i]/10.0;
					y[i] = 0;
				}
				FFT(x, y, 128);
				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);

				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ifWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ifWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &ifWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 4:				/*²Î¿¼µçÁ÷ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = irWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);

				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);

				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &irWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &irWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &irWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 5:				/*µ÷ÖÆ²¨*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = dbWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);
				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dbWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dbWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dbWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 6:			/*Îó²îµçÁ÷ÏÔÊ¾*/
			if(scopeCH1IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = dtWave[0][i]/10.0;
					y[i] = 0;
				}
				FFT(x, y, 128);
				t = 0.0;
				for(i=2; i<63; i++)
				{
					t += x[i]*x[i]+y[i]*y[i];
				}
				t = sqrt(t);
				t /= sqrt(x[1]*x[1]+y[1]*y[1]);
				t *= 100;
				SetCtrlVal(tabHandle,  TABWAVE_NUMERICTHD1, t);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dtWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dtWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH1, &dtWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		default:
			break;
	}
	DeleteGraphPlot (tabHandle, TABWAVE_GRAPHCH2,-1, VAL_NO_DRAW);
	GetCtrlVal(tabHandle, TABWAVE_RINGCH2SEL, &chsel);

	switch(chsel)
	{
		case 0:				/*ÏµÍ³µçÑ¹ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = usWave[0][i]/10.0;
					y[i] = 0.0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				//ÏÈ×öµ¥Ïà£¬ÈýÏàÏÔÊ¾ÎÊÌâ»¹ÐèÒªÏë°ì·¨½â¾ö
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &usWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &usWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &usWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 1:				/*ÏµÍ³µçÁ÷ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = isWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &isWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &isWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &isWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 2:				/*¸ºÔØµçÁ÷ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = ilWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ilWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ilWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ilWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 3:				/*²¹³¥µçÁ÷ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = ifWave[0][i]/10.0;
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ifWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ifWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &ifWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 4:				/*²Î¿¼µçÁ÷ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = irWave[0][i]/2;
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &irWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &irWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &irWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 5:				/*µ÷ÖÆ²¨*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = dbWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dbWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dbWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dbWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		case 6:				/*Îó²îµçÁ÷ÏÔÊ¾*/
			if(scopeCH2IsFd)
			{
				for(i=0; i<128; i++)
				{
					x[i] = dtWave[0][i];
					y[i] = 0;
				}
				FFT(x, y, 128);
				for(i=0; i<51; i++)
				{
					x[i] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
				}
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, x, 51, VAL_DOUBLE, 1.0, 0.0, 0.0, 1.0, VAL_BASE_ZERO_VERTICAL_BAR, VAL_SMALL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			}
			else
			{
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dtWave[0][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dtWave[1][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				PlotWaveform (tabHandle, TABWAVE_GRAPHCH2, &dtWave[2][0], disPlayLen, VAL_SHORT_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
		default:
			break;
	}
}
/*----------------------------------------------------------------------------
	ÍË³öÏµÍ³
------------------------------------------------------------------------------*/
int CVICALLBACK exitSysCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(swPanelHandle);
			HidePanel(panel);
			// QuitUserInterface(0);

			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------
	»ñÈ¡ÏµÍ³Ê±¼ä»Øµ÷º¯Êý£¬0.5SÖ´ÐÐÒ»´Î
------------------------------------------------------------------------------*/
int CVICALLBACK CBGetSysTime (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int year, mon, day;
	int hour, min, sec;
	char buf[32];
	int dataType;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(++comTst > 10)
			{
				comTst = 0;
				dataType = CMD_HB;
				CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			}

			GetSystemDate(&mon, &day, &year);
			GetSystemTime(&hour, &min, &sec);
			sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
			SetCtrlVal(panelHandle, PANEL_SYSTIMEDISPLAY, buf);
			// USE Same Clock
			SetCtrlVal(disPanelHandle, PANEL_A_SYSTIMEDISPLAY, buf);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	¸üÐÂÊµÊ±Êý¾ÝÏÔÊ¾
-----------------------------------------------------------------------------*/
void updateRTDisplay(RTDataType *pdat)
{
	int parentHandle;
	float temp;
	int i, j;
	int temp2;
	u16 k;
	static u16 ubit[6] = {0, 0, 0, 0, 0, 0};
	u16 ubit_t[6];
	s8 buf[32];

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 0, &parentHandle);
	SetCtrlAttribute(parentHandle, TABPANEL_TABLEGRID, ATTR_VISIBLE, 0);
	//µçÑ¹Êý¾Ý¸üÐÂ
	for(i=0; i<3; i++)
	{
		temp = pdat->u[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(2, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->thdu[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(11, 2+i), ATTR_CTRL_VAL, temp);
	}
	//ÏµÍ³ºÍ¸ºÔØµçÁ÷¸üÐÂ
	for(i=0; i<4; i++)
	{
		k = pdat->is[i];
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(3, 2+i), ATTR_CTRL_VAL, k);
		temp = pdat->il[i];
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(4, 2+i), ATTR_CTRL_VAL, temp);
	}
	//ÏµÍ³ºÍ¸ºÔØÓÐ¹¦¡¢ÎÞ¹¦¹¦ÂÊ¸üÐÂ
	for(i=0; i<4; i++)
	{
		temp = pdat->ps[i]/100.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(5, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->qs[i]/100.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(6, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->pl[i]/100.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(7, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->ql[i]/100.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(8, 2+i), ATTR_CTRL_VAL, temp);
	}
	//ÏµÍ³ºÍ¸ºÔØ¹¦ÂÊÒòÊý¸üÐÂ
	for(i=0; i<4; i++)
	{
		temp = pdat->pfs[i]/1000.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(9, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->pfl[i]/1000.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(10, 2+i), ATTR_CTRL_VAL, temp);
	}
	//ÏµÍ³ºÍ¸ºÔØµçÁ÷×Ü»û±äÂÊ¸üÐÂ
	for(i=0; i<4; i++)
	{
		temp = pdat->thdis[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(12, 2+i), ATTR_CTRL_VAL, temp);
		temp = pdat->thdil[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEGRID, MakePoint(13, 2+i), ATTR_CTRL_VAL, temp);
	}
	SetCtrlAttribute(parentHandle, TABPANEL_TABLEGRID, ATTR_VISIBLE, 1);
	SetCtrlAttribute(parentHandle, TABPANEL_TABLEAPF, ATTR_VISIBLE, 0);
	//Ö¸ÁîµçÁ÷/²¹³¥µçÁ÷/ÎÂ¶È/ÏµÍ³Î»ÒÆ¹¦Òò/¸ºÔØÎ»ÒÆ¹¦Òò¸üÐÂ
	for(i=0; i<4; i++)
	{
		temp = pdat->ir[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(2, 1+i), ATTR_CTRL_VAL, temp);
		temp = pdat->ifb[i]/10.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(4, 1+i), ATTR_CTRL_VAL, temp);
		temp = pdat->temp[i];
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(6, 1+i), ATTR_CTRL_VAL, temp);
		temp = pdat->dpfs[i]/1000.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(8, 1+i), ATTR_CTRL_VAL, temp);
		temp = pdat->dpfl[i]/1000.0;
		SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(10, 1+i), ATTR_CTRL_VAL, temp);
	}
	//µçÑ¹²»Æ½ºâ¶È¸üÐÂ
	temp = pdat->unb/10.0;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(2, 5), ATTR_CTRL_VAL, temp);
	//ÏµÍ³µçÁ÷²»Æ½ºâ¶È¸üÐÂ
	temp = pdat->isnb/10.0;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(4, 5), ATTR_CTRL_VAL, temp);
	//¸ºÔØµçÁ÷²»Æ½ºâ¶È¸üÐÂ
	temp = pdat->ilnb/10.0;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(6, 5), ATTR_CTRL_VAL, temp);
	//µçÍøÆµÂÊ¸üÐÂ
	temp = pdat->freq/10.0;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(8, 5), ATTR_CTRL_VAL, temp);
	//ÉÏµçÈÝÖ±Ñ¹¸üÐÂ
	temp = pdat->udc;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(10, 5), ATTR_CTRL_VAL, temp);
	//ÏÂµçÈÝÖ±Ñ¹¸üÐÂ
	temp = pdat->udc2;
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(10, 6), ATTR_CTRL_VAL, temp);
	//²Î¿¼ÎÞ¹¦µçÁ÷
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(2, 6), ATTR_CTRL_VAL, pdat->iqref);
	//·´À¡ÎÞ¹¦µçÁ÷
	SetTableCellAttribute(parentHandle, TABPANEL_TABLEAPF, MakePoint(4, 6), ATTR_CTRL_VAL, pdat->iqfeed);
	//
	//--------------------------------------------------------------------------------------------

	SetCtrlAttribute(parentHandle, TABPANEL_TABLEAPF, ATTR_VISIBLE, 1);
	//¸üÐÂFPGA×´Ì¬×Ö
	SetCtrlAttribute(parentHandle, TABPANEL_TABLEBIT, ATTR_VISIBLE, 0);
	for(i=0; i<16; i++)
	{
		///////////////////////////////////////////////////////
//		if((pdat->fpgaState[0]&(0x0001<<i))){
		temp2 = (pdat->fpgaState[0]&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 2), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 2), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 2), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
		//////////////////////////////////////////////////////
//		if((pdat->fpgaState[1]&(0x0001<<i))){
		temp2 = (pdat->fpgaState[1]&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 4), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 4), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 4), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
		/////////////////////////////////////////////////////
//		if((pdat->fpgaState[2]&(0x0001<<i))){
		temp2 = (pdat->fpgaState[2]&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 6), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 6), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 6), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
		//////////////////////////////////////////////////////
//		if((pdat->fpgaState[3]&(0x0001<<i))){
		temp2 = (pdat->fpgaState[3]&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 8), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 8), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 8), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
		//////////////////////////////////////////////////////
//		if((pdat->ioState&(0x0001<<i))){
		temp2 = (pdat->ioState&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 10), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 10), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 10), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
		//////////////////////////////////////////////////////
//		if((pdat->devState&(0x0001<<i))){
		temp2 = (pdat->devState&(0x0001<<i))>>i;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 12), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 12), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 12), ATTR_TEXT_COLOR, VAL_BLACK);
//		}
	}
	for(i=0; i<16; i++)
	{
		j = i + 16;
		temp2 = (pdat->devState&(0x0001<<j))>>j;
		SetTableCellValFromIndex(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 14), temp2);
		if(temp2)  SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 14), ATTR_TEXT_COLOR, VAL_RED);
		else	   SetTableCellAttribute(parentHandle, TABPANEL_TABLEBIT, MakePoint(i+1, 14), ATTR_TEXT_COLOR, VAL_BLACK);
	}


	SetCtrlAttribute(parentHandle, TABPANEL_TABLEBIT, ATTR_VISIBLE, 1);
//¸üÐÂAPFµ±Ç°×´Ì¬

	//¸üÐÂÖÕ¶ËÊ±ÖÓ
//	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", pdat->rtcDat.year, pdat->rtcDat.mon, pdat->rtcDat.day, pdat->rtcDat.hour, pdat->rtcDat.min, pdat->rtcDat.sec);
//	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &parentHandle);
//	SetCtrlVal(parentHandle, TABPANEL_4_STRING, buf);

}

/*---------------------------------------------------------------------------------------------------
	Í¨ÐÅÖ¸Ê¾
-----------------------------------------------------------------------------------------------------*/
void flashRunLed(void)
{
	static int count = 0;
	count++;
	if(count == LED_ON_OFFSET_TICKS)
	{
		SetCtrlVal(panelHandle, PANEL_LED_RUN, 1);
		SetCtrlVal(portHandle, PANEL_Scop_LED_RUN, 0);
		SetCtrlVal(disPanelHandle, PANEL_A_LED_RUN, 0);
		SetCtrlVal(scopePanelHandle, PANEL_Scop_LED_RUN, 0);
	}
	else if(count >= LED_FLASH_PERIOD_TICKS)
	{
		count = 0;
		SetCtrlVal(panelHandle, PANEL_LED_RUN, 0);
		// ADD the OTHER Panel  edit 2025-0806
		SetCtrlVal(portHandle, PANEL_DCF_LED_RUN, 1);
		SetCtrlVal(portHandle, PANEL_Scop_LED_RUN, 1);
		SetCtrlVal(disPanelHandle, PANEL_A_LED_RUN, 1);
		SetCtrlVal(scopePanelHandle, PANEL_Scop_LED_RUN, 1);
	}
}

/*----------------------------------------------------------------------------------------------------
	ÊµÊ±²¨ÐÎÍ¨µÀ1Ê±ÓòÓëÆµÓòÏÔÊ¾Ñ¡Ôñ
------------------------------------------------------------------------------------------------------*/
int CVICALLBACK scopeDisTypeCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &scopeCH1IsFd);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------
	ÊµÊ±²¨ÐÎÍ¨µÀ2Ê±ÓòÓëÆµÓòÏÔÊ¾Ñ¡Ôñ
-----------------------------------------------------------------------------------------------------*/
int CVICALLBACK scopeDisTypeCB2 (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &scopeCH2IsFd);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------
	¶ÁÈ¡ÉèÖÃÊý¾Ý	 Write
-----------------------------------------------------------------------------------------------------*/
void readPRDataFromTable(PrDataType *pdat)
{
	int parentHandle;
	char buf[64];
	int temp;
	double t;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 3, &parentHandle);

	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 1), &t);
	pdat->k1 = t;																										//µçÑ¹Ç°À¡K1
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 1), &t);
	pdat->kp = t;																										//Ö±Ñ¹±ÈÀýÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 1), &t);
	pdat->ki = t;																										//Ö±Ñ¹»ý·ÖÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 1), &t);
	pdat->kr = t;																										//µçÁ÷ÄÚ»·±ÈÀýÏµÊý
	//---------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 2), &t);
	pdat->prkr = t;																										//Ð³Õñ¿ØÖÆ±ÈÀýÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 2), &t);
	pdat->prki = t;																										//Ð³Õñ¿ØÖÆ»ý·ÖÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 2), &t);
	pdat->cwp = t;																										//ÔØ²¨ÒÆÏà
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 2), &t);
	pdat->th3 = t;																										//3´ÎÐ³²¨ÔöÒæ
	//---------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 3), &t);
	pdat->th5 = t;																										//5´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 3), &t);
	pdat->th7 = t;																										//7´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 3), &t);
	pdat->th9 = t;																										//9´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 3), &t);
	pdat->th11 = t;																										//11´ÎÐ³²¨ÔöÒæ
	//---------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 4), &t);
	pdat->th13 = t;																										//13´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 4), &t);
	pdat->th15 = t;																										//15´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 4), &t);
	pdat->th17 = t;																										//17´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 4), &t);
	pdat->th19 = t;																										//19´ÎÐ³²¨ÔöÒæ
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 5), &t);
	pdat->th21 = t;																										//21´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 5), &t);
	pdat->th23 = t;																										//23´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 5), &t);
	pdat->th25 = t;																										//25´ÎÐ³²¨ÔöÒæ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 5), &t);
	pdat->tha  = t;																										//Å¼´Î¼°26~50´ÎÐ³²¨ÔöÒæ
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 6), &t);
	pdat->cts = t;																										//ÏµÍ³CT±ä±È
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 6), &t);
	pdat->ctl = t;																										//¸ºÔØCT±ä±È
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 6), &t);
	pdat->uov = t;																										//¹ýÑ¹ÃÅÏÞ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 6), &t);
	pdat->udn = t;																										//Ç·Ñ¹ÃÅÏÞ
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 7), &t);
	pdat->unb = t;																										//²»Æ½ºâ¶ÈÃÅÏÞ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 7), &t);
	pdat->pir = t;																										//µ¥Ôª¶î¶¨µçÁ÷
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 7), &t);
	pdat->cov = t;																										//¹ýÁ÷ÃÅÏÞ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 7), &t);
	pdat->csc = t;																										//ËÙ¶ÏÃÅÏÞ
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 8), &t);
	pdat->tov = t;																										//ÎÂ¶ÈÃÅÏÞ
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 8), buf);											//¿ØÖÆÄ£Ê½
	GetTableCellRingIndexFromValue(parentHandle, TABPANEL_2_TABLEPR, 0, MakePoint(4, 8), &temp, buf);
	pdat->ctrlMode = (s16)temp;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 8), &t);
	pdat->msMode = t;																									//Ö÷´ÓÄ£Ê½
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 8), &t);
	pdat->sysId = t;																									//Éè±¸µØÖ·
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 9), &t);
	pdat->vkp = t;																										//Íâ»·±ÈÀýÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 9), &t);
	pdat->vki = t;																										//Íâ»·»ý·ÖÏµÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 9), &t);
	pdat->tpf = t;																										//Ä¿±ê¹¦ÂÊÒòÊý
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 9), &t);
	pdat->vacRef = t;																									//½»Á÷µçÑ¹²Î¿¼
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 10), &t);											//Ð³²¨×ÜÔöÒæµ÷½Ú
	pdat->k2h = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 10), &t);											//¸ø¶¨ÎÞ¹¦µçÁ÷
	pdat->iqref = t*14.14213;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 10), &t);											//¸ºÐòµçÁ÷Êµ²¿¸ø¶¨
	pdat->inqreal = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 10), &t);											//¸ºÐòµçÁ÷Ðé²¿¸ø¶¨
	pdat->inqimag = t;
	//--------------------------------------------------------------------------
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 11), &pdat->punum);									//µ¥ÔªÊýÁ¿

	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 11), buf);
	GetTableCellRingIndexFromValue(parentHandle, TABPANEL_2_TABLEPR, 0, MakePoint(4, 11), &temp, buf);					//²ÉÑùÄ£Ê½
	pdat->sampMode = temp;

	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 11), &t);											//ÁãÐòµçÁ÷Êµ²¿¸ø¶¨
	pdat->izqreal = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 11), &t);											//ÁãÐòµçÁ÷Ðé²¿¸ø¶¨
	pdat->izqimag = t;

	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 12), &t);											//¾ùÑ¹¿ØÖÆ±ÈÀýÏµÊý
	pdat->kp2 = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 12), &t);											//¾ùÑ¹¿ØÖÆ»ý·ÖÏµÊý
	pdat->ki2 = t;

}
/*---------------------------------------------------------------------------------------------------
	¸üÐÂÉèÖÃÊý¾Ý	  Write
-----------------------------------------------------------------------------------------------------*/
void updatePRDataToTable(PrDataType *pdat)
{
	int parentHandle;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 3, &parentHandle);

	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 1), ATTR_CTRL_VAL, (double)pdat->k1);				//µçÑ¹Ç°À¡K1
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 1), ATTR_CTRL_VAL, (double)pdat->kp);				//Ö±Ñ¹±ÈÀýÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 1), ATTR_CTRL_VAL, (double)pdat->ki);				//Ö±Ñ¹»ý·ÖÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 1), ATTR_CTRL_VAL, (double)pdat->kr);				//µçÁ÷ÄÚ»·±ÈÀýÏµÊý
	//---------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 2), ATTR_CTRL_VAL, (double)pdat->prkr);			//Ð³Õñ¿ØÖÆ±ÈÀýÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 2), ATTR_CTRL_VAL, (double)pdat->prki);			//Ð³Õñ¿ØÖÆ»ý·ÖÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 2), ATTR_CTRL_VAL, (double)pdat->cwp);				//ÔØ²¨ÒÆÏà
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 2), ATTR_CTRL_VAL, (double)pdat->th3);				//3´ÎÐ³²¨ÔöÒæ
	//---------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 3), ATTR_CTRL_VAL, (double)pdat->th5);				//5´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 3), ATTR_CTRL_VAL, (double)pdat->th7);				//7´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 3), ATTR_CTRL_VAL, (double)pdat->th9);				//9´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 3), ATTR_CTRL_VAL, (double)pdat->th11);			//11´ÎÐ³²¨ÔöÒæ
	//---------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 4), ATTR_CTRL_VAL, (double)pdat->th13);			//13´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 4), ATTR_CTRL_VAL, (double)pdat->th15);			//15´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 4), ATTR_CTRL_VAL, (double)pdat->th17);			//17´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 4), ATTR_CTRL_VAL, (double)pdat->th19);			//19´ÎÐ³²¨ÔöÒæ
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 5), ATTR_CTRL_VAL, (double)pdat->th21);			//21´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 5), ATTR_CTRL_VAL, (double)pdat->th23);			//23´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 5), ATTR_CTRL_VAL, (double)pdat->th25);			//25´ÎÐ³²¨ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 5), ATTR_CTRL_VAL, (double)pdat->tha);				//Å¼´Î¼°26~50´ÎÐ³²¨ÔöÒæ
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 6), ATTR_CTRL_VAL, (double)pdat->cts);				//ÏµÍ³CT±ä±È
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 6), ATTR_CTRL_VAL, (double)pdat->ctl);				//¸ºÔØCT±ä±È
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 6), ATTR_CTRL_VAL, (double)pdat->uov);				//¹ýÑ¹ÃÅÏÞ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 6), ATTR_CTRL_VAL, (double)pdat->udn);				//Ç·Ñ¹ÃÅÏÞ
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 7), ATTR_CTRL_VAL, (double)pdat->unb);				//²»Æ½ºâ¶ÈÃÅÏÞ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 7), ATTR_CTRL_VAL, (double)pdat->pir);				//µ¥Ôª¶î¶¨µçÁ÷
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 7), ATTR_CTRL_VAL, (double)pdat->cov);				//¹ýÁ÷ÃÅÏÞ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 7), ATTR_CTRL_VAL, (double)pdat->csc);				//ËÙ¶ÏÃÅÏÞ
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 8), ATTR_CTRL_VAL, (double)pdat->tov);				//ÎÂ¶ÈÃÅÏÞ
	SetTableCellValFromIndex(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 8), pdat->ctrlMode);							//¿ØÖÆÄ£Ê½
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 8), ATTR_CTRL_VAL, (double)pdat->msMode);			//Ö÷´ÓÄ£Ê½
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 8), ATTR_CTRL_VAL, (double)pdat->sysId);			//Éè±¸µØÖ·
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 9), ATTR_CTRL_VAL, (double)pdat->vkp);				//Íâ»·±ÈÀýÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 9), ATTR_CTRL_VAL, (double)pdat->vki);				//Íâ»·»ý·ÖÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 9), ATTR_CTRL_VAL, (double)pdat->tpf);				//Ä¿±ê¹¦ÂÊÒòÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 9), ATTR_CTRL_VAL, (double)pdat->vacRef);			//½»Á÷µçÑ¹²Î¿¼
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 10), ATTR_CTRL_VAL, (double)pdat->k2h);			//Ð³²¨µçÁ÷ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 10), ATTR_CTRL_VAL, (double)pdat->iqref/14.14213); //²Î¿¼ÎÞ¹¦µçÁ÷
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 10), ATTR_CTRL_VAL, (double)pdat->inqreal);		//Ð³²¨µçÁ÷ÔöÒæ
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 10), ATTR_CTRL_VAL, (double)pdat->inqimag); 		//²Î¿¼ÎÞ¹¦µçÁ÷
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 11), ATTR_CTRL_VAL, pdat->punum);					//µ¥ÔªÊýÁ¿
	SetTableCellValFromIndex(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 11), pdat->sampMode);							//²ÉÑùÄ£Ê½
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(6, 11), ATTR_CTRL_VAL, (double)pdat->izqreal);		//ÁãÐòµçÁ÷Êµ²¿²Î¿¼
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(8, 11), ATTR_CTRL_VAL, (double)pdat->izqimag);		//ÁãÐòµçÁ÷Ðé²¿²Î¿¼
	//--------------------------------------------------------------------------
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(2, 12), ATTR_CTRL_VAL, (double)pdat->kp2);			//¾ùÑ¹¿ØÖÆ±ÈÀýÏµÊý
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLEPR, MakePoint(4, 12), ATTR_CTRL_VAL, (double)pdat->ki2);			//¾ùÑ¹¿ØÖÆ»ý·ÖÏµÊý

}
/*---------------------------------------------------------------------------------------
	¶ÁÈ¡Ð£×¼ÏµÊý	Write
-----------------------------------------------------------------------------------------*/
void readCRDataFromTable(CrDataType *pdat)
{
	int parentHandle;
	double t;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 3, &parentHandle);

	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 1), &t);
	pdat->ad1cor[0] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 1), &t);
	pdat->ad1cor[1] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 1), &t);
	pdat->ad1cor[2] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 1), &t);
	pdat->ad1cor[3] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 2), &t);
	pdat->ad1cor[4] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 2), &t);
	pdat->ad1cor[5] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 2), &t);
	pdat->ad1cor[6] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 2), &t);
	pdat->ad1cor[7] = t;

	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 3), &t);
	pdat->ad2cor[0] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 3), &t);
	pdat->ad2cor[1] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 3), &t);
	pdat->ad2cor[2] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 3), &t);
	pdat->ad2cor[3] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 4), &t);
	pdat->ad2cor[4] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 4), &t);
	pdat->ad2cor[5] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 4), &t);
	pdat->ad2cor[6] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 4), &t);
	pdat->ad2cor[7] = t;

	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 5), &t);
	pdat->ad3cor[0] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 5), &t);
	pdat->ad3cor[1] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 5), &t);
	pdat->ad3cor[2] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 5), &t);
	pdat->ad3cor[3] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 6), &t);
	pdat->ad3cor[4] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 6), &t);
	pdat->ad3cor[5] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 6), &t);
	pdat->ad3cor[6] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 6), &t);
	pdat->ad3cor[7] = t;

	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 7), &t);
	pdat->ad4cor[0] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 7), &t);
	pdat->ad4cor[1] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 7), &t);
	pdat->ad4cor[2] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 7), &t);
	pdat->ad4cor[3] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 8), &t);
	pdat->ad4cor[4] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 8), &t);
	pdat->ad4cor[5] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 8), &t);
	pdat->ad4cor[6] = t;
	GetTableCellVal(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 8), &t);
	pdat->ad4cor[7] = t;
}



void SetPRDataFromDCP(ART_DataType *irt_data)
{
	int temp,ringIndex;
	int t=0;

	t= irt_data->example_n[0];
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_AC1_Cap, t);
	irt_data->example_n[1]=t;
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_AC2_Cap, t);
	t=irt_data->example_n[2]=t;
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC1_Cap, t);
	t=irt_data->example_n[3]=t;
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC2_Cap, t);
	t=irt_data->example_n[4]=t;
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC3_Cap, t);
	t= irt_data->example_n[5]=t;
	SetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC4_Cap, t);

	ringIndex= irt_data->example_r1[0];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_AC1_1, ringIndex);
	ringIndex= irt_data->example_r1[1];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_AC2_1, ringIndex);
	ringIndex= irt_data->example_r1[2];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC1_1, ringIndex);
	// Ring1
	ringIndex= irt_data->example_r1[3];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC2_1, ringIndex);
	ringIndex= irt_data->example_r1[4];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC3_1, ringIndex);
	ringIndex= irt_data->example_r1[5];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC4_1, ringIndex);
	// Ring2
	ringIndex= irt_data->example_r1[0];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_AC1_2, ringIndex);
	ringIndex= irt_data->example_r1[1];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_AC2_2, ringIndex);
	ringIndex= irt_data->example_r1[2];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC1_2, ringIndex);

	ringIndex= irt_data->example_r1[3];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC2_2, ringIndex);
	ringIndex= irt_data->example_r1[4];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC3_2, ringIndex);
	ringIndex= irt_data->example_r1[5];
	SetCtrlIndex(portHandle, PANEL_DCF_RING_DC4_2, ringIndex);
}
/*
	¶ÁÈ¡²ÎÊýÉèÖÃÒ³ÃæµÄÖµ
*/
void readPRDataFromDCP(ART_DataType *irt_data)
{
	int temp,ringIndex[6];
	int t[6];

	// NUMERIC  Line1
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_AC1_Cap, &t[0]);
	irt_data->example_n[0]=t[0];
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_AC2_Cap, &t[1]);
	irt_data->example_n[1]=t[1];
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC1_Cap, &t[2]);
	irt_data->example_n[2]=t[2];
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC2_Cap, &t[3]);
	irt_data->example_n[3]=t[3];
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC3_Cap, &t[4]);
	irt_data->example_n[4]=t[4];
	GetCtrlVal (portHandle, PANEL_DCF_NUMERIC_DC4_Cap, &t[5]);
	irt_data->example_n[5]=t[5];

	// RING		Line 2
	GetCtrlVal(portHandle, PANEL_DCF_RING_AC1_1, &ringIndex[0]);
	irt_data->example_r1[0]=ringIndex[0];
	GetCtrlVal(portHandle, PANEL_DCF_RING_AC2_1, &ringIndex[1]);
	irt_data->example_r1[1]=ringIndex[1];

	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC1_1, &ringIndex[2]);
	irt_data->example_r1[2]=ringIndex[2];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC2_1, &ringIndex[3]);
	irt_data->example_r1[3]=ringIndex[3];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC3_1, &ringIndex[4]);
	irt_data->example_r1[4]=ringIndex[4];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC4_1, &ringIndex[5]);
	irt_data->example_r1[5]=ringIndex[5];

	// RING  Line 3
	GetCtrlIndex(portHandle, PANEL_DCF_RING_AC1_2, &ringIndex[0]);
	irt_data->example_r2[0]=ringIndex[0];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_AC2_2, &ringIndex[1]);
	irt_data->example_r2[1]=ringIndex[1];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC1_2, &ringIndex[2]);
	irt_data->example_r2[2]=ringIndex[2];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC2_2, &ringIndex[3]);
	irt_data->example_r2[3]=ringIndex[3];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC3_2, &ringIndex[4]);
	irt_data->example_r2[4]=ringIndex[4];
	GetCtrlIndex(portHandle, PANEL_DCF_RING_DC4_2, &ringIndex[5]);
	irt_data->example_r2[5]=ringIndex[5];
}

/*---------------------------------------------------------------------------------------
	¸üÐÂÐ£×¼Êý¾ÝÏÔÊ¾   Write
-----------------------------------------------------------------------------------------*/
void updateCRDataToTable(CrDataType *pdat)
{
	int parentHandle;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 3, &parentHandle);

	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 1), ATTR_CTRL_VAL, (double)pdat->ad1cor[0]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 1), ATTR_CTRL_VAL, (double)pdat->ad1cor[1]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 1), ATTR_CTRL_VAL, (double)pdat->ad1cor[2]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 1), ATTR_CTRL_VAL, (double)pdat->ad1cor[3]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 2), ATTR_CTRL_VAL, (double)pdat->ad1cor[4]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 2), ATTR_CTRL_VAL, (double)pdat->ad1cor[5]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 2), ATTR_CTRL_VAL, (double)pdat->ad1cor[6]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 2), ATTR_CTRL_VAL, (double)pdat->ad1cor[7]);

	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 3), ATTR_CTRL_VAL, (double)pdat->ad2cor[0]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 3), ATTR_CTRL_VAL, (double)pdat->ad2cor[1]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 3), ATTR_CTRL_VAL, (double)pdat->ad2cor[2]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 3), ATTR_CTRL_VAL, (double)pdat->ad2cor[3]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 4), ATTR_CTRL_VAL, (double)pdat->ad2cor[4]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 4), ATTR_CTRL_VAL, (double)pdat->ad2cor[5]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 4), ATTR_CTRL_VAL, (double)pdat->ad2cor[6]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 4), ATTR_CTRL_VAL, (double)pdat->ad2cor[7]);

	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 5), ATTR_CTRL_VAL, (double)pdat->ad3cor[0]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 5), ATTR_CTRL_VAL, (double)pdat->ad3cor[1]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 5), ATTR_CTRL_VAL, (double)pdat->ad3cor[2]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 5), ATTR_CTRL_VAL, (double)pdat->ad3cor[3]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 6), ATTR_CTRL_VAL, (double)pdat->ad3cor[4]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 6), ATTR_CTRL_VAL, (double)pdat->ad3cor[5]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 6), ATTR_CTRL_VAL, (double)pdat->ad3cor[6]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 6), ATTR_CTRL_VAL, (double)pdat->ad3cor[7]);

	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 7), ATTR_CTRL_VAL, (double)pdat->ad4cor[0]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 7), ATTR_CTRL_VAL, (double)pdat->ad4cor[1]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 7), ATTR_CTRL_VAL, (double)pdat->ad4cor[2]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 7), ATTR_CTRL_VAL, (double)pdat->ad4cor[3]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(2, 8), ATTR_CTRL_VAL, (double)pdat->ad4cor[4]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(4, 8), ATTR_CTRL_VAL, (double)pdat->ad4cor[5]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(6, 8), ATTR_CTRL_VAL, (double)pdat->ad4cor[6]);
	SetTableCellAttribute(parentHandle, TABPANEL_2_TABLECR, MakePoint(8, 8), ATTR_CTRL_VAL, (double)pdat->ad4cor[7]);
}
/*---------------------------------------------------------------------------------------
	¸üÐÂ¸÷´ÎÐ³²¨Ð³²¨ÏÔÊ¾
-----------------------------------------------------------------------------------------*/
void updateHarmDisplay(void)
{
	static int count = 0;
	static int step  = 0;
	int i;
	int tabHandle;
	double x[128], y[128];
	double amp[50], degree[50], th[50];

	if(++count < 5) return;
	count = 0;
	if(++step >= 3) step = 0;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 5, &tabHandle);
	SetCtrlAttribute(tabHandle, TABPANEL_5_TABLE, ATTR_VISIBLE, 0);
//	if(step == 0) {
	//AÏàµçÑ¹
	for(i=0; i<128; i++)
	{
		x[i] = usWave[0][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(2, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(3, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(4, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//BÏàµçÑ¹
	for(i=0; i<128; i++)
	{
		x[i] = usWave[1][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(5, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(6, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(7, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//CÏàµçÑ¹
	for(i=0; i<128; i++)
	{
		x[i] = usWave[2][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(8, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(9, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(10, 3+i), ATTR_CTRL_VAL, th[i]);
	}
//	} else if(step == 1) {
	//AÏàÏµÍ³µçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = isWave[0][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(11, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(12, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(13, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//BÏàÏµÍ³µçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = isWave[1][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(14, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(15, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(16, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//CÏàÏµÍ³µçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = isWave[2][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(17, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(18, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(19, 3+i), ATTR_CTRL_VAL, th[i]);
	}
//	} else if(step == 2) {
	//---------------------------------
	//AÏà¸ºÔØµçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = ilWave[0][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(20, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(21, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(22, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//BÏà¸ºÔØµçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = ilWave[1][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(23, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(24, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(25, 3+i), ATTR_CTRL_VAL, th[i]);
	}
	//CÏà¸ºÔØµçÁ÷
	for(i=0; i<128; i++)
	{
		x[i] = ilWave[2][127-i]/10.0;
		y[i] = 0.0;
	}
	FFT(x, y, 128);
	//¼ÆËã¸÷´ÎÐ³²¨ÓÐÐ§Öµ£¬ÏàÎ»£¬º¬ÓÐÂÊ
	amp[0] = sqrt((x[1]*x[1] + y[1]*y[1])/2.0)/64;
	if(y[1] != 0) degree[0] = atan(x[1]/y[1])*180/PI;
	else          degree[0] = 90;
	if(degree[0] > 0)
	{
		if(x[1] < 0) degree[0] -= 180;
	}
	else
	{
		if(x[1] > 0) degree[0] += 180;
	}
	th[0]   = 100;
	for(i=2; i<51; i++)
	{
		amp[i-1] = sqrt((x[i]*x[i] + y[i]*y[i])/2.0)/64;
		if(y[i] != 0) degree[i-1] = atan(x[i]/y[i])*180/PI;
		else          degree[i-1] = 90;
		if(degree[i-1] > 0)
		{
			if(x[i] < 0) degree[i-1] -= 180;
		}
		else
		{
			if(x[i] > 0) degree[i-1] += 180;
		}
		th[i-1]  = amp[i-1]*100/amp[0];
	}
	for(i=0; i<50; i++)
	{
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(26, 3+i), ATTR_CTRL_VAL, amp[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(27, 3+i), ATTR_CTRL_VAL, degree[i]);
		SetTableCellAttribute(tabHandle, TABPANEL_5_TABLE, MakePoint(28, 3+i), ATTR_CTRL_VAL, th[i]);
	}
//	}
	SetCtrlAttribute(tabHandle, TABPANEL_5_TABLE, ATTR_VISIBLE, 1);
}
/*---------------------------------------------------------------------------------------
	»ñÈ¡ÉèÖÃ²ÎÊý		 CmtWriteTSQData
-----------------------------------------------------------------------------------------*/
int CVICALLBACK GetPrDataCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_READPRDATA;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------------------
	±£´æÉèÖÃ²ÎÊý		 CmtWriteTSQData
-----------------------------------------------------------------------------------------*/
int CVICALLBACK SetPrDataCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			readPRDataFromTable(&prdata);
			prdata.sysId = devInfo[debugCurDev].id;
			dataType = CMD_WRITEPRDATA;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*--------------------------------------------------------------------------------------
	¶ÁÈ¡Ð£×¼Êý¾Ý		 CmtWriteTSQData
----------------------------------------------------------------------------------------*/
int CVICALLBACK GetCrCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_READCRDATA;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*--------------------------------------------------------------------------------------
	ÏÂ·¢Ð£×¼Êý¾Ý		 CmtWriteTSQData
----------------------------------------------------------------------------------------*/
int CVICALLBACK SetCrCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			readCRDataFromTable(&crdata);
			dataType = CMD_WRITECRDATA;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}

/*--------------------------------------------------------------------------------
	ÏÂ·¢Çå¹ÊÕÏÃüÁî		CmtWriteTSQData
----------------------------------------------------------------------------------*/
int CVICALLBACK CmdClrDoneCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_CLRFDONE;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*------------------------------------------------------------------------------
	ÏÂ·¢Çå¹ÊÕÏ³·ÏúÃüÁî	   CmtWriteTSQData
--------------------------------------------------------------------------------*/
int CVICALLBACK CmdClrCanselCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_CLRFCANSEL;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*------------------------------------------------------------------------------
	ÏÂ·¢ºÏ³äµçµç×èÃüÁî
--------------------------------------------------------------------------------*/
int CVICALLBACK CmdSRDoneCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SRON;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*------------------------------------------------------------------------------
	ÏÂ·¢·Öµç×èÃüÁî
--------------------------------------------------------------------------------*/
int CVICALLBACK CmdSrCanselCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SROFF;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*------------------------------------------------------------------------------
	ÏÂ·¢ºÏÖ÷¿ª¹ØÃüÁî
--------------------------------------------------------------------------------*/
int CVICALLBACK CmdSMDoneCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SMON;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*-----------------------------------------------------------------------------
	ÏÂ·¢·ÖÖ÷¿ª¹ØÃüÁî
-------------------------------------------------------------------------------*/

int CVICALLBACK CmdSMCanselCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SMOFF;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*-----------------------------------------------------------------------------
	ÏÂ·¢¹¤×÷ÃüÁî
-------------------------------------------------------------------------------*/

int CVICALLBACK CmdWorkDoneCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;

	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_WORKDONE;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*-----------------------------------------------------------------------------
	³·Ïú¹¤×÷ÃüÁî
-------------------------------------------------------------------------------*/

int CVICALLBACK CmdWorkCanselCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_WORKCANSEL;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------
	·¢ËÍ¼±Í£ÃüÁî
------------------------------------------------------------------------------*/
int CVICALLBACK CmdEmrDoneCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_EMRDONE;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------
	³·Ïú¼±Í£ÃüÁî
------------------------------------------------------------------------------*/
int CVICALLBACK CmdEmrCanselCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;

	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_EMRCANSEL;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------
	·¢ËÍÇå³ýÊÂ¼þÃüÁî
------------------------------------------------------------------------------*/

int CVICALLBACK clrEventCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
//	u16 dataType;
	int tabHandle;

	switch (event)
	{
		case EVENT_COMMIT:
//			dataType = CMD_CLREVENT;
//			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			clearEventDisplay();
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	¶ÁÈ¡ÊÂ¼þ			  No use
-----------------------------------------------------------------------------*/
int CVICALLBACK readEventCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;

	switch (event)
	{
		case EVENT_COMMIT:
			evtStart = 1;
			evtIndex = 0;

			break;
	}
	return 0;
}
/*--------------------------------------------------------------------------
	¶ÔÊ±ÃüÁî
----------------------------------------------------------------------------*/
int CVICALLBACK checkTimeCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;

	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_CHECKTIME;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------
	ÅÐ¶ÏÁ¬½ÓÊÇ·ñÕý³£
-----------------------------------------------------------------------*/
int CVICALLBACK CheckConnectCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int dataType;
	static u16 c = 0;
	int tabHandle;
	int i;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			for(i=0; i<DEV_NUM; i++)
			{
				if(++devInfo[i].wdtCnt >= DEV_TMO)
				{
					devInfo[i].state = DEV_OFFLINE;
					if(debugCurDev == i) SetCtrlVal(panelHandle, PANEL_LED_ONLINE, 0);
				}
			}

			GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &tabHandle);

			if(evtStart)
			{
				SetCtrlVal(tabHandle, TABPANEL_4_PROGRESSBAR, evtIndex);

				if(evtReady)
				{
					if(evtIndex >= 256) evtStart = 0;
					else
					{
						evtReady = 0;
						dataType = CMD_READEVENT;
						CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
						c = 0;
					}
				}
				else
				{
					if(++c > 50)
					{
						c = 0;
						evtIndex++;
						evtReady = 1;
					}
				}
			}
			else
			{
				SetCtrlVal(tabHandle, TABPANEL_4_PROGRESSBAR, 0);
			}
			break;
	}
	return 0;
}
/*--------------------------------------------------------------------
	ÔØÈëÄ¬ÈÏÐ£×¼²ÎÊý
----------------------------------------------------------------------*/
int CVICALLBACK ldDfCrDatCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
#if(0)
			crdata.ad1cor[0]  = 10030;
			crdata.ad1cor[1]  = 5060;
			crdata.ad1cor[2]  = 5049;
			crdata.ad1cor[3]  = 5035;
			crdata.ad1cor[4]  = 0;
			crdata.ad1cor[5]  = 0;
			crdata.ad1cor[6]  = 0;
			crdata.ad1cor[7]  = 0;

			crdata.ad2cor[0]  = 5780;
			crdata.ad2cor[1]  = 5780;
			crdata.ad2cor[2]  = 5780;
			crdata.ad2cor[3]  = 10135;
			crdata.ad2cor[4]  = 10011;
			crdata.ad2cor[5]  = 10024;
			crdata.ad2cor[6]  = 10030;
			crdata.ad2cor[7]  = 10030;

			crdata.ad3cor[0]  = 500;
			crdata.ad3cor[1]  = 500;
			crdata.ad3cor[2]  = 5780;
			crdata.ad3cor[3]  = 5780;
			crdata.ad3cor[4]  = 5780;
			crdata.ad3cor[5]  = 5780;
			crdata.ad3cor[6]  = 5780;
			crdata.ad3cor[7]  = 5780;
#endif
			//---------------------------------------
			crdata.ad1cor[0]  = -5780;
			crdata.ad1cor[1]  = 10030;
			crdata.ad1cor[2]  = 10030;
			crdata.ad1cor[3]  = 10030;
			crdata.ad1cor[4]  = 10030;
			crdata.ad1cor[5]  = 10030;
			crdata.ad1cor[6]  = 10030;
			crdata.ad1cor[7]  = 0;

			crdata.ad2cor[0]  = -5780;
			crdata.ad2cor[1]  = -5780;
			crdata.ad2cor[2]  = -5780;
			crdata.ad2cor[3]  = -5780;
			crdata.ad2cor[4]  = -5780;
			crdata.ad2cor[5]  = -5780;
			crdata.ad2cor[6]  = -5780;
			crdata.ad2cor[7]  = -5780;

			crdata.ad3cor[0]  = 0;
			crdata.ad3cor[1]  = 0;
			crdata.ad3cor[2]  = 0;
			crdata.ad3cor[3]  = 5060;
			crdata.ad3cor[4]  = 5060;
			crdata.ad3cor[5]  = 5060;
			crdata.ad3cor[6]  = -500;
			crdata.ad3cor[7]  = -500;


			updateCRDataToTable(&crdata);
			break;
	}
	return 0;
}
/*------------------------------------------------------------
	ÔØÈë
--------------------------------------------------------------*/
int CVICALLBACK ldPrDataCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			prdata.k1 = 10224;
			prdata.kp = 32;
			prdata.ki = 4;
			prdata.kr = 85;
			prdata.prki = 220;
			prdata.cts = 40;
			prdata.ctl = 40;
			prdata.pir = 50;
			prdata.cov = 120;
			prdata.csc = 130;
			prdata.inqimag = 0;
			prdata.inqreal = 0;
			prdata.tpf = 100;
			prdata.th3 = 32767;
			prdata.th5 = 32767;
			prdata.th7 = 32767;
			prdata.th9 = 32767;
			prdata.th11 = 32767;
			prdata.th13 = 32767;
			prdata.th15 = 32767;
			prdata.th17 = 32767;
			prdata.th19 = 32767;
			prdata.th21 = 32767;
			prdata.th23 = 32767;
			prdata.th25 = 32767;
			prdata.tha  = 32767;
			prdata.udn  = 80;
			prdata.uov  = 120;
			prdata.unb  = 20;
			prdata.vacRef = 220;
			prdata.sysId = 0x01;
			prdata.msMode = 0x00;
			prdata.punum = 0x01;

			updatePRDataToTable(&prdata);

			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------------------------
	Æô¶¯Í£Ö¹Â¼²¨   CallBack
------------------------------------------------------------------------------------------------*/
int CVICALLBACK recordCheckCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int val;
	int dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &val);
			if(val)
			{
				dataType = CMD_RECORDSTART;
			}
			else
			{
				dataType = CMD_RECORDEND;
			}
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------------
	ÐÞ¸ÄÇúÏßÍ¼   CallBack
---------------------------------------------------------------------------------------------*/
int CVICALLBACK traceCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int traceNum = 0;
	int ctrlarray;
	int ctrl;
	int i;
	int state;
	int color;
	unsigned long u = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			ctrlarray = GetCtrlArrayFromResourceID(panel, CTRLARRAY);
			for(i=0; i<30; i++)
			{
				ctrl = GetCtrlArrayItem(ctrlarray, i);
				GetCtrlVal(panel, ctrl, &state);
				if(state)
				{
					u |= (0x01ul<<i);
					traceNum++;
				}
			}
			trace1Conf = u;
			ClearStripChart(panel,  TABPANEL_3_STRIPCHARTCH1);
			if(traceNum < 1) traceNum = 1;
			SetCtrlAttribute(panel, TABPANEL_3_STRIPCHARTCH1, ATTR_NUM_TRACES, traceNum);

			traceNum = 0;
			for(i=0; i<30; i++)
			{
				ctrl = GetCtrlArrayItem(ctrlarray, i);
				GetCtrlVal(panel, ctrl, &state);
				if(state)
				{
					traceNum++;
					GetCtrlAttribute(panel, ctrl, ATTR_LABEL_BGCOLOR, &color);
					SetTraceAttribute(panel, TABPANEL_3_STRIPCHARTCH1, traceNum, ATTR_TRACE_COLOR, color);
				}
			}

			break;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------------
	ÐÞ¸ÄÇúÏßÍ¼   CallBack
---------------------------------------------------------------------------------------------*/
int CVICALLBACK traceCB2 (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int traceNum = 0;
	int ctrlarray;
	int ctrl;
	int i;
	int state;
	int color;
	unsigned long u = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			ctrlarray = GetCtrlArrayFromResourceID(panel, CTRLARRAY_2);
			for(i=0; i<30; i++)
			{
				ctrl = GetCtrlArrayItem(ctrlarray, i);
				GetCtrlVal(panel, ctrl, &state);
				if(state)
				{
					u |= (0x01ul<<i);
					traceNum++;
				}
			}
			trace2Conf = u;
			ClearStripChart(panel,  TABPANEL_3_STRIPCHARTCH2);
			if(traceNum < 1) traceNum = 1;
			SetCtrlAttribute(panel, TABPANEL_3_STRIPCHARTCH2, ATTR_NUM_TRACES, traceNum);

			traceNum = 0;
			for(i=0; i<30; i++)
			{
				ctrl = GetCtrlArrayItem(ctrlarray, i);
				GetCtrlVal(panel, ctrl, &state);
				if(state)
				{
					traceNum++;
					GetCtrlAttribute(panel, ctrl, ATTR_LABEL_BGCOLOR, &color);
					SetTraceAttribute(panel, TABPANEL_3_STRIPCHARTCH2, traceNum, ATTR_TRACE_COLOR, color);
				}
			}

			break;
	}
	return 0;
}
/*------------------------------------------------------------------------------
	ÊÂ¼þ±í¸ñ»Øµ÷º¯Êý APF×´Ì¬
--------------------------------------------------------------------------------*/
int CVICALLBACK evtTableSelCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	Point p;
	int tabHandle;
	u16 iostate;
	u16 fpgastate[4];
	u32 devstate;
	u16 temp2;
	u16 i;

	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 4, &tabHandle);
			GetActiveTableCell(tabHandle, TABPANEL_4_TABLE, &p);
			if(p.y < 2) break;
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(4, p.y), &iostate);
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(5, p.y), &fpgastate[0]);
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(6, p.y), &fpgastate[1]);
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(7, p.y), &fpgastate[2]);
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(8, p.y), &fpgastate[3]);
			GetTableCellVal(tabHandle,  TABPANEL_4_TABLE, MakePoint(9, p.y), &devstate);

			SetCtrlAttribute(tabHandle, TABPANEL_4_TABLEBIT, ATTR_VISIBLE, 0);  // ±ÜÃâÊý¾Ý¸üÐÂÊ±ÉÁË¸
			for(i=0; i<16; i++)
			{
				///////////////////////////////////////////////////////
				temp2 = (fpgastate[0]&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 2), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 2), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 2), ATTR_TEXT_COLOR, VAL_BLACK);

				//////////////////////////////////////////////////////
				temp2 = (fpgastate[1]&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 4), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 4), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 4), ATTR_TEXT_COLOR, VAL_BLACK);
				/////////////////////////////////////////////////////
				temp2 = (fpgastate[2]&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 6), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 6), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 6), ATTR_TEXT_COLOR, VAL_BLACK);
				//////////////////////////////////////////////////////
				temp2 = (fpgastate[3]&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 8), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 8), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 8), ATTR_TEXT_COLOR, VAL_BLACK);
				//////////////////////////////////////////////////////
				temp2 = (iostate&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 10), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 10), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 10), ATTR_TEXT_COLOR, VAL_BLACK);
				//////////////////////////////////////////////////////
				temp2 = (devstate&(0x0001<<i))>>i;
				SetTableCellValFromIndex(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 12), temp2);
				if(temp2)  SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 12), ATTR_TEXT_COLOR, VAL_RED);
				else	   SetTableCellAttribute(tabHandle, TABPANEL_4_TABLEBIT, MakePoint(i+1, 12), ATTR_TEXT_COLOR, VAL_BLACK);
			}
			SetCtrlAttribute(tabHandle, TABPANEL_4_TABLEBIT, ATTR_VISIBLE, 1);

			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	Ñ¡ÔñÏµÍ³µçÁ÷  CallBack
-----------------------------------------------------------------------------*/
int CVICALLBACK CmdSelDoneCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SELCTSYS;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);

			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	Ñ¡Ôñ¸ºÔØµçÁ÷   CallBack
-----------------------------------------------------------------------------*/
int CVICALLBACK CmdSelCanselCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_SELCTLOAD;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);

			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	²Ù×÷³öÏß¶ÏÂ·Æ÷  CallBack
-----------------------------------------------------------------------------*/
int CVICALLBACK CmdQF3DoneCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_QF3ON;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	²Ù×÷³öÏß¶ÏÂ·Æ÷   CallBack
-----------------------------------------------------------------------------*/
int CVICALLBACK CmdQF3CanselCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_QF3OFF;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	Æô¶¯ºÍÍ£Ö¹ÃüÁî
-----------------------------------------------------------------------------*/
int CVICALLBACK CB_STARTSTOP (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int status = 0;
	int dataType = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_BINARYSWITCH, &status);
			if(status == 1)
			{
				dataType = CMD_AUTOSTART;
				CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			}
			else if(status == 0)
			{
				dataType = CMD_AUTOSTOP;
				CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			}
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	·¢ËÍ¶ÁÈ¡²Î¿¼µçÁ÷ÃüÁî
-----------------------------------------------------------------------------*/
int CVICALLBACK CMB_REFReadCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	u16	dataType = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_READREF;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	Ð´²Î¿¼µçÁ÷
-----------------------------------------------------------------------------*/
void getRefData(ELTSTDat *val);
int CVICALLBACK CMB_REFWriteCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int dataType = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_WRITEREF;
			getRefData(&elTstVal);
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}
/*---------------------------------------------------------------------------
	»ñÈ¡²Î¿¼Ö¸ÁîµçÁ÷
-----------------------------------------------------------------------------*/
ELTSTDat elTstVal;

void getRefData(ELTSTDat *val)
{
	int tabHandle;
	float t;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 6, &tabHandle);
	//ÕýÐò»ù²¨µçÁ÷·ùÖµ
	GetCtrlVal(tabHandle, CurRef_IBaseAmpRef, &t);
	val->prefval = t*10.0f;
	//ÕýÐò»ù²¨µçÁ÷¹¦ÂÊÒòÊý
	GetCtrlVal(tabHandle, CurRef_IBasePfRef, &t);
	val->qrefval = t * 1000.0f;
	//¸ºÐò»ù²¨µçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_INRealRef, &t);
	val->nrefreal = t*10.0f;
	//¸ºÐò»ù²¨µçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_INImagRef, &t);
	val->nrefimag = t*10.0f;
	//ÁãÐò»ù²¨µçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_IZRealRef, &t);
	val->zrefreal = t*10.0f;
	//ÁãÐò»ù²¨µçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_IZImagRef, &t);
	val->zrefimag = t*10.0f;

	//2´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH2RealRef, &t);
	val->th2real = t*10.0f;
	//2´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH2ImagRef, &t);
	val->th2imag = t*10.0f;

	//3´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH3RealRef, &t);
	val->th3real = t*10.0f;
	//3´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH3ImagRef, &t);
	val->th3imag = t*10.0f;

	//4´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH4RealRef, &t);
	val->th4real = t*10.0f;
	//4´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH4ImagRef, &t);
	val->th4imag = t*10.0f;

	//5´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH5RealRef, &t);
	val->th5real = t*10.0f;
	//5´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH5ImagRef, &t);
	val->th5imag = t*10.0f;

	//6´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH6RealRef, &t);
	val->th6real = t*10.0f;
	//6´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH6ImagRef, &t);
	val->th6imag = t*10.0f;

	//7´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH7RealRef, &t);
	val->th7real = t*10.0f;
	//7´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH7ImagRef, &t);
	val->th7imag = t*10.0f;

	//8´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH8RealRef, &t);
	val->th8real = t*10.0f;
	//8´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH8ImagRef, &t);
	val->th8imag = t*10.0f;

	//9´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH9RealRef, &t);
	val->th9real = t*10.0f;
	//9´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH9ImagRef, &t);
	val->th9imag = t*10.0f;

	//10´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH10RealRef, &t);
	val->th10real = t*10.0f;
	//10´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH10ImagRef, &t);
	val->th10imag = t*10.0f;

	//11´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH11RealRef, &t);
	val->th11real = t*10.0f;
	//11´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH11ImagRef, &t);
	val->th11imag = t*10.0f;

	//12´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH12RealRef, &t);
	val->th12real = t*10.0f;
	//12´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH12ImagRef, &t);
	val->th12imag = t*10.0f;

	//13´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH13RealRef, &t);
	val->th13real = t*10.0f;
	//13´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH13ImagRef, &t);
	val->th13imag = t*10.0f;

	//14´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH14RealRef, &t);
	val->th14real = t*10.0f;
	//14´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH14ImagRef, &t);
	val->th14imag = t*10.0f;

	//15´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH15RealRef, &t);
	val->th15real = t*10.0f;
	//15´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH15ImagRef, &t);
	val->th15imag = t*10.0f;

	//16´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH16RealRef, &t);
	val->th16real = t*10.0f;
	//16´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH16ImagRef, &t);
	val->th16imag = t*10.0f;

	//17´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH17RealRef, &t);
	val->th17real = t*10.0f;
	//17´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH17ImagRef, &t);
	val->th17imag = t*10.0f;

	//18´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH18RealRef, &t);
	val->th18real = t*10.0f;
	//18´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH18ImagRef, &t);
	val->th18imag = t*10.0f;

	//19´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH19RealRef, &t);
	val->th19real = t*10.0f;
	//19´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH19ImagRef, &t);
	val->th19imag = t*10.0f;

	//20´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH20RealRef, &t);
	val->th20real = t*10.0f;
	//20´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH20ImagRef, &t);
	val->th20imag = t*10.0f;

	//21´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH21RealRef, &t);
	val->th21real = t*10.0f;
	//21´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH21ImagRef, &t);
	val->th21imag = t*10.0f;

	//22´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH22RealRef, &t);
	val->th22real = t*10.0f;
	//22´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH22ImagRef, &t);
	val->th22imag = t*10.0f;

	//23´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH23RealRef, &t);
	val->th23real = t*10.0f;
	//23´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH23ImagRef, &t);
	val->th23imag = t*10.0f;

	//24´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH24RealRef, &t);
	val->th24real = t*10.0f;
	//24´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH24ImagRef, &t);
	val->th24imag = t*10.0f;

	//25´ÎµçÁ÷Êµ²¿
	GetCtrlVal(tabHandle, CurRef_TH25RealRef, &t);
	val->th25real = t*10.0f;
	//25´ÎµçÁ÷Ðé²¿
	GetCtrlVal(tabHandle, CurRef_TH25ImagRef, &t);
	val->th25imag = t*10.0f;
}
/*---------------------------------------------------------------------------
	ÏÔÊ¾²Î¿¼µçÁ÷
-----------------------------------------------------------------------------*/
void refCurDisplay(ELTSTDat *val)
{
	int tabHandle;
	float t;

	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 6, &tabHandle);
	//ÕýÐò»ù²¨µçÁ÷·ùÖµ
	t = val->prefval*0.1f;
	SetCtrlVal(tabHandle, CurRef_IBaseAmpCur, t);

	//ÕýÐò»ù²¨µçÁ÷¹¦ÂÊÒòÊý
	t = val->qrefval * 0.001f;
	SetCtrlVal(tabHandle, CurRef_IBasePfCur, t);

	//¸ºÐò»ù²¨µçÁ÷Êµ²¿
	t = val->nrefreal*0.1f;
	SetCtrlVal(tabHandle, CurRef_INRealCur, t);
	//¸ºÐò»ù²¨µçÁ÷Ðé²¿
	t = val->nrefimag*0.1f;
	SetCtrlVal(tabHandle, CurRef_INImagCur, t);

	//ÁãÐò»ù²¨µçÁ÷Êµ²¿
	t = val->zrefreal*0.1f;
	SetCtrlVal(tabHandle, CurRef_IZRealCur, t);
	//ÁãÐò»ù²¨µçÁ÷Ðé²¿
	t = val->zrefimag*0.1f;
	SetCtrlVal(tabHandle, CurRef_IZImagCur, t);

	//2´ÎµçÁ÷Êµ²¿
	t = val->th2real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH2RealCur, t);
	//2´ÎµçÁ÷Ðé²¿
	t = val->th2imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH2ImagCur, t);

	//3´ÎµçÁ÷Êµ²¿
	t = val->th3real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH3RealCur, t);
	//3´ÎµçÁ÷Ðé²¿
	t = val->th3imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH3ImagCur, t);

	//4´ÎµçÁ÷Êµ²¿
	t = val->th4real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH4RealCur, t);
	//4´ÎµçÁ÷Ðé²¿
	t = val->th4imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH4ImagCur, t);

	//5´ÎµçÁ÷Êµ²¿
	t = val->th5real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH5RealCur, t);
	//5´ÎµçÁ÷Ðé²¿
	t = val->th5imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH5ImagCur, t);

	//6´ÎµçÁ÷Êµ²¿
	t = val->th6real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH6RealCur, t);
	//6´ÎµçÁ÷Ðé²¿
	t = val->th6imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH6ImagCur, t);

	//7´ÎµçÁ÷Êµ²¿
	t = val->th7real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH7RealCur, t);
	//7´ÎµçÁ÷Ðé²¿
	t = val->th7imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH7ImagCur, t);

	//8´ÎµçÁ÷Êµ²¿
	t = val->th8real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH8RealCur, t);
	//8´ÎµçÁ÷Ðé²¿
	t = val->th8imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH8ImagCur, t);

	//9´ÎµçÁ÷Êµ²¿
	t = val->th9real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH9RealCur, t);
	//9´ÎµçÁ÷Ðé²¿
	t = val->th9imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH9ImagCur, t);

	//10´ÎµçÁ÷Êµ²¿
	t = val->th10real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH10RealCur, t);
	//10´ÎµçÁ÷Ðé²¿
	t = val->th10imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH10ImagCur, t);

	//11´ÎµçÁ÷Êµ²¿
	t = val->th11real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH11RealCur, t);
	//11´ÎµçÁ÷Ðé²¿
	t = val->th11imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH11ImagCur, t);

	//12´ÎµçÁ÷Êµ²¿
	t = val->th12real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH12RealCur, t);
	//12´ÎµçÁ÷Ðé²¿
	t = val->th12imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH12ImagCur, t);

	//13´ÎµçÁ÷Êµ²¿
	t = val->th13real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH13RealCur, t);
	//13´ÎµçÁ÷Ðé²¿
	t = val->th13imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH13ImagCur, t);

	//14´ÎµçÁ÷Êµ²¿
	t = val->th14real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH14RealCur, t);
	//14´ÎµçÁ÷Ðé²¿
	t = val->th14imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH14ImagCur, t);

	//15´ÎµçÁ÷Êµ²¿
	t = val->th15real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH15RealCur, t);
	//15´ÎµçÁ÷Ðé²¿
	t = val->th15imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH15ImagCur, t);

	//16´ÎµçÁ÷Êµ²¿
	t = val->th16real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH16RealCur, t);
	//16´ÎµçÁ÷Ðé²¿
	t = val->th16imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH16ImagCur, t);

	//17´ÎµçÁ÷Êµ²¿
	t = val->th17real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH17RealCur, t);
	//17´ÎµçÁ÷Ðé²¿
	t = val->th17imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH17ImagCur, t);

	//18´ÎµçÁ÷Êµ²¿
	t = val->th18real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH18RealCur, t);
	//18´ÎµçÁ÷Ðé²¿
	t = val->th18imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH18ImagCur, t);

	//19´ÎµçÁ÷Êµ²¿
	t = val->th19real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH19RealCur, t);
	//19´ÎµçÁ÷Ðé²¿
	t = val->th19imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH19ImagCur, t);

	//20´ÎµçÁ÷Êµ²¿
	t = val->th20real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH20RealCur, t);
	//20´ÎµçÁ÷Ðé²¿
	t = val->th20imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH20ImagCur, t);

	//21´ÎµçÁ÷Êµ²¿
	t = val->th21real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH21RealCur, t);
	//21´ÎµçÁ÷Ðé²¿
	t = val->th21imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH21ImagCur, t);

	//22´ÎµçÁ÷Êµ²¿
	t = val->th22real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH22RealCur, t);
	//22´ÎµçÁ÷Ðé²¿
	t = val->th22imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH22ImagCur, t);

	//23´ÎµçÁ÷Êµ²¿
	t = val->th23real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH23RealCur, t);
	//23´ÎµçÁ÷Ðé²¿
	t = val->th23imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH23ImagCur, t);

	//24´ÎµçÁ÷Êµ²¿
	t = val->th24real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH24RealCur, t);
	//24´ÎµçÁ÷Ðé²¿
	t = val->th24imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH24ImagCur, t);

	//25´ÎµçÁ÷Êµ²¿
	t = val->th25real*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH25RealCur, t);
	//25´ÎµçÁ÷Ðé²¿
	t = val->th25imag*0.1f;
	SetCtrlVal(tabHandle, CurRef_TH25ImagCur, t);
}

int CVICALLBACK STSSW_CB (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			switch(control)
			{
				case PANEL_SW_CMDSTSQF1_ON:
					dataType = CMD_STSQF1ON;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSQF1_OFF:
					dataType = CMD_STSQF1OFF;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSQF2_ON:
					dataType = CMD_STSQF2ON;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSQF2_OFF:
					dataType = CMD_STSQF2OFF;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSCUT_ON:
					dataType = CMD_STSCUTON;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSCUT_OFF:
					dataType = CMD_STSCUTOFF;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSKM1_ON:
					dataType = CMD_STSKM1ON;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
				case PANEL_SW_CMDSTSKM1_OFF:
					dataType = CMD_STSKM1OFF;
					CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
					break;
			}
			break;
	}
	return 0;
}


/*---------------------------------------------------------------------------------------------------
	×´Ì¬¼ì²é   Á½ÖÖ×´Ì¬ ÁÁÃð
-----------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------
	Panel ¿ØÖÆ
-----------------------------------------------------------------------------------------------------*/
int CVICALLBACK Scope (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int error;
	switch (event)
	{
		case EVENT_COMMIT:
			if (scopePanelHandle <= 0) errChk(scopePanelHandle = LoadPanel(0, "APF_DEBUG.uir", PANEL_DCF));
			errChk(DisplayPanel(scopePanelHandle));

			//errChk(DisplayPanel(scopePanelHandle2));

			break;
	}
	return 0;
Error:
	return error;
}


int CVICALLBACK iDebugPanel (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int error = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if (swPanelHandle <= 0) errChk(swPanelHandle = LoadPanel(0, "APF_DEBUG.uir", PANEL_SW));
			if (panelHandle <= 0) errChk(panelHandle = LoadPanel(0, "APF_DEBUG.uir", PANEL));
			errChk(DisplayPanel(panelHandle));
			errChk(DisplayPanel(swPanelHandle));
			// errChk(DisplayPanel(debugHandle));
			break;
		default:
			break;
	}

	return 0;
Error:
	MessagePopup("iDebugPanel", "Error  swicth !");
	return error;
}

int CVICALLBACK SysConf (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int error = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if (portHandle <= 0) errChk(portHandle = LoadPanel(0, "APF_DEBUG.uir", PANEL_DCF));
			errChk(DisplayPanel(portHandle));
			// DisplayPanel(portHandle);
			break;
	}
	return 0;
Error:
	return error;
}

int CVICALLBACK CloseSysConf (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//HidePanel(panel);
			DiscardPanel(panel);
			QuitUserInterface(0);
			break;
	}
	return 0;
}



int CVICALLBACK DeviceInfo (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			break;
	}
	return 0;
}

int CVICALLBACK EnventLog (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int activePanel;
	switch (event)
	{
		case EVENT_COMMIT:
			activePanel = GetActivePanel();
			SetCtrlVal(disPanelHandle, PANEL_A_Num_PanelID, activePanel);

			break;
	}
	return 0;
}

int CVICALLBACK AutoStop (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK AutoStart (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}


int CVICALLBACK InitSysLdDatCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// irt_data;
			for(int i=0; i<6; i++)
			{
				irt_data.example_n[i]=32767;
				irt_data.example_r1[i]=1;

				// printf("%d irt_data IS %d\n",i,irt_data.example_n[i]) ;
			}
			SetPRDataFromDCP(&irt_data);
			break;
	}
	return 0;
}

int CVICALLBACK GetSysPrDataCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			dataType = CMD_GETADATA;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			break;
	}
	return 0;
}

int CVICALLBACK SetSysDatCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	u16 dataType;
	switch (event)
	{
		case EVENT_COMMIT:
			readPRDataFromDCP(&irt_data); //Read the DCP panel Numric Ring1 Ring2 Data
			// irt_data.sysId = devInfo[debugCurDev].id;
			dataType = CMD_SENDADATA ;
			CmtWriteTSQData(tsqTxId, &dataType, 1, TSQ_INFINITE_TIMEOUT, NULL);
			//	printf("Size of ART_DataType: %lu bytes\n", sizeof(ART_DataType));
			// 	printf("Size of RTDataType: %lu bytes,\n", sizeof(RTDataType));

			break;
	}
	return 0;
}
/********************************************************
* Function name :updateARTdisplay
* Description : updateARTdisplay ¸üÐÂÄÜÁ¿½»»»»úV1.0
* Parameter : ART_DataType   ¸üÐÂÄÜÁ¿½»»»»úV1.0µÄ disPanelHandle
**********************************************************/
u16 getMaxTemp(const u16 *arr, int size)
{
	if (arr == NULL || size <= 0)
	{
		// Handle error: empty or null array
		fprintf(stderr, "Error: Invalid array or size passed to getMaxTemp.\n");
		return 0; // Or USHRT_MIN to indicate an invalid state, depending on context
	}
	u16 maxVal = arr[0]; // Initialize with the first element
	for (int i = 1; i < size; i++)   // Start from the second element
	{
		if (arr[i] > maxVal)
		{
			maxVal = arr[i];
		}
	}
	return maxVal;
}
/*
	¸üÐÂPANEL_AµÄ ×´Ì¬ ,
*/

void updateARTdisplay(ART_DataType *irt_data,int disPanelHandle)
{
	float atemp=114; // test data Rmove
	int i, j;
	int temp2;
	double ik;
	static u16 ubit[6] = {0, 0, 0, 0, 0, 0};
	u16 ubit_t[6];
	s8 buf[32];

	// Clear the Display ClearARTdata(disPanelHandle); Display The Need Data  STS High Voltage Input
	// Voltage
	atemp = irt_data->stsvol[0]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_VA,ATTR_CTRL_VAL,atemp);
	atemp = irt_data->stsvol[1]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_VB,ATTR_CTRL_VAL,atemp);
	atemp = irt_data->stsvol[2]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_VC,ATTR_CTRL_VAL,atemp);
	// Current
	ik = irt_data->stscur[0];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_IA, ATTR_CTRL_VAL,ik);
	ik = irt_data->stscur[1];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_IB,ATTR_CTRL_VAL, ik);
	ik = irt_data->stscur[2];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_IC,ATTR_CTRL_VAL, ik);

	// Power : System and Load Active/Reactive Power Update.
	atemp = irt_data->stsp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_P1,ATTR_CTRL_VAL,ik);
	atemp = irt_data->stsq/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_Q1, ATTR_CTRL_VAL,ik);
	atemp = irt_data->stspf/1000.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_PF1,ATTR_CTRL_VAL,ik);
	// ÎÂ¶È
	atemp=  irt_data->stsTemp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_InPort_T, ATTR_CTRL_VAL,atemp);


	//  High Voltage Input AC1
	// Voltage
	atemp = irt_data->havol[0]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VA_IN, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->havol[1]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VB_IN,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->havol[2]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VC_IN, ATTR_CTRL_VAL,atemp);

	// Current
	ik = irt_data->hacur[0];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IA_IN, ATTR_CTRL_VAL,ik);
	ik = irt_data->hacur[1];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IB_IN,ATTR_CTRL_VAL, ik);
	ik = irt_data->hacur[2];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IC_IN,ATTR_CTRL_VAL, ik);

	// Power : System and Load Active/Reactive Power Update.
	atemp = irt_data->hap/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_P_IN,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->haq/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_Q_IN,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->hapf/1000.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_PF_IN, ATTR_CTRL_VAL,atemp);
//High Voltage Input AC2
	// Voltage
	atemp = irt_data->hbvol[0]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VA_IN, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->hbvol[1]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VB_IN, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->hbvol[2]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VC_IN,ATTR_CTRL_VAL, atemp);

	// Current
	ik = irt_data->hbcur[0];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IA_IN, ATTR_CTRL_VAL,ik);
	ik = irt_data->hbcur[1];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IB_IN, ATTR_CTRL_VAL,ik);
	ik = irt_data->hbcur[2];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IC_IN, ATTR_CTRL_VAL,ik);

	// Power : System and Load Active/Reactive Power Update.
	atemp = irt_data->hbp/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_P_IN, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->hbq/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_Q_IN,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->hbpf/1000.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_PF_IN,ATTR_CTRL_VAL, atemp);

	// Low Voltage Input
	// Voltage AC1 && AC2
	atemp = irt_data->lavol[0]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VA, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->lavol[1]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VB,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lavol[2]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_VC, ATTR_CTRL_VAL,atemp);

	atemp = irt_data->lbvol[0]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VA, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->lbvol[1]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VB,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbvol[2]/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_VC, ATTR_CTRL_VAL,atemp);

	// Current AC1 && AC2
	atemp = irt_data->lacur[0];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IA,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lacur[1];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IB, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->lacur[2];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IC, ATTR_CTRL_VAL,atemp);

	atemp = irt_data->lbcur[0];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IA,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbcur[1];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IB, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->lbcur[2];
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IC,ATTR_CTRL_VAL, atemp);

	// Power : System and Load Active/Reactive Power Update.   lack the Temperture And Frequency
	atemp = irt_data->lap/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_P, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->laq/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_Q,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lapf/1000.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_PF,ATTR_CTRL_VAL,atemp);

	atemp = irt_data->lbp/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_P,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbq/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_Q,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbpf/1000.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_PF, ATTR_CTRL_VAL, atemp);

	// Udcp && Udcn Votage	  Cp+ Cn-   && IDC output Current
	atemp = irt_data->laudcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_Udcp,ATTR_CTRL_VAL,  atemp);
	atemp = irt_data->laudcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_Udcn,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->lbudcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_Udcp,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbudcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_Udcn,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->ladccur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_IOut,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbdccur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_IOut, ATTR_CTRL_VAL,atemp);


	//ÆµÂÊ
	atemp = irt_data->lafreq/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC1_Freq,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->lbfreq/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_AC2_Freq, ATTR_CTRL_VAL,atemp);

	//	µçÈÝ DC1  DC2 DC2  DC3 DC4
	atemp = irt_data->dc1udcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_Udcp,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc1udcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_Udcn,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->dc2udcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_Udcp,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc2udcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_Udcn, ATTR_CTRL_VAL,atemp);

	atemp = irt_data->dc3udcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_Udcp,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc3udcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_Udcn,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->dc4udcp;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_Udcp,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc4udcn;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_Udcn, ATTR_CTRL_VAL,atemp);



	// Êä³öµçÑ¹ DC1
	atemp = irt_data->dc1outVolA/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_VAOUT, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc1outVolB/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_VBOUT,ATTR_CTRL_VAL, atemp);
	// ½øµçÁ÷   DC1
	atemp = irt_data->dc1InCur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_CurIN,ATTR_CTRL_VAL, atemp);
	// ÊäµçÁ÷   DC1
	atemp = irt_data->dc1outCurA;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_CurAOUT, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc1outCurB;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_CurBOUT,ATTR_CTRL_VAL, atemp);

	// Êä³öµçÑ¹ DC2
	atemp = irt_data->dc2outVolA/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_VAOUT, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc2outVolB/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_VBOUT,ATTR_CTRL_VAL, atemp);
	// ½øµçÁ÷   DC2
	atemp = irt_data->dc2InCur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_CurIN,ATTR_CTRL_VAL, atemp);
	// ÊäµçÁ÷   DC2
	atemp = irt_data->dc2outCurA;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_CurAOUT,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc2outCurB;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_CurBOUT,ATTR_CTRL_VAL, atemp);

	// Êä³öµçÑ¹ DC3
	atemp = irt_data->dc3outVolA/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_VAOUT, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc3outVolB/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_VBOUT,ATTR_CTRL_VAL, atemp);
	// ½øµçÁ÷   DC3
	atemp = irt_data->dc3InCur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_CurIN,ATTR_CTRL_VAL, atemp);
	// ÊäµçÁ÷   DC3
	atemp = irt_data->dc3outCurA;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_CurAOUT, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc3outCurB;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_CurBOUT,ATTR_CTRL_VAL, atemp);

	// Êä³öµçÑ¹ DC4
	atemp = irt_data->dc4outVolA/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_VAOUT,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc4outVolB/10.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_VBOUT, ATTR_CTRL_VAL,atemp);
	// ½øµçÁ÷   DC4
	atemp = irt_data->dc4InCur;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_CurIN, ATTR_CTRL_VAL,atemp);
	// ÊäµçÁ÷   DC4
	atemp = irt_data->dc4outCurA;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_CurAOUT,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc4outCurB;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_CurBOUT, ATTR_CTRL_VAL,atemp);

	// ¹¦ÂÊ
	atemp = irt_data->dc1outPA/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_PA, ATTR_CTRL_VAL,atemp);
	atemp = irt_data->dc1outPB/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_PB,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->dc2outPA/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_PA,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc2outPB/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_PB,ATTR_CTRL_VAL, atemp);

	atemp = irt_data->dc3outPA/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_PA,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc3outPB/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_PB, ATTR_CTRL_VAL,atemp);

	atemp = irt_data->dc4outPA/100.0;
	SetCtrlAttribute(disPanelHandle,  PANEL_A_Num_DC4_PA,ATTR_CTRL_VAL, atemp);
	atemp = irt_data->dc4outPB/100.0;
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_PB, ATTR_CTRL_VAL,atemp);


	//  Temperature
	atemp= getMaxTemp(irt_data->dc1Temp, sizeof(irt_data->dc1Temp) / sizeof(irt_data->dc1Temp[0]));
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC1_Temp,ATTR_CTRL_VAL, atemp);
	atemp= getMaxTemp(irt_data->dc2Temp, sizeof(irt_data->dc2Temp) / sizeof(irt_data->dc2Temp[0]));
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC2_Temp, ATTR_CTRL_VAL,atemp);
	atemp= getMaxTemp(irt_data->dc3Temp, sizeof(irt_data->dc3Temp) / sizeof(irt_data->dc3Temp[0]));
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC3_Temp,ATTR_CTRL_VAL, atemp);
	atemp= getMaxTemp(irt_data->dc4Temp, sizeof(irt_data->dc4Temp) / sizeof(irt_data->dc4Temp[0]));
	SetCtrlAttribute(disPanelHandle, PANEL_A_Num_DC4_Temp,ATTR_CTRL_VAL, atemp);

	// @Note: temperture is Get max Data or Arrage Date
	//
	//  for (int i = 0; i < 4; i++)
	//{
	//	printf("**** dc1ErrSta[%d] = %#x \n", i, irt_data-> dc1ErrSta[i]);
	//}

	// 	LED
	// STS
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_IN_K1,ATTR_CTRL_VAL, irt_data->hasw != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_IN_K2,ATTR_CTRL_VAL, irt_data->hasw != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_K2, ATTR_CTRL_VAL,irt_data->hbsw != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_K1,ATTR_CTRL_VAL, irt_data->stssw != 1);

	// ¸ßÑ¹
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_KV_K1, ATTR_CTRL_VAL,irt_data->hasw != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_KV_K2, ATTR_CTRL_VAL,irt_data->hbsw != 0);
	// µÍÑ¹
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_K1, ATTR_CTRL_VAL,irt_data->laErrSta[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC2_K1, ATTR_CTRL_VAL,irt_data->lbErrSta != 0);


	// DC ½ø¿Ú	ÐèÒªÌí¼ÓIO±êÊ¶
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1InQF1, ATTR_CTRL_VAL,irt_data->iostate[0] != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2InQF1,ATTR_CTRL_VAL, irt_data->iostate[0] != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3InQF1, ATTR_CTRL_VAL,irt_data->iostate[0] != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4InQF1, ATTR_CTRL_VAL,irt_data->iostate[0] != 1);
	// DC ³ö¿Ú
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1OutQF1, ATTR_CTRL_VAL,irt_data->iostate[0] != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2OutQF1, ATTR_CTRL_VAL,irt_data->iostate[0]!= 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3OutQF1,ATTR_CTRL_VAL, irt_data->iostate[0]!= 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4OutQF1,ATTR_CTRL_VAL, irt_data->iostate[0] != 1);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4OutQF2,ATTR_CTRL_VAL, irt_data->iostate[0] != 1);

	// ·´À¡  -----------------------------------------
	// STS
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_IN_K1FB,ATTR_CTRL_VAL, irt_data->stsErrSta[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_IN_K2FB, ATTR_CTRL_VAL,irt_data->stsErrSta[1] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_K2FB,ATTR_CTRL_VAL, irt_data->stsErrSta[2] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_K1FB,ATTR_CTRL_VAL, irt_data->stsErrSta[3] != 0);

	// ¸ßÑ¹		 Ê¹ÓÃ±¸ÓÃ×Ö			OK
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_KV_K1FB, ATTR_CTRL_VAL,irt_data->laBw[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_KV_K2FB, ATTR_CTRL_VAL, irt_data->lbBw[1] != 0);
	// µÍÑ¹
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_K1FB,ATTR_CTRL_VAL, irt_data->laBw[1] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC2_K1FB, ATTR_CTRL_VAL,irt_data->lbBw[0] != 0);


	// DC ½ø¿Ú
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1InQF1FB, ATTR_CTRL_VAL,irt_data->dc1ErrSta[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2InQF1FB,ATTR_CTRL_VAL, irt_data->dc2ErrSta[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3InQF1FB,ATTR_CTRL_VAL, irt_data->dc3ErrSta[0] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4InQF1FB, ATTR_CTRL_VAL,irt_data->dc4ErrSta[0] != 0);
	// DC ³ö¿Ú
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1OutQF1FB,ATTR_CTRL_VAL, irt_data->dc1ErrSta[2] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2OutQF1FB, ATTR_CTRL_VAL,irt_data->dc2ErrSta[2] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3OutQF1FB, ATTR_CTRL_VAL,irt_data->dc3ErrSta[2] != 0);

	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4OutQF1FB, ATTR_CTRL_VAL,irt_data->dc4ErrSta[2] != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4OutQF2FB,ATTR_CTRL_VAL, irt_data->dc4ErrSta[3] != 0);

	//¹¤×÷·´À¡  OK----------------------------------------
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_WORK, ATTR_CTRL_VAL,irt_data->stswrkSta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_WORK, ATTR_CTRL_VAL,irt_data->lawrksta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC2_WORK,ATTR_CTRL_VAL, irt_data->lbwrksta != 0);

	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_WORK,ATTR_CTRL_VAL, irt_data->lawrksta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC2_WORK,ATTR_CTRL_VAL, irt_data->lbwrksta != 0);

	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1_WORK,ATTR_CTRL_VAL, irt_data->dc1wrkSta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2_WORK,ATTR_CTRL_VAL, irt_data->dc2wrkSta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3_WORK,ATTR_CTRL_VAL, irt_data->dc3wrkSta != 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4_WORK, ATTR_CTRL_VAL,irt_data->dc4wrkSta != 0);
	//¹ÊÕÏ·´À¡   È¡·´ Óë¹¤×÷·´À¡»¥³â
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_STS_Error,ATTR_CTRL_VAL, irt_data->stswrkSta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_WORK,ATTR_CTRL_VAL, irt_data->lawrksta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC2_WORK, ATTR_CTRL_VAL,irt_data->lbwrksta = 0);

	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_Error,ATTR_CTRL_VAL, irt_data->lawrksta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_AC1_Error,ATTR_CTRL_VAL, irt_data->lbwrksta = 0);

	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC1_Error,ATTR_CTRL_VAL, irt_data->dc1wrkSta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC2_Error,ATTR_CTRL_VAL, irt_data->dc2wrkSta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC3_Error,ATTR_CTRL_VAL, irt_data->dc3wrkSta = 0);
	SetCtrlAttribute(disPanelHandle, PANEL_A_Led_DC4_Error, ATTR_CTRL_VAL,irt_data->dc4wrkSta = 0);

}


int CVICALLBACK UpdateOnSwitchStates(ART_DataType *irt_data,int disPanelHandle)
{
	return 0   ;
}

int CVICALLBACK QuitWave (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(panel);
			// DiscardPanel (panel);
			break;
	}
	return 0;
}
/*
	¹¤×÷×´Ì¬¼ì²â
*/
int CVICALLBACK STSQFCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}

int CVICALLBACK AC1WorkCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}

int CVICALLBACK AC2WorkCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}

int CVICALLBACK DC1WorkCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}

int CVICALLBACK GetID (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int activePanel;
	switch (event)
	{
		case EVENT_COMMIT:
			activePanel = GetActivePanel();
			SetCtrlAttribute(portHandle, PANEL_DCF_Num_PanelID, ATTR_CTRL_VAL,activePanel);

			break;
	}
	return 0;
}

int CVICALLBACK iGetID (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int activePanel;
	unsigned char sourceBuffer[WAVE_LENGTH * 2];
	short waveData[WAVE_LENGTH];
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			// updateg();
			activePanel = GetActivePanel();
			SetCtrlVal(scopePanelHandle, PANEL_Scop_Num_PanelID, activePanel);
			break;
	}
	return 0;
}

int CVICALLBACK QuitWave2 (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK iscopeDisTypeCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			break;
	}
	return 0;
}

static double windowSize = 100.0;
static double maxTime = 3000.0;
int CVICALLBACK Timescales1(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	double sliderValue;
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal(panel, control, &sliderValue);
			if (sliderValue < windowSize)
				sliderValue = windowSize;
			if (sliderValue > maxTime)
				sliderValue = maxTime;
			SetAxisRange(
				scopePanelHandle,
				PANEL_Scop_GRAPH,
				VAL_MANUAL, sliderValue - windowSize, sliderValue,
				VAL_AUTOSCALE, 0.0, 0.0);
			SetCtrlAttribute(panel, control, ATTR_MIN_VALUE, windowSize);
			SetCtrlAttribute(panel, control, ATTR_MAX_VALUE, maxTime);

			break;
	}
	return 0;
}
int CVICALLBACK Timescales2(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	double sliderValue;
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal(panel, control, &sliderValue);
			SetAxisRange(
				scopePanelHandle,
				PANEL_Scop_GRAPH,
				VAL_MANUAL, 0.0, sliderValue,
				VAL_AUTOSCALE, 0.0, 0.0);
			SetAxisRange(
				scopePanelHandle,
				PANEL_Scop_STRIPCHART,
				VAL_MANUAL, 0.0, sliderValue,
				VAL_AUTOSCALE, 0.0, 0.0);
			break;
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
/* Plots the fractal based on the viewable area of the graph.                 */
/*----------------------------------------------------------------------------*/
/// HIFN Plots the fractal based on the viewable area of the graph.
/// HIRET Indicates whether the function was successful.
int DoPlot(int panel, int control)
{
	int i, j, iteration, maxIt = 0;
	int numX, numY, error = 0;
	double x, y, xMin, xMax, yMin, yMax, xStep, yStep;
	int *zArray = NULL;
	static int isPlotting = 0;


	if (isPlotting)
	{
		return 0;
	}

	isPlotting = 1;
	SetWaitCursor(1);


	do
	{
		errChk(ProcessDrawEvents());
		errChk(GetCtrlAttribute(scopePanelHandle, PANEL_Scop_GRAPH, ATTR_PLOT_AREA_WIDTH, &numX));
		errChk(GetCtrlAttribute(scopePanelHandle, PANEL_Scop_GRAPH, ATTR_PLOT_AREA_HEIGHT, &numY));

		nullChk(zArray = malloc(numX * numY * sizeof(int)));

		errChk(GetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_BOTTOM_XAXIS, NULL, &xMin, &xMax));
		errChk(GetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_LEFT_YAXIS, NULL, &yMin, &yMax));

		errChk(SetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, (xMax + xMin) / 2));
		errChk(SetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, (yMax + yMin) / 2));

		xStep = (xMax - xMin) / numX;
		yStep = (yMax - yMin) / numY;
		plotHandle = PlotScaledIntensity(scopePanelHandle,
										 PANEL_Scop_GRAPH,
										 zArray, numX, numY, VAL_INTEGER,
										 (yMax - yMin) / numY, yMin, (xMax - xMin) / numX+10, xMin,
										 colorArray, VAL_BLACK, maxIterations, 1, 1);
	}
	while(0);

Error:

	if (zArray)
	{
		free(zArray);
		zArray = NULL;
	}

	SetWaitCursor(0);
	isPlotting = 0;

	return error;
}
// Í¼ÏñËõ·Å´¦Àí
int CVICALLBACK GRAPHCB(int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	// Static variable definitions
	static int panning = 0;              //  Panning state flag
	static int lastEventTime = 0;

	//  Local variable definitions
	double xMin, xMax, yMin, yMax;       //  Axis range values
	int replot;                          //  Replot flag
	int currentTime = Timer();           //  Current time
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:        // ÓÒ¼üËÉ¿ª
			if (panning)
			{

				GetCtrlVal(panel, control, &replot);
				if (replot && (currentTime - lastEventTime > 0.1))
				{
					DoPlot(panel, control);
					lastEventTime = currentTime;
				}
				//   ·Å´ó·¶Î§
				GetAxisScalingMode(panel, control, VAL_BOTTOM_XAXIS, 0, &xMin, &xMax);
				GetAxisScalingMode(panel, control, VAL_LEFT_YAXIS, 0, &yMin, &yMax);

				//  (X-axis center)
				SetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, (xMax + xMin) / 2);
				// (Y-axis center)
				SetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, (yMax + yMin) / 2);

				panning = 0;  // ÍË³öÆ½ÒÆ
			}
			break;
		case EVENT_AXIS_VAL_CHANGE:      // ×ø±êÖá´¦Àí

			GetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, &replot);
			//·À¶¶
			if (replot && (currentTime - lastEventTime > 0.1))
			{
				DoPlot(panel, control);
				lastEventTime = currentTime;
			}
			break;

		case EVENT_ZOOM:                 //Ëõ·ÅÏà¹Ø
			GetCtrlVal(scopePanelHandle, PANEL_Scop_GRAPH, &replot);

			switch (eventData1)
			{
				case ZOOM_IN:            // ·Å´ó
				case ZOOM_OUT:           // ËõÐ¡
					// ·À¶¶
					if (replot && (currentTime - lastEventTime > 0.1))
					{
						DoPlot(panel, control);
						lastEventTime = currentTime; // ×îºóµÄÊ±¼ä / Update last event time
					}
					break;

				case ZOOM_PAN:           // Æ½ÒÆ/ Pan mode
					panning = 1;
					SetMouseCursor(VAL_CLOSED_HAND_CURSOR);
					break;

				case ZOOM_RESTORE:       //  Restore original view
					if (replot)
					{
						DoPlot(panel, control);  //ÖØ»æ
					}
					break;

				default:                 //  Other cases
					break;
			}
			break;
	}

	return 0;
}

int CVICALLBACK RestScopeZ1(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 100);
			SetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -32768, 32768);
			SetWaitCursor(1);
			DoPlot(scopePanelHandle, PANEL_Scop_GRAPH);
			SetWaitCursor(0);
			break;
	}
	return 0;
}
/*
int CVICALLBACK RestScopeZ2(int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 100);
			SetAxisScalingMode(scopePanelHandle, PANEL_Scop_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -32768, 32768);
			SetWaitCursor(1);
			DoPlot(scopePanelHandle, PANEL_Scop_GRAPH);
			SetWaitCursor(0);
			break;
	}
	return 0;
}
*/
// Í¼ÏñËõ·Å´¦Àí½áÊø
//»æÍ¼
/*
int CVICALLBACK Ppoint(int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		{
			#define MAX_POINTS 1000
			#define AMPLITUDE 32760.0
			#define FREQUENCY 0.1

			static double timeArray[MAX_POINTS];
			static double data1[MAX_POINTS], data2[MAX_POINTS], data3[MAX_POINTS];
			static int currentIndex = 0;
			static double currentTime = 0.0;
			static int plotHandle1 = -1, plotHandle2 = -1, plotHandle3 = -1;

			// ??????
			double omega_t = 2 * PI * FREQUENCY * currentTime;
			timeArray[currentIndex] = currentTime;
			data1[currentIndex] = AMPLITUDE * sin(omega_t);
			data2[currentIndex] = AMPLITUDE * sin(omega_t + 2*PI/3);
			data3[currentIndex] = AMPLITUDE * sin(omega_t + 4*PI/3);

			currentIndex++;
			currentTime += 0.1;
			if (plotHandle1 >= 0) DeleteGraphPlot(panel, PANEL_Scop_GRAPH, plotHandle1, VAL_DELAYED_DRAW);
			if (plotHandle2 >= 0) DeleteGraphPlot(panel, PANEL_Scop_GRAPH, plotHandle2, VAL_DELAYED_DRAW);
			if (plotHandle3 >= 0) DeleteGraphPlot(panel, PANEL_Scop_GRAPH, plotHandle3, VAL_DELAYED_DRAW);


			plotHandle1 = PlotXY(panel, PANEL_Scop_GRAPH, timeArray, data1, currentIndex,
								VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE,
								VAL_SOLID, 1, VAL_RED);

			plotHandle2 = PlotXY(panel, PANEL_Scop_GRAPH, timeArray, data2, currentIndex,
								VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE,
								VAL_SOLID, 1, VAL_GREEN);

			plotHandle3 = PlotXY(panel, PANEL_Scop_GRAPH, timeArray, data3, currentIndex,
								VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE,
								VAL_SOLID, 1, VAL_BLUE);


			if (currentIndex >= MAX_POINTS) {

				for (int i = 0; i < MAX_POINTS - 1; i++) {
					timeArray[i] = timeArray[i + 1];
					data1[i] = data1[i + 1];
					data2[i] = data2[i + 1];
					data3[i] = data3[i + 1];
				}
				currentIndex = MAX_POINTS - 1;
			}

			// ????X?????
			if (currentIndex > 50) {  // ????50??
				SetAxisScalingMode(panel, PANEL_Scop_GRAPH, VAL_BOTTOM_XAXIS,
								   VAL_MANUAL, timeArray[currentIndex-50], timeArray[currentIndex-1]);
			}

			break;
		}
	}
	return 0;
}
*/
/*
int CVICALLBACK Ppoint2(int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		{
			#define AMPLITUDE 32760.0
			#define FREQUENCY 1 // 0.1 Hz = 10???
			#define NUM_TRACES 3    //

			static double currentTime = 0.0;
			static int isFirstPlot = 1;

			if (isFirstPlot) {

				ClearStripChart(panel, PANEL_Scop_STRIPCHART);

				SetCtrlAttribute(panel, PANEL_Scop_STRIPCHART, ATTR_NUM_TRACES, NUM_TRACES);

				//SetCtrlAttribute(panel, PANEL_Scop_STRIPCHART, ATTR_SCROLL_MODE, VAL_CONTINUOUS);

				SetCtrlAttribute(panel, PANEL_Scop_STRIPCHART, ATTR_POINTS_PER_SCREEN, 501);

				SetAxisScalingMode(panel, PANEL_Scop_STRIPCHART, VAL_LEFT_YAXIS,
								   VAL_MANUAL, -AMPLITUDE*1.2, AMPLITUDE*1.2);

				SetTraceAttribute(panel, PANEL_Scop_STRIPCHART, 1, ATTR_TRACE_COLOR, VAL_RED);
				SetTraceAttribute(panel, PANEL_Scop_STRIPCHART, 2, ATTR_TRACE_COLOR, VAL_GREEN);
				SetTraceAttribute(panel, PANEL_Scop_STRIPCHART, 3, ATTR_TRACE_COLOR, VAL_BLUE);

				isFirstPlot = 0;
			}
			double omega_t = 2 * PI * FREQUENCY * currentTime;
			double values[NUM_TRACES];

			values[0] = AMPLITUDE * sin(omega_t);
			values[1] = AMPLITUDE * sin(omega_t + 2*PI/3);
			values[2] = AMPLITUDE * sin(omega_t + 4*PI/3);

			PlotStripChart(panel, PANEL_Scop_STRIPCHART, values, NUM_TRACES, 0, 0, VAL_DOUBLE);
			currentTime += 0.1;

			break;
		}
	}
	return 0;
}
  */
int CVICALLBACK ShiftFuncx(int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	double Shift;
	int Interval_Shif;

	switch (event)
	{
		case EVENT_VAL_CHANGED:
		{
			// ??????
			GetCtrlVal(panel, control, &Shift);
			GetCtrlVal(panel, PANEL_Scop_RINGSLIDE, &Interval_Shif);
			SetCtrlAttribute(scopePanelHandle, PANEL_Scop_STRIPCHART, ATTR_SCROLL_MODE, VAL_BLOCK);

			SetAxisScalingMode(scopePanelHandle, PANEL_Scop_STRIPCHART, VAL_BOTTOM_XAXIS,
							   VAL_MANUAL, Shift - Interval_Shif, Shift);
			break;
		}
	}
	return 0;
}



int CVICALLBACK ShiftFuncy(int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	double Shift;
	if (event == EVENT_VAL_CHANGED)
	{
		GetCtrlVal(panel, control, &Shift);  // Assuming UPDATE_SHIFT is the control ID for the shift input
		SetAxisScalingMode (scopePanelHandle, PANEL_Scop_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -Shift, Shift);
		SetAxisScalingMode (scopePanelHandle, PANEL_Scop_STRIPCHART, VAL_LEFT_YAXIS, VAL_MANUAL, -Shift, Shift);
	}
	return 0;
}

int CVICALLBACK TogglePauseCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int isPaused;

	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal(panel, control ,&isPaused);
			isPaused = !isPaused;
			printf("* %d\n",isPaused);
			//SetCtrlAttribute(panel, PANEL_Scop_STRIPCHART, ATTR_STRIP_CHART_PAUSED, isPaused);
			break;
	}
	return 0;
}
