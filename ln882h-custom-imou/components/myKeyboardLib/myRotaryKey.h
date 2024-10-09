/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myRotaryKey.h                                                 *
* Description :                                                               *
*               my rotary Key处理任务头文件                                    *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2021-08-23                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_ROTARY_KEY_H__
#define __MY_ROTARY_KEY_H__

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
/* 
 * rotary key state define
 * rotary key position information(00-state1, 01-state2, 11-state3, 10-state4)
 */
typedef enum
{
    ROTARY_KEY_STATE_INIT,
    ROTARY_KEY_STATE_1,
    ROTARY_KEY_STATE_2,
    ROTARY_KEY_STATE_3,
    ROTARY_KEY_STATE_4,
    ROTARY_KEY_STATE_NUM,
} rotaryKeyState_t;

/*
 * get ms tick function
 * this function get the ms tick value for rotary key process,
 * user must to complete this function, this is very importent
 * this function be called in myLampInit function
*/
typedef uint32_t (*rotaryGetMsTickFunc)(void);

/*
 * rotary call back function
 * this function be called in when key was rotate             
 * user need to complete this function when do something when key ratate
*/
typedef void(*rotaryKeyCallback)(uint8_t pressFlag, int rotaryCnt);

/*
 * get key io status function
 * this function get the key io status value for keyboard,
 * user must to complete this function, this is very importent
 * this function be called in myKeyboardRegisterKey function
*/
typedef int (*rotaryKeyGetIOStatus)(uint8_t *pPressKeyVal, uint8_t *pRotaryLeftKeyVal, uint8_t *pRotaryRightKeyVal);
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
/******************************************************************************
 Function    : myRotaryKeyGetState
 Description : get rotary key current state
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : state
 Other       : (none)
******************************************************************************/
rotaryKeyState_t myRotaryKeyGetState();

/******************************************************************************
 Function    : myRotaryKeyInit
 Description : rotary key init
 Note        : this function complete rotary key init,
               user must call this function first when usb the lib
 Input Para  : intervalMs -- key check interval time (unit: ms)
               filterCountMs -- key check filter count (filter time = (keyCheckInterval*keyFilterCount)ms)
               protaryGetMsTickFunc -- get systick(unit:ms), this function must be complete by user
               protaryKeyGetIOStatusFunc -- get key io status by keyvalue,this function must be complete by user
               protaryKeyCallbackFunc -- key rotate event callback function, complete by user if need, or can be NULL
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myRotaryKeyInit(uint32_t intervalMs,
                     uint32_t filterCountMs,
                     rotaryGetMsTickFunc protaryGetMsTickFunc,
                     rotaryKeyGetIOStatus protaryKeyGetIOStatusFunc,
                     rotaryKeyCallback protaryKeyCallbackFunc);

/******************************************************************************
 Function    : myRotaryKeyLoop
 Description : roraty key main loop
 Note        : this function complete rotary key main loop,
               user must loop call this function within keyCheckInterval time when usb the lib
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myRotaryKeyLoop(void);

#ifdef __cplusplus
}
#endif

#endif /*__MY_ROTARY_KEY_H__*/



