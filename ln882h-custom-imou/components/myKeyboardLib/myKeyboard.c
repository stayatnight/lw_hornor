/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myKeyboard.c                                                  *
* Description :                                                               *
*               my keyboard处理任务源文件                                      *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2018-09-03                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myKeyboard.h"
#if MY_LIB_KEYBOARD_VERSION != 103
#error Wrong include file(myKeyboard.h)
#endif
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_KEYBOARD_VALUE_NULL                            0x00000000

#define MY_KEYBOARD_KEY_CNT_MAX                           32

#define MY_KEY_MATRIX_ROW_MAX                             10

#define MY_KEY_MATRIX_COL_MAX                             10

#define MY_LIB_KEYBOARD_MATRIX_KEY_1_VALUE                0x00000001
#define MY_LIB_KEYBOARD_MATRIX_KEY_2_VALUE                0x00000002
#define MY_LIB_KEYBOARD_MATRIX_KEY_3_VALUE                0x00000004
#define MY_LIB_KEYBOARD_MATRIX_KEY_4_VALUE                0x00000008
#define MY_LIB_KEYBOARD_MATRIX_KEY_5_VALUE                0x00000010
#define MY_LIB_KEYBOARD_MATRIX_KEY_6_VALUE                0x00000020
#define MY_LIB_KEYBOARD_MATRIX_KEY_7_VALUE                0x00000040
#define MY_LIB_KEYBOARD_MATRIX_KEY_8_VALUE                0x00000080
#define MY_LIB_KEYBOARD_MATRIX_KEY_9_VALUE                0x00000100
#define MY_LIB_KEYBOARD_MATRIX_KEY_10_VALUE                0x00000200
#define MY_LIB_KEYBOARD_MATRIX_KEY_11_VALUE                0x00000400
#define MY_LIB_KEYBOARD_MATRIX_KEY_12_VALUE                0x00000800
#define MY_LIB_KEYBOARD_MATRIX_KEY_13_VALUE                0x00001000
#define MY_LIB_KEYBOARD_MATRIX_KEY_14_VALUE                0x00002000
#define MY_LIB_KEYBOARD_MATRIX_KEY_15_VALUE                0x00004000
#define MY_LIB_KEYBOARD_MATRIX_KEY_16_VALUE                0x00008000
#define MY_LIB_KEYBOARD_MATRIX_KEY_17_VALUE                0x00010000
#define MY_LIB_KEYBOARD_MATRIX_KEY_18_VALUE                0x00020000
#define MY_LIB_KEYBOARD_MATRIX_KEY_19_VALUE                0x00040000
#define MY_LIB_KEYBOARD_MATRIX_KEY_20_VALUE                0x00080000
#define MY_LIB_KEYBOARD_MATRIX_KEY_21_VALUE                0x00100000
#define MY_LIB_KEYBOARD_MATRIX_KEY_22_VALUE                0x00200000
#define MY_LIB_KEYBOARD_MATRIX_KEY_23_VALUE                0x00400000
#define MY_LIB_KEYBOARD_MATRIX_KEY_24_VALUE                0x00800000
#define MY_LIB_KEYBOARD_MATRIX_KEY_25_VALUE                0x01000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_26_VALUE                0x02000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_27_VALUE                0x04000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_28_VALUE                0x08000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_29_VALUE                0x10000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_30_VALUE                0x20000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_31_VALUE                0x40000000
#define MY_LIB_KEYBOARD_MATRIX_KEY_32_VALUE                0x80000000
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef enum
{
    MY_KEYBOARD_KEY_TYPE_MATRIX,
    MY_KEYBOARD_KEY_TYPE_IO,
    MY_KEYBOARD_KEY_TYPE_NUM
} myKeyboardKeyType_t;

