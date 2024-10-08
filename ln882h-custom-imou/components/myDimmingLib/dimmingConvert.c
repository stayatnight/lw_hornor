/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : dimmingConvert.c                                              *
* Description :                                                               *
*               dimming Color to pwm Argorithm处理任务源文件                   *
* Note        : (none)                                                        *
* Author      : Zhijun Li                                                     *
* Date:       : 2017-10-20                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "dimmingConvert.h"
#if MY_LIB_CTPA_USE_MATH_LIB
#include <math.h>
#endif
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#if MY_LIB_CTPA_DEBUG_ENABLE
#define myCTPALog                                         printf
#else
#define myCTPALog(format, ...)
#endif

#define MY_LIB_CTPA_MAX_VAL(a, b)                         (((a) < (b)) ? (b) : (a))
#define MY_LIB_CTPA_MIN_VAL(a, b)                         (((a) < (b)) ? (a) : (b))
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
const float RGBC2XYZ[3][3] = 
{
{ 0.1924f, 0.05826f,  0.1044f},
{ 0.09065f, 0.2735f, -0.01818f},
{-0.04376f, -0.15228f,  0.70548f}
};

const float XYZ2RGBPWM[3][3] = 
{
{ 0.02481f,  -0.00479f, -0.00374f},
{-0.00812f,   0.01882f, -0.000185f},
{ 0.000505f, -0.00119f,  0.00563f}
};

const float XYZ2RGBLinear[3][3] = 
{
{ 3.2406f, -1.5372f, -0.4988f},
{-0.9689f,  1.8758f,  0.0415f},
{ 0.0557f, -0.2041f,  1.0573f}
};

const float sRGB2RGBPWM[3][3] = 
{
{ 0.76648f, 0.41115f,  0.05470f},
{ 0.04820f, 0.82975f, -0.03221f},
{ 0.00622f, 0.00762f,  0.46485f}
};

const float BRI2RGBWPWM_2000_3200[4][3] = 
{
{-2.3403f, 186.04f, 622},
{-0.7603f, 62.328f, 105.97f},
{ 0.0f,     0.0f,     0.0f},
{ 0.467f,  6.6562f, 34.567f},
};

const float BRI2RGBWPWM_3200_5700[4][4] = 
{
{0.0f,     0.0f,     0.0f,       0.0f},
{0.0013f, -0.4766f, 81.032f, -4199.8f},
{0.0019f, -0.6631f, 100.11f, -4909.4f},
{0.0012f, -0.3506f, 39.755f,  2166.3f},
};

