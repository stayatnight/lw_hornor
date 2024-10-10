/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalPwm.h                                                    *
* Description :                                                               *
*               my hal pwm header file                                        *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_HAL_PWM_H__
#define __MY_HAL_PWM_H__

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
#pragma pack(1)
typedef enum
{
    MY_HAL_PWM_0,
    MY_HAL_PWM_1,
    MY_HAL_PWM_2,
    MY_HAL_PWM_3,
    MY_HAL_PWM_4,
    MY_HAL_PWM_5,
    MY_HAL_PWM_6,
    MY_HAL_PWM_7,
    MY_HAL_PWM_NUM,
} myHalPwm_t;

typedef enum
{
    MY_HAL_PWM_POL_POS,
    MY_HAL_PWM_POL_NEG,
    MY_HAL_PWM_POL_NUM,
} myHalPwmPol_t;
#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
int8_t myHalPwmInit(myHalPwm_t pwm, uint32_t hz, uint32_t maxPwmVal);
int8_t myHalPwmOutput(myHalPwm_t pwm, uint32_t ulPwmVal);

#ifdef __cplusplus
}
#endif

#endif   /*__MY_HAL_PWM_H__*/

