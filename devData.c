//==============================================================================
//
// Title:		devData.c
// Purpose:		A short description of the implementation.
//
// Created on:	2025/7/13 at 22:45:21 by wind.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <userint.h>
#include <utility.h>
#include <cvitdms.h>
#include <ansi_c.h>
#include <udpsupp.h>
#include "APF_DEBUG.h"
#include "datatype.h"
#include "devData.h"

//==============================================================================
// Constants
char *devName[DEV_NUM] = {"AC_DC1端口", "AC_DC2端口", "DC_DC1端口", "DC_DC2端口", "DC_DC3端口", "DC_DC4端口"};
//==============================================================================
// Types

//==============================================================================
// Static global variables
u16 debugCurDev = 0;			//默认Id = 1;
devInfo_t devInfo[DEV_NUM];		// Use the Struct devInfo_t 
//==============================================================================
// Static functions

//==============================================================================
// Global variables
extern int panelHandle;

RTDataType  devRtdata[DEV_NUM] = {0};
//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
void devInfoInit (void)
{
	u16 i;
	
	//清除RING显示
	ClearListCtrl(panelHandle, PANEL_RING_DEV); 
	
	for(i=0; i<DEV_NUM; i++) {
		devInfo[i].id = i+1;
		devInfo[i].state = DEV_OFFLINE;
		strcpy(devInfo[i].devName, devName[i]);
		if(i == 0) InsertListItem(panelHandle, PANEL_RING_DEV, 0, devName[i], devInfo[i].id);
		else	   InsertListItem(panelHandle, PANEL_RING_DEV, -1, devName[i], devInfo[i].id);
	}
	debugCurDev = 0;
	SetCtrlIndex(panelHandle, PANEL_RING_DEV, debugCurDev);
}

u8 getDevIndexFromId(u16 id, u16 *index) {
	u16 i;
	
	for(i=0; i<DEV_NUM; i++) {
		if(devInfo[i].id == id) break;
	}
	if(i == DEV_NUM) return 0;
	else {
		*index = i;
		return 1;
	}
}

int CVICALLBACK SEL_DEVCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int index;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlIndex(panelHandle, PANEL_RING_DEV, &index);
			debugCurDev = index;
			break;
	}
	return 0;
}
