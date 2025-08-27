//==============================================================================
//
// Title:		devData.h
// Purpose:		A short description of the interface.
//
// Created on:	2025/7/13 at 22:45:21 by wind.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __devData_H__
#define __devData_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files
		
#include <udpsupp.h>    
#include "cvidef.h"

//==============================================================================
// Constants
//设备名称长度
#define		DEVNAME_LEN			32
//设备数量
#define		DEV_NUM				6
		
#define		DEV_OFFLINE			0
#define		DEV_ONLINE			1
//通信超时3S
#define		DEV_TMO				30
//==============================================================================
// Types
#define		MAX_IP4_STRING_LENGTH		15 
typedef char IPAddr[MAX_IP4_STRING_LENGTH+1];  	// Define the IPAdrr type	dot 3 ;char max is 255(3)x4=12=15

typedef struct{
	unsigned int id;			//设备ID								 
	char devName[DEVNAME_LEN];  //设备名称
	IPAddr   rIP;				//远程IP
	unsigned int rport;			//远程端口
	unsigned int state;			//状态 0-掉线 1-运行
	unsigned int wdtCnt;		//看门狗，超时未通信则认为掉线
}devInfo_t;
//==============================================================================
// External variables
extern u16 debugCurDev;			//默认Id = 1; 
extern devInfo_t devInfo[DEV_NUM];
extern RTDataType  devRtdata[DEV_NUM];
//==============================================================================
// Global functions

void devInfoInit (void);
u8 getDevIndexFromId(u16 id, u16 *index);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __devData_H__ */
