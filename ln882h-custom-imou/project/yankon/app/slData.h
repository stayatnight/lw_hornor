/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : slData.h                                                      *
* Description :                                                               *
*               smart lamp data header file                                   *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-08                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __SL_DATA_H__
#define __SL_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHal.h"
#include "dimmingLib.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_COMPILE_VER                                     "v1.0.7"

#if (APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_READING)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.1.3"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光博荣台灯"
#define RL_DEV_MODEL                                       "MTSL2010"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_1000MS
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_CEILING)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.1.3"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光星辰吸顶灯"
#define RL_DEV_MODEL                                       "MXL1151"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_500MS
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_BEDSIDE)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.3"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光荣皓床头灯"
#define RL_DEV_MODEL                                       "MTSL2011"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_1000MS
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_AURORA)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.5"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光AURORA氛围灯"
#define RL_DEV_MODEL                                       "MXL1151"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_500MS
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_AIRER_BASE)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.3"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光荣云晾衣架L561"
#define RL_DEV_MODEL                                       "L561"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_500MS
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_BATHBULLY_BASE)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.7"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光荣星浴霸Y681"
#define RL_DEV_MODEL                                       "Y681"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_500MS
#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT_PTJX)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.4"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光荣旭夜灯MTSL2015"
#define RL_DEV_MODEL                                       "MTSL2015"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_500MS
#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING_L)
#define RL_FIRMWARE_VER                                    MY_COMPILE_VER//"v1.0.0"
#define RL_HARDWARE_VER                                    "v1.0.0"
#define RL_DEV_NAME_DEF                                    "阳光台灯"
#define RL_DEV_MODEL                                       "MTSLXXXX"
#define RL_SUB_DEV_TYPE                                    "0"
#define RL_DEV_MCU_VER                                     ""
#define RL_DEV_DURATION_DEF                                RL_ADJ_1000MS

#endif

//配置数据(在头上加) AF CD 88 03 (0x388cdaf) <大端序(网络序)>
#define RL_MAC_FLASH_ADDR                                  0
#define RL_FCT_DATA_FLASH_ADDR                             4096
#define RL_CFG_DATA_FLASH_ADDR                             4096*2
#define RL_CFG_DATA_FLASH_ADDR_BACKUP                      4096*3
#define CFG_DATA_OK                                        (0x388cdafe)
#define FCT_DATA_OK                                        (0xA55A5AA5)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
typedef enum
{
    RL_FLAG_TASK_LAMP_RUNNING,
    RL_FLAG_TASK_KEY_RUNNING,
    RL_FLAG_TASK_FCT_RUNNING,
    RL_FLAG_TASK_PWR_RUNNING,
    RL_FLAG_SYS_DEV_ONLINE,
    RL_FLAG_SYS_FACTORY_WINDOW,
    RL_FLAG_SYS_FACTORY_RESET,
    RL_FLAG_SYS_DATA_SYNC_NEED,
    RL_FLAG_SYS_CFG_DATA_SAVE, 
    RL_FLAG_LAMP_BRI_DIRECTION,   
    RL_FLAG_MAGIC_REPORT_NEED,
    RL_FLAG_MAGIC_REPORT_LIGHT_NEED,
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    RL_FLAG_POS_PICKUP,
    RL_FLAG_MAGIC_REPORT_BAT_NEED,
    RL_FLAG_MAGIC_REPORT_POS_NEED,
#endif
} eRlFlag_t;

typedef enum
{
    RL_ADJ_NONE,
    RL_ADJ_500MS,
    RL_ADJ_1000MS,
    RL_ADJ_1500MS,
    RL_ADJ_2000MS,
    RL_ADJ_2500MS,
    RL_ADJ_3000MS,
    RL_ADJ_NUM
} eRlAdjDuation_t;