/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_HL_PWM
/******************************************************************************
 Function    : myCTPAcct2HLPwm
 Description : MY CCT to high low pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPAcct2HLPwm(my_uint16_t uwCCT, const myCTPAcct2HLPwmParam_t* const pcct2HLPwmParam, my_uint8_t bri, my_uint32_t ulPWM[2])
{
    my_float_t afPWM[2] = {0.0f, 0.0f};
    my_float_t rateCCT = 0.0f;

    uwCCT = MY_LIB_CTPA_MAX_VAL(pcct2HLPwmParam->cctMin, uwCCT);
    uwCCT = MY_LIB_CTPA_MIN_VAL(pcct2HLPwmParam->cctMax, uwCCT);
    rateCCT = uwCCT/1000.0f;
    afPWM[0] = pcct2HLPwmParam->cctArray[0][0] + pcct2HLPwmParam->cctArray[0][1] * rateCCT + pcct2HLPwmParam->cctArray[0][2] * rateCCT * rateCCT;
    afPWM[1] = pcct2HLPwmParam->cctArray[1][0] + pcct2HLPwmParam->cctArray[1][1] * rateCCT + pcct2HLPwmParam->cctArray[1][2] * rateCCT * rateCCT;

    afPWM[0] = MY_LIB_CTPA_MIN_VAL(1.000f, afPWM[0]); 
    afPWM[1] = MY_LIB_CTPA_MIN_VAL(1.000f, afPWM[1]);
    afPWM[0] = (0.001f > afPWM[0]) ? 0.0f : afPWM[0];
    afPWM[1] = (0.001f > afPWM[1]) ? 0.0f : afPWM[1];
    afPWM[0] *= pcct2HLPwmParam->pwmMax;
    afPWM[1] *= pcct2HLPwmParam->pwmMax;

    if ( pcct2HLPwmParam->powerConfig.powerOutType == MY_POWER_OUT_TYPE_FIX ) {
        my_float_t totalPWM = afPWM[0]+afPWM[1];
    
        if ( totalPWM > pcct2HLPwmParam->powerConfig.powerFixVal )
        {
            afPWM[0] = afPWM[0]*pcct2HLPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[1] = afPWM[1]*pcct2HLPwmParam->powerConfig.powerFixVal/totalPWM;
        }
    }

    ulPWM[0] = (my_uint32_t)(afPWM[0] * bri / 255.0f);
    ulPWM[1] = (my_uint32_t)(afPWM[1] * bri / 255.0f);

    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_CCT_2_RGBW_PWM
/******************************************************************************
 Function    : myCTPAcct2RGBWPwm
 Description : MY CCT to RGBW pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPAcct2RGBWPwm(my_uint16_t uwCCT, const myCTPAcct2RGBWPwmParam_t* const pcct2RGBWPwmParam, my_uint8_t bri, my_uint32_t ulPWM[4])
{
    my_float_t afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    my_float_t fTempDiv, fTempDiv2, fTempDiv3;   
    const my_float_t (*p)[4] = pcct2RGBWPwmParam->cctArray;

    uwCCT = MY_LIB_CTPA_MAX_VAL(pcct2RGBWPwmParam->cctMin, uwCCT);
    uwCCT = MY_LIB_CTPA_MIN_VAL(pcct2RGBWPwmParam->cctMax, uwCCT);
    fTempDiv = (uwCCT / 1000.0);
    fTempDiv2 = fTempDiv * fTempDiv;
    fTempDiv3 = fTempDiv2 * fTempDiv; 
    afPWM[3] = p[0][0] * fTempDiv3 + p[0][1] * fTempDiv2 + p[0][2] * fTempDiv + p[0][3];
    afPWM[0] = p[1][0] * fTempDiv3 + p[1][1] * fTempDiv2 + p[1][2] * fTempDiv + p[1][3];
    afPWM[1] = p[2][0] * fTempDiv3 + p[2][1] * fTempDiv2 + p[2][2] * fTempDiv + p[2][3];
    afPWM[2] = p[3][0] * fTempDiv3 + p[3][1] * fTempDiv2 + p[3][2] * fTempDiv + p[3][3]; 
    //myCTPALog("pwm:%d, %d, %d, %d", (int)afPWM[0], (int)afPWM[1], (int)afPWM[2], (int)afPWM[3]);  

    afPWM[0] = MY_LIB_CTPA_MAX_VAL(0.0f, afPWM[0]);
    afPWM[1] = MY_LIB_CTPA_MAX_VAL(0.0f, afPWM[1]);
    afPWM[2] = MY_LIB_CTPA_MAX_VAL(0.0f, afPWM[2]);
    afPWM[3] = MY_LIB_CTPA_MAX_VAL(0.0f, afPWM[3]);

    afPWM[0] = MY_LIB_CTPA_MIN_VAL(1.0f, afPWM[0]);
    afPWM[1] = MY_LIB_CTPA_MIN_VAL(1.0f, afPWM[1]);
    afPWM[2] = MY_LIB_CTPA_MIN_VAL(1.0f, afPWM[2]);
    afPWM[3] = MY_LIB_CTPA_MIN_VAL(1.0f, afPWM[3]);

    //归一化
    {
        my_float_t maxPWM = 0.0f;

        maxPWM = MY_LIB_CTPA_MAX_VAL(afPWM[0], afPWM[1]);
        maxPWM = MY_LIB_CTPA_MAX_VAL(maxPWM, afPWM[2]);
        maxPWM = MY_LIB_CTPA_MAX_VAL(maxPWM, afPWM[3]);
        afPWM[0] = (afPWM[0] / maxPWM) * pcct2RGBWPwmParam->pwmMax;
        afPWM[1] = (afPWM[1] / maxPWM) * pcct2RGBWPwmParam->pwmMax;
        afPWM[2] = (afPWM[2] / maxPWM) * pcct2RGBWPwmParam->pwmMax;
        afPWM[3] = (afPWM[3] / maxPWM) * pcct2RGBWPwmParam->pwmMax;
    }

    if ( pcct2RGBWPwmParam->powerConfig.powerOutType == MY_POWER_OUT_TYPE_FIX ) {
        my_float_t totalPWM = afPWM[0]+afPWM[1]+afPWM[2]+afPWM[3];
    
        if(totalPWM > pcct2RGBWPwmParam->powerConfig.powerFixVal)
        {
            afPWM[0] = afPWM[0]*pcct2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[1] = afPWM[1]*pcct2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[2] = afPWM[2]*pcct2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[3] = afPWM[3]*pcct2RGBWPwmParam->powerConfig.powerFixVal/totalPWM; 
        }
    }

    {
        ulPWM[0] = (my_uint32_t)((afPWM[0]) * (my_float_t)bri / 255.0);
        ulPWM[1] = (my_uint32_t)((afPWM[1]) * (my_float_t)bri / 255.0);
        ulPWM[2] = (my_uint32_t)((afPWM[2]) * (my_float_t)bri / 255.0);
        ulPWM[3] = (my_uint32_t)((afPWM[3]) * (my_float_t)bri / 255.0);
    }
    
    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGB_PWM
/******************************************************************************
 Function    : myCTPAsRGBtoRGBPwm
 Description : MY sRGB to RGB pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPAsRGBtoRGBPwm(const my_uint8_t sRGB[3], const myCTPAsRGB2RGBPwmParam_t* const psRGB2RGBPwmParam, my_uint16_t uwBri, my_uint32_t ulPWM[3])
{
    my_float_t tmp;
    my_float_t RGBlinear[3];
    my_float_t afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    const my_float_t (*p)[3] = psRGB2RGBPwmParam->xyzObj;
    /* step 1 */
    tmp = sRGB[0] / 255.0;
    (tmp <= 0.04045) ? (RGBlinear[0] = tmp / 12.92) : (RGBlinear[0] = pow((tmp + 0.055)/1.055, 2.4));
    tmp = sRGB[1] / 255.0;
    (tmp <= 0.04045) ? (RGBlinear[1] = tmp / 12.92) : (RGBlinear[1] = pow((tmp + 0.055)/1.055, 2.4));
    tmp = sRGB[2] / 255.0;
    (tmp <= 0.04045) ? (RGBlinear[2] = tmp / 12.92) : (RGBlinear[2] = pow((tmp + 0.055)/1.055, 2.4));

    /* step 2*/
    afPWM[0] = p[0][0] * RGBlinear[0] + p[0][1] * RGBlinear[1] + p[0][2] * RGBlinear[2];
    afPWM[1] = p[1][0] * RGBlinear[0] + p[1][1] * RGBlinear[1] + p[1][2] * RGBlinear[2];
    afPWM[2] = p[2][0] * RGBlinear[0] + p[2][1] * RGBlinear[1] + p[2][2] * RGBlinear[2];

