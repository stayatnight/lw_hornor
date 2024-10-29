#include"flash.h"
#include "hal/hal_flash.h"
#define RL_MAC_FLASH_ADDR                                  0
#define RL_FCT_DATA_FLASH_ADDR                             4096
#define RL_CFG_DATA_FLASH_ADDR                             4096*2
#define RL_CFG_DATA_FLASH_ADDR_BACKUP                      4096*3
int myhal_flash_read(uint32_t addr, uint8_t *buf,  uint32_t len)
{
    int res;

    if (!buf || addr+len>=MY_HAL_FLASH_SIZE) {
        return -1;
    }
hal_flash_read(MY_HAL_FLASH_BASE+addr, len, buf);
return (res == 0 ? 0 : -2);
}
int myhal_flash_write(uint32_t addr, uint8_t *buf,  uint32_t len)
{
    int res;
    if (!buf || addr+len>=MY_HAL_FLASH_SIZE) {
        return -1;
    }
//NOTE:把别人的一些返回值切换成自己的返回值的技巧
   res= hal_flash_program(MY_HAL_FLASH_BASE+addr, len, buf);
   return (res == 0 ? 0 : -2);
}
void myhal_flash_erase(uint32_t addr)
{
    if ( addr >= MY_HAL_FLASH_SIZE) {
        return -1;
    }
    addr -= addr % 4096;
    hal_flash_erase(MY_HAL_FLASH_BASE+addr,0x1000);
return 0;
}