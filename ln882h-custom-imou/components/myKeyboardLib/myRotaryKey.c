/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myRotaryKey.c                                                 *
* Description :                                                               *
*               my rotary Key处理任务源文件                                    *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2021-08-23                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myRotaryKey.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#if 0
#include "../../../proj/tl_common.h"
#define Printf(fmt, ...) my_printf_uart("[%s]" fmt, __func__, ##__VA_ARGS__)
#else
#define Printf(fmt, ...)
#endif

#ifndef NULL
#define NULL 0
#endif
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef enum
{
    ROTARY_KEY_ACTION_NONE,
    ROTARY_KEY_ACTION_1,
    ROTARY_KEY_ACTION_2,
    ROTARY_KEY_ACTION_3,
    ROTARY_KEY_ACTION_4,
    ROTARY_KEY_ACTION_NUM,
} rotaryKeyAction_t;



typedef enum
{
    ROTARY_KEY_DIR_CLOCKWISE,
    ROTARY_KEY_DIR_ANTICLOCKWISE,
    ROTARY_KEY_DIR_NUM
} rotaryKeyDirection_t;

typedef struct
{
    rotaryKeyState_t curState;
    rotaryKeyAction_t curAction;
    rotaryKeyDirection_t curDirection;
    uint8_t curPressedKeyOn;
    uint8_t isPressKeyReleaseRun;
    uint8_t curRotaryVal;
    uint8_t init;
    int rotaryCount;
    uint32_t resetCountDelay;
    uint32_t lastResetCountTick;
    uint32_t intervalMs;
    rotaryGetMsTickFunc pFuncGetMsTick;
    rotaryKeyGetIOStatus pFuncRotaryKeyGetIOStatus;
    rotaryKeyCallback pFuncRotaryKeyCallback;
} myRotaryKeyInfo_t;
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
myRotaryKeyInfo_t myRotaryKeyInfo;
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
 Function    : rotarySwitchStatusSet
 Description : status 状态设置
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatusSet(rotaryKeyState_t status)
{
    if (status == myRotaryKeyInfo.curState) {
        return;
    }

    switch (status) {
    case ROTARY_KEY_STATE_INIT:
        Printf("rs Status set to Init\r\n");
        break;
    case ROTARY_KEY_STATE_1:
        if (myRotaryKeyInfo.curState == ROTARY_KEY_STATE_INIT) {
            Printf("rs Status set to type %d counts %d 1\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
        } else {
            if (myRotaryKeyInfo.curDirection == ROTARY_KEY_DIR_CLOCKWISE) {
                Printf("rs Status set to type %d counts %d ->1\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            } else {
                Printf("rs Status set to type %d counts %d 1<-\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
        }
        break;
    case ROTARY_KEY_STATE_2:
        if (myRotaryKeyInfo.curState == ROTARY_KEY_STATE_INIT) {
            Printf("rs Status set to type %d counts %d 2\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
        } else {
            if (myRotaryKeyInfo.curDirection == ROTARY_KEY_DIR_CLOCKWISE) {
                Printf("rs Status set to type %d counts %d ->2\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            } else {
                Printf("rs Status set to type %d counts %d 2<-\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
        }
        break;
    case ROTARY_KEY_STATE_3:
        if (myRotaryKeyInfo.curState == ROTARY_KEY_STATE_INIT) {
            Printf("rs Status set to type %d counts %d 3\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
        } else {
            if (myRotaryKeyInfo.curDirection == ROTARY_KEY_DIR_CLOCKWISE) {
                Printf("rs Status set to type %d counts %d ->3\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            } else {
                Printf("rs Status set to type %d counts %d 3<-\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
        }  
        break;
    case ROTARY_KEY_STATE_4:
        if (myRotaryKeyInfo.curState == ROTARY_KEY_STATE_INIT) {
            Printf("rs Status set to type %d counts %d 4\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
        } else {
            if (myRotaryKeyInfo.curDirection == ROTARY_KEY_DIR_CLOCKWISE) {
                Printf("rs Status set to type %d counts %d ->4\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            } else {
                Printf("rs Status set to type %d counts %d 4<-\r\n", myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
        }
        break;
    default:
        Printf("rs Status set to unknown\r\n");
        break;
    }
    myRotaryKeyInfo.curState = status;
}

/******************************************************************************
 Function    : rotarySwitchStatusInitProcess
 Description : init 状态处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatusInitProcess()
{
    myRotaryKeyInfo.rotaryCount = 0;
    switch (myRotaryKeyInfo.curAction) {
    case ROTARY_KEY_ACTION_1:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_1);
        break;
    case ROTARY_KEY_ACTION_2:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_2);
        break;
    case ROTARY_KEY_ACTION_3:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_3);
        break;
    case ROTARY_KEY_ACTION_4:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_4);
        break;
    default:
        break;
    }
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
}

/******************************************************************************
 Function    : rotarySwitchStatus1Process
 Description : status 1 状态处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatus1Process()
{
    switch (myRotaryKeyInfo.curAction) {
    case ROTARY_KEY_ACTION_1:
        break;
    case ROTARY_KEY_ACTION_2:
        myRotaryKeyInfo.rotaryCount += 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_CLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_2); 
        break;
    case ROTARY_KEY_ACTION_3:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_3);
        break;
    case ROTARY_KEY_ACTION_4:
        myRotaryKeyInfo.rotaryCount -= 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_ANTICLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_4);
        break;
    default:
        break;
    }
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
}

/******************************************************************************
 Function    : rotarySwitchStatus2Process
 Description : status 2 状态处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatus2Process()
{
    switch (myRotaryKeyInfo.curAction) {
    case ROTARY_KEY_ACTION_1:
        myRotaryKeyInfo.rotaryCount -= 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_ANTICLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_1);
        break;
    case ROTARY_KEY_ACTION_2:
        break;
    case ROTARY_KEY_ACTION_3:
        myRotaryKeyInfo.rotaryCount += 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_CLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_3);
        break;
    case ROTARY_KEY_ACTION_4:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_4);
        break;
    default:
        break;
    }
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
}

/******************************************************************************
 Function    : rotarySwitchStatus3Process
 Description : status 3 状态处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatus3Process()
{
    switch (myRotaryKeyInfo.curAction) {
    case ROTARY_KEY_ACTION_1:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_1);
        break;
    case ROTARY_KEY_ACTION_2:
        myRotaryKeyInfo.rotaryCount -= 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_ANTICLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_2);
        break;
    case ROTARY_KEY_ACTION_3:
        break;
    case ROTARY_KEY_ACTION_4:
        myRotaryKeyInfo.rotaryCount += 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_CLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_4);
        break;
    default:
        break;
    }
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
}

/******************************************************************************
 Function    : rotarySwitchStatus4Process
 Description : status 4 状态处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatus4Process()
{
    switch (myRotaryKeyInfo.curAction) {
    case ROTARY_KEY_ACTION_1:
        myRotaryKeyInfo.rotaryCount += 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_CLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_1);
        break;
    case ROTARY_KEY_ACTION_2:
        rotarySwitchStatusSet(ROTARY_KEY_STATE_2);
        break;
    case ROTARY_KEY_ACTION_3:
        myRotaryKeyInfo.rotaryCount -= 1;
        myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_ANTICLOCKWISE;
        rotarySwitchStatusSet(ROTARY_KEY_STATE_3);
        break;
    case ROTARY_KEY_ACTION_4:
        break;
    default:
        break;
    }
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
}

/******************************************************************************
 Function    : rotarySwitchStatusMachine
 Description : rotary switch状态机
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchStatusMachine() 
{
    switch(myRotaryKeyInfo.curState)
    {
    case ROTARY_KEY_STATE_INIT:
        rotarySwitchStatusInitProcess();
        break;
    case ROTARY_KEY_STATE_1:
        rotarySwitchStatus1Process();
        break;
    case ROTARY_KEY_STATE_2:
        rotarySwitchStatus2Process();
        break;
    case ROTARY_KEY_STATE_3:
        rotarySwitchStatus3Process();
        break;
    case ROTARY_KEY_STATE_4:
        rotarySwitchStatus4Process();
        break;
    default:
        myRotaryKeyInfo.curState = ROTARY_KEY_STATE_INIT;
        break;
    }
}

/******************************************************************************
 Function    : rotaryEventProc
 Description : rotary event process
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotaryEventProc()
{
    uint32_t curTick = 0;

    if (myRotaryKeyInfo.rotaryCount != 0 ) {
        myRotaryKeyInfo.isPressKeyReleaseRun = 0;
    }

    if (myRotaryKeyInfo.pFuncGetMsTick) {
        curTick = myRotaryKeyInfo.pFuncGetMsTick();
        if (curTick - myRotaryKeyInfo.lastResetCountTick >= myRotaryKeyInfo.resetCountDelay) {
            if (myRotaryKeyInfo.rotaryCount != 0) {
                myRotaryKeyInfo.lastResetCountTick = curTick;
            } 
            if ((myRotaryKeyInfo.pFuncRotaryKeyCallback != NULL) && myRotaryKeyInfo.rotaryCount != 0) {
                myRotaryKeyInfo.pFuncRotaryKeyCallback(myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
            myRotaryKeyInfo.rotaryCount = 0;
        }
    } else {
        if (curTick++ >= myRotaryKeyInfo.resetCountDelay) {
            curTick = 0;
            if ((myRotaryKeyInfo.pFuncRotaryKeyCallback != NULL) && myRotaryKeyInfo.rotaryCount != 0) {
                myRotaryKeyInfo.pFuncRotaryKeyCallback(myRotaryKeyInfo.curPressedKeyOn, myRotaryKeyInfo.rotaryCount);
            }
            myRotaryKeyInfo.rotaryCount = 0;
        }
    }
}

/******************************************************************************
 Function    : rotarySwitchRead
 Description : rotary switch read
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void rotarySwitchRead() 
{  
    uint8_t rsVal1 = 0, rsVal2 = 0;
    uint8_t pressKeyVal = 0;
    uint32_t curTick = 0;
    static uint32_t lastTick = 0;

    if (myRotaryKeyInfo.pFuncGetMsTick) {
        curTick = myRotaryKeyInfo.pFuncGetMsTick();
        if (curTick - lastTick < myRotaryKeyInfo.intervalMs) {
            return;
        }
        lastTick = curTick;
    }

    myRotaryKeyInfo.pFuncRotaryKeyGetIOStatus(&pressKeyVal, &rsVal1, &rsVal2);
    if (myRotaryKeyInfo.curPressedKeyOn != pressKeyVal) {
        myRotaryKeyInfo.curPressedKeyOn = pressKeyVal;
        myRotaryKeyInfo.rotaryCount = 0;
    } 

    if (((rsVal1<<1) | rsVal2) != myRotaryKeyInfo.curRotaryVal) {
        myRotaryKeyInfo.curRotaryVal = ((rsVal1<<1) | rsVal2);
        Printf("rs value read %d\r\n", myRotaryKeyInfo.curRotaryVal);
        switch (myRotaryKeyInfo.curRotaryVal) {
        case 0:
            myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_1;
            break;
        case 1:
            myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_2;
            break;
        case 3:
            myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_3;
            break;
        case 2:
            myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_4;
            break;
        } 
    }
}

/******************************************************************************
 Function    : myRotaryKeyGetState
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
rotaryKeyState_t myRotaryKeyGetState()
{
    return myRotaryKeyInfo.curState;
}

/******************************************************************************
 Function    : myRotaryKeyInit
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myRotaryKeyInit(uint32_t intervalMs,
                     uint32_t filterCountMs, 
                     rotaryGetMsTickFunc protaryGetMsTickFunc,
                     rotaryKeyGetIOStatus protaryKeyGetIOStatusFunc,
                     rotaryKeyCallback protaryKeyCallbackFunc)
{
    myRotaryKeyInfo.intervalMs = intervalMs;
    myRotaryKeyInfo.curState = ROTARY_KEY_STATE_INIT;
    myRotaryKeyInfo.curAction = ROTARY_KEY_ACTION_NONE;
    myRotaryKeyInfo.curDirection = ROTARY_KEY_DIR_CLOCKWISE;
    myRotaryKeyInfo.curPressedKeyOn = 0;
    myRotaryKeyInfo.rotaryCount = 0;
    myRotaryKeyInfo.isPressKeyReleaseRun = 0;
    myRotaryKeyInfo.curRotaryVal = 0xFF;
    myRotaryKeyInfo.resetCountDelay = filterCountMs;
    myRotaryKeyInfo.pFuncGetMsTick = protaryGetMsTickFunc;
    myRotaryKeyInfo.pFuncRotaryKeyGetIOStatus = protaryKeyGetIOStatusFunc;
    myRotaryKeyInfo.pFuncRotaryKeyCallback = protaryKeyCallbackFunc;
    myRotaryKeyInfo.init = 1;
}

/******************************************************************************
 Function    : myRotaryKeyLoop
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myRotaryKeyLoop()
{
    if (myRotaryKeyInfo.init) {
        rotarySwitchRead();
        rotarySwitchStatusMachine();
        rotaryEventProc();
    }
}

#ifdef __cplusplus
}
#endif