//  afPWM[3] = MY_LIB_CTPA_MAX_VAL(afPWM[1], afPWM[2]);
//  afPWM[3] = MY_LIB_CTPA_MAX_VAL(afPWM[0], afPWM[3]);
//
//  /* step 3*/
//  afPWM[0] = afPWM[0] / afPWM[3];
//  afPWM[1] = afPWM[1] / afPWM[3];
//  afPWM[2] = afPWM[2] / afPWM[3];

    afPWM[0] = (1.000 < afPWM[0]) ? 1.000 : afPWM[0];
    afPWM[1] = (1.000 < afPWM[1]) ? 1.000 : afPWM[1];
    afPWM[2] = (1.000 < afPWM[2]) ? 1.000 : afPWM[2];

    afPWM[0] = (0.001 > afPWM[0]) ? 0.0 : afPWM[0];
    afPWM[1] = (0.001 > afPWM[1]) ? 0.0 : afPWM[1];
    afPWM[2] = (0.001 > afPWM[2]) ? 0.0 : afPWM[2];

    afPWM[0] *= psRGB2RGBPwmParam->pwmMax;
    afPWM[1] *= psRGB2RGBPwmParam->pwmMax;
    afPWM[2] *= psRGB2RGBPwmParam->pwmMax;

    if ( psRGB2RGBPwmParam->powerConfig.powerOutType == MY_POWER_OUT_TYPE_FIX ) {
        my_float_t totalPWM = afPWM[0]+afPWM[1]+afPWM[2];
    
        if(totalPWM > psRGB2RGBPwmParam->powerConfig.powerFixVal)
        {
            afPWM[0] = afPWM[0]*psRGB2RGBPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[1] = afPWM[1]*psRGB2RGBPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[2] = afPWM[2]*psRGB2RGBPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[3] = afPWM[3]*psRGB2RGBPwmParam->powerConfig.powerFixVal/totalPWM; 
        }
    }

    /*step 4*/
    ulPWM[0] = (my_uint32_t)(afPWM[0] * (my_float_t)uwBri / 65535.0);
    ulPWM[1] = (my_uint32_t)(afPWM[1] * (my_float_t)uwBri / 65535.0);
    ulPWM[2] = (my_uint32_t)(afPWM[2] * (my_float_t)uwBri / 65535.0);

    
    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_SRGB_2_RGBW_PWM