typedef enum
{
    MY_KEYBOARD_STATUS_IDLE = 0,
    MY_KEYBOARD_STATUS_SHORT_PRESSED,
    MY_KEYBOARD_STATUS_SHORT_RELEASE,  
    MY_KEYBOARD_STATUS_LONG_PRESSED,
    MY_KEYBOARD_STATUS_LONG_RELEASE,  
    MY_KEYBOARD_STATUS_NUM,
} MyKeyStatus_t;

typedef struct
{
    matrixRowStatusSet pFuncMatrixRowStatusSet;
} matrixRow_t;

typedef struct
{
    matrixColStatusGet pFuncMatrixColStatusGet;
} matrixCol_t;

typedef struct
{
    uint32_t keyValue;
    uint32_t rowNum;
    uint32_t colNum;
    matrixRow_t matrixRow[MY_KEY_MATRIX_ROW_MAX];
    matrixCol_t matrixCol[MY_KEY_MATRIX_COL_MAX];
} myKeyboardMatrixKey_t;

typedef struct
{
    uint32_t keyValue;
    uint32_t keyType;
    uint32_t keyStatus;
    uint32_t keyTmpStatus;
    uint32_t keyFilter;
} myKeyboardKeyInfo_t;

typedef struct
{
    uint32_t keyNum;
    myKeyboardKeyInfo_t key[MY_KEYBOARD_KEY_CNT_MAX];
} myKeyboardKey_t;

typedef struct
{
    uint32_t keyValue;
    uint32_t keyStatus;
    uint32_t keyTaskInterval;
    uint32_t keyPressKeepTime;     // keep time for long press
    uint32_t keyPressCnt;          // click count 
    uint32_t keyDoublePressDelay;
    uint32_t keyDoublePressValue;
    uint32_t keyLongPressCnt;      // key long press callback count
} myKeyboardCtrl_t;

typedef struct
{
    uint32_t keyCheckInterval;
    uint32_t keyFilterCount;
    uint32_t keyLongPressKeepTime;
    uint32_t keyLongPressLoopTime;
    uint32_t keyDoublePressInterval;
} myKeyboardParam_t;

typedef struct
{
    myKeyboardMatrixKey_t myKeyboardMatrixKey;
    myKeyboardCtrl_t myKeyboardCtrl;
    myKeyboardParam_t myKeyboardParam;
    myKeyboardKey_t myKeyboardKey;
    getMsTickFunc pFuncGetMsTick;
    myKeyboardKeyStatusGetFunc pFuncKeyStatusGet;
    myKeyboardEventsCallbackFunc pFuncKeyShortPress;
    myKeyboardEventsCallbackFunc pFuncKeyShortRelease;
    myKeyboardEventsCallbackFunc pFuncKeyLongPress;
    myKeyboardEventsCallbackFunc pFuncKeyLongRelease;
} myKeyboard_t;

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
myKeyboard_t myKeyboard;
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
 Function    : myKeyboardMatrixKeyStatusGet
 Description : MY keyboard matrix key status get
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : key status
 Other       : (none)
******************************************************************************/
static uint32_t myKeyboardMatrixKeyStatusGet(uint32_t keyValue)
{
    return (myKeyboard.myKeyboardMatrixKey.keyValue & keyValue);
}

