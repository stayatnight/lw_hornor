/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : halFlash.c                                                    *
* Description :                                                               *
*               hal flash source file                                         *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-08                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalFlash.h"
#include "flash_api.h"
#include "device_lock.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_HAL_FLASH_BASE                                 0x001E0000
#define MY_HAL_FLASH_SIZE                                 60*1024
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
static flash_t flash;
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
 Function    : myHalFlashRead
 Description : hal read Flash
 Note        : (none)
 Input Para  : addr -- 起始地址
               len -- 长度
               buf -- 数据缓存
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalFlashRead(uint32_t addr, uint8_t *buf, uint32_t len)
{
    int res;

    if (!buf || addr+len>=MY_HAL_FLASH_SIZE) {
        return -1;
    }

    device_mutex_lock(RT_DEV_LOCK_FLASH);
    res = flash_stream_read(&flash, MY_HAL_FLASH_BASE+addr, len, buf);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);

    return (res == 1 ? 0 : -2);
}

/******************************************************************************
 Function    : myHalFlashWrite
 Description : hal write Flash
 Note        : (none)
 Input Para  : addr -- 起始地址
               len -- 长度
               buf -- 数据缓存
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalFlashWrite(uint32_t addr, uint8_t *buf, uint32_t len)
{
    int res;

    if (!buf || addr+len>=MY_HAL_FLASH_SIZE) {
        return -1;
    }

    device_mutex_lock(RT_DEV_LOCK_FLASH);
    res = flash_stream_write(&flash, MY_HAL_FLASH_BASE+addr, len, buf);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);

    return (res == 1 ? 0 : -2);
}

/******************************************************************************
 Function    : myHalFlashErase
 Description : hal erase Flash
 Note        : (none)
 Input Para  : addr -- 起始地址
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalFlashErase(uint32_t addr)
{
    if ( addr >= MY_HAL_FLASH_SIZE) {
        return -1;
    }
    addr -= addr % 4096;
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    flash_erase_sector(&flash, MY_HAL_FLASH_BASE+addr);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
    return 0;
}

#ifdef __cplusplus
}
#endif

