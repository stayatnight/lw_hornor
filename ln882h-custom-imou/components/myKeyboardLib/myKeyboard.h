/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myKeyboard.h                                                  *
* Description :                                                               *
*               my keyboard处理任务头文件                                      *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2018-09-03                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_LIB_KEYBOARD_H__
#define __MY_LIB_KEYBOARD_H__

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
#define MY_LIB_KEYBOARD_VERSION                               (103)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
/*
 * get ms tick function
 * this function get the ms tick value for keyboard,
 * user must to complete this function, this is very importent
 * this function be called in myKeyboardInit function
*/
typedef uint32_t (*getMsTickFunc)(void);

/*
 * get key status function
 * this function get the key status value for keyboard,
 * user must to complete this function, this is very importent
 * this function be called in myKeyboardRegisterKey function
*/
typedef uint32_t (*myKeyboardKeyStatusGetFunc)(uint32_t keyValue);

/*
 * key events callback function
 * this function proccess the key's event callback for keyboard,
 * user need to complete this function if need, or NULL if not
 * this function be called in myKeyboardInit function
*/
typedef void (*myKeyboardEventsCallbackFunc)(uint32_t keyValue, uint32_t flag);

/*
 * set matrix row status function
 * this function set the matrix row status value for matrix keyboard,
 * user must to complete this function, this is very importent
 * this function be called in myKeyboardMatrixKeyRigisterRow function
*/
typedef void (*matrixRowStatusSet)(uint8_t flag);

/*
 * get matrix col status function
 * this function get the matrix col status value for matrix keyboard,
 * user must to complete this function, this is very importent
 * this function be called in myKeyboardMatrixKeyRigisterCol function
*/
typedef uint32_t (*matrixColStatusGet)(void);

//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
/******************************************************************************
 Function    : myKeyboardRegisterKey
 Description : register a key object with init param
 Note        : this function register a key object, after call myKeyboardInit function
               user need to call this function to add some keys and then user can call
               other function to proccess these keys.
 Input Para  : keyValue -- key value (1bit means one key, most 32 keys support)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardRegisterKey(uint32_t keyValue);

/******************************************************************************
 Function    : myKeyboardUnRegisterKey
 Description : unregister a key object
 Note        : this function unregister a key object, after register a key object
               user can call this function to unrigister the key by the key value
 Input Para  : keyValue -- key value
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardUnRegisterKey(uint32_t keyValue);

/******************************************************************************
 Function    : myKeyboardRigisterCallback
 Description : keyboard register callback
 Note        : this function complete keyboard callback function register,
               user must call this function to register at least one callback function when use lib
 Input Para  : pFuncKeyShortPress -- key short press event callback function, complete by user if need, or can be NULL
               pFuncKeyShortRelease -- key short release event callback function, complete by user if need, or can be NULL
               pFuncKeyLongPress -- key long press event callback function, complete by user if need, or can be NULL
               pFuncKeyLongRelease -- key long release event callback function, complete by user if need, or can be NULL
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardRigisterCallback(myKeyboardEventsCallbackFunc pFuncKeyShortPress,
                                   myKeyboardEventsCallbackFunc pFuncKeyShortRelease,
                                   myKeyboardEventsCallbackFunc pFuncKeyLongPress,
                                   myKeyboardEventsCallbackFunc pFuncKeyLongRelease);

/******************************************************************************
 Function    : myKeyboardMatrixKeyRigisterRow
 Description : register a matrix row object for matrix key
 Note        : this function register a matrix row object, if the keyboard is matrix,
               user need to call this function to add some matrix rows info and then user can call
               other function to proccess these keys.
 Input Para  : row -- matrix row index (0 start)
               pFuncMatrixRowStatusSet -- row status set function, system use this function to set the row status, 
                                    user must complete this function when register a matrix row
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardMatrixKeyRigisterRow(uint8_t row, matrixRowStatusSet pFuncMatrixRowStatusSet);

/******************************************************************************
 Function    : myKeyboardMatrixKeyRigisterCol
 Description : register a matrix col object for matrix key
 Note        : this function register a matrix col object, if the keyboard is matrix,
               user need to call this function to add some matrix cols info and then user can call
               other function to proccess these keys.
 Input Para  : col -- matrix col index (0 start)
               pFuncMatrixColStatusGet -- col status get function, system use this function to get the col status, 
                                    user must complete this function when register a matrix col
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardMatrixKeyRigisterCol(uint8_t col, matrixColStatusGet pFuncMatrixColStatusGet);

/******************************************************************************
 Function    : myKeyboardMatrixKeyValueGet
 Description : get a matrix keyboard value
 Note        : this function get the matrix keyboard value
 Input Para  : (none)
 Output Para : (none)
 Return      : key value
 Other       : (none)
******************************************************************************/
uint32_t myKeyboardMatrixKeyValueGet(void);

/******************************************************************************
 Function    : myKeyboardInit
 Description : keyboard init
 Note        : this function complete keyboard init,
               user must call this function first when usb the lib
 Input Para  : keyCheckInterval -- key check interval time (unit: ms)
               keyFilterCount -- key check filter count (filter time = (keyCheckInterval*keyFilterCount)ms)
               keyLongPressKeepTime -- key pressed keep this time change to key long press status (unit: ms)
               keyLongPressLoopTime -- when at key long press status, every this time key long press callback 
                                       function will be called (unit: ms)
               pFuncGetMsTick -- get systick(unit:ms), this function must be complete by user
               pFuncKeyStatusGet -- get key status by keyvalue,this function must be complete by user
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
int8_t myKeyboardInit(uint32_t keyCheckInterval, 
                       uint32_t keyFilterCount, 
                       uint32_t keyLongPressKeepTime,  
                       uint32_t keyLongPressLoopTime,
                       uint32_t keyDoublePressInterval,
                       getMsTickFunc pFuncGetMsTick,
                       myKeyboardKeyStatusGetFunc pFuncKeyStatusGet);

/******************************************************************************
 Function    : myKeyboardLoop
 Description : keyboard main loop
 Note        : this function complete keyboard main loop,
               user must loop call this function within keyCheckInterval time when usb the lib
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myKeyboardLoop(void);

#ifdef __cplusplus
}
#endif

#endif /*__MY_LIB_KEYBOARD_H__*/



