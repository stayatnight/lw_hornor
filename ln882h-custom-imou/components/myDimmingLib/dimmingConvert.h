/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingConvert.h                                              *
* Description :                                                               *
*               dimming Color to pwm Argorithm处理任务头文件                   *
* Note        : (none)                                                        *
* Author      : Zhijun Li                                                     *
* Date:       : 2017-07-25                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_COLOR_CONVERT_H__
#define __MY_COLOR_CONVERT_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/

/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_LIB_CTPA_DEBUG_ENABLE                                 (0)

#define MY_LIB_CTPA_USE_MATH_LIB                                 (1)

#define MY_DIMMING_CONVERT_SUPPORT_CCT_2_HL_PWM                  (0)

#define MY_DIMMING_CONVERT_SUPPORT_CCT_2_RGBW_PWM                (0)

#define MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGB_PWM                (0)

#define MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGBW_PWM               (0)

#define MY_DIMMING_CONVERT_SUPPORT_RGBC_2_RGB_PWM                (0)

#define MY_DIMMING_CONVERT_SUPPORT_RGBC_2_SRGB                   (0)

#define MY_DIMMING_CONVERT_SUPPORT_PWM_2_SRGB                    (0)

#define MY_DIMMING_CONVERT_SUPPORT_BRI_2_CCT_2_RGBW_PWM          (0)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
typedef signed char     my_int8_t;
typedef unsigned char   my_uint8_t;
typedef signed short    my_int16_t;
typedef unsigned short  my_uint16_t;
typedef signed int      my_int32_t;
typedef unsigned int    my_uint32_t;
typedef float           my_float_t;
 
typedef enum
{
    MY_SENSOR_COLOR_C,
    MY_SENSOR_COLOR_R,
    MY_SENSOR_COLOR_G,
    MY_SENSOR_COLOR_B,
    MY_SENSOR_COLOR_NUM,
} mySensorColor_t;

typedef enum
{
    MY_POWER_OUT_TYPE_MAX,
    MY_POWER_OUT_TYPE_FIX,
    MY_POWER_OUT_TYPE_NUM
} myPowerOutType_t;

typedef struct
{
    my_uint32_t powerOutType;
    float powerFixVal;
}myCTPApowerConfig_t;

#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_HL_PWM
typedef struct
{
    float pwmMax;
    my_uint32_t cctMin;
    my_uint32_t cctMax;   
    float cctArray[2][4];
    myCTPApowerConfig_t powerConfig;
} myCTPAcct2HLPwmParam_t;
#endif

typedef struct
{
    float pwmMax;
    myCTPApowerConfig_t powerConfig;
} myCTPAcct2RGBPwmParam_t;

#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_RGBW_PWM
typedef struct
{
    float pwmMax;
    my_uint32_t cctMin;
    my_uint32_t cctMax;
    float cctArray[4][4];
    myCTPApowerConfig_t powerConfig;
} myCTPAcct2RGBWPwmParam_t;
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGB_PWM
typedef struct
{
    float pwmMax;
    float xyzObj[3][3];
    myCTPApowerConfig_t powerConfig;
} myCTPAsRGB2RGBPwmParam_t;
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGBW_PWM
typedef struct
{
    float pwmMax;
    float xyzObj[3][3];
    float sORGB[3][3];
    float wrg[4][3];
    float wgb[4][3];
    float wrb[4][3];
    float rgb[4][3];
    myCTPApowerConfig_t powerConfig;
} myCTPAsRGB2RGBWPwmParam_t;
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_RGB_PWM
typedef struct
{
    float pwmMax;
    float xyzObj[3][3];
    float sORGB[3][3];
    myCTPApowerConfig_t powerConfig;
} myCTPArgbc2RGBPwmParam_t;
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_SRGB
typedef struct
{
    float xyzObj[3][3];
    float rgbLinear[3][3];
} myCTPArgbc2sRGBParam_t;
#endif

#if MY_DIMMING_CONVERT_SUPPORT_PWM_2_SRGB
typedef struct
{
    float pwmMax;
    float xyzObj[3][4];
    float rgbLinear[3][3];
} myCTPApwm2sRGBParam_t;
#endif

//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_HL_PWM
my_int8_t myCTPAcct2HLPwm(my_uint16_t uwCCT, const myCTPAcct2HLPwmParam_t* const pcct2HLPwmParam, my_uint8_t bri, my_uint32_t ulPWM[2]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_RGBW_PWM
my_int8_t myCTPAcct2RGBWPwm(my_uint16_t uwCCT, const myCTPAcct2RGBWPwmParam_t* const pcct2RGBWPwmParam, my_uint8_t bri, my_uint32_t ulPWM[4]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGB_PWM
my_int8_t myCTPAsRGBtoRGBPwm(const my_uint8_t sRGB[3], const myCTPAsRGB2RGBPwmParam_t* const psRGB2RGBPwmParam, my_uint16_t uwBri, my_uint32_t ulPWM[3]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGBW_PWM
my_int8_t myCTPAsRGBtoRGBWPwm(const my_uint8_t sRGB[3], const myCTPAsRGB2RGBWPwmParam_t* const psRGB2RGBWPwmParam, my_uint8_t bri, my_uint32_t ulPWM[4]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_RGB_PWM
my_int8_t myCTPArgbc2RGBPwm(const my_uint16_t rgbc[4], const myCTPArgbc2RGBPwmParam_t* const prgbc2RGBPwmParam, my_uint8_t bri, my_uint16_t uwPwm[4]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_SRGB
my_int8_t myCTPArgbc2sRGB(const my_uint16_t rgbc[4],  const myCTPArgbc2sRGBParam_t* const prgbc2sRGBParam, my_uint8_t sRGB[3]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_PWM_2_SRGB
my_int8_t myCTPApwm2sRGB(const my_uint32_t ulPWM[4], const myCTPApwm2sRGBParam_t* const ppwm2sRGBParam, my_uint8_t sRGB[3]);
#endif

#if MY_DIMMING_CONVERT_SUPPORT_BRI_2_CCT_2_RGBW_PWM
my_int8_t myCTPAbri2CCTRGBWPwm(const my_uint8_t bri, my_uint16_t uwPWM[4]);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__MY_COLOR_CONVERT_H__*/



