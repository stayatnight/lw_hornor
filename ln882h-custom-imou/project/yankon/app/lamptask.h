#include"dimmingLib.h"
#define RL_PWM_W                                           MY_HAL_PWM_0
#define RL_PWM_C                                           MY_HAL_PWM_4
#define RL_PWM_MAX                                         (9999)
#define LIGHT_BRIGHT_MAX                                   (65535)
#define LIGHT_BRIGHT_MIN                                   (uint16_t)(LIGHT_BRIGHT_MAX * 1.0f / 100.0f + 0.5f)
#define LIGHT_COLOR_CCT_MIN                                (3000)
#define LIGHT_COLOR_CCT_MAX                                (5700)

#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING) //reading
#define LIGHT_CCT_ENABLE                                   (1)
#define LIGHT_PWM_FREQ                                     (4000)
#define LIGHT_PWM_OUTPUT_MAX_POWER_RATIO                   (1.00)  //最大100%功率输出
#define LIGHT_PWM_BRIGHT_MIN_PERCENT                       (0.01f) //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_PWM_BRIGHT_DEF_PERCENT                       (1.0f)  //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_CCT_DEFAULT                                  (LIGHT_COLOR_CCT_MID)
#define LIGHT_PWM_MAX                                      RL_PWM_MAX
#define LIGHT_PWM_MIN                                      (uint32_t)(RL_PWM_MAX * 0.0f / 100)
#define LIGHT_LAMP_SOFT_CAPACITY_VAL                       (LIGHT_PWM_MIN * 30)
#define LIGHT_PWM_CURVE                                    MY_DIMMING_CURVE_CIE1976
#define LIGHT_SCENE_MODE_DEFAULT                           RL_LAMP_LIGHT_MODE_READ
#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_CEILING) //celling
#define LIGHT_PWM_FREQ                                     (500)
#define LIGHT_CCT_ENABLE                                   (1)
#define LIGHT_PWM_OUTPUT_MAX_POWER_RATIO                   (1.00)  //最大100%功率输出
#define LIGHT_PWM_BRIGHT_MIN_PERCENT                       (0.01f) //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_PWM_BRIGHT_DEF_PERCENT                       (1.0f)  //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_CCT_DEFAULT                                  (LIGHT_COLOR_CCT_MID)
#define LIGHT_PWM_MAX                                      RL_PWM_MAX
#define LIGHT_PWM_MIN                                      (uint32_t)(RL_PWM_MAX * 8.0f / 100)
#define LIGHT_LAMP_SOFT_CAPACITY_VAL                       (LIGHT_PWM_MIN * 30)
#define LIGHT_PWM_CURVE                                    MY_DIMMING_CURVE_CIE1976
#define LIGHT_SCENE_MODE_DEFAULT                           CL_LAMP_LIGHT_MODE_GUEST
#define LIGHT_STATE_SAVE_EN                                1
#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT_PTJX) //night lamp
#define LIGHT_CCT_ENABLE                                   (1)
#define LIGHT_PWM_FREQ                                     (4000)
#define LIGHT_PWM_OUTPUT_MAX_POWER_RATIO                   (1.00)  //最大100%功率输出
#define LIGHT_PWM_BRIGHT_MIN_PERCENT                       (0.00f) //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_PWM_BRIGHT_DEF_PERCENT                       (1.0f)  //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_CCT_DEFAULT                                  (LIGHT_COLOR_CCT_MID)
#define LIGHT_PWM_MAX                                      RL_PWM_MAX
#define LIGHT_PWM_MIN                                      (uint32_t)(RL_PWM_MAX * 0.0f / 100)
#define LIGHT_LAMP_SOFT_CAPACITY_VAL                       (LIGHT_PWM_MIN * 30)
#define LIGHT_PWM_CURVE                                    MY_DIMMING_CURVE_CIE1976
#define LIGHT_SCENE_MODE_DEFAULT                           RL_LAMP_LIGHT_MODE_READ
#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING_L) //resding_L
#define LIGHT_CCT_ENABLE                                   (1)
#define LIGHT_PWM_FREQ                                     (4000)
#define LIGHT_PWM_OUTPUT_MAX_POWER_RATIO                   (1.00)  //最大100%功率输出
#define LIGHT_PWM_BRIGHT_MIN_PERCENT                       (0.01f) //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_PWM_BRIGHT_DEF_PERCENT                       (1.0f)  //percent: 0.00f-1.00f : 0%-100%
#define LIGHT_CCT_DEFAULT                                  (LIGHT_COLOR_CCT_MID)
#define LIGHT_PWM_MAX                                      RL_PWM_MAX
#define LIGHT_PWM_MIN                                      (uint32_t)(RL_PWM_MAX * 0.0f / 100)
#define LIGHT_LAMP_SOFT_CAPACITY_VAL                       (LIGHT_PWM_MIN * 30)
#define LIGHT_PWM_CURVE                                    MY_DIMMING_CURVE_CIE1976
#define LIGHT_SCENE_MODE_DEFAULT                           RL_LAMP_LIGHT_MODE_READ
#else
#endif

#define LIGHT_COLOR_CCT_MID                                (LIGHT_COLOR_CCT_MIN+(LIGHT_COLOR_CCT_MAX-LIGHT_COLOR_CCT_MIN)/2)

#ifndef LIGHT_CCT_DEFAULT
#define LIGHT_CCT_DEFAULT                                  LIGHT_COLOR_CCT_MID
#endif

#define LIGHT_PWM_BRIGHT_MIN                               (uint16_t)(LIGHT_BRIGHT_MAX * LIGHT_PWM_BRIGHT_MIN_PERCENT + 0.5f)
#define LIGHT_BRIGHT_DEFAULT                               (uint16_t)(LIGHT_BRIGHT_MAX * LIGHT_PWM_BRIGHT_DEF_PERCENT + 0.5f)
#define LIGHT_BRIGHT_LOCAL_TO_PWM(b)                       (uint16_t)(LIGHT_PWM_BRIGHT_MIN + (float)((b)-LIGHT_BRIGHT_MIN) * (LIGHT_BRIGHT_MAX - LIGHT_PWM_BRIGHT_MIN) / (LIGHT_BRIGHT_MAX-LIGHT_BRIGHT_MIN) + 0.5f)

/* bright: value <---> percent */
#define LIGHT_BRIGHT_TO_PERCENT(b)                         (uint32_t)((b) * 100.0f / 65535 + 0.5f)
#define LIGHT_PERCENT_TO_BRIGHT(p)                         (uint16_t)((p) * 65535.0f / 100 + 0.5f)

#define RL_LAMP_REPORT_INTERVAL                            (800) //ms
static uint8_t gucLampId;
static myLampParam_t s_stCurLampParam = {0};
static myLampParam_t s_stLstLampParam = {0};