/******************************************************************************
 Function    : myKeyboardGetKeyPressNum
 Description : MY keyboard get key press num
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t myKeyboardGetKeyPressNum(uint32_t keyValue)
{
    uint8_t cnt = 0;
    while (keyValue != 0){
        keyValue = keyValue & (keyValue - 1);
        cnt++;
    }
    
    return cnt;
}

/******************************************************************************
 Function    : myKeyboardStatusMachine
 Description : MY keyboard 状态机
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void myKeyboardStatusMachine(uint32_t curKeyboardValue) 
{
    switch(myKeyboard.myKeyboardCtrl.keyStatus)
    {
    case MY_KEYBOARD_STATUS_IDLE:
        {
            if(curKeyboardValue != MY_KEYBOARD_VALUE_NULL)
            {
                myKeyboard.myKeyboardCtrl.keyValue = curKeyboardValue;
                myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_SHORT_PRESSED;
                myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;
                if (myKeyboard.myKeyboardCtrl.keyDoublePressValue != myKeyboard.myKeyboardCtrl.keyValue) {
                    myKeyboard.myKeyboardCtrl.keyPressCnt = 1;
                    myKeyboard.myKeyboardCtrl.keyDoublePressDelay = 0;
                    myKeyboard.myKeyboardCtrl.keyDoublePressValue = myKeyboard.myKeyboardCtrl.keyValue;
                } else {
                    myKeyboard.myKeyboardCtrl.keyPressCnt += 1;
                }
                if (myKeyboard.pFuncKeyShortPress) myKeyboard.pFuncKeyShortPress(myKeyboard.myKeyboardCtrl.keyValue, myKeyboard.myKeyboardCtrl.keyPressCnt);
            }
        } 
        break;
    case MY_KEYBOARD_STATUS_SHORT_PRESSED:
        if (curKeyboardValue == MY_KEYBOARD_VALUE_NULL) {
            if ((myKeyboard.myKeyboardCtrl.keyPressCnt > 1) || (myKeyboard.myKeyboardCtrl.keyDoublePressValue == 0 && myKeyboard.myKeyboardCtrl.keyPressCnt != 0)) {
                if (myKeyboard.pFuncKeyShortRelease) myKeyboard.pFuncKeyShortRelease(myKeyboard.myKeyboardCtrl.keyValue, myKeyboard.myKeyboardCtrl.keyPressCnt);
            }   
            myKeyboard.myKeyboardCtrl.keyValue = curKeyboardValue;
            myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_SHORT_RELEASE;
            myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;  
        } else if (curKeyboardValue == myKeyboard.myKeyboardCtrl.keyValue) {
            myKeyboard.myKeyboardCtrl.keyPressKeepTime += myKeyboard.myKeyboardCtrl.keyTaskInterval;
            if (myKeyboard.myKeyboardCtrl.keyPressKeepTime > myKeyboard.myKeyboardParam.keyLongPressKeepTime) {
                myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;
                myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_LONG_PRESSED;
                myKeyboard.myKeyboardCtrl.keyLongPressCnt = 0;
                if (myKeyboard.pFuncKeyLongPress) myKeyboard.pFuncKeyLongPress(myKeyboard.myKeyboardCtrl.keyValue, 0);
            }
        } else {
            if (myKeyboardGetKeyPressNum(curKeyboardValue) > myKeyboardGetKeyPressNum(myKeyboard.myKeyboardCtrl.keyValue)) {
                if (myKeyboard.myKeyboardCtrl.keyDoublePressValue != myKeyboard.myKeyboardCtrl.keyValue) {
                    myKeyboard.myKeyboardCtrl.keyPressCnt = 1;
                    myKeyboard.myKeyboardCtrl.keyDoublePressDelay = 0;
                    myKeyboard.myKeyboardCtrl.keyDoublePressValue = myKeyboard.myKeyboardCtrl.keyValue;
                } else {
                    myKeyboard.myKeyboardCtrl.keyPressCnt += 1;
                }
                if (myKeyboard.pFuncKeyShortPress) myKeyboard.pFuncKeyShortPress(curKeyboardValue, myKeyboard.myKeyboardCtrl.keyPressCnt);
            }
            myKeyboard.myKeyboardCtrl.keyValue = curKeyboardValue;
            myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_SHORT_PRESSED;
            myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;
        }
        break;
    case MY_KEYBOARD_STATUS_LONG_PRESSED:
        if (curKeyboardValue == MY_KEYBOARD_VALUE_NULL) {
            if (myKeyboard.pFuncKeyLongRelease)myKeyboard.pFuncKeyLongRelease(myKeyboard.myKeyboardCtrl.keyValue, myKeyboard.myKeyboardCtrl.keyLongPressCnt);
            myKeyboard.myKeyboardCtrl.keyValue = curKeyboardValue;
            myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_LONG_RELEASE;
            myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;  
        } else if (curKeyboardValue == myKeyboard.myKeyboardCtrl.keyValue) {
            myKeyboard.myKeyboardCtrl.keyPressKeepTime += myKeyboard.myKeyboardCtrl.keyTaskInterval;
            if (myKeyboard.myKeyboardCtrl.keyPressKeepTime > myKeyboard.myKeyboardParam.keyLongPressLoopTime) {
                myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;
                myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_LONG_PRESSED;
                if (myKeyboard.pFuncKeyLongPress) myKeyboard.pFuncKeyLongPress(myKeyboard.myKeyboardCtrl.keyValue, ++myKeyboard.myKeyboardCtrl.keyLongPressCnt);
            }
        } else {
            if (myKeyboardGetKeyPressNum(curKeyboardValue) > myKeyboardGetKeyPressNum(myKeyboard.myKeyboardCtrl.keyValue)) {
                if (myKeyboard.myKeyboardCtrl.keyDoublePressValue != myKeyboard.myKeyboardCtrl.keyValue) {
                    myKeyboard.myKeyboardCtrl.keyPressCnt = 1;
                    myKeyboard.myKeyboardCtrl.keyDoublePressDelay = 0;
                    myKeyboard.myKeyboardCtrl.keyDoublePressValue = myKeyboard.myKeyboardCtrl.keyValue;
                } else {
                    myKeyboard.myKeyboardCtrl.keyPressCnt += 1;
                }
                if (myKeyboard.pFuncKeyShortPress) myKeyboard.pFuncKeyShortPress(curKeyboardValue, myKeyboard.myKeyboardCtrl.keyPressCnt);
            }
            myKeyboard.myKeyboardCtrl.keyValue = curKeyboardValue;
            myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_SHORT_PRESSED;
            myKeyboard.myKeyboardCtrl.keyPressKeepTime = 0;
        }
        break;
    case MY_KEYBOARD_STATUS_SHORT_RELEASE:
    case MY_KEYBOARD_STATUS_LONG_RELEASE:
        myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_IDLE;
        break; 
    default:
        myKeyboard.myKeyboardCtrl.keyStatus = MY_KEYBOARD_STATUS_IDLE;
        break;
    }
}

/******************************************************************************
 Function    : myKeyboardFilterReset
 Description : MY keyboard filter reset
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void myKeyboardFilterReset(void) 
{
    uint8_t i=0;

    for (i=0; i<myKeyboard.myKeyboardKey.keyNum; i++) {
        myKeyboard.myKeyboardKey.key[i].keyFilter = myKeyboard.myKeyboardParam.keyFilterCount;
    }
}

/******************************************************************************
 Function    : myKeyboardRead
 Description : MY KEY 读取
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t myKeyboardRead(void) 
{
    uint8_t i=0;
    uint32_t keyStatus = 0;
    uint32_t keyVlaue = 0;

    for (i=0; i<myKeyboard.myKeyboardKey.keyNum; i++) {
        myKeyboardKeyInfo_t *pKey = &myKeyboard.myKeyboardKey.key[i];

        if (myKeyboard.myKeyboardKey.key[i].keyType == MY_KEYBOARD_KEY_TYPE_IO ) {
            keyStatus = myKeyboard.pFuncKeyStatusGet(myKeyboard.myKeyboardKey.key[i].keyValue);
        } else if (myKeyboard.myKeyboardKey.key[i].keyType == MY_KEYBOARD_KEY_TYPE_MATRIX) {
            keyStatus = myKeyboardMatrixKeyStatusGet(myKeyboard.myKeyboardKey.key[i].keyValue);
        }        

        if (keyStatus == pKey->keyStatus) {
            pKey->keyFilter = 0;
            pKey->keyTmpStatus = pKey->keyStatus;  
        } else if (keyStatus != pKey->keyTmpStatus) {
            myKeyboardFilterReset();
            pKey->keyFilter = myKeyboard.myKeyboardParam.keyFilterCount;
            pKey->keyTmpStatus = keyStatus;
        } else {
            pKey->keyFilter = pKey->keyFilter > 0 ? pKey->keyFilter-1 : 0;
            if(!pKey->keyFilter)
            {
                pKey->keyStatus = keyStatus;
            }
        }
    }

    for (i=0; i<myKeyboard.myKeyboardKey.keyNum; i++) {
        if (myKeyboard.myKeyboardKey.key[i].keyStatus) {
            keyVlaue |= myKeyboard.myKeyboardKey.key[i].keyValue;
        }   
    }

    return keyVlaue;
}

/******************************************************************************
 Function    : myKeyboardCheckKeyValid
 Description : MY keyboard check if key valid
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t myKeyboardCheckKeyValid(uint32_t keyValue)
{
    uint8_t cnt = myKeyboardGetKeyPressNum(keyValue);

    if (cnt != 1) {
        return -1;
    }
    return 0;
}

/******************************************************************************
 Function    : myKeyboardFindKey
 Description : MY keyboard find key
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t myKeyboardFindKey(uint32_t keyValue, uint32_t *pPosition)
{
    uint8_t i=0;

    if(!pPosition) 
        return -1;

    for (i=0; i<myKeyboard.myKeyboardKey.keyNum; i++) {
        if (keyValue == myKeyboard.myKeyboardKey.key[i].keyValue) {
            *pPosition = i;
            return 0;
        }
    }

    return -2;
}

/******************************************************************************
 Function    : myKeyboardRegisterKey
 Description : MY keyboard register key
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t myKeyboardRegisterKeyByType(uint32_t keyValue, uint32_t type)
{
    uint32_t keyPosition;
    uint32_t index = myKeyboard.myKeyboardKey.keyNum;

    if (myKeyboard.myKeyboardKey.keyNum >= MY_KEYBOARD_KEY_CNT_MAX) {
        return -1;
    }

    if (0 != myKeyboardCheckKeyValid(keyValue)) {
        return -2;
    }

    if (0 == myKeyboardFindKey(keyValue, &keyPosition)) {
        return -3;
    }

    myKeyboard.myKeyboardKey.key[index].keyValue = keyValue;
    myKeyboard.myKeyboardKey.key[index].keyType = type;
    myKeyboard.myKeyboardKey.key[index].keyStatus = 0;
    myKeyboard.myKeyboardKey.key[index].keyTmpStatus = 0;
    myKeyboard.myKeyboardKey.key[index].keyFilter = 0;
    myKeyboard.myKeyboardKey.keyNum++;

    return 0;
}

/******************************************************************************
 Function    : myKeyboardRegisterKey
 Description : MY keyboard register key
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardRegisterKey(uint32_t keyValue)
{
    return myKeyboardRegisterKeyByType(keyValue, MY_KEYBOARD_KEY_TYPE_IO);
}

/******************************************************************************
 Function    : myKeyboardUnRegisterKey
 Description : MY keyboard unregister key
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardUnRegisterKey(uint32_t keyValue)
{
    uint32_t keyPosition;

    if (0 != myKeyboardFindKey(keyValue, &keyPosition)) {
        return -1;
    }

    for (; keyPosition < myKeyboard.myKeyboardKey.keyNum-1; keyPosition++) {
        myKeyboard.myKeyboardKey.key[keyPosition] = myKeyboard.myKeyboardKey.key[keyPosition+1];
    }

    myKeyboard.myKeyboardKey.keyNum--;

    return 0;
}

/******************************************************************************
 Function    : myKeyboardRigisterCallback
 Description : MY keyboard register callback function
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardRigisterCallback(myKeyboardEventsCallbackFunc pFuncKeyShortPress,
                                   myKeyboardEventsCallbackFunc pFuncKeyShortRelease,
                                   myKeyboardEventsCallbackFunc pFuncKeyLongPress,
                                   myKeyboardEventsCallbackFunc pFuncKeyLongRelease) 
{
    myKeyboard.pFuncKeyShortPress = pFuncKeyShortPress;
    myKeyboard.pFuncKeyShortRelease = pFuncKeyShortRelease;
    myKeyboard.pFuncKeyLongPress = pFuncKeyLongPress;
    myKeyboard.pFuncKeyLongRelease = pFuncKeyLongRelease;
    return 0;
}

/******************************************************************************
 Function    : myKeyboardMatrixKeyScan
 Description : MY keyboard matrix key scan
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void myKeyboardMatrixKeyScan()
{
    uint8_t row, col;
    uint32_t keyIndex, status;
    myKeyboardMatrixKey_t *pMatrixKey = &myKeyboard.myKeyboardMatrixKey;

    for (row = 0; row < pMatrixKey->rowNum; row++) {
        pMatrixKey->matrixRow[row].pFuncMatrixRowStatusSet(1);
        for (col = 0; col < pMatrixKey->colNum; col++) {
            keyIndex = (col*pMatrixKey->rowNum)+row;
            status = pMatrixKey->matrixCol[col].pFuncMatrixColStatusGet();

            if (status) {
                myKeyboard.myKeyboardMatrixKey.keyValue |= 1<<keyIndex;
            } else {
                myKeyboard.myKeyboardMatrixKey.keyValue &= ~(1<<keyIndex);
            }
        }
        pMatrixKey->matrixRow[row].pFuncMatrixRowStatusSet(0);
    }
}

/******************************************************************************
 Function    : myKeyboardMatrixKeyUpdate
 Description : MY keyboard matrix key update
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t myKeyboardMatrixKeyUpdate()
{
    uint8_t i;
    uint32_t keyValue; ;
    myKeyboardMatrixKey_t *pMatrixKey = &myKeyboard.myKeyboardMatrixKey;
    uint32_t matrixKeyNum = pMatrixKey->rowNum * pMatrixKey->colNum;

    if (matrixKeyNum > 32) {
        return -1;
    }

    for (i=0; i<matrixKeyNum; i++) {
        keyValue = 1<<i;
        myKeyboardRegisterKeyByType(keyValue, MY_KEYBOARD_KEY_TYPE_MATRIX);
    }
    return 0;
}

/******************************************************************************
 Function    : myKeyboardMatrixKeyRigisterRow
 Description : MY keyboard matrix key register row
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardMatrixKeyRigisterRow(uint8_t row, matrixRowStatusSet pFuncMatrixRowStatusSet)
{ 
    int8_t res = 0;
    myKeyboardMatrixKey_t *pMatrixKey = &myKeyboard.myKeyboardMatrixKey;

    if (row > pMatrixKey->rowNum) {
        return -1;
    }
    if (!pFuncMatrixRowStatusSet) {
        return -2;
    }
    pMatrixKey->matrixRow[row].pFuncMatrixRowStatusSet = pFuncMatrixRowStatusSet;
    if (row == pMatrixKey->rowNum) {
        pMatrixKey->rowNum++;
        res = myKeyboardMatrixKeyUpdate();
        if (res) {
            pMatrixKey->rowNum--;
            return -3;
        }
    } 
    return 0;
}

/******************************************************************************
 Function    : myKeyboardMatrixKeyRigisterCol
 Description : MY keyboard matrix key register col
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardMatrixKeyRigisterCol(uint8_t col, matrixColStatusGet pFuncMatrixColStatusGet)
{
    int8_t res = 0;
    myKeyboardMatrixKey_t *pMatrixKey = &myKeyboard.myKeyboardMatrixKey;

    if (col > pMatrixKey->colNum) {
        return -1;
    }
    if (!pFuncMatrixColStatusGet) {
        return -2;
    }
    pMatrixKey->matrixCol[col].pFuncMatrixColStatusGet = pFuncMatrixColStatusGet;
    if (col == pMatrixKey->colNum) {
        pMatrixKey->colNum++;
        res = myKeyboardMatrixKeyUpdate();
        if (res) {
            pMatrixKey->colNum--;
            return -3;
        }
    }
    return 0;
}

/******************************************************************************
 Function    : myKeyboardMatrixKeyValueGet
 Description : MY keyboard matrix key value get
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint32_t myKeyboardMatrixKeyValueGet()
{
    return myKeyboard.myKeyboardMatrixKey.keyValue;
}
/******************************************************************************
 Function    : myKeyboardInit
 Description : MY KEY Proc模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myKeyboardInit(uint32_t keyCheckInterval, 
                       uint32_t keyFilterCount, 
                       uint32_t keyLongPressKeepTime,  
                       uint32_t keyLongPressLoopTime,
                       uint32_t keyDoublePressInterval,
                       getMsTickFunc pFuncGetMsTick,
                       myKeyboardKeyStatusGetFunc pFuncKeyStatusGet) 
{  
    if (!pFuncGetMsTick) {
        return -1;
    }

    myKeyboard.pFuncGetMsTick = pFuncGetMsTick;
    myKeyboard.pFuncKeyStatusGet = pFuncKeyStatusGet;
    myKeyboard.myKeyboardParam.keyCheckInterval = keyCheckInterval;
    myKeyboard.myKeyboardParam.keyFilterCount = keyFilterCount;
    myKeyboard.myKeyboardParam.keyLongPressKeepTime = keyLongPressKeepTime;
    myKeyboard.myKeyboardParam.keyLongPressLoopTime = keyLongPressLoopTime;
    myKeyboard.myKeyboardParam.keyDoublePressInterval = keyDoublePressInterval;

    myKeyboard.myKeyboardCtrl.keyDoublePressDelay = 0;
    myKeyboard.myKeyboardCtrl.keyPressCnt = 0;
    myKeyboard.myKeyboardCtrl.keyDoublePressValue = 0;

    return 0;
}

/******************************************************************************
 Function    : myKeyboardLoop
 Description : MY light key 任务模块处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myKeyboardLoop(void)
{
    uint32_t curKeyboardValue;

    static uint32_t lTick = 0;

    if (myKeyboard.pFuncGetMsTick) {
        uint32_t cTick = myKeyboard.pFuncGetMsTick();
        if (cTick - lTick < myKeyboard.myKeyboardParam.keyCheckInterval) {
            return;
        }
        myKeyboard.myKeyboardCtrl.keyTaskInterval = cTick - lTick;
        lTick = cTick;
    }
    else {
        myKeyboard.myKeyboardCtrl.keyTaskInterval = myKeyboard.myKeyboardParam.keyCheckInterval;
    }

    myKeyboardMatrixKeyScan();
    curKeyboardValue = myKeyboardRead();

    if (myKeyboard.myKeyboardCtrl.keyDoublePressValue != 0) {
        myKeyboard.myKeyboardCtrl.keyDoublePressDelay += myKeyboard.myKeyboardCtrl.keyTaskInterval;
        if (myKeyboard.myKeyboardCtrl.keyDoublePressDelay >= myKeyboard.myKeyboardParam.keyDoublePressInterval) {
            if ((curKeyboardValue == MY_KEYBOARD_VALUE_NULL) && (myKeyboard.myKeyboardCtrl.keyPressCnt == 1)) {
                if (myKeyboard.pFuncKeyShortRelease) myKeyboard.pFuncKeyShortRelease(myKeyboard.myKeyboardCtrl.keyDoublePressValue, myKeyboard.myKeyboardCtrl.keyPressCnt);
                myKeyboard.myKeyboardCtrl.keyPressCnt = 0;
            }
            myKeyboard.myKeyboardCtrl.keyDoublePressValue = 0;
            myKeyboard.myKeyboardCtrl.keyDoublePressDelay = 0; 
        }
    }  
    myKeyboardStatusMachine(curKeyboardValue);  
}

#ifdef __cplusplus
}
#endif

