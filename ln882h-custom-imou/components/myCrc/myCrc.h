/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myCrc.h                                                       *
* Description :                                                               *
*               my crc 处理头文件                                              *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-15                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_CRC_H__
#define __MY_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include <stdint.h>
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
*                              Declarations                                   *
******************************************************************************/
uint8_t myCalcSumCheck(const uint8_t* pucData, uint8_t ucLen);
uint16_t usMBCRC16( uint8_t* pucFrame, uint16_t usLen );
uint16_t myCalcCRC(uint8_t* pucData, uint16_t uwLenth);

#ifdef __cplusplus
}
#endif

#endif /*__MY_CRC_H__*/


