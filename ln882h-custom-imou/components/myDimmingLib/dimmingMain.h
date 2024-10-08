/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingMain.h                                                 *
* Description :                                                               *
*               my Lamp Library control header file                           *
* Note        : (none)                                                        *
* Author      : lizhijun                                                      *
* Date:       : 2018-08-09                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_LIB_LAMP_H__
#define __MY_LIB_LAMP_H__

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
#define MY_LIB_LAMP_VERSION                               (202)

#define MY_LIB_LAMP_CFG_INT_BLINKING                      (0)

#define MY_LIB_LAMP_CFG_USE_DEFAULT_ALGORITHM             (0)

#define MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT             (1)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
/* switch control status */
typedef enum
{
    MY_LAMP_CTRL_SW_OFF = 0, 
    MY_LAMP_CTRL_SW_ON, 
    MY_LAMP_CTRL_SW_NUM
} MY_LAMP_CTRL_SW_t;

/* lamp control mode */
typedef enum
{
    MY_LAMP_CTRL_TYPE_RGB = 0, 
    MY_LAMP_CTRL_TYPE_CCT, 
    MY_LAMP_CTRL_TYPE_END
} MY_LAMP_CTRL_TYPE;

/* 
 * lamp parameter define
 * user can use pSelfParam point to other self parameter when need
 */
typedef struct
{
    uint8_t  ucSwitch;                      
    uint8_t  ucSceneNo;
    uint16_t uwBri;
    uint8_t  ucCtrlType;
    uint8_t  ucHue[3];
    uint16_t uwCCT;
    uint32_t minPwm;
    uint32_t maxPwm;
    void *pSelfParam;
} myLampParam_t;

/*
 * pwm output function
 * this function can output max 4 channel pwm
 * user need to complete this function for different hardware
 * this function be called in myLampCreateLamp function
*/
typedef void (*pwmOutputFunc)(uint32_t ulPwm1, uint32_t ulPwm2, uint32_t ulPwm3, uint32_t ulPwm4);

/*
 * color to pwm convert function
 * this function complete the transfer from color parameter to pwm value
 * user need to complete this function for different transfer
 * this function be called in myLampCreateLamp function
*/
typedef int8_t (*color2PwmFunc)(uint32_t ulPwm[4], myLampParam_t *plampParam);

/*
 * step pwm calculate function
 * this function complete the pwm value at each control step
 * user need to complete this function for different control method
 * this function be called in myLampCreateLamp function
*/
typedef uint32_t (*stepPwmCalculateFunc)(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);

/*
 * dimming end call back function
 * this function be called in when lamp control arrived last step              
 * user need to complete this function when do something after lamp control finished
*/
typedef void (*dimmingEndFunc)(uint8_t lampId);

/*
 * dimming status control call back function
 * this function be called in when lamp status be control              
 * user can register this function for doing something when need
*/
typedef void (*dimmingStatusCtrlHookFunc)(uint8_t lampId, const myLampParam_t *pLampParam);
/*
 * flash call back function
 * this function be called in when lamp flash control arrived period              
 * user need to complete this function when do something when lamp flash arrived peroid
*/
typedef void (*flashCallbackFunc)(uint8_t isFirst, uint32_t currentCnt, myLampParam_t *pLampParam, uint32_t *pStepMs);

/*
 * flash end call back function
 * this function be called in when lamp flash control arrived last step              
 * user need to complete this function when do something after lamp flash control finished
*/
typedef void (*flashEndFunc)(uint32_t arg);

/*
 * get ms tick function
 * this function get the ms tick value for lamp control,
 * user must to complete this function, this is very importent
 * this function be called in myLampInit function
*/
//typedef uint32_t (*dimmingGetMsTickFunc)(void);

/*
 * mutex lock function
 * this function used to protect the share resource
 * user need to complete this function when use os
 * this function be called in myLampInit function
*/
//typedef void (*myMutexLockFunc)(void);

