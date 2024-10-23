#include "hal/hal_common.h"
#include "hal/hal_gpio.h"
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "ln_test_common.h"
#include"gpio.h"
uint32_t data_buf[100];
uint8_t pin_sta = 0;
int8_t myhal_gpioa_init(gpio_pin_t pin,hal_mode_t mode)
{
if(mode>=HAL_IO_MODE_NUM){
    LOG(LOG_LVL_INFO,"GPIOA INIT FAILED");
    return -1;}
gpio_init_t_def gpio_init;
memset(&gpio_init, 0, sizeof(gpio_init));
gpio_init.pin = pin;
gpio_init.speed=GPIO_HIGH_SPEED;
hal_gpio_init(GPIOA_BASE, &gpio_init);
int pin_sta = 0;
switch (mode)
{
case HAL_IO_MODE_IN_PULLUP :
    //上拉输入
    hal_gpio_pin_pull_set(GPIOA_BASE,pin,GPIO_PULL_UP);
    hal_gpio_pin_direction_set(GPIOA_BASE, pin, GPIO_INPUT);
    pin_sta=hal_gpio_pin_read(GPIOA_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOA  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_IN_PULLDOWN :
    //下拉输入
    hal_gpio_pin_pull_set(GPIOA_BASE,pin,GPIO_PULL_DOWN);
    hal_gpio_pin_direction_set(GPIOA_BASE, pin, GPIO_INPUT);
    pin_sta=hal_gpio_pin_read(GPIOA_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOA  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_OUT_PULLUP:
    //上拉输出
    hal_gpio_pin_pull_set(GPIOA_BASE,pin,GPIO_PULL_UP);
    hal_gpio_pin_direction_set(GPIOA_BASE, pin, GPIO_OUTPUT);
    pin_sta=hal_gpio_pin_read(GPIOA_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOA  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_OUT_PULLDOWN: 
    //下拉输出
    hal_gpio_pin_pull_set(GPIOA_BASE,pin,GPIO_PULL_DOWN);
    hal_gpio_pin_direction_set(GPIOA_BASE, pin, GPIO_OUTPUT);
    pin_sta=hal_gpio_pin_read(GPIOA_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOA  STATUS:%d\n",pin_sta);
    break;
default:
    return -1;
    break;
}
return 0;
}
int8_t myhal_gpiob_init(gpio_pin_t pin,hal_mode_t mode)
{
gpio_init_t_def gpio_init;
memset(&gpio_init, 0, sizeof(gpio_init));
gpio_init.pin = pin;
gpio_init.speed=GPIO_HIGH_SPEED;
hal_gpio_init(GPIOB_BASE, &gpio_init);
switch (mode)
{
case HAL_IO_MODE_IN_PULLUP :
    //上拉输入
    hal_gpio_pin_pull_set(GPIOB_BASE,pin,GPIO_PULL_UP);
    hal_gpio_pin_direction_set(GPIOB_BASE, pin, GPIO_INPUT);
    pin_sta=hal_gpio_pin_read(GPIOB_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOB  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_IN_PULLDOWN :
    //下拉输入
    hal_gpio_pin_pull_set(GPIOB_BASE,pin,GPIO_PULL_DOWN);
    hal_gpio_pin_direction_set(GPIOB_BASE, pin, GPIO_INPUT);
    pin_sta=hal_gpio_pin_read(GPIOB_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOB  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_OUT_PULLUP:
    //上拉输出
    hal_gpio_pin_pull_set(GPIOB_BASE,pin,GPIO_PULL_UP);
    hal_gpio_pin_direction_set(GPIOB_BASE, pin, GPIO_OUTPUT);
    pin_sta=hal_gpio_pin_read(GPIOB_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOB  STATUS:%d\n",pin_sta);
    break;
case HAL_IO_MODE_OUT_PULLDOWN: 
    //下拉输出
    hal_gpio_pin_pull_set(GPIOB_BASE,pin,GPIO_PULL_DOWN);
    hal_gpio_pin_direction_set(GPIOB_BASE, pin, GPIO_OUTPUT);
    pin_sta=hal_gpio_pin_read(GPIOB_BASE, pin);
    LOG(LOG_LVL_INFO,"GPIOB  STATUS:%d\n",pin_sta);
    break;
default:
    break;
}
return 0;
}
int8_t myhal_gpio_set_status(gpio_pin_t pin,hal_io_status_t status)
{



}
int8_t myhal_gpiob_get_status(gpio_pin_t pin ,hal_io_status_t *status)
{
*status = hal_gpio_pin_read(GPIOB_BASE,pin);
if(*status>=HAL_IO_NUM)
return -1;
else
{
LOG(LOG_LVL_INFO,"get IO STATUS %d\n",*status);
return *status;
}
}