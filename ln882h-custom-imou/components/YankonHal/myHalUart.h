/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalUart.h                                                   *
* Description :                                                               *
*               my hal UART 处理头文件                                         *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-20                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_HAL_UART_H__
#define __MY_HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHal.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_HAL_UART_MODE_IT_SUPPORT                           1
#define MY_HAL_UART_MODE_DMA_SUPPORT                          1
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
#pragma pack(1)
typedef enum
{
    MY_HAL_UART_0,
    MY_HAL_UART_1,
    MY_HAL_UART_NUM
} myHalUart_t;

typedef enum
{
    MY_HAL_UART_MODE_NONE,
#if MY_HAL_UART_MODE_IT_SUPPORT
    MY_HAL_UART_MODE_IT,
#endif
#if MY_HAL_UART_MODE_DMA_SUPPORT
    MY_HAL_UART_MODE_DMA,
#endif
    MY_HAL_UART_MODE_NUM
} myHalUartMode_t;

typedef struct
{
    uint32_t ulRxSize;
    uint8_t *pucRxBuf;
    uint32_t ulTxSize;
    uint8_t *pucTxBuf;
} myHalUartBuf_t;
#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
int myHalUartInit(myHalUart_t uart, uint32_t baudrate, myHalUartBuf_t *pstHalBuf, myHalUartMode_t mode);

//int myHalUartIrqConfig(myHalUart_t uart, uint8_t rxEn, uint8_t txEn);

int myHalUartSend(myHalUart_t uart, uint8_t* pucData, uint32_t ulDataLen);

int myHalUartRecv(myHalUart_t uart, uint8_t *pucData, uint32_t ulDataLen);

//uint8_t myHalUartGetBusy(myHalUart_t uart);

uint32_t myHalUartGetRecvLen(myHalUart_t uart);

//void myHalUartRxISR(myHalUart_t uart);
//
//void myHalUartIdleISR(myHalUart_t uart);
//
//void myHalUartTxISR(myHalUart_t uart);
//
//void myHalUartTcISR(myHalUart_t uart);

//uint32_t myHalUartPrintf(char* fmt, ...);

#ifdef __cplusplus
}
#endif



#endif   /*__MY_HAL_UART_H__*/

