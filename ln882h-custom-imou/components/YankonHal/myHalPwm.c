/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalPwm.c                                                    *
* Description :                                                               *
*               my HAL层pwm处理源文件                                          *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalPwm.h"
#include "pwmout_api.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct 
{
    myHalPwm_t halPwm;
    PinName pin;
} myHalPwmPin_t;
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
const myHalPwmPin_t myHalPwmPinTable[] = 
{
#if (APP_DEVICE_HARDWARE_TYPE_USED == APP_DEVICE_HARDWARE_TYPE_BL3357)
    {MY_HAL_PWM_0, PA_11},
    {MY_HAL_PWM_1, PA_12},
#elif (APP_DEVICE_HARDWARE_TYPE_USED == APP_DEVICE_HARDWARE_TYPE_BL3383)
    {MY_HAL_PWM_0, PA_20},
    {MY_HAL_PWM_1, PA_1},
#endif    
    {MY_HAL_PWM_2, PA_2},
    {MY_HAL_PWM_3, PA_3},
    {MY_HAL_PWM_4, PA_4},
    {MY_HAL_PWM_5, PA_17},
    {MY_HAL_PWM_6, PA_18},
    {MY_HAL_PWM_7, PA_19},
};

pwmout_t myHalPwm[MY_HAL_PWM_NUM];
uint32_t myHalPwmMaxVal[MY_HAL_PWM_NUM];
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : myHalPwmInit
 Description : my Hal PWM 模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : 0    - 成功
               其它 - 失败码
 Other       : (none)
******************************************************************************/
int8_t myHalPwmInit(myHalPwm_t pwm, uint32_t hz, uint32_t maxPwmVal)
{
    if (pwm >= MY_HAL_PWM_NUM) {
        return -1;
    }

    pwmout_init(&myHalPwm[pwm], myHalPwmPinTable[pwm].pin);
    pwmout_write(&myHalPwm[pwm], 0.0f);
    pwmout_period_us(&myHalPwm[pwm], 1000000/hz); //25k Hz
//  pwmout_period_us(&myHalPwm[pwm], 40); //25k Hz
    //pwmout_start(&myHalPwm[pwm]);
    myHalPwmMaxVal[pwm] = maxPwmVal;
    return 0;
}

/******************************************************************************
 Function    : myHalPwmOutput
 Description : my hal PWM output
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t myHalPwmOutput(myHalPwm_t pwm, uint32_t ulPwmVal)
{
    if (pwm >= MY_HAL_PWM_NUM) {
        return -1;
    }

    ulPwmVal = ulPwmVal > myHalPwmMaxVal[pwm] ? myHalPwmMaxVal[pwm] : ulPwmVal;
    pwmout_write(&myHalPwm[pwm], ulPwmVal*1.0f/myHalPwmMaxVal[pwm]);
    return 0;
}

#ifdef __cplusplus
}
#endif

