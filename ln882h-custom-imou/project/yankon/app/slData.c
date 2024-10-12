/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : slData.c                                                      *
* Description :                                                               *
*               smart lamp data source file                                   *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-08                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "slData.h"
#include "myCrc.h"
#include "magiclink.h"
//#include "sys_api.h"
//#include "semphr.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct
{
    uint8_t ucDuationIndex;
    uint16_t uwDuration;
} rlDataDurationMap_t;
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
const rlDataDurationMap_t rlDataAdjDurationMap[] =
{
    {RL_ADJ_NONE,   0},
    {RL_ADJ_500MS,  500},
    {RL_ADJ_1000MS, 1000},
    {RL_ADJ_1500MS, 1500},
    {RL_ADJ_2000MS, 2000},
    {RL_ADJ_2500MS, 2500},
    {RL_ADJ_3000MS, 3000},
};

#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
const rlDataDurationMap_t rlDataSleepDurationMap[] =
{
    {SL_SLEEP_5MIN,  5*60},
    {SL_SLEEP_10MIN, 10*60},
    {SL_SLEEP_15MIN, 15*60},
    {SL_SLEEP_20MIN, 20*60},
    {SL_SLEEP_25MIN, 25*60},
    {SL_SLEEP_30MIN, 30*60},
    {SL_SLEEP_FOREVER, 0},
};
#endif

rlData_t g_stRlData;
static SemaphoreHandle_t rlFlagSetMutex;
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : rlFlagSet
 Description : reading lamp set flag
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void rlFlagSet(eRlFlag_t eFlag, uint8_t enable)
{
    xSemaphoreTake(rlFlagSetMutex, portMAX_DELAY);
    if (enable) {
        g_stRlData.liveData.ulFlag |= (1 << eFlag);
    } else {
        g_stRlData.liveData.ulFlag &= ~(1<<eFlag);
    }
    xSemaphoreGive(rlFlagSetMutex);
}

