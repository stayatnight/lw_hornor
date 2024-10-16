#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#define PIN_NUM 20
typedef enum
{
    HAL_IO_MODE_IN_NOPULL,
    HAL_IO_MODE_IN_PULLUP,
    HAL_IO_MODE_IN_PULLDOWN,
    HAL_IO_MODE_OUT_NOPULL,
    HAL_IO_MODE_OUT_PULLUP,
    HAL_IO_MODE_OUT_PULLDOWN,
    HAL_IO_MODE_NUM
} hal_mode_t;
typedef enum
{
    HAL_IO_LOW,
    HAL_IO_HIGH,
    HAL_IO_NUM
} hal_io_status_t;
int8_t myhal_gpioa_init(gpio_pin_t pin,hal_mode_t mode);
int8_t myhal_gpiob_init(gpio_pin_t pin,hal_mode_t mode);
int8_t myhal_gpiob_get_status(gpio_pin_t pin ,hal_io_status_t *status);
int8_t myhal_gpio_set_status(gpio_pin_t pin,hal_io_status_t status);
#endif /* __LN_TY_ATE_H__ */
