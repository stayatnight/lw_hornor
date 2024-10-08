/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingMain.c                                                 *
* Description :                                                               *
*               my Lamp library process source file                           *
* Note        : (none)                                                        *
* Author      : lizhijun                                                      *
* Date:       : 2018-08-09                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_LIB_LAMP_MAX_NUM                               (1)

#define MY_LIB_LAMP_COLOR_CONVERT_SIMPLIFY_ENABLE         1

#define MY_LIB_LAMP_USE_STATIC_RAM                        1

#define MY_LIB_LAMP_CTRL_PWM_MAX                          (9999)

#define MY_LIB_LAMP_CTRL_BRI_MAX                          (65535)

#define MY_LIB_LAMP_CTRL_BRI_MIN                          (0)

#define MY_LIB_LAMP_CTRL_CCT_MAX                          (65000)

#define MY_LIB_LAMP_CTRL_CCT_MIN                          (2)

#define MY_LIB_LAMP_COLOR_BUTT                            4

#define MY_LIB_LAMP_MAX_VAL(a, b)                         (((a) < (b)) ? (b) : (a))

#define MY_ABS(a)                                         (((a)>0)?((a)):(-(a)))

#ifndef NULL
#define NULL                                               (0)
#endif
/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dimmingMain.h"
//#include"utils_log.h"

#if MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT
#include "dimmingCurve.h"
#endif

#if MY_LIB_LAMP_VERSION != 202
#error Wrong include file(dimmingMain.h)
#endif
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
/* lamp control struct define */
typedef struct
{
    uint8_t ucAdjusting;
    uint8_t ucFlashing;
    uint8_t ucDimmingCurveIndex;
    flashCallbackFunc pFuncFlashCallback;
    uint32_t falshArg;
    myLampParam_t backupLampParam;
    uint32_t ulFlashCount;
    uint32_t ulFlashStepMs;
    uint32_t ulFlashPeriod;
    uint32_t ulTotalSteps;
    uint32_t ulCurStep;
    uint16_t uwSrcPwms[MY_LIB_LAMP_COLOR_BUTT];
    uint16_t uwDstPwms[MY_LIB_LAMP_COLOR_BUTT]; 
    pwmOutputFunc pFuncPwmOutput;
    color2PwmFunc pFuncColor2Pwm;
    stepPwmCalculateFunc pFuncstepPwmCalculate;
    dimmingEndFunc pFuncDimmingEnd;
    dimmingStatusCtrlHookFunc dimmingStatusCtrlHook;
    flashEndFunc pFuncflashEnd;
} lampCtrl_t;

/* lamp status struct define */
typedef struct
{
    myLampParam_t lampParam;
    uint16_t auwCurrPwms[MY_LIB_LAMP_COLOR_BUTT];
    uint16_t auwLastPwms[MY_LIB_LAMP_COLOR_BUTT];
} lampStatus_t;

typedef struct
{
    uint32_t minPwm;
    uint32_t maxPwm;
} myLampSetting_t;

/* lamp data struct define */
typedef struct 
{
    myLampSetting_t myLampSetting;
    lampCtrl_t lampCtrl;
    lampStatus_t lampStatus;//本灯具的当前状态 
} lampData_t;

/* lamp table struct define */
typedef struct
{
    uint32_t valid;
    lampData_t *pLampData;
#if MY_LIB_LAMP_USE_STATIC_RAM
    lampData_t lampData;
#endif
} lampTable_t;

/* lamp struct define */
typedef struct
{
    uint32_t lampNum;
    uint32_t ulStepInterval;
    //dimmingGetMsTickFunc pFuncGetMsTick;
    //myMutexLockFunc pFuncMutexLock;
    //myMutexUnlockFunc pFuncMutexUnlock;
    uint32_t (*pFuncGetMsTick)(void);
    void (*pFuncMutexLock)(void);
    void (*pFuncMutexUnlock)(void); 
    lampTable_t lampTable[MY_LIB_LAMP_MAX_NUM]; 
} lamp_t;
/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
#if MY_LIB_LAMP_CFG_USE_DEFAULT_ALGORITHM
const float defaultCCT2RGBWPWM_2500_3100[4][3] = 
{
{ 0.0f, -0.9303f,  2.9726f},
{ 0.0f, -0.2945f,  0.9447f},
{ 0.0f,  0.0f,     0.0f},
{ 0.0f,  0.1826f, -0.3315}
};

const float defaultCCT2RGBWPWM_3100_6500[4][3] = 
{
{ 0.0f,     0.0f,     0.0f},
{-0.0103f,  0.1396f, -0.3319f},
{-0.0065f,  0.1298f, -0.3477f},
{ 0.0037f, -0.0537f,  0.3837f},
};

const float defaultsRGB2RGBPWM[3][3] = 
{
{ 0.76648f, 0.41115f,  0.05470f},
{ 0.04820f, 0.82975f, -0.03221f},
{ 0.00622f, 0.00762f,  0.46485f}
};
#endif

static lamp_t lamp;
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
#if MY_LIB_LAMP_CFG_USE_DEFAULT_ALGORITHM
/******************************************************************************
 Function    : defaultColorCCT2RGBWPWM
 Description : MY CCT to RGBW pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t defaultColorCCT2RGBWPWM(uint16_t uwPWM[4], uint16_t uwCCT, uint8_t bri)
{
    float fTempDiv = (uwCCT / 1000.0);
    float fTempDiv2 = fTempDiv * fTempDiv;
    float afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    if ((2200 <= uwCCT) && (3099 >= uwCCT))
    {
        afPWM[0] = defaultCCT2RGBWPWM_2500_3100[0][0] * fTempDiv2 + defaultCCT2RGBWPWM_2500_3100[0][1] * fTempDiv + defaultCCT2RGBWPWM_2500_3100[0][2];
        afPWM[1] = defaultCCT2RGBWPWM_2500_3100[1][0] * fTempDiv2 + defaultCCT2RGBWPWM_2500_3100[1][1] * fTempDiv + defaultCCT2RGBWPWM_2500_3100[1][2];
        afPWM[2] = defaultCCT2RGBWPWM_2500_3100[2][0] * fTempDiv2 + defaultCCT2RGBWPWM_2500_3100[2][1] * fTempDiv + defaultCCT2RGBWPWM_2500_3100[2][2];
        afPWM[3] = defaultCCT2RGBWPWM_2500_3100[3][0] * fTempDiv2 + defaultCCT2RGBWPWM_2500_3100[3][1] * fTempDiv + defaultCCT2RGBWPWM_2500_3100[3][2];
    }
    else if ((3100 <= uwCCT) && (6500 >= uwCCT))
    {
        afPWM[0] = defaultCCT2RGBWPWM_3100_6500[0][0] * fTempDiv2 + defaultCCT2RGBWPWM_3100_6500[0][1] * fTempDiv + defaultCCT2RGBWPWM_3100_6500[0][2];
        afPWM[1] = defaultCCT2RGBWPWM_3100_6500[1][0] * fTempDiv2 + defaultCCT2RGBWPWM_3100_6500[1][1] * fTempDiv + defaultCCT2RGBWPWM_3100_6500[1][2];
        afPWM[2] = defaultCCT2RGBWPWM_3100_6500[2][0] * fTempDiv2 + defaultCCT2RGBWPWM_3100_6500[2][1] * fTempDiv + defaultCCT2RGBWPWM_3100_6500[2][2];
        afPWM[3] = defaultCCT2RGBWPWM_3100_6500[3][0] * fTempDiv2 + defaultCCT2RGBWPWM_3100_6500[3][1] * fTempDiv + defaultCCT2RGBWPWM_3100_6500[3][2];
    }
    else
    {
        return -1;
    }

    

    afPWM[0] = (0.0f > afPWM[0]) ? 0.0f : afPWM[0];
    afPWM[1] = (0.0f > afPWM[1]) ? 0.0f : afPWM[1];
    afPWM[2] = (0.0f > afPWM[2]) ? 0.0f : afPWM[2];
    afPWM[3] = (0.0f > afPWM[3]) ? 0.0f : afPWM[3];

    afPWM[0] = (1.0 < afPWM[0]) ? 1.0f : afPWM[0];
    afPWM[1] = (1.0 < afPWM[1]) ? 1.0f : afPWM[1];
    afPWM[2] = (1.0 < afPWM[2]) ? 1.0f : afPWM[2];
    afPWM[3] = (1.0 < afPWM[3]) ? 1.0f : afPWM[3];

    //归一化
    {
        float maxPWM = MY_LIB_LAMP_MAX_VAL(afPWM[0], afPWM[1]);

        maxPWM = MY_LIB_LAMP_MAX_VAL(maxPWM, afPWM[2]);
        maxPWM = MY_LIB_LAMP_MAX_VAL(maxPWM, afPWM[3]);

        afPWM[0] = (afPWM[0] / maxPWM) * 3.0 / 3.0;
        afPWM[1] = (afPWM[1] / maxPWM) * 3.0 / 3.0;
        afPWM[2] = (afPWM[2] / maxPWM) * 3.0 / 3.0;
        afPWM[3] = (afPWM[3] / maxPWM) * 3.0 / 3.0;
    }

    {
        afPWM[0] = afPWM[0] * MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[1] = afPWM[1] * MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[2] = afPWM[2] * MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[3] = afPWM[3] * MY_LIB_LAMP_CTRL_PWM_MAX;
    }

    {
        uwPWM[0] = (uint16_t)((afPWM[0]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[1] = (uint16_t)((afPWM[1]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[2] = (uint16_t)((afPWM[2]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[3] = (uint16_t)((afPWM[3]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
    }
    
    return 0;
}

/******************************************************************************
 Function    : defaultColorRGBtoRGBPWM
 Description : MY sRGB to RGB pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t defaultColorRGBtoRGBPWM(uint16_t uwPWM[4], const uint8_t sRGB[3], uint8_t bri)
{
    float RGBlinear[3];
    float afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};
#if MY_LIB_LAMP_COLOR_CONVERT_SIMPLIFY_ENABLE	
	{
        float tmp = sRGB[0] / 255.0f;

        RGBlinear[0] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[1] / 255.0f;
        RGBlinear[1] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[2] / 255.0f;
        RGBlinear[2] = 1.0923f * tmp *tmp - 0.1114f * tmp;

        RGBlinear[0] = RGBlinear[0] < 0.0f ? 0.0f : RGBlinear[0];
        RGBlinear[1] = RGBlinear[1] < 0.0f ? 0.0f : RGBlinear[1];
        RGBlinear[2] = RGBlinear[2] < 0.0f ? 0.0f : RGBlinear[2];
    }
#else	
    {
        float tmp = sRGB[0] / 255.0;
        if(tmp <= 0.04045)
        {
            RGBlinear[0] = tmp / 12.92;
        }
        else
        {
            RGBlinear[0] = pow((tmp + 0.055)/1.055, 2.4);
            
        }
    }

    {
        float tmp = sRGB[1] / 255.0;
        if(tmp <= 0.04045)
        {
            RGBlinear[1] = tmp / 12.92;
        }
        else
        {
            RGBlinear[1] = pow((tmp + 0.055)/1.055, 2.4);
            
        }
    }

    {
        float tmp = sRGB[2] / 255.0;
        if(tmp <= 0.04045)
        {
            RGBlinear[2] = tmp / 12.92;
        }
        else
        {
            RGBlinear[2] = pow((tmp + 0.055)/1.055, 2.4);
            
        }
    }
#endif
    afPWM[0] = defaultsRGB2RGBPWM[0][0] * RGBlinear[0] + defaultsRGB2RGBPWM[0][1] * RGBlinear[1] + defaultsRGB2RGBPWM[0][2] * RGBlinear[2];
    afPWM[1] = defaultsRGB2RGBPWM[1][0] * RGBlinear[0] + defaultsRGB2RGBPWM[1][1] * RGBlinear[1] + defaultsRGB2RGBPWM[1][2] * RGBlinear[2];
    afPWM[2] = defaultsRGB2RGBPWM[2][0] * RGBlinear[0] + defaultsRGB2RGBPWM[2][1] * RGBlinear[1] + defaultsRGB2RGBPWM[2][2] * RGBlinear[2];
    //
    afPWM[3] = MY_LIB_LAMP_MAX_VAL(afPWM[1], afPWM[2]);
    afPWM[3] = MY_LIB_LAMP_MAX_VAL(afPWM[0], afPWM[3]);
    //
    afPWM[0] = afPWM[0] / afPWM[3];
    afPWM[1] = afPWM[1] / afPWM[3];
    afPWM[2] = afPWM[2] / afPWM[3];
    afPWM[3] = 0;

    afPWM[0] = (1.000 < afPWM[0]) ? 1.000 : afPWM[0];
    afPWM[1] = (1.000 < afPWM[1]) ? 1.000 : afPWM[1];
    afPWM[2] = (1.000 < afPWM[2]) ? 1.000 : afPWM[2];
    afPWM[3] = (1.000 < afPWM[3]) ? 1.000 : afPWM[3];
    //
    afPWM[0] = (0.001 > afPWM[0]) ? 0.0 : afPWM[0];
    afPWM[1] = (0.001 > afPWM[1]) ? 0.0 : afPWM[1];
    afPWM[2] = (0.001 > afPWM[2]) ? 0.0 : afPWM[2];
    afPWM[3] = (0.001 > afPWM[3]) ? 0.0 : afPWM[3];
    //
    {
        uwPWM[0] = (uint16_t)((MY_LIB_LAMP_CTRL_PWM_MAX * afPWM[0]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[1] = (uint16_t)((MY_LIB_LAMP_CTRL_PWM_MAX * afPWM[1]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[2] = (uint16_t)((MY_LIB_LAMP_CTRL_PWM_MAX * afPWM[2]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[3] = (uint16_t)((MY_LIB_LAMP_CTRL_PWM_MAX * afPWM[3]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
    }
    
    return 0;
}

/******************************************************************************
 Function    : defaultColorRGBtoRGBWPWM
 Description : MY sRGB to RGBW pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t defaultColorRGBtoRGBWPWM(uint16_t uwPWM[4], const uint8_t sRGB[3], uint8_t bri)
{
    float RGBlinear[3];
    float afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float x, y, sum;
    float sORG, sORB, sOGB;

    //step 1-2
#if MY_LIB_LAMP_COLOR_CONVERT_SIMPLIFY_ENABLE
    {
        float tmp = sRGB[0] / 255.0f;

        RGBlinear[0] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[1] / 255.0f;
        RGBlinear[1] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[2] / 255.0f;
        RGBlinear[2] = 1.0923f * tmp *tmp - 0.1114f * tmp;

        RGBlinear[0] = RGBlinear[0] < 0.0f ? 0.0f : RGBlinear[0];
        RGBlinear[1] = RGBlinear[1] < 0.0f ? 0.0f : RGBlinear[1];
        RGBlinear[2] = RGBlinear[2] < 0.0f ? 0.0f : RGBlinear[2];
    }
    
#else
    {
        float tmp = sRGB[0] / 255.0f;
        if(tmp <= 0.04045f)
        {
            RGBlinear[0] = tmp / 12.92f;
        }
        else
        {
            RGBlinear[0] = pow((tmp + 0.055f)/1.055f, 2.4f);
        }
    }

    {
        float tmp = sRGB[1] / 255.0f;
        if(tmp <= 0.04045f)
        {
            RGBlinear[1] = tmp / 12.92f;
        }
        else
        {
            RGBlinear[1] = pow((tmp + 0.055f)/1.055f, 2.4f);
            
        }
    }

    {
        float tmp = sRGB[2] / 255.0f;
        if(tmp <= 0.04045f)
        {
            RGBlinear[2] = tmp / 12.92f;
        }
        else
        {
            RGBlinear[2] = pow((tmp + 0.055f)/1.055f, 2.4f);
            
        }
    }
#endif

    //step 3
    afPWM[0] = 0.4124f * RGBlinear[0] + 0.3576f * RGBlinear[1] + 0.1805f * RGBlinear[2];
    afPWM[1] = 0.2126f * RGBlinear[0] + 0.7152f * RGBlinear[1] + 0.0722f * RGBlinear[2];
    afPWM[2] = 0.0193f * RGBlinear[0] + 0.1192f * RGBlinear[1] + 0.9502f * RGBlinear[2];

    //step 4
    sum = afPWM[0] + afPWM[1] + afPWM[2];
    x = sum > 0 ? afPWM[0] / sum : 0.0f;
    y = sum > 0 ? afPWM[1] / sum : 0.0f;
    
    //x = afPWM[0] / (afPWM[0] + afPWM[1] + afPWM[2]);
    //y = afPWM[1] / (afPWM[0] + afPWM[1] + afPWM[2]);

    //step 5
    sORG = fabs(3.365f - 3.091f*x - 3.991f*y);
    sORB = fabs(0.055f + 2.255f*x - 5.258f*y);
    sOGB = fabs(-0.709f + 5.45f*x - 0.192f*y);

    //step 6
    if((sORG < sORB) && (sORG < sOGB)) //WRG
    {
        afPWM[0] = 0.5980f * RGBlinear[0] + 0.3210f * RGBlinear[1] - 1.0816f * RGBlinear[2];
        afPWM[1] = 0.0298f * RGBlinear[0] + 0.7831f * RGBlinear[1] - 0.9093f * RGBlinear[2];
        afPWM[2] = 0.0f;
        afPWM[3] = 0.0164f * RGBlinear[0] + 0.0187f * RGBlinear[1] + 1.1776f * RGBlinear[2];
    }
    else if((sORB < sORG) && (sORB < sOGB)) //WRB
    {
        afPWM[0] = 0.5601f * RGBlinear[0] - 0.6740f * RGBlinear[1] + 0.0738f * RGBlinear[2];
        afPWM[1] = 0.0f;
        afPWM[2] = -0.0150f * RGBlinear[0] - 0.3954f * RGBlinear[1] + 0.4591f * RGBlinear[2];
        afPWM[3] = 0.5601f * RGBlinear[0] + 1.0625f * RGBlinear[1] - 0.0345f * RGBlinear[2];
    }
    else if((sOGB < sORG) && (sOGB < sORB)) //WGB
    {
        afPWM[0] = 0.0f;
        afPWM[1] = -0.4408f * RGBlinear[0] + 0.5305f * RGBlinear[1] - 0.0581f * RGBlinear[2];
        afPWM[2] = -0.2376f * RGBlinear[0] - 0.1275f * RGBlinear[1] + 0.4298f * RGBlinear[2];
        afPWM[3] = 0.6437f * RGBlinear[0] + 0.355f * RGBlinear[1] + 0.0429f * RGBlinear[2];
    }

    //step 7
    {
#if (0)
        FLOAT maxPWM = MY_LIB_LAMP_MAX_VAL(afPWM[0], afPWM[1]);

        maxPWM = MY_LIB_LAMP_MAX_VAL(maxPWM, afPWM[2]);
        maxPWM = MY_LIB_LAMP_MAX_VAL(maxPWM, afPWM[3]);
        
        afPWM[0] = maxPWM > 0 ? afPWM[0]/maxPWM*MY_LIB_LAMP_CTRL_PWM_MAX : 0.0f;
        afPWM[1] = maxPWM > 0 ? afPWM[1]/maxPWM*MY_LIB_LAMP_CTRL_PWM_MAX : 0.0f;
        afPWM[2] = maxPWM > 0 ? afPWM[2]/maxPWM*MY_LIB_LAMP_CTRL_PWM_MAX : 0.0f;
        afPWM[3] = maxPWM > 0 ? afPWM[3]/maxPWM*MY_LIB_LAMP_CTRL_PWM_MAX : 0.0f;  
#else
        afPWM[0] = afPWM[0]*MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[1] = afPWM[1]*MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[2] = afPWM[2]*MY_LIB_LAMP_CTRL_PWM_MAX;
        afPWM[3] = afPWM[3]*MY_LIB_LAMP_CTRL_PWM_MAX; 
#endif

        afPWM[0] = afPWM[0] < 0 ? 0 : afPWM[0];
        afPWM[1] = afPWM[1] < 0 ? 0 : afPWM[1];
        afPWM[2] = afPWM[2] < 0 ? 0 : afPWM[2];
        afPWM[3] = afPWM[3] < 0 ? 0 : afPWM[3];

        afPWM[0] = afPWM[0] > MY_LIB_LAMP_CTRL_PWM_MAX ? MY_LIB_LAMP_CTRL_PWM_MAX : afPWM[0];
        afPWM[1] = afPWM[1] > MY_LIB_LAMP_CTRL_PWM_MAX ? MY_LIB_LAMP_CTRL_PWM_MAX : afPWM[1];
        afPWM[2] = afPWM[2] > MY_LIB_LAMP_CTRL_PWM_MAX ? MY_LIB_LAMP_CTRL_PWM_MAX : afPWM[2];
        afPWM[3] = afPWM[3] > MY_LIB_LAMP_CTRL_PWM_MAX ? MY_LIB_LAMP_CTRL_PWM_MAX : afPWM[3];
    }

    //step 8
    {
        float totalPWM = afPWM[0]+afPWM[1]+afPWM[2]+afPWM[3];
    
        if(totalPWM > MY_LIB_LAMP_CTRL_PWM_MAX)
        {
            afPWM[0] = afPWM[0]*MY_LIB_LAMP_CTRL_PWM_MAX/totalPWM;
            afPWM[1] = afPWM[1]*MY_LIB_LAMP_CTRL_PWM_MAX/totalPWM;
            afPWM[2] = afPWM[2]*MY_LIB_LAMP_CTRL_PWM_MAX/totalPWM;
            afPWM[3] = afPWM[3]*MY_LIB_LAMP_CTRL_PWM_MAX/totalPWM; 
        }
    }

    {
        uwPWM[0] = (uint16_t)((afPWM[0]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[1] = (uint16_t)((afPWM[1]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[2] = (uint16_t)((afPWM[2]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX);
        uwPWM[3] = (uint16_t)((afPWM[3]) * (float)bri / MY_LIB_LAMP_CTRL_BRI_MAX * 0.6f);
    }

    return 0;
}
#endif
/******************************************************************************
 Function    : color2PwmFuncDefault
 Description : default color to pwm value convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int8_t color2PwmFuncDefault(uint32_t ulPwm[4], myLampParam_t *plampParam)
{
#if MY_LIB_LAMP_CFG_USE_DEFAULT_ALGORITHM
    uint16_t uwPwm[4];

    if(plampParam->ucCtrlType == MY_LAMP_CTRL_TYPE_RGB)
    {
        defaultColorRGBtoRGBPWM(uwPwm, plampParam->ucHue, plampParam->uwBri); 
    }
    else if(plampParam->ucCtrlType == MY_LAMP_CTRL_TYPE_CCT)
    {
        defaultColorCCT2RGBWPWM(uwPwm, plampParam->uwCCT, plampParam->uwBri); 
    } 
    else
    {
        return -1;
    }

    ulPwm[0] = uwPwm[0];
    ulPwm[1] = uwPwm[1];
    ulPwm[2] = uwPwm[2];
    ulPwm[3] = uwPwm[3];
#endif
    return 0;
}

/******************************************************************************
 Function    : color2PwmFuncDefault
 Description : default color to pwm value convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t stepPwmCalculateFuncDefault(uint32_t srcPwm, uint32_t dstPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps)
{
    float afPWMVal;

    if(curStep > totalSteps) {
        return srcPwm;
    }

    if (srcPwm <= dstPwm) {
        afPWMVal = (float)srcPwm + curStep * (dstPwm - srcPwm) / (float)(totalSteps);
        afPWMVal = afPWMVal < srcPwm ? srcPwm : afPWMVal;
        afPWMVal = afPWMVal > dstPwm ? dstPwm : afPWMVal;
    } else {
        afPWMVal = (float)srcPwm - curStep * (srcPwm - dstPwm) / (float)(totalSteps);
        afPWMVal = afPWMVal < dstPwm ? dstPwm : afPWMVal;
        afPWMVal = afPWMVal > srcPwm ? srcPwm : afPWMVal;
    }

    return (uint32_t)afPWMVal;
}

/******************************************************************************
 Function    : lampStepProc
 Description : 灯处理步长PWM输出
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void lampStepProc(uint8_t lampId)
{
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;
    lampCtrl_t *pCtrl = &pLamp->lampCtrl;
    uint16_t srcPWM = 0, dstPWM = 0;
    uint32_t curTotalSteps = 0;
    uint32_t ulPWMVal[MY_LIB_LAMP_COLOR_BUTT];

    if(!pCtrl->ucAdjusting) {
        return;
    }

    pCtrl->ulCurStep++;
    if(pCtrl->ulCurStep < pCtrl->ulTotalSteps) {
        uint8_t ucSeq = 0;
        
        for (ucSeq = 0; ucSeq < MY_LIB_LAMP_COLOR_BUTT; ucSeq++) {
            srcPWM = pCtrl->uwSrcPwms[ucSeq];
            dstPWM = pCtrl->uwDstPwms[ucSeq];
            ulPWMVal[ucSeq] = (uint32_t)dstPWM;

            if (srcPWM != dstPWM ) {
//              curTotalSteps = MY_ABS(srcPWM-dstPWM) > pCtrl->ulTotalSteps ? pCtrl->ulTotalSteps : MY_ABS(srcPWM-dstPWM);
                curTotalSteps = pCtrl->ulTotalSteps;
                if (pCtrl->ulCurStep < curTotalSteps ) {
                    ulPWMVal[ucSeq] = pCtrl->pFuncstepPwmCalculate(srcPWM, dstPWM, pLamp->myLampSetting.minPwm, pLamp->myLampSetting.maxPwm, pCtrl->ulCurStep, curTotalSteps);
                } else {
                    ulPWMVal[ucSeq] = (uint32_t)dstPWM;
                } 
            }
            pLamp->lampStatus.auwCurrPwms[ucSeq] = (uint16_t)ulPWMVal[ucSeq];
        }
        pCtrl->pFuncPwmOutput(ulPWMVal[0], ulPWMVal[1], ulPWMVal[2], ulPWMVal[3]);
    } else {
        uint8_t ucSeq = 0;
        for (ucSeq = 0; ucSeq < MY_LIB_LAMP_COLOR_BUTT; ucSeq++) {
            pLamp->lampStatus.auwCurrPwms[ucSeq] = pLamp->lampCtrl.uwDstPwms[ucSeq];
            pLamp->lampCtrl.uwSrcPwms[ucSeq] = pLamp->lampCtrl.uwDstPwms[ucSeq];
        }

        pCtrl->pFuncPwmOutput((uint32_t)pLamp->lampCtrl.uwDstPwms[0], (uint32_t)pLamp->lampCtrl.uwDstPwms[1], (uint32_t)pLamp->lampCtrl.uwDstPwms[2], (uint32_t)pLamp->lampCtrl.uwDstPwms[3]);
        pCtrl->ucAdjusting = 0;
        if ( (pCtrl->pFuncDimmingEnd) && (pCtrl->ucFlashing == 0)) {
            pCtrl->pFuncDimmingEnd(lampId);
        }
        if ( (pCtrl->ulFlashCount == 0) && (pCtrl->ucFlashing == 1) ) {
            pCtrl->ucFlashing = 0;
            if ( pCtrl->pFuncflashEnd ) {
                pCtrl->pFuncflashEnd(pCtrl->falshArg);
            }
        }
    }
}

/******************************************************************************
 Function    : lampDevStatusCtrl
 Description : lamp dev status control
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void lampDevStatusCtrl(uint32_t lampId, uint32_t ulPeriodMs)
{
    int8_t res = 0;
    uint32_t ulPwm[4];
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if (pLamp->lampCtrl.dimmingStatusCtrlHook) {
        pLamp->lampCtrl.dimmingStatusCtrlHook(lampId, &pLamp->lampStatus.lampParam);
    }

    if(pLamp->lampStatus.lampParam.ucSwitch == MY_LAMP_CTRL_SW_OFF)
    {
        ulPwm[0] = ulPwm[1] = ulPwm[2] = ulPwm[3] = 0;
    }
    else
    {
        res = pLamp->lampCtrl.pFuncColor2Pwm(ulPwm, &pLamp->lampStatus.lampParam);
        if(res != 0)
        {
            return;
        }
    }
    myLampPwmsCtrl(lampId, ulPeriodMs, ulPwm);  
}

/******************************************************************************
 Function    : lampIsFlashing
 Description : lamp is falshing
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int lampIsFlashing(uint32_t lampId)
{
    if(lamp.lampTable[lampId].pLampData->lampCtrl.ucFlashing > 0)
    {
        if ( !MY_LIB_LAMP_CFG_INT_BLINKING ) {
            return 1;
        }
        lamp.lampTable[lampId].pLampData->lampCtrl.ucFlashing = 0;
        lamp.lampTable[lampId].pLampData->lampCtrl.ulFlashCount = 0;
        lamp.lampTable[lampId].pLampData->lampStatus.lampParam = lamp.lampTable[lampId].pLampData->lampCtrl.backupLampParam;
    }
    return 0;
}

/******************************************************************************
 Function    : lampFlashProc
 Description : lamp falsh process
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void lampFlashProc()
{
    uint8_t lampId = 0;
    uint8_t isFirst = 0;
    static uint32_t delay[MY_LIB_LAMP_MAX_NUM];

    for(lampId=0; lampId < MY_LIB_LAMP_MAX_NUM; lampId++) {
        if(lamp.lampTable[lampId].valid) {
            lampData_t *pLamp = lamp.lampTable[lampId].pLampData;
            if ( !pLamp->lampCtrl.ulFlashCount ) {delay[lampId] = 0xFFFFFFFF; continue;}
            delay[lampId] < (0xFFFFFFFF-lamp.ulStepInterval) ? (isFirst = 0, delay[lampId] += lamp.ulStepInterval):(isFirst = 1, delay[lampId] = 0);
            if ( delay[lampId] >= pLamp->lampCtrl.ulFlashPeriod ) {
                delay[lampId] = 0;
                if ( pLamp->lampCtrl.pFuncFlashCallback ) {
                    pLamp->lampCtrl.pFuncFlashCallback(isFirst, pLamp->lampCtrl.ulFlashCount, &pLamp->lampStatus.lampParam, &pLamp->lampCtrl.ulFlashStepMs);
                    if ( pLamp->lampCtrl.ulFlashCount == 1 ) {
                        pLamp->lampStatus.lampParam = pLamp->lampCtrl.backupLampParam;
                    }
                } else {
                    pLamp->lampStatus.lampParam.ucSwitch = (pLamp->lampStatus.lampParam.ucSwitch) ?  0 :  1;  
                } 
                lampDevStatusCtrl(lampId, pLamp->lampCtrl.ulFlashStepMs); 
                pLamp->lampCtrl.ulFlashCount--;
            }
        }  
    }
}

/******************************************************************************
 Function    : myLampCreateLamp
 Description : create a lamp object with init param
 Note        : (none)
 Input Para  : colorParam
 Output Para : pLampId
 Return      : success:0, failed:errcode
 Other       : (none)
******************************************************************************/
int8_t myLampCreateLamp(uint8_t *pLampId, myLampParam_t *plampParam,
                         pwmOutputFunc pFuncPwmOutput,
                         color2PwmFunc pFuncColor2Pwm,
                         stepPwmCalculateFunc pFuncStepPwmCal,
                         dimmingEndFunc pFuncDimmingEnd,
                         flashEndFunc pFuncflashEnd)
{
    uint8_t i = 0;

    if(lamp.lampNum >= MY_LIB_LAMP_MAX_NUM) {
        return -1;
    }

    if ((!pFuncPwmOutput) || (!pLampId)) {
        return -2;
    }

    for (i = 0; i < MY_LIB_LAMP_MAX_NUM; i++) {
        if (!lamp.lampTable[i].valid) {
            if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
            lamp.lampTable[i].valid = 1;
            
#if MY_LIB_LAMP_USE_STATIC_RAM
            lamp.lampTable[i].pLampData = &(lamp.lampTable[i].lampData);
#else
            lamp.lampTable[i].pLampData = (lampData_t*)malloc(sizeof(lampData_t));
#endif
            if(!lamp.lampTable[i].pLampData) {
                return -2;
            }
            memset(lamp.lampTable[i].pLampData, 0, sizeof(lampData_t));
            if (plampParam) {
                memcpy((uint8_t *)&lamp.lampTable[i].pLampData->lampStatus.lampParam, plampParam, sizeof(myLampParam_t));
                lamp.lampTable[i].pLampData->myLampSetting.maxPwm = plampParam->maxPwm;
                lamp.lampTable[i].pLampData->myLampSetting.minPwm = plampParam->minPwm;
            } else {
                lamp.lampTable[i].pLampData->lampStatus.lampParam.ucSwitch = 1;
                lamp.lampTable[i].pLampData->lampStatus.lampParam.uwBri = MY_LIB_LAMP_CTRL_BRI_MAX;
                lamp.lampTable[i].pLampData->lampStatus.lampParam.ucCtrlType = 0;
                lamp.lampTable[i].pLampData->lampStatus.lampParam.uwCCT = 3000;
                lamp.lampTable[i].pLampData->myLampSetting.maxPwm = MY_LIB_LAMP_CTRL_PWM_MAX;
            }
            
            lamp.lampTable[i].pLampData->lampCtrl.pFuncPwmOutput = pFuncPwmOutput;
            if (pFuncColor2Pwm) {
                lamp.lampTable[i].pLampData->lampCtrl.pFuncColor2Pwm = pFuncColor2Pwm;
            } else {
                lamp.lampTable[i].pLampData->lampCtrl.pFuncColor2Pwm = color2PwmFuncDefault;
            }

            if (pFuncStepPwmCal) {
                lamp.lampTable[i].pLampData->lampCtrl.ucDimmingCurveIndex = 0;
                lamp.lampTable[i].pLampData->lampCtrl.pFuncstepPwmCalculate = pFuncStepPwmCal;
            } else {
                lamp.lampTable[i].pLampData->lampCtrl.ucDimmingCurveIndex = 0;
                lamp.lampTable[i].pLampData->lampCtrl.pFuncstepPwmCalculate = stepPwmCalculateFuncDefault;
            }
            lamp.lampTable[i].pLampData->lampCtrl.pFuncDimmingEnd = pFuncDimmingEnd;
            lamp.lampTable[i].pLampData->lampCtrl.dimmingStatusCtrlHook = NULL;
            lamp.lampTable[i].pLampData->lampCtrl.pFuncflashEnd = pFuncflashEnd;
            lampDevStatusCtrl(i, 1);
            lamp.lampNum++;
            *pLampId = i;
            if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
            return 0;
        }
    }

    return -3;
}

