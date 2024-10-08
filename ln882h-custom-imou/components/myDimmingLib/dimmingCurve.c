/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingcurve.c                                                     *
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
#ifndef NULL
#define NULL 0
#endif
/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "dimmingCurve.h"
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#if !MY_LIB_LAMP_COLOR_CONVERT_SIMPLIFY_ENABLE
#include <math.h>
#endif

#if 0
#define dimmingCurveLog(fmt, ...) my_printf_uart("[%s]" fmt, __func__, ##__VA_ARGS__)
#else
#define dimmingCurveLog(fmt, ...)
#endif
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct
{
    MY_DIMMING_CURVE_t curveIndex;
    MyDimmingCurveFunc myDimmingCurve;
} myDimmingCurveItem_t;
/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
#if MY_DIMMING_CURVE_SUPPORT_LINEAR
static uint32_t DimmingCurve_Linear(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);
#endif
#if MY_DIMMING_CURVE_SUPPORT_DELAY_POWER_ONOFF
static uint32_t DimmingCurve_DelayPowerOnOff(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);
#endif
#if MY_DIMMING_CURVE_SUPPORT_EXPONENTIAL
static uint32_t DimmingCurve_Exponential(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);
#endif
#if MY_DIMMING_CURVE_SUPPORT_CIE1976
static uint32_t DimmingCurve_CIE1976(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps);
#endif
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
const myDimmingCurveItem_t myDimmingCurveItemTable[] = 
{
#if MY_DIMMING_CURVE_SUPPORT_LINEAR
    {MY_DIMMING_CURVE_LINEAR, DimmingCurve_Linear},
#endif
#if MY_DIMMING_CURVE_SUPPORT_DELAY_POWER_ONOFF
    {MY_DIMMING_CURVE_DELAY_POWER_ON_OFF, DimmingCurve_DelayPowerOnOff},
#endif
#if MY_DIMMING_CURVE_SUPPORT_EXPONENTIAL
    {MY_DIMMING_CURVE_EXPONENTIAL, DimmingCurve_Exponential},
#endif
#if MY_DIMMING_CURVE_SUPPORT_CIE1976
    {MY_DIMMING_CURVE_CIE1976, DimmingCurve_CIE1976},
#endif
};
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
#if MY_DIMMING_CURVE_SUPPORT_LINEAR
/******************************************************************************
 Function    : DimmingCurve_Linear
 Description : color to pwm value convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t DimmingCurve_Linear(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps)
{
    uint32_t afPWMVal;

    if(curStep > totalSteps) {
        return srcPwm;
    }

    if (srcPwm <= dstPwm) {
        afPWMVal = srcPwm + curStep * (dstPwm - srcPwm) / totalSteps;
        afPWMVal = afPWMVal < srcPwm ? srcPwm : afPWMVal;
        afPWMVal = afPWMVal > dstPwm ? dstPwm : afPWMVal;
    } else {
        afPWMVal = srcPwm - curStep * (srcPwm - dstPwm) / totalSteps;
        afPWMVal = afPWMVal < dstPwm ? dstPwm : afPWMVal;
        afPWMVal = afPWMVal > srcPwm ? srcPwm : afPWMVal;
    }

    return afPWMVal;
}
#endif

#if MY_DIMMING_CURVE_SUPPORT_DELAY_POWER_ONOFF
/******************************************************************************
 Function    : DimmingCurve_DelayPowerOnOff
 Description : color to pwm value convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t DimmingCurve_DelayPowerOnOff(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps)
{
    uint32_t afPWMVal;
    uint32_t startStep = 50;

    if(curStep > totalSteps) {
        return srcPwm;
    }

    if ((srcPwm == 0) && (dstPwm > minPwm) && (totalSteps > startStep)) {
        if (curStep < startStep) {
            afPWMVal = srcPwm + curStep * (minPwm - srcPwm) / startStep;
            afPWMVal = afPWMVal < srcPwm ? srcPwm : afPWMVal;
            afPWMVal = afPWMVal > minPwm ? minPwm : afPWMVal;
        }
        else {
            afPWMVal = DimmingCurve_CIE1976(minPwm, dstPwm, minPwm, maxPwm, curStep - startStep, totalSteps - startStep);
//          afPWMVal = minPwm + (curStep-50) * (dstPwm - minPwm) / (totalSteps-50);
//          afPWMVal = afPWMVal < minPwm ? minPwm : afPWMVal;
//          afPWMVal = afPWMVal > dstPwm ? dstPwm : afPWMVal;
        }
    }
    else if ((dstPwm == 0) && (curStep < totalSteps)) {
        afPWMVal = DimmingCurve_CIE1976(srcPwm, minPwm, minPwm, maxPwm, curStep, totalSteps);
    } else {
        if (srcPwm <= dstPwm) {
            afPWMVal = srcPwm + curStep * (dstPwm - srcPwm) / totalSteps;
            afPWMVal = afPWMVal < srcPwm ? srcPwm : afPWMVal;
            afPWMVal = afPWMVal > dstPwm ? dstPwm : afPWMVal;
        } else {
            afPWMVal = srcPwm - curStep * (srcPwm - dstPwm) / totalSteps;
            afPWMVal = afPWMVal < dstPwm ? dstPwm : afPWMVal;
            afPWMVal = afPWMVal > srcPwm ? srcPwm : afPWMVal;
        }
    }

    return afPWMVal;
}
#endif

#if MY_DIMMING_CURVE_SUPPORT_EXPONENTIAL
/******************************************************************************
 Function    : DimmingCurve_Exponential
 Description : color to pwm value convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t DimmingCurve_Exponential(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps)
{
    float cur_pwm = 0.0f;

    if (srcPwm <= dstPwm) {
        float chg = (float)(dstPwm-srcPwm)/(float)(maxPwm);

        cur_pwm = (float)srcPwm + (378.75813f * (float)(exp((float)curStep * (1000.0f / (float)totalSteps) * 0.003273f))) * chg * curStep / totalSteps;
    } else {
        float chg = (float)(srcPwm-dstPwm)/(maxPwm);

        cur_pwm = (float)dstPwm + (378.75813f * (float)(exp((float)(totalSteps - curStep) * (1000.0f / (float)totalSteps) * 0.003273f))) * chg * (totalSteps - curStep) / totalSteps;
    }

    return (uint32_t)cur_pwm;
}
#endif

#if MY_DIMMING_CURVE_SUPPORT_CIE1976
static float DimmingCurveCubeRoot(float a)
{
    float root = 1.0f;

    while (fabs(root*root*root-a)>= 0.0001) {
        root = root-(root*root*root-a)/(3*root*root);
    }
    return root;
}
#define DIMMING_CURVE_CIE1976_LUMI_SPLIT 0.008856f
/******************************************************************************
 Function    : DimmingCurve_CIE1976
 Description : color to pwm value convert
 Note        : L = 903.3 * Y             for Y <= 0.008856
               L = 116 * Y ^1/3 - 16     for Y > 0.008856
               L is sight lightness(0-100), Y is device lightness(0.0-1.0)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static uint32_t DimmingCurve_CIE1976(uint32_t srcPwm, uint32_t dstPwm, uint32_t minPwm, uint32_t maxPwm, uint32_t curStep, uint32_t totalSteps)
{
    float cur_pwm = 0.0f;
    float curBri = 0.0f;
    static uint32_t lastSrcPwm = 0, lastDstPwm = 0, lastMaxPwm = 0;
    static float srcBri = 0.0f, dstBri=0.0f;
    static float srcY = 0.0f, dstY = 0.0f; 
    
    if (srcPwm == dstPwm) {
        return dstPwm;
    }

    srcPwm = srcPwm > maxPwm ? maxPwm : srcPwm;
    dstPwm = dstPwm > maxPwm ? maxPwm : dstPwm;
    if ((srcPwm != lastSrcPwm) || (maxPwm != lastMaxPwm) ) {
        lastSrcPwm = srcPwm;
        srcY = srcPwm*1.0f/maxPwm;
        if (srcY <= DIMMING_CURVE_CIE1976_LUMI_SPLIT) {
            srcBri = 903.3*srcY;
        } else {
            srcBri = 116*DimmingCurveCubeRoot(srcY)-16;
        }
    }

    if ((dstPwm != lastDstPwm) || (maxPwm != lastMaxPwm) ) {
        lastDstPwm = dstPwm;

        dstY = dstPwm*1.0f/maxPwm;
        if (dstY <= DIMMING_CURVE_CIE1976_LUMI_SPLIT) {
            dstBri = 903.3*dstY;
        } else {
            dstBri = 116*DimmingCurveCubeRoot(dstY)-16;
        }
    }
    lastMaxPwm = maxPwm;

    if (srcBri <= dstBri) {
        curBri = srcBri + curStep * (dstBri - srcBri) / totalSteps;
    } else {
        curBri = srcBri - curStep * (srcBri - dstBri) / totalSteps;
    }

    if (curBri <= 7.999624) {
        cur_pwm = curBri / 903.3;
    } else {
        cur_pwm = (curBri+16)/116;
        cur_pwm = cur_pwm * cur_pwm * cur_pwm;
    }

    cur_pwm = cur_pwm > 1.0f ? 1.0f : cur_pwm;
    cur_pwm *= maxPwm;

    if (((srcPwm < dstPwm) && (cur_pwm > dstPwm)) ||
        ((srcPwm > dstPwm) && (cur_pwm < dstPwm))) {
        cur_pwm = dstPwm;
    }

    return (uint32_t)(cur_pwm+0.5);
}
#endif

/******************************************************************************
 Function    : getMyDimmingCurve
 Description : get my dimming curve
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
MyDimmingCurveFunc getMyDimmingCurve(MY_DIMMING_CURVE_t curve)
{
    if (curve >= MY_DIMMING_CURVE_NUM) {
        return NULL;
    }
    for (int i=0; i<MY_DIMMING_CURVE_NUM; i++) {
        if (myDimmingCurveItemTable[i].curveIndex == curve) {
            return myDimmingCurveItemTable[i].myDimmingCurve;
        }
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif

