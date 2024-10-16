#include"flash.h"
#include "hal/hal_flash.h"
int myhal_flash_read(uint32_t addr, uint8_t *buf,  uint32_t len)
{
return hal_flash_read(addr, buf, len);
}
int myhal_flash_write(uint32_t addr, uint8_t *buf,  uint32_t len)
{

return hal_flash_write(addr, buf, len);
}
int myhal_flash_erase(uint32_t addr)
{

}