/******************************************************************************
 Function    : myCTPAsRGBtoRGBWPwm
 Description : MY sRGB to RGBW pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPAsRGBtoRGBWPwm(const my_uint8_t sRGB[3], const myCTPAsRGB2RGBWPwmParam_t* const psRGB2RGBWPwmParam, my_uint8_t bri, my_uint32_t ulPWM[4])
{
    my_float_t tmp;
    my_float_t RGBlinear[3];
    my_float_t afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    my_float_t x, y, sum;
    my_float_t sORG, sORB, sOGB;

    //step 1-2
#if !MY_LIB_CTPA_USE_MATH_LIB
        tmp = sRGB[0] / 255.0f;
        RGBlinear[0] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[1] / 255.0f;
        RGBlinear[1] = 1.0923f * tmp *tmp - 0.1114f * tmp;
        tmp = sRGB[2] / 255.0f;
        RGBlinear[2] = 1.0923f * tmp *tmp - 0.1114f * tmp;

        RGBlinear[0] = RGBlinear[0] < 0.0f ? 0.0f : RGBlinear[0];
        RGBlinear[1] = RGBlinear[1] < 0.0f ? 0.0f : RGBlinear[1];
        RGBlinear[2] = RGBlinear[2] < 0.0f ? 0.0f : RGBlinear[2];
#else
        tmp = sRGB[0] / 255.0f;
        (tmp <= 0.04045f) ? (RGBlinear[0] = tmp / 12.92f) : (RGBlinear[0] = pow((tmp + 0.055f)/1.055f, 2.4f));
        tmp = sRGB[1] / 255.0f;
        (tmp <= 0.04045f) ? (RGBlinear[1] = tmp / 12.92f) : (RGBlinear[1] = pow((tmp + 0.055f)/1.055f, 2.4f));
        tmp = sRGB[2] / 255.0f;
        (tmp <= 0.04045f) ? (RGBlinear[2] = tmp / 12.92f) : (RGBlinear[2] = pow((tmp + 0.055f)/1.055f, 2.4f));
#endif
    //step 3
    afPWM[0] = psRGB2RGBWPwmParam->xyzObj[0][0] * RGBlinear[0] + psRGB2RGBWPwmParam->xyzObj[0][1] * RGBlinear[1] + psRGB2RGBWPwmParam->xyzObj[0][2] * RGBlinear[2];
    afPWM[1] = psRGB2RGBWPwmParam->xyzObj[1][0] * RGBlinear[0] + psRGB2RGBWPwmParam->xyzObj[1][1] * RGBlinear[1] + psRGB2RGBWPwmParam->xyzObj[1][2] * RGBlinear[2];
    afPWM[2] = psRGB2RGBWPwmParam->xyzObj[2][0] * RGBlinear[0] + psRGB2RGBWPwmParam->xyzObj[2][1] * RGBlinear[1] + psRGB2RGBWPwmParam->xyzObj[2][2] * RGBlinear[2];

    //step 4
    sum = afPWM[0] + afPWM[1] + afPWM[2];
    x = sum > 0 ? afPWM[0] / sum : 0.0f;
    y = sum > 0 ? afPWM[1] / sum : 0.0f;
    
    //step 5
    sORG = fabs(psRGB2RGBWPwmParam->sORGB[0][0] + psRGB2RGBWPwmParam->sORGB[0][1]*x + psRGB2RGBWPwmParam->sORGB[0][2]*y);
    sORB = fabs(psRGB2RGBWPwmParam->sORGB[1][0] + psRGB2RGBWPwmParam->sORGB[1][1]*x + psRGB2RGBWPwmParam->sORGB[1][2]*y);
    sOGB = fabs(psRGB2RGBWPwmParam->sORGB[2][0] + psRGB2RGBWPwmParam->sORGB[2][1]*x + psRGB2RGBWPwmParam->sORGB[2][2]*y);
    //step 6
    {
        const float (*p)[3] = psRGB2RGBWPwmParam->wrg;

        if((sORG < sORB) && (sORG < sOGB)) p = psRGB2RGBWPwmParam->wrg; //WRG
        else if((sORB < sORG) && (sORB < sOGB)) p = psRGB2RGBWPwmParam->wrb; //WRB
        else if((sOGB < sORG) && (sOGB < sORB)) p = psRGB2RGBWPwmParam->wgb; //WGB
        afPWM[0] = p[1][0] * RGBlinear[0] + p[1][1] * RGBlinear[1] + p[1][2] * RGBlinear[2];
        afPWM[1] = p[2][0] * RGBlinear[0] + p[2][1] * RGBlinear[1] + p[2][2] * RGBlinear[2];
        afPWM[2] = p[3][0] * RGBlinear[0] + p[3][1] * RGBlinear[1] + p[3][2] * RGBlinear[2];
        afPWM[3] = p[0][0] * RGBlinear[0] + p[0][1] * RGBlinear[1] + p[0][2] * RGBlinear[2];
    }

    //step 7
    {
#if 1
        my_float_t maxPWM = MY_LIB_CTPA_MAX_VAL(afPWM[0], afPWM[1]);

        maxPWM = MY_LIB_CTPA_MAX_VAL(maxPWM, afPWM[2]);
        maxPWM = MY_LIB_CTPA_MAX_VAL(maxPWM, afPWM[3]);
        
        afPWM[0] = maxPWM > 0 ? afPWM[0]/maxPWM*psRGB2RGBWPwmParam->pwmMax : 0.0f;
        afPWM[1] = maxPWM > 0 ? afPWM[1]/maxPWM*psRGB2RGBWPwmParam->pwmMax : 0.0f;
        afPWM[2] = maxPWM > 0 ? afPWM[2]/maxPWM*psRGB2RGBWPwmParam->pwmMax : 0.0f;
        afPWM[3] = maxPWM > 0 ? afPWM[3]/maxPWM*psRGB2RGBWPwmParam->pwmMax : 0.0f;  
#else
        afPWM[0] = afPWM[0] * psRGB2RGBWPwmParam->pwmMax;
        afPWM[1] = afPWM[1] * psRGB2RGBWPwmParam->pwmMax;
        afPWM[2] = afPWM[2] * psRGB2RGBWPwmParam->pwmMax;
        afPWM[3] = afPWM[3] * psRGB2RGBWPwmParam->pwmMax; 
#endif

        afPWM[0] = afPWM[0] < 0 ? 0 : afPWM[0];
        afPWM[1] = afPWM[1] < 0 ? 0 : afPWM[1];
        afPWM[2] = afPWM[2] < 0 ? 0 : afPWM[2];
        afPWM[3] = afPWM[3] < 0 ? 0 : afPWM[3];

        afPWM[0] = afPWM[0] > psRGB2RGBWPwmParam->pwmMax ? psRGB2RGBWPwmParam->pwmMax : afPWM[0];
        afPWM[1] = afPWM[1] > psRGB2RGBWPwmParam->pwmMax ? psRGB2RGBWPwmParam->pwmMax : afPWM[1];
        afPWM[2] = afPWM[2] > psRGB2RGBWPwmParam->pwmMax ? psRGB2RGBWPwmParam->pwmMax : afPWM[2];
        afPWM[3] = afPWM[3] > psRGB2RGBWPwmParam->pwmMax ? psRGB2RGBWPwmParam->pwmMax : afPWM[3];
    }

    //step 8
     if ( psRGB2RGBWPwmParam->powerConfig.powerOutType == MY_POWER_OUT_TYPE_FIX ) {
        my_float_t totalPWM = afPWM[0]+afPWM[1]+afPWM[2]+afPWM[3];
    
        if(totalPWM > psRGB2RGBWPwmParam->powerConfig.powerFixVal)
        {
            afPWM[0] = afPWM[0]*psRGB2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[1] = afPWM[1]*psRGB2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[2] = afPWM[2]*psRGB2RGBWPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[3] = afPWM[3]*psRGB2RGBWPwmParam->powerConfig.powerFixVal/totalPWM; 
        }
    }

    {
        ulPWM[0] = (my_uint32_t)((afPWM[0]) * (my_float_t)bri / 255.0f);
        ulPWM[1] = (my_uint32_t)((afPWM[1]) * (my_float_t)bri / 255.0f);
        ulPWM[2] = (my_uint32_t)((afPWM[2]) * (my_float_t)bri / 255.0f);
        ulPWM[3] = (my_uint32_t)((afPWM[3]) * (my_float_t)bri / 255.0f);
    }

    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_RGB_PWM
/******************************************************************************
 Function    : myCTPArgbc2RGBPwm
 Description : my RGBC to RGB pwm
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPArgbc2RGBPwm(const my_uint16_t rgbc[4], const myCTPArgbc2RGBPwmParam_t* const prgbc2RGBPwmParam, my_uint8_t bri, my_uint16_t ulPwm[3])
{
    float IR;
    float R1, G1, B1;
    float X, Y, Z;
    float afPWM[4];

    IR = (rgbc[MY_SENSOR_COLOR_R] + rgbc[MY_SENSOR_COLOR_G] + rgbc[MY_SENSOR_COLOR_B] - rgbc[MY_SENSOR_COLOR_C])/2.0f;
    IR = IR < 0.0f ? 0.0f : IR;

    R1 = rgbc[MY_SENSOR_COLOR_R] - IR;
    G1 = rgbc[MY_SENSOR_COLOR_G] - IR;
    B1 = rgbc[MY_SENSOR_COLOR_B] - IR;

    X = (prgbc2RGBPwmParam->xyzObj[0][0] * R1) + (prgbc2RGBPwmParam->xyzObj[0][1] * G1) + (prgbc2RGBPwmParam->xyzObj[0][2] * B1);
	Y = (prgbc2RGBPwmParam->xyzObj[1][0] * R1) + (prgbc2RGBPwmParam->xyzObj[1][1] * G1) + (prgbc2RGBPwmParam->xyzObj[1][2] * B1);
	Z = (prgbc2RGBPwmParam->xyzObj[2][0] * R1) + (prgbc2RGBPwmParam->xyzObj[2][1] * G1) + (prgbc2RGBPwmParam->xyzObj[2][2] * B1);

    afPWM[0] = (prgbc2RGBPwmParam->sORGB[0][0] * X) + (prgbc2RGBPwmParam->sORGB[0][1] * Y) + (prgbc2RGBPwmParam->sORGB[0][2] * Z);
	afPWM[1] = (prgbc2RGBPwmParam->sORGB[1][0] * X) + (prgbc2RGBPwmParam->sORGB[1][1] * Y) + (prgbc2RGBPwmParam->sORGB[1][2] * Z);
	afPWM[2] = (prgbc2RGBPwmParam->sORGB[2][0] * X) + (prgbc2RGBPwmParam->sORGB[2][1] * Y) + (prgbc2RGBPwmParam->sORGB[2][2] * Z);

    afPWM[3] = MY_LIB_CTPA_MAX_VAL(afPWM[1], afPWM[2]);
    afPWM[3] = MY_LIB_CTPA_MAX_VAL(afPWM[0], afPWM[3]);

    afPWM[0] = afPWM[0] / afPWM[3];
    afPWM[1] = afPWM[1] / afPWM[3];
    afPWM[2] = afPWM[2] / afPWM[3];
    afPWM[3] = 0.0f;

    afPWM[0] = (1.000f < afPWM[0]) ? 1.000f : afPWM[0];
    afPWM[1] = (1.000f < afPWM[1]) ? 1.000f : afPWM[1];
    afPWM[2] = (1.000f < afPWM[2]) ? 1.000f : afPWM[2];   

    afPWM[0] *= (0.001f > afPWM[0]) ? 0.0f : afPWM[0];
    afPWM[1] *= (0.001f > afPWM[1]) ? 0.0f : afPWM[1];
    afPWM[2] *= (0.001f > afPWM[2]) ? 0.0f : afPWM[2];

    afPWM[0] = prgbc2RGBPwmParam->pwmMax;
    afPWM[1] = prgbc2RGBPwmParam->pwmMax;
    afPWM[2] = prgbc2RGBPwmParam->pwmMax;

     if ( prgbc2RGBPwmParam->powerConfig.powerOutType == MY_POWER_OUT_TYPE_FIX ) {
        my_float_t totalPWM = afPWM[0]+afPWM[1]+afPWM[2];
    
        if(totalPWM > prgbc2RGBPwmParam->powerConfig.powerFixVal)
        {
            afPWM[0] = afPWM[0]*prgbc2RGBPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[1] = afPWM[1]*prgbc2RGBPwmParam->powerConfig.powerFixVal/totalPWM;
            afPWM[2] = afPWM[2]*prgbc2RGBPwmParam->powerConfig.powerFixVal/totalPWM; 
        }
    }

    {
        ulPwm[0] = (my_uint32_t)(afPWM[0] * (my_float_t)bri / 255.0f);
        ulPwm[1] = (my_uint32_t)(afPWM[1] * (my_float_t)bri / 255.0f);
        ulPwm[2] = (my_uint32_t)(afPWM[2] * (my_float_t)bri / 255.0f);
    }

    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_RGBC_2_SRGB
/******************************************************************************
 Function    : myCTPArgbc2sRGB
 Description : MY RGBC to sRGB convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPArgbc2sRGB(const my_uint16_t rgbc[4],  const myCTPArgbc2sRGBParam_t* const prgbc2sRGBParam, my_uint8_t sRGB[3])
{
    float IR;
    float R1, G1, B1, maxRGB1;
    float X, Y, Z, maxXYZ;
    float rgbLinear[3], maxLinear;

    IR = (rgbc[MY_SENSOR_COLOR_R] + rgbc[MY_SENSOR_COLOR_G] + rgbc[MY_SENSOR_COLOR_B] - rgbc[MY_SENSOR_COLOR_C])/2.0f;
    IR = IR < 0.0f ? 0.0f : IR;

    R1 = rgbc[MY_SENSOR_COLOR_R] - IR;
    G1 = rgbc[MY_SENSOR_COLOR_G] - IR;
    B1 = rgbc[MY_SENSOR_COLOR_B] - IR;

    maxRGB1 = MY_LIB_CTPA_MAX_VAL(R1, G1);
    maxRGB1 = MY_LIB_CTPA_MAX_VAL(maxRGB1, B1);

    R1 = R1 / maxRGB1 * 1000.0f;
    G1 = G1 / maxRGB1 * 1000.0f;
    B1 = B1 / maxRGB1 * 1000.0f;

    R1 = (R1 - 4.0f * 100.0f);
    G1 = (G1 - 8.0f * 90.57f);
    B1 = (B1 - 8.0f * 84.66f);


    X = (prgbc2sRGBParam->xyzObj[0][0] * R1) + (prgbc2sRGBParam->xyzObj[0][1] * G1) + (prgbc2sRGBParam->xyzObj[0][2] * B1);
	Y = (prgbc2sRGBParam->xyzObj[1][0] * R1) + (prgbc2sRGBParam->xyzObj[1][1] * G1) + (prgbc2sRGBParam->xyzObj[1][2] * B1);
	Z = (prgbc2sRGBParam->xyzObj[2][0] * R1) + (prgbc2sRGBParam->xyzObj[2][1] * G1) + (prgbc2sRGBParam->xyzObj[2][2] * B1);

    maxXYZ = MY_LIB_CTPA_MAX_VAL(X, Y);
    maxXYZ = MY_LIB_CTPA_MAX_VAL(maxXYZ, Z);

    X = X / maxXYZ;
    Y = Y / maxXYZ;
    Z = Z / maxXYZ;

    rgbLinear[0] = prgbc2sRGBParam->rgbLinear[0][0] * X + prgbc2sRGBParam->rgbLinear[0][1] * Y + prgbc2sRGBParam->rgbLinear[0][2] * Z;
    rgbLinear[1] = prgbc2sRGBParam->rgbLinear[1][0] * X + prgbc2sRGBParam->rgbLinear[1][1] * Y + prgbc2sRGBParam->rgbLinear[1][2] * Z;
    rgbLinear[2] = prgbc2sRGBParam->rgbLinear[2][0] * X + prgbc2sRGBParam->rgbLinear[2][1] * Y + prgbc2sRGBParam->rgbLinear[2][2] * Z;

    rgbLinear[0] = rgbLinear[0] < 0 ? 0 : rgbLinear[0];
    rgbLinear[1] = rgbLinear[1] < 0 ? 0 : rgbLinear[1];
    rgbLinear[2] = rgbLinear[2] < 0 ? 0 : rgbLinear[2];

#if !MY_LIB_CTPA_USE_MATH_LIB
    rgbLinear[0] = (0.1114f + sqrt(0.1114f * 0.1114f + 4.3692f * rgbLinear[0]))/2.1846f * 255.0f;
    rgbLinear[1] = (0.1114f + sqrt(0.1114f * 0.1114f + 4.3692f * rgbLinear[1]))/2.1846f * 255.0f;
    rgbLinear[2] = (0.1114f + sqrt(0.1114f * 0.1114f + 4.3692f * rgbLinear[2]))/2.1846f * 255.0f;
#else
    rgbLinear[0] = (rgbLinear[0] < 0.003f) ? rgbLinear[0] * 12.92f : 1.055f * (float)pow(rgbLinear[0], 1/2.4f) - 0.055f;
    rgbLinear[1] = (rgbLinear[1] < 0.003f) ? rgbLinear[1] * 12.92f : 1.055f * (float)pow(rgbLinear[1], 1/2.4f) - 0.055f;
    rgbLinear[2] = (rgbLinear[2] < 0.003f) ? rgbLinear[2] * 12.92f : 1.055f * (float)pow(rgbLinear[2], 1/2.4f) - 0.055f;
#endif


    maxLinear = MY_LIB_CTPA_MAX_VAL(rgbLinear[0], rgbLinear[1]);
    maxLinear = MY_LIB_CTPA_MAX_VAL(maxLinear, rgbLinear[2]); 

    rgbLinear[0] = rgbLinear[0] / maxLinear *255.0;
    rgbLinear[1] = rgbLinear[1] / maxLinear *255.0;
    rgbLinear[2] = rgbLinear[2] / maxLinear *255.0;


    sRGB[0] = (my_uint8_t)rgbLinear[0];
    sRGB[1] = (my_uint8_t)rgbLinear[1];
    sRGB[2] = (my_uint8_t)rgbLinear[2];

    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_PWM_2_SRGB
/******************************************************************************
 Function    : myCTPApwm2sRGB
 Description : MY pwm to sRGB convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPApwm2sRGB(const my_uint32_t ulPWM[4], const myCTPApwm2sRGBParam_t* const ppwm2sRGBParam, my_uint8_t sRGB[3])
{
    my_float_t sObj[3];
    my_float_t maxObj = 0.0f;
    my_float_t rgbLinear[3];
    my_float_t fRGB[3];

    //sObj[0] = 88.51 * ulPWM[3] + 65.71 * ulPWM[0] + 16.61 * ulPWM[1] + 35.40 * ulPWM[2];
    //sObj[1] = 92.51 * ulPWM[3] + 29.18 * ulPWM[0] + 77.16 * ulPWM[1] + 17.98 * ulPWM[2];
    //sObj[2] = 90.82 * ulPWM[3] + 0.0927 * ulPWM[0] + 13.01 * ulPWM[1] + 213.79 * ulPWM[2];

    sObj[0] = ppwm2sRGBParam->xyzObj[0][0] * ulPWM[3] + ppwm2sRGBParam->xyzObj[0][1] * ulPWM[0] + ppwm2sRGBParam->xyzObj[0][2] * ulPWM[1] + ppwm2sRGBParam->xyzObj[0][3] * ulPWM[2];
    sObj[1] = ppwm2sRGBParam->xyzObj[1][0] * ulPWM[3] + ppwm2sRGBParam->xyzObj[1][1] * ulPWM[0] + ppwm2sRGBParam->xyzObj[1][2] * ulPWM[1] + ppwm2sRGBParam->xyzObj[1][3] * ulPWM[2];
    sObj[2] = ppwm2sRGBParam->xyzObj[2][0] * ulPWM[3] + ppwm2sRGBParam->xyzObj[2][1] * ulPWM[0] + ppwm2sRGBParam->xyzObj[2][2] * ulPWM[1] + ppwm2sRGBParam->xyzObj[2][3] * ulPWM[2];

    sObj[0] /= ppwm2sRGBParam->pwmMax;
    sObj[1] /= ppwm2sRGBParam->pwmMax;
    sObj[2] /= ppwm2sRGBParam->pwmMax;

    maxObj = MY_LIB_CTPA_MAX_VAL(sObj[0], sObj[1]);
    maxObj = MY_LIB_CTPA_MAX_VAL(maxObj, sObj[2]);

    sObj[0] = sObj[0] / maxObj;
    sObj[1] = sObj[1] / maxObj;
    sObj[2] = sObj[2] / maxObj;

    //rgbLinear[0] = 1.1518 * sObj[0] - 0.2184 * sObj[1] - 0.1627 * sObj[2];
    //rgbLinear[1] = -0.3512 * sObj[0] + 0.7832 * sObj[1] - 0.0213 * sObj[2];
    //rgbLinear[2] = 0.0206 * sObj[0] - 0.0463 * sObj[1] + 0.2710 * sObj[2];

    rgbLinear[0] = ppwm2sRGBParam->rgbLinear[0][0] * sObj[0] + ppwm2sRGBParam->rgbLinear[0][1] * sObj[1] + ppwm2sRGBParam->rgbLinear[0][2] * sObj[2];
    rgbLinear[1] = ppwm2sRGBParam->rgbLinear[1][0] * sObj[0] + ppwm2sRGBParam->rgbLinear[1][1] * sObj[1] + ppwm2sRGBParam->rgbLinear[1][2] * sObj[2];
    rgbLinear[2] = ppwm2sRGBParam->rgbLinear[2][0] * sObj[0] + ppwm2sRGBParam->rgbLinear[2][1] * sObj[1] + ppwm2sRGBParam->rgbLinear[2][2] * sObj[2];

    fRGB[0] = 1.055 * pow(rgbLinear[0], 1/2.4) - 0.055;
    fRGB[1] = 1.055 * pow(rgbLinear[1], 1/2.4) - 0.055;
    fRGB[2] = 1.055 * pow(rgbLinear[2], 1/2.4) - 0.055;

    fRGB[0] = (1.000 < fRGB[0]) ? 1.000 : fRGB[0];
    fRGB[1] = (1.000 < fRGB[1]) ? 1.000 : fRGB[1];
    fRGB[2] = (1.000 < fRGB[2]) ? 1.000 : fRGB[2];
    //
    fRGB[0] = (0.001 > fRGB[0]) ? 0.0 : fRGB[0];
    fRGB[1] = (0.001 > fRGB[1]) ? 0.0 : fRGB[1];
    fRGB[2] = (0.001 > fRGB[2]) ? 0.0 : fRGB[2];

    sRGB[0] = (my_uint8_t)(fRGB[0] * 255);
    sRGB[1] = (my_uint8_t)(fRGB[1] * 255);
    sRGB[2] = (my_uint8_t)(fRGB[2] * 255);

    return 0;
}
#endif

#if MY_DIMMING_CONVERT_SUPPORT_BRI_2_CCT_2_RGBW_PWM
#define MY_COLOR_CONVERT_PWM_MAX                                (9999*1.0)
/******************************************************************************
 Function    : myCTPAbri2CCTRGBWPwm
 Description : MY Bri to CCT RGBW pwm convert
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
my_int8_t myCTPAbri2CCTRGBWPwm(const my_uint8_t bri, my_uint16_t uwPWM[4])
{
    my_float_t fTempDiv = bri;
    my_float_t fTempDiv2 = fTempDiv * fTempDiv;
    my_float_t fTempDiv3 = fTempDiv * fTempDiv * fTempDiv;
    my_float_t afPWM[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    if (83 > bri)
    {
        afPWM[0] = BRI2RGBWPWM_2000_3200[0][0] * fTempDiv2 + BRI2RGBWPWM_2000_3200[0][1] * fTempDiv + BRI2RGBWPWM_2000_3200[0][2];
        afPWM[1] = BRI2RGBWPWM_2000_3200[1][0] * fTempDiv2 + BRI2RGBWPWM_2000_3200[1][1] * fTempDiv + BRI2RGBWPWM_2000_3200[1][2];
        afPWM[2] = BRI2RGBWPWM_2000_3200[2][0] * fTempDiv2 + BRI2RGBWPWM_2000_3200[2][1] * fTempDiv + BRI2RGBWPWM_2000_3200[2][2];
        afPWM[3] = BRI2RGBWPWM_2000_3200[3][0] * fTempDiv2 + BRI2RGBWPWM_2000_3200[3][1] * fTempDiv + BRI2RGBWPWM_2000_3200[3][2];
    }
    else if (83 <= bri)
    {
        afPWM[0] = BRI2RGBWPWM_3200_5700[0][0] * fTempDiv3 + BRI2RGBWPWM_3200_5700[0][1] * fTempDiv2 + BRI2RGBWPWM_3200_5700[0][2] * fTempDiv + BRI2RGBWPWM_3200_5700[0][3];
        afPWM[1] = BRI2RGBWPWM_3200_5700[1][0] * fTempDiv3 + BRI2RGBWPWM_3200_5700[1][1] * fTempDiv2 + BRI2RGBWPWM_3200_5700[1][2] * fTempDiv + BRI2RGBWPWM_3200_5700[1][3];
        afPWM[2] = BRI2RGBWPWM_3200_5700[2][0] * fTempDiv3 + BRI2RGBWPWM_3200_5700[2][1] * fTempDiv2 + BRI2RGBWPWM_3200_5700[2][2] * fTempDiv + BRI2RGBWPWM_3200_5700[2][3];
        afPWM[3] = BRI2RGBWPWM_3200_5700[3][0] * fTempDiv3 + BRI2RGBWPWM_3200_5700[3][1] * fTempDiv2 + BRI2RGBWPWM_3200_5700[3][2] * fTempDiv + BRI2RGBWPWM_3200_5700[3][3];
    }
    else
    {
        return -1;
    }

    

    afPWM[0] = (0.0f > afPWM[0]) ? 0.0f : afPWM[0];
    afPWM[1] = (0.0f > afPWM[1]) ? 0.0f : afPWM[1];
    afPWM[2] = (0.0f > afPWM[2]) ? 0.0f : afPWM[2];
    afPWM[3] = (0.0f > afPWM[3]) ? 0.0f : afPWM[3];

    afPWM[0] = (MY_COLOR_CONVERT_PWM_MAX < afPWM[0]) ? MY_COLOR_CONVERT_PWM_MAX : afPWM[0];
    afPWM[1] = (MY_COLOR_CONVERT_PWM_MAX < afPWM[1]) ? MY_COLOR_CONVERT_PWM_MAX : afPWM[1];
    afPWM[2] = (MY_COLOR_CONVERT_PWM_MAX < afPWM[2]) ? MY_COLOR_CONVERT_PWM_MAX : afPWM[2];
    afPWM[3] = (MY_COLOR_CONVERT_PWM_MAX < afPWM[3]) ? MY_COLOR_CONVERT_PWM_MAX : afPWM[3];

    {
        uwPWM[0] = (my_uint16_t)afPWM[0];
        uwPWM[1] = (my_uint16_t)afPWM[1];
        uwPWM[2] = (my_uint16_t)afPWM[2];
        uwPWM[3] = (my_uint16_t)afPWM[3];
    }
    
    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

