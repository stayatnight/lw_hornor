/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalLog.c                                                    *
* Description :                                                               *
*               my HAL层log处理源文件                                          *
* Note        : (none)                                                        *
* Author      : Kris li                                                       *
* Date:       : 2022-10-21                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalLog.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define my_hal_log_mutex_create() if( !my_hal_log_mutex ){my_hal_log_mutex = xSemaphoreCreateMutex();}
#define my_hal_log_mutex_take() if( my_hal_log_mutex ){xSemaphoreTake(my_hal_log_mutex, portMAX_DELAY);}
#define my_hal_log_mutex_give() if( my_hal_log_mutex ){xSemaphoreGive(my_hal_log_mutex);}
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
static uint8_t s_halLogCurrentLevel = APP_LOG_LEVEL_DEFAULT;
static xSemaphoreHandle my_hal_log_mutex = NULL;
static char s_halLogBuf[256];
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
 Function    : myHalLog
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myHalLog(uint8_t ucLevel, const char *func, int line, const char *message, ...)
{
    if (ucLevel < s_halLogCurrentLevel) {
        return;
    }
    else {
        uint32_t systick = xTaskGetTickCount();
        int sec = systick / 1000;
        int mSecond = systick % 1000;
        int second = sec % 60;
        int minute = (sec / 60) % 60;
        int hour = (second / 3600) % 24;
        va_list ap;

        my_hal_log_mutex_create();
        va_start(ap, message);
        my_hal_log_mutex_take();
        vsprintf(s_halLogBuf, message, ap);
        printf("[%02d:%02d:%02d-%03d][%s-%d]: %s", hour, minute, second, mSecond, func, line, s_halLogBuf);
        my_hal_log_mutex_give();
        va_end(ap);
    }
}

/******************************************************************************
 Function    : myHalLogHexdump
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myHalLogHexdump(uint8_t ucLevel, const char *func, int line, const char *message, const void *data, int length, ...)
{
    va_list ap;

    if (ucLevel < s_halLogCurrentLevel) {
        return;
    }

    my_hal_log_mutex_create();
    va_start(ap, message);
    my_hal_log_mutex_take();
    vsprintf(s_halLogBuf, message, ap);
    printf("[%ums] %s %d: %s", xTaskGetTickCount(), func, line, s_halLogBuf);
    for (int index = 0; index < length; index++) {
        unsigned char *pData = (unsigned char *)data;
        printf("%02X ", (int)(pData[index]));
        if ((index + 1) % 16 == 0) {
            printf("\r\n");
            continue;
        }
    }
    printf("\r\n");
    my_hal_log_mutex_give();
    va_end(ap);
}

/******************************************************************************
 Function    : myHalLogSetLevel
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void myHalLogSetLevel(uint8_t ucLevel) 
{
    s_halLogCurrentLevel = ucLevel;
}

/******************************************************************************
 Function    : myHalLogGetLevel
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t myHalLogGetLevel(void) 
{
    return s_halLogCurrentLevel;
}



