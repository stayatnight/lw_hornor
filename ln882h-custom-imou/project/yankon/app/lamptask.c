#include"lamptask.h"
#include "utils/debug/log.h"
int LampSwitchCtrl(int status, int ulPeroidMs)
{
    myLampParam_t *pLampParam = &s_stCurLampParam;

    LOG(LOG_LVL_INFO, "lamp switch ctrl %d\r\n", status);
    pLampParam->ucSwitch = status ? 1 : 0;
    return myLampSwitchCtrl(gucLampId, ulPeroidMs, pLampParam->ucSwitch);
}
