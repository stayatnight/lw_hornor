/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalWifi.c                                                   *
* Description :                                                               *
*               my hal wifi source file                                       *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-15                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalWifi.h"
#include "wifi_conf.h"
#include "wifi_util.h"
#include "dhcp/dhcps.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
/*Static IP ADDRESS*/
#ifndef AP_IP_ADDR0
#define AP_IP_ADDR0   192
#define AP_IP_ADDR1   168
#define AP_IP_ADDR2   43
#define AP_IP_ADDR3   1
#endif
   
/*NETMASK*/
#ifndef AP_NETMASK_ADDR0
#define AP_NETMASK_ADDR0   255
#define AP_NETMASK_ADDR1   255
#define AP_NETMASK_ADDR2   255
#define AP_NETMASK_ADDR3   0
#endif

/*Gateway Address*/
#ifndef AP_GW_ADDR0
#define AP_GW_ADDR0   192
#define AP_GW_ADDR1   168
#define AP_GW_ADDR2   43
#define AP_GW_ADDR3   1  
#endif
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
static uint8_t myHalCfgSoftAPChannel = 6;
//static myHalWifiMode_t halWifiMode = MY_HAL_WIFI_MODE_STATION;

static rtw_ap_info_t ap = {0};
static unsigned char password[65] = {0};
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/
extern struct netif xnetif[NET_IF_NUM];
//extern void myNetRegWifiStatus(myHalWifiConnectStatus_t st);
/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : transMacAddrValue
 Description : trans mac addr value
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : mac addr value
 Other       : (none)
******************************************************************************/
static uint8_t transMacAddrValue(char Chr1, char Chr2)
{
    uint8_t val = 0;

    if (('0' <= Chr1) && ('9' >= Chr1)) {
        val = (Chr1 - '0');
    }
    else if ( ('a' <= Chr1) && ('f' >= Chr1) ) {
        val = (Chr1 - 'a' + 10);
    } else {
        val = (Chr1 - 'A' + 10);
    }

    val *= 16;

    if (('0' <= Chr2) && ('9' >= Chr2)) {
        val += (Chr2 - '0');
    } else if ( ('a' <= Chr2) && ('f' >= Chr2) ) {
        val += (Chr2 - 'a' + 10);
    } else {
        val += (Chr2 - 'A' + 10);
    }
    return val;
}

