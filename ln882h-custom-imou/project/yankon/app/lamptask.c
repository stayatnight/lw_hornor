#include"lamptask.h"
#include "utils/debug/log.h"
#include "slData.h"
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
    LOG(LOG_LVL_INFO, "lamp bri percent ctrl %d\r\n", ucPercent);
   // pLampParam->uwBri = APP_RANGE(pLampParam->uwBri, LIGHT_BRIGHT_MIN, LIGHT_BRIGHT_MAX);
    return myLampBriCtrl(gucLampId, ulPeroidMs, pLampParam->uwBri);
}
int lamp_flash_count(void)
{
    myLampParam_t *pLampParam = &s_stCurLampParam;
    myLampFlashCtrl(gucLampId,500,1000,3,NULL,0);
    LOG(LOG_LVL_INFO,"is flashing\r\n");
    rlFlagSet(RL_FLAG_SYS_FACTORY_RESET,0);
    slDataFactoryReset();
    return 0;
}