/******************************************************************************
 Function    : myLampDeleteLamp
 Description : delete a lamp object
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : success:0, failed:errcode
 Other       : (none)
******************************************************************************/
int8_t myLampDeleteLamp(uint8_t lampId)
{
    if (!lamp.lampTable[lampId].valid) {
        return -1;
    }
    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    lamp.lampTable[lampId].valid = 0;
#if !MY_LIB_LAMP_USE_STATIC_RAM
    if(lamp.lampTable[lampId].pLampData) {
        free(lamp.lampTable[lampId].pLampData);
    }
#endif
    lamp.lampNum--;
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}

    return 0;
}

/******************************************************************************
 Function    : myLampSetDimmingCurve
 Description : set dimming curve
 Note        : this function set the dimming curve, when after create a lamp object,
               user can call this function to set the dimming curve by lampId
 Input Para  : lampId -- lamp ID
               curve -- curve index(reference dimmingCurve.h)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
#if MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT
int8_t myLampSetDimmingCurve(uint8_t lampId, uint8_t curve)
{
    stepPwmCalculateFunc pCurve = getMyDimmingCurve((MY_DIMMING_CURVE_t)curve);

    if ((!lamp.lampTable[lampId].valid) || (pCurve == NULL)) {
        return -1;
    }
    lamp.lampTable[lampId].pLampData->lampCtrl.ucDimmingCurveIndex = curve;
    lamp.lampTable[lampId].pLampData->lampCtrl.pFuncstepPwmCalculate = pCurve;
    return 0;
}
#endif

/******************************************************************************
 Function    : myLampGetDimmingCurve
 Description : set dimming curve
 Note        : this function get the dimming curve, when after create a lamp object,
               user can call this function to get the dimming curve by lampId
 Input Para  : lampId -- lamp ID
               curve -- curve index(reference dimmingCurve.h)
 Output Para : (none)
 Return      : 0 -- success, other -- failed
 Other       : (none)
******************************************************************************/
#if MY_LIB_LAMP_CFG_DIMMING_CURVE_SUPPORT
uint8_t myLampGetDimmingCurve(uint8_t lampId)
{
    if (!lamp.lampTable[lampId].valid) {
        return 0;
    }
    return lamp.lampTable[lampId].pLampData->lampCtrl.ucDimmingCurveIndex;
}
#endif