/******************************************************************************
 Function    : softAP_start
 Description : opple hal soft ap start
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiStartSoftAP(const char* ap_name, const char *ap_password)
{
	int timeout = 20;
    int ret = 0;
    rtw_security_t my_rtw_security = (!ap_password) ? RTW_SECURITY_OPEN : RTW_SECURITY_WPA2_AES_PSK;

#if CONFIG_LWIP_LAYER
	ip4_addr_t ipaddr;
	ip4_addr_t netmask;
	ip4_addr_t gw;
	struct netif * pnetif = &xnetif[1];
#endif
	
//  wifi_off();
//  LOS_TaskDelay(20);
//  if (wifi_on(RTW_MODE_AP) < 0){
//  	myHalWifiLog("Wifi on failed!\r\n");
//  	return -1;
//  }

#if CONFIG_LWIP_LAYER
	dhcps_deinit();
	IP4_ADDR(&ipaddr, AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
	IP4_ADDR(&netmask, AP_NETMASK_ADDR0, AP_NETMASK_ADDR1 , AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
	IP4_ADDR(&gw, AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);
	netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
    netif_set_link_up(pnetif);
#endif
	
#ifdef CONFIG_WPS_AP
	//wpas_wps_dev_config(pnetif->hwaddr, 1);
#endif

    wifi_disable_powersave();//add to close powersave
    if(wifi_start_ap((char*)ap_name, my_rtw_security, (char*)ap_password, strlen((const char *)ap_name), strlen((const char *)ap_password), myHalCfgSoftAPChannel) != RTW_SUCCESS) {
        my_hal_log_error("wifi start ap mode failed!\n\r");
        return -1;
    }

	while(1) {
		char essid[33];
		if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) > 0) {
			if(strcmp((const char *) essid, (const char *)ap_name) == 0) {
				//printf("%s started\n", ap_name);
				ret = 0;
				break;
			}
		}

		if(timeout == 0) {
			my_hal_log_error("Start AP timeout!\n\r");
			ret = -1;
			break;
		}

        vTaskDelay(1000);
		timeout --;
	}
#if CONFIG_LWIP_LAYER
	dhcps_init(pnetif);
#endif
    vTaskDelay(1000);
	return ret;
}

/******************************************************************************
 Function    : station_start
 Description : myle hal station start
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if 0
static int station_start()
{
	wifi_off();
    LOS_TaskDelay(20);
	if (wifi_on(RTW_MODE_STA) < 0){
		printf("Wifi on failed!\r\n");
		return -1;
	}

	return 0;
}
#endif

/******************************************************************************
 Function    : myHalWifiInit
 Description : myle hal wifi Init
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if 0
void myHalWifiInit()
{
    //softAP_start("aaa", NULL);

    station_start();

//  startLowerPowerAp();
}
#endif

/******************************************************************************
 Function    : myHalWifiGetMacAddr
 Description : my wifi get mac address
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiGetMacAddr(char *mac, uint32_t macBufSize)
{
    char buf[32] = { 0 };

    if (!mac || macBufSize < 32) {
        return -1;
    }
    if (RTW_SUCCESS != wifi_get_mac_address(buf)) {
        return -2;
    }
    for (uint8_t i = 0; i < strlen(buf); i++) {
        if (buf[i] >= 'a' && buf[i] <= 'f') {
            buf[i] -= 32;
        }
    }
    memcpy(&mac[0], &buf[0], 2);
    memcpy(&mac[2], &buf[3], 2);
    memcpy(&mac[4], &buf[6], 2);
    memcpy(&mac[6], &buf[9], 2);
    memcpy(&mac[8], &buf[12], 2);
    memcpy(&mac[10], &buf[15], 2);
	return 0;
}

/******************************************************************************
 Function    : myHalWifiGetMacAddrByte
 Description : my wifi get mac address byte
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiGetMacAddrByte(uint8_t mac[6]) 
{
    char buf[32] = { 0 };

    if (RTW_SUCCESS != wifi_get_mac_address(buf)) {
        return -2;
    }

    mac[0] = transMacAddrValue(buf[0], buf[1]);
    mac[1] = transMacAddrValue(buf[3], buf[4]);
    mac[2] = transMacAddrValue(buf[6], buf[7]);
    mac[3] = transMacAddrValue(buf[9], buf[10]);
    mac[4] = transMacAddrValue(buf[12], buf[13]);
    mac[5] = transMacAddrValue(buf[15], buf[16]);

    return 0;
}

/******************************************************************************
 Function    : myHalWifiGetConnectStatus
 Description : myle wifi get connect status
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiGetConnectStatus()
{
	if ( wifi_is_connected_to_ap() == RTW_SUCCESS ) {
		return 1;
	} 
	return 0;
}

/******************************************************************************
 Function    : myHalWifiStationConnect
 Description : myle hal wifi station connect
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiStationConnect(char *pSSID, uint8_t ssidLen, char *pKey, uint8_t keyLen)
{
    int err = RTW_NO_ERROR;

	wifi_set_autoreconnect(1);
    if ( wifi_connect((char*)pSSID, RTW_SECURITY_WPA2_AES_PSK, (char*)pKey, ssidLen, keyLen, -1, NULL) !=  RTW_SUCCESS) {
        my_hal_log_error("wifi_connect failed\r\n");
    }
    err = wifi_get_last_error();
    if ( err ==  RTW_NO_ERROR) {
        my_hal_log_info("connect to ssid:%s, key:%s\r\n", (char *)pSSID, (char *)pKey);
    } else {
        my_hal_log_error("connect failed:%d\r\n", err);
    }
    return err;
}
//TODO 重连myHalWifiStationConnect
/******************************************************************************
 Function    : myHalWifiHilinkConnect
 Description : myle hal wifi hilink connect
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if 0
void myHalWifiHilinkConnect()
{
    //hilink_entry();
    myHalWifiLog("Hilink Connect\r\n");
}
#endif

/******************************************************************************
 Function    : myNetSetWifiMode
 Description : myle net set wifi mode
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if 0
void myHalWifiSetMode(myHalWifiMode_t wifiMode, char *pSoftAPSsid)
{
    myHalWifiLog("wifi set mode %d\r\n", wifiMode);
    if ( wifiMode > MY_HAL_WIFI_MODE_NUM) return;
    if ( halWifiMode == wifiMode) return;  
	myNetRegWifiStatus(MY_HAL_WIFI_ST_DISCONNECTED);
    switch ( wifiMode )
    {
    case MY_HAL_WIFI_MODE_STATION:
        station_start();
        break;
    case MY_HAL_WIFI_MODE_SOFTAP:
        softAP_start(pSoftAPSsid, NULL);
        break;
    default:
        break;
    }
    halWifiMode = wifiMode;
}
#endif

/******************************************************************************
 Function    : myHalWifiGetMode
 Description : myle net get wifi mode
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
#if 0
myHalWifiMode_t myHalWifiGetMode()
{
    return halWifiMode;
}
#endif

/******************************************************************************
 Function    : myHalWifiStartStaAp
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int myHalWifiStartStaAp(const char *ssid_ap, const char *key_ap)
{
	int timeout = 20;//, mode;
	volatile int ret = RTW_SUCCESS;
#if CONFIG_LWIP_LAYER
	struct netif * pnetiff = (struct netif *)&xnetif[1];
#endif

    if (!ssid_ap) {
        my_hal_log_error("Error: ssid_ap can not be NULL\r\n");
        ret = RTW_BADARG;
        goto exit;
    }

    ap.ssid.len = strlen(ssid_ap);
    if (ap.ssid.len > 32) {
        my_hal_log_error("Error: SSID length can't exceed 32\r\n");
        ret = RTW_BADARG;
        goto exit;
    }
    strncpy((char *)ap.ssid.val, (char*)ssid_ap, sizeof(ap.ssid.val));
    
    if (key_ap) {
        ap.password_len = strlen(key_ap);
        if (ap.password_len < 8 || ap.password_len > 64) {
            my_hal_log_error("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
            ret = RTW_INVALID_KEY;
            goto exit;
        }
        strncpy((char *)password, key_ap, sizeof(password));
        ap.password = password;   
    }
    else {
        ap.password = NULL;
        ap.password_len = 0;
    }

	my_hal_log_info("MYHAL_WLAN_AP_STA_ACTIVATE\n\r"); 
	if(ap.password == NULL){
      ap.security_type = RTW_SECURITY_OPEN;
    }
	else{
		if(ap.password_len <= RTW_WPA2_MAX_PSK_LEN &&
			ap.password_len >= RTW_MIN_PSK_LEN){ 
			ap.security_type = RTW_SECURITY_WPA2_AES_PSK;
			if(ap.password_len == RTW_WPA2_MAX_PSK_LEN){//password_len=64 means pre-shared key, pre-shared key should be 64 hex characters
				unsigned char i,j;
				for(i = 0;i < RTW_WPA2_MAX_PSK_LEN;i++){
					j = ap.password[i];
					if(!((j >='0' && j<='9') || (j >='A' && j<='F') || (j >='a' && j<='f'))){
						my_hal_log_error("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
						ret = RTW_INVALID_KEY;
						goto exit;
					}
				}
			}
		}
	}

    ap.channel = 1;

#if CONFIG_LWIP_LAYER
	dhcps_deinit();
#endif

#if defined(CONFIG_PLATFORM_8710C) && (defined(CONFIG_BT) && CONFIG_BT)
	if (wifi_set_mode(RTW_MODE_STA_AP) < 0){
		my_hal_log_error("\n\rERROR: Wifi on failed!");
		ret = RTW_ERROR;
		goto exit;
	}
#else
	wifi_off();
	vTaskDelay(20);
	if ((ret = wifi_on(RTW_MODE_STA_AP)) < 0){
		my_hal_log_error("\n\rERROR: Wifi on failed!");
		ret = RTW_ERROR;
		goto exit;
	}
#endif

	my_hal_log_info("\n\rStarting AP ...");
	if((ret = wifi_start_ap((char*)ap.ssid.val, ap.security_type, (char*)ap.password, ap.ssid.len, ap.password_len, ap.channel)) < 0) {
		my_hal_log_error("\n\rERROR: Operation failed!");
		goto exit;
	}
	while(1) {
		char essid[33];

		if(wext_get_ssid(WLAN1_NAME, (unsigned char *) essid) > 0) {
			if(strcmp((const char *) essid, (const char *)ap.ssid.val) == 0) {
				my_hal_log_info("\n\r%s started\n", ap.ssid.val);
				ret = RTW_SUCCESS;
				break;
			}
		}

		if(timeout == 0) {
			my_hal_log_error("\n\rERROR: Start AP timeout!");
			ret = RTW_TIMEOUT;
			break;
		}

		vTaskDelay(1 * configTICK_RATE_HZ);
		timeout --;
	}
#if CONFIG_LWIP_LAYER
	LwIP_UseStaticIP(&xnetif[1]);
#ifdef CONFIG_DONT_CARE_TP
	pnetiff->flags |= NETIF_FLAG_IPSWITCH;
#endif
	dhcps_init(pnetiff);
#endif

#if defined( CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE )&&( CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE == 1 )
	wifi_set_ap_polling_sta(1);
#endif
exit:
#if defined(CONFIG_INIC_CMD_RSP) && CONFIG_INIC_CMD_RSP
	inic_c2h_wifi_info("ATWB", ret);
#endif
//  init_wifi_struct();

    return ret;
}

#ifdef __cplusplus
}
#endif

