/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : halIO.c                                                       *
* Description :                                                               *
*               hal io source file                                            *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalIO.h"
#include "gpio_api.h"
#include "gpio_irq_api.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct
{
    halIO_t io;
    PinName pin;
}halIOItem_t;
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
const halIOItem_t halIOTable[] = 
{
    {HAL_IO_PA0,  PA_0},
    {HAL_IO_PA2,  PA_2},
    {HAL_IO_PA3,  PA_3},
    {HAL_IO_PA4,  PA_4},
    {HAL_IO_PA7,  PA_7},
    {HAL_IO_PA8,  PA_8},
    {HAL_IO_PA11,  PA_11},
    {HAL_IO_PA12,  PA_12},
    {HAL_IO_PA13,  PA_13},
    {HAL_IO_PA14,  PA_14},
    {HAL_IO_PA15,  PA_15},
    {HAL_IO_PA16,  PA_16},
    {HAL_IO_PA17,  PA_17},
    {HAL_IO_PA18,  PA_18},
    {HAL_IO_PA19,  PA_19},
    {HAL_IO_PA20,  PA_20},
};

static gpio_t halGpio[HAL_IO_NUM];
static gpio_irq_t halGpio_irq[HAL_IO_NUM];
static halIoIrqFunc fHalIoIrq[HAL_IO_NUM];
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
static void _halIoIrqHandle(uint32_t id, gpio_irq_event event) 
{
    if (fHalIoIrq[id]) {
        fHalIoIrq[id](id, (halIOIrqEvent_t)event);
    } 
}

/******************************************************************************
 Function    : halIOInit
 Description : OPP Hal IO模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : OPP_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t halIOInit(halIO_t io, halIOMode_t mode, halIOIrq_t irq, halIoIrqFunc irqfunc)
{
    if (io >= HAL_IO_NUM || mode >= HAL_IO_MODE_NUM || irq >= HAL_IO_IRQ_NUM) {
        my_hal_log_error("hal IO Init %d failed!\r\n", io);
        return -1;
    }

    if (irq == HAL_IO_IRQ_NULL) {
        gpio_init(&halGpio[io], halIOTable[io].pin);
        switch(mode) {
        case HAL_IO_MODE_IN_NOPULL:
            gpio_dir(&halGpio[io], PIN_INPUT);
            gpio_mode(&halGpio[io], PullNone);
            break;
        case HAL_IO_MODE_IN_PULLUP:
            gpio_dir(&halGpio[io], PIN_INPUT);
            gpio_mode(&halGpio[io], PullUp);
            break;
        case HAL_IO_MODE_IN_PULLDOWN:
            gpio_dir(&halGpio[io], PIN_INPUT);
            gpio_mode(&halGpio[io], PullDown);
            break;
        case HAL_IO_MODE_OUT_NOPULL:
            gpio_dir(&halGpio[io], PIN_OUTPUT);
            gpio_mode(&halGpio[io], PullNone);
            break;
        case HAL_IO_MODE_OUT_PULLUP:
            gpio_dir(&halGpio[io], PIN_OUTPUT);
            gpio_mode(&halGpio[io], PullUp);
            break;
        case HAL_IO_MODE_OUT_PULLDOWN:
            gpio_dir(&halGpio[io], PIN_OUTPUT);
            gpio_mode(&halGpio[io], PullDown);
            break;
        default:
            return -1;
            break;
        }
    }
    
    fHalIoIrq[io] = irqfunc;
    switch(irq) {
    case HAL_IO_IRQ_NULL:
        return 0;
        break;
    case HAL_IO_IRQ_FALLING:
        gpio_irq_init(&halGpio_irq[io], halIOTable[io].pin, _halIoIrqHandle, (uint32_t)io);
        gpio_irq_set(&halGpio_irq[io], IRQ_FALL, 1);   // Falling Edge Trigger
        gpio_irq_enable(&halGpio_irq[io]);
        break;
    case HAL_IO_IRQ_RISING:
        gpio_irq_init(&halGpio_irq[io], halIOTable[io].pin, _halIoIrqHandle, (uint32_t)io);
        gpio_irq_set(&halGpio_irq[io], IRQ_RISE, 1);   // Rising Edge Trigger
        gpio_irq_enable(&halGpio_irq[io]);
        break;
    case HAL_IO_IRQ_FALLING_RISING:
        gpio_irq_init(&halGpio_irq[io], halIOTable[io].pin, _halIoIrqHandle, (uint32_t)io);
        gpio_irq_set(&halGpio_irq[io], IRQ_FALL, 1);   // Falling and rising Edge Trigger
        gpio_irq_set(&halGpio_irq[io], IRQ_RISE, 1);
        gpio_irq_enable(&halGpio_irq[io]);
        break;
    default:
        return -4;
        break;
    }

    return 0;
}

/******************************************************************************
 Function    : halIOIrqEnable
 Description : OPP Hal io irq enable
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t halIOIrqEnable(halIO_t io) 
{
    gpio_irq_enable(&halGpio_irq[io]);
}

/******************************************************************************
 Function    : halIOIrqDisable
 Description : OPP Hal io irq disable
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t halIOIrqDisable(halIO_t io) 
{
    gpio_irq_disable(&halGpio_irq[io]);
}

/******************************************************************************
 Function    : halIOSetStatus
 Description : OPP Hal IO set status
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : OPP_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t halIOSetStatus(halIO_t io, halIOStatus_t status)
{
    if (io >= HAL_IO_NUM || status >= HAL_IO_STATUS_NUM ) {
        return -1;
    }

    gpio_write(&halGpio[io], status);

    return 0;
}

/******************************************************************************
 Function    : halIOGetStatus
 Description : OPP Hal IO get status
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : OPP_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t halIOGetStatus(halIO_t io, halIOStatus_t *pStatus)
{
    if (io >= HAL_IO_NUM || !pStatus) {
        return -1;
    }

    if ( gpio_read(&halGpio[io]) ) {
        *pStatus = HAL_IO_STATUS_HIGH;
    } else {
        *pStatus = HAL_IO_STATUS_LOW;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

