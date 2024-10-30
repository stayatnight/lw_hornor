#pragma once
#include"dimmingLib.h"
#include "hal/hal_common.h"
#include "hal/hal_gpio.h"
#define PWM_W                                           GPIO_PIN_6
#define PWM_C                                           GPIO_PIN_7
#define PWM_MAX                                         (100)
#define LIGHT_BRIGHT_MAX                                   (100)
#define LIGHT_BRIGHT_MIN                                   (uint16_t)(LIGHT_BRIGHT_MAX * 1.0f / 100.0f + 0.5f)
#define LIGHT_COLOR_CCT_MIN                                (3000)
#define LIGHT_COLOR_CCT_MAX                                (5700)
//渐变时间
#define DIMMING_TRANSIT_TIME                               (1000) //ms
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING) //reading
#define LIGHT_CCT_ENABLE                                   (1)
#define LIGHT_PWM_FREQ                                     (100000)
#define LIGHT_PWM_OUTPUT_MAX_POWER_RATIO                   (1.00)  //最大100%功率输出
#define LIGHT_PWM_BRIGHT_MIN_PERCENT                       (0.01f) //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_PWM_BRIGHT_DEF_PERCENT                       (1.0f)  //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_CCT_DEFAULT                                  (LIGHT_COLOR_CCT_MID)
#define LIGHT_PWM_MAX                                      PWM_MAX
#define LIGHT_PWM_MIN                                      (uint32_t)(PWM_MAX * 2.0f / 100)
#define LIGHT_LAMP_SOFT_CAPACITY_VAL                       (LIGHT_PWM_MIN * 30)
#define LIGHT_PWM_CURVE                                     MY_DIMMING_CURVE_SUPPORT_LINEAR
#define LIGHT_SCENE_MODE_DEFAULT                           RL_LAMP_LIGHT_MODE_READ
#endif
#define LIGHT_COLOR_CCT_MID                                (LIGHT_COLOR_CCT_MIN+(LIGHT_COLOR_CCT_MAX-LIGHT_COLOR_CCT_MIN)/2)

#ifndef LIGHT_CCT_DEFAULT
#define LIGHT_CCT_DEFAULT                                  LIGHT_COLOR_CCT_MID
#endif

#define LIGHT_PWM_BRIGHT_MIN                               (uint16_t)(LIGHT_BRIGHT_MAX * LIGHT_PWM_BRIGHT_MIN_PERCENT + 0.5f)
#define LIGHT_BRIGHT_DEFAULT                               (uint16_t)(LIGHT_BRIGHT_MAX * LIGHT_PWM_BRIGHT_DEF_PERCENT + 0.5f)
#define LIGHT_BRIGHT_LOCAL_TO_PWM(b)                       (uint16_t)(LIGHT_PWM_BRIGHT_MIN + (float)((b)-LIGHT_BRIGHT_MIN) * (LIGHT_BRIGHT_MAX - LIGHT_PWM_BRIGHT_MIN) / (LIGHT_BRIGHT_MAX-LIGHT_BRIGHT_MIN) + 0.5f)

/* bright: value <---> percent */
#define LIGHT_BRIGHT_TO_PERCENT(b)                         (uint32_t)((b) * 100.0f /100 + 0.5f)
#define LIGHT_PERCENT_TO_BRIGHT(p)                         (uint16_t)((p) * 100.0f / 100 + 0.5f)
#define APP_MIN_VAL(a, b)                                  (((a) > (b)) ? (b) : (a))
#define RL_LAMP_REPORT_INTERVAL                            (800) //ms
static uint8_t gucLampId;
static myLampParam_t s_stCurLampParam = {0};
static myLampParam_t s_stLstLampParam = {0};
//灯光控制函数
int LampSwitchCtrl(int status, int ulPeroidMs);
int LampBriPercentCtrl(int ucPercent, int ulPeroidMs);
int lamp_flash_count(void);
//台灯各种模式
#define LIGHT_BRIGHT_MODE_READING                           (8)
#define LIGHT_BRIGHT_MODE_MOON                             (10)
#define LIGHT_BRIGHT_MODE_WRITE                             (11)
//灯光模式的具体亮度
#define LIGHT_BRIGHT_MODE_READING_VAL                       (100)
#define LIGHT_BRIGHT_MODE_MOON_VAL                         (15)
#define LIGHT_BRIGHT_MODE_WRITE_VAL                        (50)
//MCU版本
#define LN_MCU_VERSION                                     ""

//PWM打印
#define PRINT_PWM   0