#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
typedef enum
{
    SL_SLEEP_5MIN,
    SL_SLEEP_10MIN,
    SL_SLEEP_15MIN,
    SL_SLEEP_20MIN,
    SL_SLEEP_25MIN,
    SL_SLEEP_30MIN,
    SL_SLEEP_FOREVER,
    SL_SLEEP_NUM
} eSlSleepDuration_t;
#endif

#if (APP_DEV_TYPE_USED  == APP_DEV_TYPE_AIRER_BASE)
typedef struct
{
    uint8_t ucLightOnoff;
    uint8_t ucAirerOnoff;
    uint8_t ucMotionCtrl;
    uint8_t ucPosition;
} stSlDevAirerData_t;
#endif

#if (APP_DEV_TYPE_USED  == APP_DEV_TYPE_BATHBULLY_BASE)
typedef struct
{
    uint8_t ucLightOnoff;
    uint8_t ucLightMode;
    uint8_t ucDryOnoff;
    uint8_t ucFanOnoff;
    uint8_t ucVentilationOnoff;
    uint8_t ucWarmFanOnoff;
    uint8_t ucWarmFanGear;
    int8_t scTemperature;
} stSlDevBathBullyData_t;
#endif

typedef struct
{
    uint32_t fctMode;
    uint32_t fctOk;
    char sn[32];
    uint8_t ucRev[2];
    uint16_t crc;
} stRlFctData_t;

//配置数据子结构
typedef struct 
{
    uint32_t ulUsed;
    char chDevName[64];
    eRlAdjDuation_t eDuration;
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    eSlSleepDuration_t eSleepDuration;
#endif
    uint8_t ucWallSwitchOperCnt;
    uint8_t ucLampWallSwitchMode; //wall switch light mode
    uint8_t ucStartMode; //normal or switch mode
    myLampParam_t stLampSaveParam;
#if (APP_DEV_TYPE_USED  == APP_DEV_TYPE_AIRER_BASE)
    stSlDevAirerData_t stSlDevAirerData;
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_BATHBULLY_BASE)
    stSlDevBathBullyData_t stSlDevBathBullyData;
#endif
    uint32_t ulRev;
    uint16_t uwRev;
    uint16_t crc; 
} stRlSaveData_t;

typedef struct
{
    uint32_t ulFlag;
    uint16_t uwAdjDuration;
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
    uint16_t uwSleepDuration;
#endif
    uint8_t ucLightMode;
    uint8_t ucBatteryValue;
#if (APP_DEV_TYPE_USED  == APP_DEV_TYPE_AIRER_BASE)
    stSlDevAirerData_t stSlDevAirerData;
#elif (APP_DEV_TYPE_USED  == APP_DEV_TYPE_BATHBULLY_BASE)
    stSlDevBathBullyData_t stSlDevBathBullyData;
#endif
} stRlLiveData_t;

typedef struct
{
    stRlFctData_t fctData;
    stRlSaveData_t saveData;
    stRlLiveData_t liveData;
} rlData_t;
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/
extern rlData_t g_stRlData;
/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
void rlFlagSet(eRlFlag_t eFlag, uint8_t enable);
void rlFlagRevert(eRlFlag_t eFlag);
uint8_t rlFlagGet(eRlFlag_t eFlag);
const char* rlDataGetFwVersion(void);
int rlDataReadFctData(void);
int rlDataWriteFctData(void);
int rlDataReadConfigData(void);
int rlDataWriteConfigData(void);
int rlDataRestoreFactory(void);
void slDataFactoryReset(void);
int rlDataSetDevName(const char *pName);
int rlDataGetDevName(char *pName, uint32_t nameBufSize);
int rlDataSetAdjDuration(eRlAdjDuation_t eDuration);
uint8_t rlDataGetAdjDuration(void);
#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE)
int rlDataSetSleepDuration(eSlSleepDuration_t eDuration);
uint8_t rlDataGetSleepDuration(void);
#endif

int rlDataInit(void);


#ifdef __cplusplus
}
#endif

#endif /*__SL_DATA_H__*/


