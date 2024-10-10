/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalIO.h                                                     *
* Description :                                                               *
*               my hal io header file                                         *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_HAL_IO_H__
#define __MY_HAL_IO_H__

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

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
typedef enum
{
    HAL_IO_PA0,
    HAL_IO_PA2,
    HAL_IO_PA3,
    HAL_IO_PA4,
    HAL_IO_PA7,
    HAL_IO_PA8,
    HAL_IO_PA11,
    HAL_IO_PA12,
    HAL_IO_PA13,
    HAL_IO_PA14,
    HAL_IO_PA15,
    HAL_IO_PA16,
    HAL_IO_PA17,
    HAL_IO_PA18,
    HAL_IO_PA19,
    HAL_IO_PA20,
    HAL_IO_NUM
} halIO_t;

typedef enum
{
    HAL_IO_MODE_IN_NOPULL,
    HAL_IO_MODE_IN_PULLUP,
    HAL_IO_MODE_IN_PULLDOWN,
    HAL_IO_MODE_OUT_NOPULL,
    HAL_IO_MODE_OUT_PULLUP,
    HAL_IO_MODE_OUT_PULLDOWN,
    HAL_IO_MODE_NUM
} halIOMode_t;

typedef enum
{
    HAL_IO_IRQ_NULL,
    HAL_IO_IRQ_FALLING,
    HAL_IO_IRQ_RISING,
    HAL_IO_IRQ_FALLING_RISING,
    HAL_IO_IRQ_NUM
} halIOIrq_t;

typedef enum
{
    HAL_IO_IRQ_EVENT_NULL,
    HAL_IO_IRQ_EVENT_RISING,
    HAL_IO_IRQ_EVENT_FALLING,
    HAL_IO_IRQ_EVENT_NUM
} halIOIrqEvent_t;

typedef enum
{
    HAL_IO_STATUS_LOW,
    HAL_IO_STATUS_HIGH,
    HAL_IO_STATUS_NUM
} halIOStatus_t;

typedef void (*halIoIrqFunc)(uint32_t id, halIOIrqEvent_t event);
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
int8_t halIOInit(halIO_t io, halIOMode_t mode, halIOIrq_t irq, halIoIrqFunc irqfunc);
int8_t halIOIrqEnable(halIO_t io);
int8_t halIOIrqDisable(halIO_t io);
int8_t halIOSetStatus(halIO_t io, halIOStatus_t status);
int8_t halIOGetStatus(halIO_t io, halIOStatus_t *pStatus);

#ifdef __cplusplus
}
#endif



#endif   /*__MY_HAL_IO_H__*/

