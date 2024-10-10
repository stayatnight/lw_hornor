/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : hal.h                                                         *
* Description :                                                               *
*               hal header file                                               *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "appConfig.h"
#include "appDefine.h"
#include <stdint.h>
#include <stdio.h>
#include "log_service.h"
#include "myHalLog.h"
#include "myHalIO.h"
#include "myHalUart.h"
#include "myHalPwm.h"
#include "myHalFlash.h"
#include "myHalWifi.h"

//#include "oppHalSocket.h"
//#include "oppHalRtc.h"
//#include "oppHalWifi.h"
//#include "oppHalI2C.h"
//#include "oppHalSpi.h"
//#include "oppHalAdc.h"
//#include "oppHalTimer.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

#ifdef __cplusplus
}
#endif



#endif   /*__HAL_H__*/