/******************************************************************************
 Function    : myLampStatusGet
 Description : my lamp status get
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampStatusGet(uint8_t lampId, myLampParam_t *pLampParam)
{
    if (!lamp.lampTable[lampId].valid) {
        return -1;
    }

    *pLampParam = lamp.lampTable[lampId].pLampData->lampStatus.lampParam;

    return 0;
}

/******************************************************************************
 Function    : myLampCallSceneCtrl
 Description : lamp call scene
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampCallSceneCtrl(uint8_t lampId, uint32_t ulPeriodMs, myLampParam_t *pLampParam)
{
    int ret = 0;
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if(!lamp.lampTable[lampId].valid)
    {
        return -1;
    }

    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    ret = lampIsFlashing(lampId);
    if ( ret == 0 ) {
        pLamp->lampStatus.lampParam = *pLampParam;
        lampDevStatusCtrl(lampId, ulPeriodMs);
    }
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    return 0;
}

/******************************************************************************
 Function    : myLampSwitchCtrl
 Description : 灯switch控制模块
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampSwitchCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint8_t status)
{
    int ret = 0;
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if((!lamp.lampTable[lampId].valid) || (status > MY_LAMP_CTRL_SW_ON))
    {
        return -1;
    }

    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    ret = lampIsFlashing(lampId);
    if ( ret == 0 ) {
        pLamp->lampStatus.lampParam.ucSwitch = status;

        lampDevStatusCtrl(lampId, ulPeriodMs);
    }  
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    
    return 0;
}

/******************************************************************************
 Function    : myLampBriCtrl
 Description : lamp bright control
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampBriCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint16_t uwBri)
{
    int ret = 0;
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;
  //  Log_d("myLampBriCtrl|sucess");
    if((!lamp.lampTable[lampId].valid) || (pLamp->lampStatus.lampParam.ucSwitch == MY_LAMP_CTRL_SW_OFF))
    {
        return -1;
    }

    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    ret = lampIsFlashing(lampId);
    if ( ret == 0 ) {
        pLamp->lampStatus.lampParam.uwBri = uwBri <= MY_LIB_LAMP_CTRL_BRI_MIN ? MY_LIB_LAMP_CTRL_BRI_MIN : uwBri;
        pLamp->lampStatus.lampParam.uwBri = uwBri >= MY_LIB_LAMP_CTRL_BRI_MAX ? MY_LIB_LAMP_CTRL_BRI_MAX : uwBri;
        lampDevStatusCtrl(lampId, ulPeriodMs);
    }   
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    return 0;
}

/******************************************************************************
 Function    : myLampCCTCtrl
 Description : lamp cct control
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampCCTCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint16_t uwCCT, uint16_t uwBri)
{
    int ret = 0;
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if((!lamp.lampTable[lampId].valid) || (pLamp->lampStatus.lampParam.ucSwitch == MY_LAMP_CTRL_SW_OFF))
    {
        return -1;
    }

    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    ret = lampIsFlashing(lampId);
    if ( ret == 0 ) {
        pLamp->lampStatus.lampParam.ucCtrlType = MY_LAMP_CTRL_TYPE_CCT;
        pLamp->lampStatus.lampParam.uwCCT = uwCCT < MY_LIB_LAMP_CTRL_CCT_MIN ? MY_LIB_LAMP_CTRL_CCT_MIN : uwCCT;
        pLamp->lampStatus.lampParam.uwCCT = uwCCT > MY_LIB_LAMP_CTRL_CCT_MAX ? MY_LIB_LAMP_CTRL_CCT_MAX : uwCCT;  
        pLamp->lampStatus.lampParam.uwBri = uwBri;
        lampDevStatusCtrl(lampId, ulPeriodMs); 
    } 
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    return 0;
}

/******************************************************************************
 Function    : myLampColorCtrl
 Description : lamp color control
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampColorCtrl(uint8_t lampId, uint32_t ulPeriodMs, const uint8_t ucHue[3], uint16_t uwBri)
{
    int ret = 0;
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if((!lamp.lampTable[lampId].valid) || (pLamp->lampStatus.lampParam.ucSwitch == MY_LAMP_CTRL_SW_OFF))
    {
        return -1;
    }

    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    ret = lampIsFlashing(lampId);
    if ( ret == 0 ) {
        pLamp->lampStatus.lampParam.ucCtrlType = MY_LAMP_CTRL_TYPE_RGB;
        pLamp->lampStatus.lampParam.ucHue[0] = ucHue[0];
        pLamp->lampStatus.lampParam.ucHue[1] = ucHue[1];
        pLamp->lampStatus.lampParam.ucHue[2] = ucHue[2];
        pLamp->lampStatus.lampParam.uwBri = uwBri;
        lampDevStatusCtrl(lampId, ulPeriodMs); 
    }   
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    return 0;
}

/******************************************************************************
 Function    : myLampPwmsCtrl
 Description : 灯控制PWM输出
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int8_t myLampPwmsCtrl(uint8_t lampId, uint32_t ulPeriodMs, uint32_t dstPwms[4])
{
    lampData_t *pLamp = lamp.lampTable[lampId].pLampData;

    if (!lamp.lampTable[lampId].valid) {
        return -1;
    }
    if(lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    pLamp->lampStatus.auwLastPwms[0] = pLamp->lampStatus.auwCurrPwms[0];
    pLamp->lampStatus.auwLastPwms[1] = pLamp->lampStatus.auwCurrPwms[1];
    pLamp->lampStatus.auwLastPwms[2] = pLamp->lampStatus.auwCurrPwms[2];
    pLamp->lampStatus.auwLastPwms[3] = pLamp->lampStatus.auwCurrPwms[3];

    pLamp->lampCtrl.uwDstPwms[0] = (uint16_t)(dstPwms[0]);
    pLamp->lampCtrl.uwDstPwms[1] = (uint16_t)(dstPwms[1]);
    pLamp->lampCtrl.uwDstPwms[2] = (uint16_t)(dstPwms[2]);
    pLamp->lampCtrl.uwDstPwms[3] = (uint16_t)(dstPwms[3]);

    pLamp->lampCtrl.uwSrcPwms[0] = pLamp->lampStatus.auwCurrPwms[0];
    pLamp->lampCtrl.uwSrcPwms[1] = pLamp->lampStatus.auwCurrPwms[1];
    pLamp->lampCtrl.uwSrcPwms[2] = pLamp->lampStatus.auwCurrPwms[2];
    pLamp->lampCtrl.uwSrcPwms[3] = pLamp->lampStatus.auwCurrPwms[3];

    pLamp->lampCtrl.ulCurStep = 0;
    pLamp->lampCtrl.ulTotalSteps = ulPeriodMs/lamp.ulStepInterval+1;
    pLamp->lampCtrl.ucAdjusting = 1;
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}
    return 0;
}

/******************************************************************************
 Function    : myLampFlashCtrl
 Description : myle lamp flash control
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampFlashCtrl(uint8_t lampId, uint32_t ulStepMs, uint32_t ulPeriodMs, uint32_t flashCnt, flashCallbackFunc pflashCallbck, uint32_t arg)
{
    lampData_t *pLampData = lamp.lampTable[lampId].pLampData;

    if (!lamp.lampTable[lampId].valid) return -1;
    if (lamp.pFuncMutexLock){lamp.pFuncMutexLock();}
    if ( flashCnt > 0 ) {
        if (pLampData->lampCtrl.ucFlashing) {
            pLampData->lampCtrl.ulFlashCount = flashCnt * 2 + pLampData->lampCtrl.ulFlashCount%2;
        } else {
            pLampData->lampCtrl.ulFlashCount = flashCnt * 2;
            pLampData->lampCtrl.backupLampParam = pLampData->lampStatus.lampParam;
        }
        pLampData->lampCtrl.ulFlashStepMs = ulStepMs;
        pLampData->lampCtrl.ulFlashPeriod = ulPeriodMs;
        pLampData->lampCtrl.ucFlashing = 1;
        pLampData->lampCtrl.pFuncFlashCallback = pflashCallbck;
        pLampData->lampCtrl.falshArg = arg; 
    }
    
    if(lamp.pFuncMutexUnlock){lamp.pFuncMutexUnlock();}

    return 0;
}

/******************************************************************************
 Function    : myLampRegisterDimmingStatusCtrlHook
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampRegisterDimmingStatusCtrlHook(uint8_t lampId, dimmingStatusCtrlHookFunc statusHook)
{
    lamp.lampTable[lampId].pLampData->lampCtrl.dimmingStatusCtrlHook = statusHook;
    return 0;
}

/******************************************************************************
 Function    : myLampInit
 Description : 灯控制模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampInit(uint32_t stepInterval,
                   //dimmingGetMsTickFunc pFuncGetMsTick,
                   uint32_t (*pFuncGetMsTick)(void), 
                   //myMutexLockFunc pFuncMutexLock, 
                   void (*pFuncMutexLock)(void),
                   //myMutexUnlockFunc pFuncMutexUnlock
                   void (*pFuncMutexUnlock)(void))
{
    memset((uint8_t*)&lamp, 0, sizeof(lamp_t));
    lamp.pFuncGetMsTick = pFuncGetMsTick;
    lamp.pFuncMutexLock = pFuncMutexLock;
    lamp.pFuncMutexUnlock = pFuncMutexUnlock;
    lamp.ulStepInterval = (stepInterval < 1) ? 1 : stepInterval;

    return 0;
}


/************************************************************
 Function    : myLampLoop
 Description : 灯控制输出任务
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int8_t myLampLoop(void)
{
    uint32_t cTick = 0;
    static uint32_t lTick = 0;
    static uint8_t stepCtrlFlag = 0;

    if (lamp.pFuncGetMsTick) {
        cTick = lamp.pFuncGetMsTick();
        if(cTick - lTick >= lamp.ulStepInterval)
        {
            lTick = lamp.pFuncGetMsTick();
            stepCtrlFlag = 1;
        }
    } 
    else {
        stepCtrlFlag = 1;
    }

    if(stepCtrlFlag) {
        stepCtrlFlag = 0;
        lampFlashProc();
        for(uint8_t lampId=0; lampId < MY_LIB_LAMP_MAX_NUM; lampId++)
        {
            if (lamp.lampTable[lampId].valid) {
                lampStepProc(lampId);
            }
        }
        return 1;
    } 
    return 0;
}

#ifdef __cplusplus
}
#endif

