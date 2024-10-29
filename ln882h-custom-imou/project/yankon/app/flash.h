#ifndef __FLASH_H__
#define __FLASH_H__
#include"stdint.h"
#include "ln_test_common.h"
#include "ln_flash_test.h"
//user 分区，user size :64kb
#define MY_HAL_FLASH_BASE                                    (0x001E9000)
#define MY_HAL_FLASH_SIZE                                      (1024*64)
#define MAC_FLASH_ADDR                                  0
#define FCT_DATA_FLASH_ADDR                             4096
#define CFG_DATA_FLASH_ADDR                             4096*2
#define CFG_DATA_FLASH_ADDR_BACKUP                      4096*3
int myhal_flash_read(uint32_t addr, uint8_t *buf,  uint32_t len);
int myhal_flash_write(uint32_t addr, uint8_t *buf,  uint32_t len);
void myhal_flash_erase(uint32_t addr);
#endif