/*
 * mutex unlock function
 * this function used to protect the share resource
 * user need to complete this function when use os
 * this function be called in myLampInit function
*/
//typedef void (*myMutexUnlockFunc)(void);
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
/******************************************************************************
 Function    : myLampCreateLamp
 Description : create a lamp object with init param
 Note        : this function create a lamp object, after call myLampInit function
               user need to call this function to add some lamps and then user can call
               other control function to control these lamps.
 Input Para  : lampParam -- init lamp parameter
               pFuncPwmOutput -- pwm output function, must complete this function by user 
               pFuncColor2Pwm -- color parameter to pwm value function, parameter NULL will use default transer function
               pFuncStepPwmCal -- step pwm value calculate function, parameter NULL will use default calculate function
 Output Para : pLampId -- when return success, this parameter save the lamp ID
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampCreateLamp(uint8_t *pLampId, myLampParam_t *plampParam,
                         pwmOutputFunc pFuncPwmOutput,
                         color2PwmFunc pFuncColor2Pwm,
                         stepPwmCalculateFunc pFuncStepPwmCal,
                         dimmingEndFunc pFuncDimmingEnd,
                         flashEndFunc pFuncflashEnd);

/******************************************************************************
 Function    : myLampDeleteLamp
 Description : delete a lamp object
 Note        : this function delete a lamp object, after create a lamp object
               user can call this function to delete the lamp by the lamp ID
 Input Para  : lampId -- lamp ID
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampDeleteLamp(uint8_t lampId);

/******************************************************************************
 Function    : myLampSetDimmingCurve
 Description : set dimming curve
 Note        : this function set the dimming curve, when after create a lamp object,
               user can call this function to set the dimming curve by lampId
 Input Para  : lampId -- lamp ID
               curve -- curve index(reference dimmingCurve.h)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
#if MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT
int8_t myLampSetDimmingCurve(uint8_t lampId, uint8_t curve);
#endif

/******************************************************************************
 Function    : myLampGetDimmingCurve
 Description : set dimming curve
 Note        : this function get the dimming curve, when after create a lamp object,
               user can call this function to get the dimming curve by lampId
 Input Para  : lampId -- lamp ID
               curve -- curve index(reference dimmingCurve.h)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
#if MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT
uint8_t myLampGetDimmingCurve(uint8_t lampId);
#endif

/******************************************************************************
 Function    : myLampStatusGet
 Description : my lamp status get
 Note        : this function get a lamp status by lamp ID,
               user can call this function to get the lamp status
 Input Para  : lampId -- lamp ID
 Output Para : pLampParam -- when return success, this parameter save the lamp's status
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampStatusGet(uint8_t lampId, myLampParam_t *pLampParam);

/******************************************************************************
 Function    : myLampCallSceneCtrl
 Description : lamp call scene
 Note        : this function complete lamp scene call,
               user can call this function to call the lamp scene by user's lamp scene parameter
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
               pLampParam -- scene parameter
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampCallSceneCtrl(uint8_t lampId, uint32_t ulPeriodMs, myLampParam_t *pLampParam);

/******************************************************************************
 Function    : myLampSwitchCtrl
 Description : lamp switch control
 Note        : this function complete lamp switch control,
               user can call this function to control the lamp switch status by lamp ID
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
               status -- switch status the value can use MY_LAMP_CTRL_SW_OFF or MY_LAMP_CTRL_SW_ON
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampSwitchCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint8_t status);

/******************************************************************************
 Function    : myLampBriCtrl
 Description : lamp bright control
 Note        : this function complete lamp bright control,
               user can call this function to control the lamp bright status by lamp ID
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
               ucBri -- bright value range(MY_LIB_LAMP_CTRL_BRI_MIN - MY_LIB_LAMP_CTRL_BRI_MAX)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampBriCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint16_t uwBri);

/******************************************************************************
 Function    : myLampCCTCtrl
 Description : lamp cct control
 Note        : this function complete lamp cct control
               user can call this function to control the lamp cct status by lamp ID
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampCCTCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint16_t uwCCT, uint16_t uwBri);

/******************************************************************************
 Function    : myLampColorCtrl
 Description : lamp color control
 Note        : this function complete lamp RGB control
               user can call this function to control the lamp RGB status by lamp ID
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampColorCtrl(uint8_t lampId, uint32_t ulPeriodMs, const uint8_t ucHue[3], uint16_t uwBri);

/******************************************************************************
 Function    : myLampPwmsCtrl
 Description : 灯控制PWM输出
 Note        : this function complete lamp pwm control
               user can call this function to control the lamp pwm status by lamp ID
               the control time = uwStepNum * MY_LIB_LAMP_CTRL_STEP_INTERVAL
 Input Para  : lampId -- lamp ID
               ulPeriodMs -- control period ms
               dstPwms -- aim pwm value
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t myLampPwmsCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint32_t dstPwms[4]);

/******************************************************************************
 Function    : myLampFlashCtrl
 Description : myle lamp flash control
 Note        : this function complete lamp flash,
               user can call this function to control the lamp flash by lamp ID
 Input Para  : lampId -- lamp ID
               ulStepMs -- control step ms
               ulPeriodMs -- control blink period ms
               flashCnt -- lamp flash count
               arg -- callback arg
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampFlashCtrl(uint8_t lampId, uint32_t ulStepMs, uint32_t ulPeriodMs, uint32_t flashCnt, flashCallbackFunc pflashCallbck, uint32_t arg);

/******************************************************************************
 Function    : myLampRegisterDimmingStatusCtrlHook
 Description : 注册灯控制回调
 Note        : this function complete register dimming status control call back function
 Input Para  : lampId -- lamp ID
               statusHook -- status control call back function
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t myLampRegisterDimmingStatusCtrlHook(uint8_t lampId, dimmingStatusCtrlHookFunc statusHook);

/******************************************************************************
 Function    : myLampInit
 Description : lamp init
 Note        : this function complete lamp scene call,
               user must call this function first when usb the lib
 Input Para  : stepInterval -- step control interval time (unit: ms)
               pFuncGetMsTick -- get systick(unit:ms), this function must be complete by user
               pFuncMutexLock -- mutex lock of os, must be complete by user when use os, or can be NULL
               pFuncMutexUnlock -- mutex unlock of os, must be complete by user when use os, or can be NULL
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myLampInit(uint32_t stepInterval,
                   //dimmingGetMsTickFunc pFuncGetMsTick, 
                   uint32_t (*pFuncGetMsTick)(void),
                   //myMutexLockFunc pFuncMutexLock, 
                   void (*pFuncMutexLock)(void),
                   //myMutexUnlockFunc pFuncMutexUnlock
                   void (*pFuncMutexUnlock)(void));
int lightLampBrightDeltaCtrl(int deltaBri, uint32_t ulPeriodMs);
/******************************************************************************
 Function    : myLampLoop
 Description : lamp main loop
 Note        : this function complete lamp main loop,
               user must loop call this function within MY_LIB_LAMP_CTRL_STEP_INTERVAL time when usb the lib
 Input Para  : (none)
 Output Para : (none)
 Return      : 1 -- run step control, 0 -- no step control
 Other       : (none)
******************************************************************************/
int8_t myLampLoop(void);

#ifdef __cplusplus
}
#endif

#endif /*__MY_LIB_LAMP_H__*/



