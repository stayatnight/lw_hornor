/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalLog.h                                                    *
* Description :                                                               *
*               my Hal层 Log 处理头文件                                        *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-10-21                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_HAL_LOG_H__
#define __MY_HAL_LOG_H__

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
#define my_hal_log_debug(_message,...)   myHalLog(APP_LOG_LEVEL_DEBUG, __FUNCTION__, __LINE__, (_message),##__VA_ARGS__)
#define my_hal_log_info(_message,...)    myHalLog(APP_LOG_LEVEL_INFO, __FUNCTION__, __LINE__, (_message),##__VA_ARGS__)
#define my_hal_log_notice(_message,...)  myHalLog(APP_LOG_LEVEL_NOTICE, __FUNCTION__, __LINE__, (_message),##__VA_ARGS__)
#define my_hal_log_warning(_message,...) myHalLog(APP_LOG_LEVEL_WARNING, __FUNCTION__, __LINE__, (_message),##__VA_ARGS__)
#define my_hal_log_error(_message,...)   myHalLog(APP_LOG_LEVEL_ERROR, __FUNCTION__, __LINE__, (_message),##__VA_ARGS__)
#define my_hal_log_hexdump(_message, _data, _len, ...) myHalLogHexdump(APP_LOG_LEVEL_NOTICE, __FUNCTION__, __LINE__, (_message), (_data), (_len),##__VA_ARGS__)
#define my_hal_log_raw                   printf
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
void myHalLog(uint8_t ucLevel, const char *func, int line, const char *message, ...);
void myHalLogHexdump(uint8_t ucLevel, const char *func, int line, const char *message, const void *data, int length, ...);
void myHalLogSetLevel(uint8_t ucLevel);
uint8_t myHalLogGetLevel(void);

#ifdef __cplusplus
}
#endif

#endif 

