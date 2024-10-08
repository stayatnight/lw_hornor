/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingCurve.h                                                *
* Description :                                                               *
*               my Lamp Library control header file                           *
* Note        : (none)                                                        *
* Author      : lizhijun                                                      *
* Date:       : 2018-08-09                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_LIB_DIMMING_CURVE_H__
#define __MY_LIB_DIMMING_CURVE_H__

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
#define MY_DIMMING_CURVE_SUPPORT_LINEAR                    (1)
#define MY_DIMMING_CURVE_SUPPORT_DELAY_POWER_ONOFF         (1)
#define MY_DIMMING_CURVE_SUPPORT_EXPONENTIAL               (0)
#define MY_DIMMING_CURVE_SUPPORT_CIE1976                   (1)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
typedef enum
{
#if MY_DIMMING_CURVE_SUPPORT_LINEAR
    MY_DIMMING_CURVE_LINEAR,
#endif
#if MY_DIMMING_CURVE_SUPPORT_DELAY_POWER_ONOFF
    MY_DIMMING_CURVE_DELAY_POWER_ON_OFF,
#endif
#if MY_DIMMING_CURVE_SUPPORT_EXPONENTIAL
    MY_DIMMING_CURVE_EXPONENTIAL,
#endif
#if MY_DIMMING_CURVE_SUPPORT_CIE1976
    MY_DIMMING_CURVE_CIE1976,
#endif
    MY_DIMMING_CURVE_NUM
} MY_DIMMING_CURVE_t;

/*
 * my dimming curve function
 * this function complete the pwm value at each control step
 * user need to complete this function for different control method
 * this function be called in LampCreateLamp function
*/
typedef uint32_t (*MyDimmingCurveFunc)(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
MyDimmingCurveFunc getMyDimmingCurve(MY_DIMMING_CURVE_t curve);

#ifdef __cplusplus
}
#endif

#endif /*__MY_LIB_LAMP_H__*/



