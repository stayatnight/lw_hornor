#include"lamptask.h"
#include "utils/debug/log.h"
int LampSwitchCtrl(int status, int ulPeroidMs)
{
    myLampParam_t *pLampParam = &s_stCurLampParam;

    LOG(LOG_LVL_INFO, "lamp switch ctrl %d\r\n", status);
    pLampParam->ucSwitch = status ? 1 : 0;
    return myLampSwitchCtrl(gucLampId, ulPeroidMs, pLampParam->ucSwitch);
}
int LampBriPercentCtrl(int ucPercent, int ulPeroidMs)
{
    myLampParam_t *pLampParam = &s_stCurLampParam;
    
    ucPercent = APP_MIN_VAL(ucPercent, 100);
    pLampParam->uwBri = LIGHT_PERCENT_TO_BRIGHT(ucPercent);
   // pLampParam->uwBri = APP_RANGE(pLampParam->uwBri, LIGHT_BRIGHT_MIN, LIGHT_BRIGHT_MAX);
    return myLampBriCtrl(gucLampId, ulPeroidMs, pLampParam->uwBri);
}