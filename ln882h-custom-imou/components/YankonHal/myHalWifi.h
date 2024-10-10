/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalWifi.h                                                   *
* Description :                                                               *
*               my hal wifi header file                                       *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-15                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __MY_HAL_WIFI_H__
#define __MY_HAL_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHal.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
typedef enum
{
    MY_HAL_WIFI_MODE_STATION,
    MY_HAL_WIFI_MODE_SOFTAP,
    MY_HAL_WIFI_MODE_NUM
} myHalWifiMode_t;

typedef enum
{
    MY_HAL_WIFI_ST_CONNECTED,
    MY_HAL_WIFI_ST_DISCONNECTED,
    MY_HAL_WIFI_ST_NUM
} myHalWifiConnectStatus_t;
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
int myHalWifiStartSoftAP(const char *ap_name, const char *ap_password);
int myHalWifiStartStaAp(const char *ssid_ap, const char *key_ap);
int myHalWifiStationConnect(char *pSSID, uint8_t ssidLen, char *pKey, uint8_t keyLen);
int myHalWifiGetMacAddr(char *mac, uint32_t macBufSize);
int myHalWifiGetMacAddrByte(uint8_t mac[6]);
int myHalWifiGetConnectStatus(void);



#ifdef __cplusplus
}
#endif



#endif   /*__MY_HAL_WIFI_H__*/

