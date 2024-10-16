#ifndef __FLASH_H__
#define __FLASH_H__
#include"stdint.h"
int myhal_flash_read(uint32_t addr, uint8_t *buf,  uint32_t len);
int myhal_flash_write(uint32_t addr, uint8_t *buf,  uint32_t len);
int myhal_flash_erase(uint32_t addr);
#endif