/******************************************************************************
 Function    : rlFlagRevert
 Description : reading lamp revert flag
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void rlFlagRevert(eRlFlag_t eFlag)
{
    xSemaphoreTake(rlFlagSetMutex, portMAX_DELAY);
    g_stRlData.liveData.ulFlag ^= (1<<eFlag);
    xSemaphoreGive(rlFlagSetMutex);
}

/******************************************************************************
 Function    : rlFlagGet
 Description : reading lamp get flag
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t rlFlagGet(eRlFlag_t eFlag)
{
    return (g_stRlData.liveData.ulFlag & (1<<eFlag)) ? 1 : 0;
}

/******************************************************************************
 Function    : rlDataGetFwVersion
 Description : reading lamp data get firmware version
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
const char* rlDataGetFwVersion(void)
{
    return RL_FIRMWARE_VER;
}

/******************************************************************************
 Function    : rlDataReadFctData
 Description : 读取Flash fct参数
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataReadFctData(void)
{
    int ret = 0;
    uint16_t crc = 0;

    myHalFlashRead(RL_FCT_DATA_FLASH_ADDR, (uint8_t *)&g_stRlData.fctData, sizeof(stRlFctData_t));
    crc = myCalcCRC((uint8_t*)&g_stRlData.fctData, sizeof(stRlFctData_t)-2);
    if(crc != g_stRlData.fctData.crc) {
        my_hal_log_warning("\r\nFlash FCT Data crc failure\r\n");
        g_stRlData.fctData.fctMode = 0;
        g_stRlData.fctData.fctOk = 0;
        memset(g_stRlData.fctData.sn, 0, 32);
        myHalWifiGetMacAddr(g_stRlData.fctData.sn, 32);
        rlDataWriteFctData();
        my_hal_log_info("\r\nFlash FCT Data factory reset\r\n");
        ret = -1;
    }
    return ret;
}

/******************************************************************************
 Function    : rlDataWriteFctData
 Description : 保存Flash fct参数
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataWriteFctData(void)
{
    int ret = 0;

    g_stRlData.fctData.crc = myCalcCRC((uint8_t*)&g_stRlData.fctData, sizeof(stRlFctData_t)-2);
    myHalFlashErase(RL_FCT_DATA_FLASH_ADDR);
    ret = myHalFlashWrite(RL_FCT_DATA_FLASH_ADDR, (uint8_t*)&g_stRlData.fctData, sizeof(stRlFctData_t));
    if (ret != 0) {
        my_hal_log_error("rl data write fct failed %d\r\n", ret);
    }
    return ret;
}

/******************************************************************************
 Function    : rlDataReadConfigData
 Description : 读取Flash配置参数
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataReadConfigData(void)
{
    uint16_t crc = 0;

    myHalFlashRead(RL_CFG_DATA_FLASH_ADDR, (uint8_t *)&g_stRlData.saveData, sizeof(stRlSaveData_t));
    crc = myCalcCRC((uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t)-2);
    if(crc != g_stRlData.saveData.crc) {
        my_hal_log_warning("\r\nFlash config data crc failure\r\n");
        myHalFlashRead(RL_CFG_DATA_FLASH_ADDR_BACKUP, (uint8_t *)&g_stRlData.saveData, sizeof(stRlSaveData_t));
        crc = myCalcCRC((uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t)-2);
        if (crc != g_stRlData.saveData.crc) {
            my_hal_log_warning("\r\nFlash Config Data backup Crc Failure\r\n");
            rlDataRestoreFactory();  //恢复出厂的配置数据
        }
        else {
            myHalFlashErase(RL_CFG_DATA_FLASH_ADDR);
            myHalFlashWrite(RL_CFG_DATA_FLASH_ADDR, (uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t));
        }
    }
    else {
        myHalFlashRead(RL_CFG_DATA_FLASH_ADDR_BACKUP, (uint8_t *)&g_stRlData.saveData, sizeof(stRlSaveData_t));
        crc = myCalcCRC((uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t)-2);
        if (crc != g_stRlData.saveData.crc) {
            my_hal_log_warning("\r\nFlash config data backup crc failure\r\n");
            myHalFlashRead(RL_CFG_DATA_FLASH_ADDR, (uint8_t *)&g_stRlData.saveData, sizeof(stRlSaveData_t));
            myHalFlashErase(RL_CFG_DATA_FLASH_ADDR_BACKUP);
            myHalFlashWrite(RL_CFG_DATA_FLASH_ADDR_BACKUP, (uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t));
        }
    }
    return 0;
}

/******************************************************************************
 Function    : rlDataWriteConfigData
 Description : 保存Flash配置参数
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataWriteConfigData(void)
{
    g_stRlData.saveData.crc = myCalcCRC((uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t)-2);
    myHalFlashErase(RL_CFG_DATA_FLASH_ADDR);
    myHalFlashWrite(RL_CFG_DATA_FLASH_ADDR, (uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t));
    myHalFlashErase(RL_CFG_DATA_FLASH_ADDR_BACKUP);
    myHalFlashWrite(RL_CFG_DATA_FLASH_ADDR_BACKUP, (uint8_t*)&g_stRlData.saveData, sizeof(stRlSaveData_t));
    my_hal_log_debug("Flash Config Data Write\r\n");
    return 0;
}

/******************************************************************************
 Function    : rlDataRestoreFactory
 Description : Flash配置参数恢复出厂设置
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataRestoreFactory(void)
{
    stRlSaveData_t *pSaveData = &g_stRlData.saveData;
    stRlLiveData_t *pLiveData = &g_stRlData.liveData;

    memset((void *)pSaveData, 0, sizeof(stRlSaveData_t));
    pSaveData->ulUsed = CFG_DATA_OK;
    strcpy(pSaveData->chDevName, RL_DEV_NAME_DEF);
    rlDataSetAdjDuration(RL_DEV_DURATION_DEF);
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    rlDataSetSleepDuration(SL_SLEEP_15MIN);
#endif
    rlDataWriteConfigData();

    my_hal_log_info("Restore Factory End.\r\n");
    return 0;
}

/******************************************************************************
 Function    : slDataFactoryReset
 Description : smart light 恢复出厂设置
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void slDataFactoryReset(void) 
{
#if defined(APP_MAGIC_LINK_USE) && (APP_MAGIC_LINK_USE != 0)
    if (g_stRlData.fctData.fctMode == 0) {
        extern int MagicLinkReset(void);
        MagicLinkReset();
    }
#endif
    rlDataRestoreFactory();
    vTaskDelay(10);
    //sys_reset();
}

/******************************************************************************
 Function    : rlDataSetDevName
 Description : reading lamp data get device name
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataSetDevName(const char* pName) 
{
    if (!pName || strlen(pName) > 63) {
        return -1;
    }
    strcpy(g_stRlData.saveData.chDevName, pName);
    rlFlagSet(RL_FLAG_SYS_CFG_DATA_SAVE, 1);
    return 0;
}

/******************************************************************************
 Function    : rlDataGetDevName
 Description : reading lamp data get device name
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataGetDevName(char* pName, uint32_t nameBufSize) 
{
    uint32_t nameLen = 
        strlen(g_stRlData.saveData.chDevName) > nameBufSize - 1 ? 
        nameBufSize - 1 : 
        strlen(g_stRlData.saveData.chDevName);

    if (!pName || nameLen == 0) {
        return -1;
    }
    memset(pName, 0, nameBufSize);
    strncpy(pName, g_stRlData.saveData.chDevName, nameLen);
    return 0;
}

/******************************************************************************
 Function    : rlDataSetDuration
 Description : reading lamp data get adjust duration
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int rlDataSetAdjDuration(eRlAdjDuation_t eDuration) 
{
    if (eDuration >= RL_ADJ_NUM) {
        return -1;
    }
    g_stRlData.saveData.eDuration = eDuration;
    switch (g_stRlData.saveData.eDuration) {
    case RL_ADJ_NONE:
        g_stRlData.liveData.uwAdjDuration = 0;  
        break;
    case RL_ADJ_500MS:
    #if APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX
        g_stRlData.liveData.uwAdjDuration = 30;
    #else
        g_stRlData.liveData.uwAdjDuration = 500;
    #endif    
        break;
    case RL_ADJ_1000MS:
    #if APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX
        g_stRlData.liveData.uwAdjDuration = 60;
    #else
        g_stRlData.liveData.uwAdjDuration = 1000;
    #endif    
        break;
    case RL_ADJ_1500MS:
    #if APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX
        g_stRlData.liveData.uwAdjDuration = 180;
    #else
        g_stRlData.liveData.uwAdjDuration = 1500;
    #endif    
        break;
    case RL_ADJ_2000MS:
    #if APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX
        g_stRlData.liveData.uwAdjDuration = 600;
    #else
        g_stRlData.liveData.uwAdjDuration = 2000;
    #endif
        break;
    case RL_ADJ_2500MS:
    #if APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX
        g_stRlData.liveData.uwAdjDuration = 1200;
    #else
        g_stRlData.liveData.uwAdjDuration = 2500;
    #endif
        break;
    case RL_ADJ_3000MS:
        g_stRlData.liveData.uwAdjDuration = 3000;
        break;
    default:
        g_stRlData.liveData.uwAdjDuration = 1000;
        break;
    }
    rlFlagSet(RL_FLAG_SYS_CFG_DATA_SAVE, 1);
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    extern int8_t rlLampAdjustDurationCtrl(uint16_t uwDuration);
    rlLampAdjustDurationCtrl(g_stRlData.liveData.uwAdjDuration);
#endif
    return 0;
}

/******************************************************************************
 Function    : rlDataGetAdjDuration
 Description : reading lamp data get adjust duration
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t rlDataGetAdjDuration(void) 
{
    return (uint8_t)g_stRlData.saveData.eDuration;
}

/******************************************************************************
 Function    : rlDataSetSleepDuration
 Description : reading lamp data get sleep duration
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
int rlDataSetSleepDuration(eSlSleepDuration_t eDuration) 
{
    if (eDuration >= SL_SLEEP_NUM) {
        return -1;
    }
    g_stRlData.saveData.eSleepDuration = eDuration;
    switch (g_stRlData.saveData.eSleepDuration) {
    case SL_SLEEP_FOREVER:
        g_stRlData.liveData.uwSleepDuration = 0;
        break;
    case SL_SLEEP_5MIN:
        g_stRlData.liveData.uwSleepDuration = 5*60;
        break;
    case SL_SLEEP_10MIN:
        g_stRlData.liveData.uwSleepDuration = 10*60;
        break;
    case SL_SLEEP_15MIN:
        g_stRlData.liveData.uwSleepDuration = 15*60;
        break;
    case SL_SLEEP_20MIN:
        g_stRlData.liveData.uwSleepDuration = 20*60;
        break;
    case SL_SLEEP_25MIN:
        g_stRlData.liveData.uwSleepDuration = 25*60;
        break;
    case SL_SLEEP_30MIN:
        g_stRlData.liveData.uwSleepDuration = 30*60;
        break;
    default:
        g_stRlData.liveData.uwSleepDuration = 15*60;
        break;
    }
    rlFlagSet(RL_FLAG_SYS_CFG_DATA_SAVE, 1);
    extern int8_t rlLampSleepDurationCtrl(uint16_t uwDuration);
    rlLampSleepDurationCtrl(g_stRlData.liveData.uwSleepDuration);
    my_hal_log_debug("set sleep duration: %d\r\n", g_stRlData.liveData.uwSleepDuration);
    return 0;
}

/******************************************************************************
 Function    : rlDataGetSleepDuration
 Description : smart lamp data get sleep duration
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t rlDataGetSleepDuration(void) 
{
    return (uint8_t)g_stRlData.saveData.eSleepDuration;
}
#endif

/******************************************************************************
 Function    : rlDataShowVersion
 Description : reading lamp data show version
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void rlDataShowVersion(void)
{
    char temp[32] = {0};
    MagicLinkGetSDKVersion(temp, 32);
    my_hal_log_raw("\r\n*********************** Module Info *************************\r\n");
    my_hal_log_raw("PID:XXXX name %s firmware version:%s\r\n", g_stRlData.saveData.chDevName, RL_FIRMWARE_VER);
    my_hal_log_raw("release: %s %s\r\n", __DATE__, __TIME__);
    my_hal_log_raw("magiclink Version: %s\r\n", temp);
    my_hal_log_raw("*************************************************************\r\n");
}

/******************************************************************************
 Function    : rlDataInit
 Description : 数据初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : 0    - 成功
               其它 - 失败码
 Other       : (none)
******************************************************************************/
int rlDataInit(void) 
{
    stRlLiveData_t *pLiveData = &g_stRlData.liveData;

    rlFlagSetMutex = xSemaphoreCreateMutex();

    if (rlFlagSetMutex == NULL) {
        my_hal_log_error("rlFlagSetMutex create failed\r\n");
        while (1) {
            ;
        }
    }

    rlDataReadFctData();

    rlDataReadConfigData();

    rlDataShowVersion();

    #if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX)
    pLiveData->ucLightMode = 100;
    #else
    pLiveData->ucLightMode = 0;
    #endif
    pLiveData->uwAdjDuration = rlDataAdjDurationMap[g_stRlData.saveData.eDuration].uwDuration;
    my_hal_log_debug("adjust duration: %d\r\n", pLiveData->uwAdjDuration);
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    pLiveData->uwSleepDuration = rlDataSleepDurationMap[g_stRlData.saveData.eSleepDuration].uwDuration;
    my_hal_log_debug("sleep duration: %d\r\n", pLiveData->uwSleepDuration);
#endif
    rlFlagSet(RL_FLAG_SYS_DEV_ONLINE, 0);
    rlFlagSet(RL_FLAG_SYS_FACTORY_WINDOW, 1);
    rlFlagSet(RL_FLAG_LAMP_BRI_DIRECTION, 1);
    return 0;
}



#ifdef __cplusplus
}
#endif

