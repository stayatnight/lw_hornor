/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : slTaskKey.h                                                   *
* Description :                                                               *
*               smart lamp task key header file                               *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __SL_TASK_KEY_H__
#define __SL_TASK_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHal.h"
#include <hal_gpio.h>
#if defined(APP_TASK_KEY_USE) && (APP_TASK_KEY_USE == 1)
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
/* 普通按键开关 IO define */
#define APP_LIGHT_NORMAL_KEY_IO                          {\
                                                        GPIO_PIN_3 }

#define LIGHT_KEY_VAL_SWITCH                             0x00000001
const uint32_t k_a_uwBriPercent[] = 
{
    0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};

#define APP_LIGHT_NORMAL_KEY_VAL                         {\
                                                          LIGHT_KEY_VAL_SWITCH}
const gpio_pin_t s_a_normalKeyHalIo[] = APP_LIGHT_NORMAL_KEY_IO;
static uint32_t s_a_normalKeyVal[] = APP_LIGHT_NORMAL_KEY_VAL;
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)

//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
void rlTaskKey(void* arg);

#endif //APP_TASK_KEY_USE

#ifdef __cplusplus
}
#endif

#endif /*__SL_TASK_KEY_H__*/