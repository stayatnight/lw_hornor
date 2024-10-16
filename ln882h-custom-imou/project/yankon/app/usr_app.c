#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/system_parameter.h"
#include "utils/ln_psk_calc.h"
#include "utils/power_mgmt/ln_pm.h"
#include "utils/sysparam_factory_setting.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "lwip/tcpip.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "hal/hal_flash.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"

#include "rwip_config.h"
#include "llm_int.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "usr_app.h"
#include "usr_ble_app.h"
#include "ln_ble_app_kv.h"

#include "magiclink.h"
#include "demo.h"
//embed slData.c
#include "slData.h"
#include"lamptask.h"
#include "hal/hal_common.h"
#include "hal/hal_gpio.h"
#include "ln_test_common.h"
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "hal/hal_timer.h"
#include "hal/hal_clock.h"
#include"myKeyboard.h"
#include "ln_drv_pwm.h"
#include"key_task.h"
#include "gpio.h"
#include"ln_flash_test.h"
static OS_Thread_t g_usr_app_thread;
#define USR_APP_TASK_STACK_SIZE   4800 //Byte

#define WIFI_TEMP_CALIBRATE             1

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE   4*256 //Byte
#endif

//yankon light task
#if LAMP_TASK_EN && LAMP_TASK_EN==1
static OS_Thread_t g_lamp_thread;
#define LAMP_TASK_STACK_SIZE   4*256 //Byte
#endif
//yankon key task
#if KEY_TASK_EN && KEY_TASK_EN==1
static OS_Thread_t g_key_thread;
#define KEY_TASK_STACK_SIZE   4*256 //Byte
#endif


/* declaration */
static void wifi_init_ap(void);
static void wifi_init_sta(void);
static void usr_app_task_entry(void *params);
static void temp_cal_app_task_entry(void *params);

static uint8_t mac_addr[6]        = {0};
static uint8_t psk_value[40]      = {0x0};
static uint8_t ssid_value[33]      = {0x0};
static uint8_t target_sta_bssid[6] = {0x0};
static uint8_t target_sta_pwd[65] = { 0x0 };

wifi_sta_connect_t connect = {
    .ssid    = ssid_value,
    .pwd     = target_sta_pwd,
    .bssid   = target_sta_bssid,
    .psk_value = psk_value,
};

wifi_scan_cfg_t scan_cfg = {
        .channel   = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = 20,
};

wifi_softap_cfg_t ap_cfg = {
    .ssid            = "LN_AP_8899",
    .pwd             = "12345678",
    .bssid           = mac_addr,
    .ext_cfg = {
        .channel         = 6,
        .authmode        = WIFI_AUTH_WPA_WPA2_PSK,//WIFI_AUTH_OPEN,
        .ssid_hidden     = 0,
        .beacon_interval = 100,
        .psk_value = NULL,
    }
};

static uint8_t g_chip_uid[16]      = {0};
static uint8_t g_chip_wifi_mac[6]  = {0x00, 0x50, 0x00, 0x00, 0x00, 0x00};
static uint32_t rlLampGetNextClassVal(uint8_t ctrlType, uint32_t curVal,  const uint32_t *pTable, uint32_t maxClass)
{
    uint32_t newVal = curVal;

    if(ctrlType == 0) {
        if(newVal > pTable[0]) {
            uint8_t i=maxClass-1;
            while(i > 0) { if(newVal > pTable[i]) { newVal= pTable[i]; break; } else i--; }
            if(i==0) newVal=pTable[0];
        }
    } else {
        if(newVal < pTable[maxClass-1]) {
            uint8_t i=0;
            while(i < maxClass-1) { if(newVal < pTable[i]) {newVal = pTable[i]; break;} else i++; }
            if( i == maxClass-1 ) newVal=pTable[maxClass-1];
        }
    }
    return newVal;
}
static uint32_t djb_hash_hexdata(const char *input, uint32_t len)
{
    uint32_t hash = 5381;
    int c = *input;

    for (uint32_t i = 0; i < len; i++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        input++;
        c = *input;
    }
    return hash;
}

static void chip_mac_gen(void)
{
    hal_flash_read_unique_id(g_chip_uid);
    uint32_t hash = djb_hash_hexdata((const char *)g_chip_uid, sizeof(g_chip_uid));
    g_chip_wifi_mac[0] = 0x00;
    g_chip_wifi_mac[1] = 0x50;
    g_chip_wifi_mac[2] = (uint8_t)((hash) & 0xFF);
    g_chip_wifi_mac[3] = (uint8_t)((hash >> 8) & 0xFF);
    g_chip_wifi_mac[4] = (uint8_t)((hash >> 16) & 0xFF);
    g_chip_wifi_mac[5] = (uint8_t)((hash >> 24) & 0xFF);
}
//翻转开关函数
int8_t rlLampSwitchRevert(uint32_t ulPeroidMs)
{
    myLampParam_t *pLampParam = &s_stCurLampParam;

  //  //my_hal_log_info("rl lamp switch revert\r\n");
    pLampParam->ucSwitch = !pLampParam->ucSwitch;
    return myLampSwitchCtrl(gucLampId, ulPeroidMs, pLampParam->ucSwitch);
}
int8_t rlLampBriCtrlNextClass(uint32_t ulPeroidMs)
{
    int8_t ret = 0;
    uint16_t uwNewBri = 0;
    uint32_t percent = 0;
    myLampParam_t *pLampParam = &s_stCurLampParam;
    uint8_t ucBriDirection = rlFlagGet(RL_FLAG_LAMP_BRI_DIRECTION);

    percent = LIGHT_BRIGHT_TO_PERCENT(pLampParam->uwBri);
    percent = rlLampGetNextClassVal(ucBriDirection, percent,  k_a_uwBriPercent, sizeof(k_a_uwBriPercent) / sizeof(uint32_t));
    uwNewBri = LIGHT_PERCENT_TO_BRIGHT(percent);
    uwNewBri = APP_RANGE(uwNewBri, LIGHT_BRIGHT_MIN, LIGHT_BRIGHT_MAX);
    if (uwNewBri != pLampParam->uwBri) {
        pLampParam->uwBri = uwNewBri;
       // my_hal_log_info("rl lamp bright ctrl next %d\r\n", pLampParam->uwBri);
        ret = myLampBriCtrl(gucLampId, ulPeroidMs, pLampParam->uwBri);
    }
    return ret;
}
static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);

    ln_list_t *list;
    uint8_t node_count = 0;
    ap_info_node_t *pnode;

    wifi_manager_ap_list_update_enable(LN_FALSE);

    // 1.get ap info list.
    wifi_manager_get_ap_list(&list, &node_count);

    // 2.print all ap info in the list.
    LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
    {
        uint8_t * mac = (uint8_t*)pnode->info.bssid;
        ap_info_t *ap_info = &pnode->info;

        LOG(LOG_LVL_INFO, "\tCH=%2d,RSSI= %3d,", ap_info->channel, ap_info->rssi);
        LOG(LOG_LVL_INFO, "BSSID:[%02X:%02X:%02X:%02X:%02X:%02X],SSID:\"%s\"\r\n", \
                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ap_info->ssid);
    }

    wifi_manager_ap_list_update_enable(LN_TRUE);
}
/************************************************KEY TASK************************************/

uint32_t _rlKeyGetTickMs() 
{
    return xTaskGetTickCount();
}

static void netdev_get_ip_callback(struct netif *nif)
{
}

static void wifi_init_sta(void)
{
    memcpy(mac_addr, g_chip_wifi_mac, sizeof(mac_addr));

    //1. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);
    netdev_get_ip_cb_set(netdev_get_ip_callback);
#if LWIP_IPV6
    struct netif * netif1 = netdev_get_netif(NETIF_IDX_STA);
    netif_create_ip6_linklocal_address(netif1, 1);
    LOG(LOG_LVL_INFO, "IPv6 link-local address: %s\r\n", ipaddr_ntoa(netif_ip_addr6(netif1, 0)));
    netif_set_ip6_autoconfig_enabled(netif1, 1);
#endif

    //2. wifi start
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);

    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, WIFI_NO_POWERSAVE)){
        LOG(LOG_LVL_ERROR, "[%s]wifi sta start filed!!!\r\n", __func__);
    }

    connect.psk_value = NULL;
    if (strlen(connect.pwd) != 0) {
        if (0 == ln_psk_calc(connect.ssid, connect.pwd, psk_value, sizeof (psk_value))) {
            connect.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    wifi_sta_connect(&connect, &scan_cfg);
}

static void ap_startup_cb(void * arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

static void wifi_init_ap(void)
{
    tcpip_ip_info_t  ip_info;
    server_config_t  server_config;

    ip_info.ip.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.gw.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.netmask.addr = ipaddr_addr((const char *)"255.255.255.0");

    server_config.server.addr   = ip_info.ip.addr;
    server_config.port          = 67;
    server_config.lease         = 2880;
    server_config.renew         = 2880;
    server_config.ip_start.addr = ipaddr_addr((const char *)"192.168.4.100");
    server_config.ip_end.addr   = ipaddr_addr((const char *)"192.168.4.150");
    server_config.client_max    = 3;
    dhcpd_curr_config_set(&server_config);

    memcpy(mac_addr, g_chip_wifi_mac, sizeof(mac_addr));
    mac_addr[5] += 1;

    //1. net device(lwip).
    netdev_set_mac_addr(NETIF_IDX_AP, mac_addr);
    netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
    netdev_set_active(NETIF_IDX_AP);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ap_startup_cb);

    ap_cfg.ext_cfg.psk_value = NULL;
    if ((strlen(ap_cfg.pwd) != 0) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP)) {
        memset(psk_value, 0, sizeof(psk_value));
        if (0 == ln_psk_calc(ap_cfg.ssid, ap_cfg.pwd, psk_value, sizeof (psk_value))) {
            ap_cfg.ext_cfg.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    //2. wifi
    if(WIFI_ERR_NONE !=  wifi_softap_start(&ap_cfg)){
        LOG(LOG_LVL_ERROR, "[%s, %d]wifi_start() fail.\r\n", __func__, __LINE__);
    }
}

uint8_t rlLampGetOnoff(void) 
{
    return s_stCurLampParam.ucSwitch;
}
//回调函数
static void _normalKeyShortPressCb(uint32_t keyVal, uint32_t flag)
{
    switch (keyVal) {
    case LIGHT_KEY_VAL_SWITCH:
       // my_hal_log_debug("key switch sp %d\r\n", flag);
        if (g_stRlData.fctData.fctMode != 0) {
          //  Printf("press key switch\r\n");
        }
        break;
    default:
        break;
    }
}
static void _normalKeyShortReleaseCb(uint32_t keyVal, uint32_t flag)
{
    stRlLiveData_t *pLiveData = &g_stRlData.liveData;
     
    switch (keyVal) {
    case LIGHT_KEY_VAL_SWITCH:
     //   printf("key switch sr %d\r\n", flag);
        if (g_stRlData.fctData.fctMode != 0) {
          //  printf("release key switch\r\n");
        }
        rlLampSwitchRevert(pLiveData->uwAdjDuration);
        break;
    default:
        break;
    }
}
static void _normalKeyLongPressCb(uint32_t keyVal, uint32_t flag)
{
    switch (keyVal) {
    case LIGHT_KEY_VAL_SWITCH:
   //     printf("key switch lp %d tick %lu\r\n", flag, xTaskGetTickCount());
        #if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT  || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX)
        if (flag == 12) {
           rlLampBlinkCtrl(3, 1000, 1, RL_LAMP_BLINK_ARG_SYS_FACTORY_RESET); 
        }
        #else
        if (0 != rlLampGetOnoff()) {
            if (flag == 0) {
                rlFlagRevert(RL_FLAG_LAMP_BRI_DIRECTION);
            }
            rlLampBriCtrlNextClass(200);
        }
        else if (flag == 25 && rlFlagGet(RL_FLAG_SYS_FACTORY_WINDOW)){
            rlFlagSet(RL_FLAG_SYS_FACTORY_RESET, 1);
            //my_hal_log_info("manual factory reset\r\n");
        }
        #endif
        break;
    default:
        break;
    }
}
static void _normalKeyLongReleaseCb(uint32_t keyVal, uint32_t flag)
{
    stRlLiveData_t *pLiveData = &g_stRlData.liveData;

    switch (keyVal) {
    case LIGHT_KEY_VAL_SWITCH:
   //     my_hal_log_debug("key switch lr %d\r\n" );
        if (g_stRlData.fctData.fctMode != 0) {
      //      Printf("release key switch\r\n");
        }
        if (0 == rlLampGetOnoff() && flag < 25) {
            rlLampSwitchRevert(pLiveData->uwAdjDuration);
        }
        break;
    default:
        break;
    }
}
static uint32_t _normalKeyboardKeyStatusGet(uint32_t keyValue)
{
    hal_io_status_t status = HAL_IO_LOW;
    for (int i = 0; i < sizeof(s_a_normalKeyHalIo) / sizeof(gpio_pin_t); i++) {
        if (keyValue == s_a_normalKeyVal[i]) {
            myhal_gpiob_get_status(s_a_normalKeyHalIo[i], &status);
            break;
        }
    }

    return (uint32_t)!status;
}
static int _rlTaskKeyInit(void) 
{
    hal_io_status_t status = HAL_IO_LOW;
myhal_gpiob_init(GPIO_PIN_3,HAL_IO_MODE_IN_PULLUP);

myKeyboardInit(10, 3, 1000, 200, 100, _rlKeyGetTickMs, _normalKeyboardKeyStatusGet);
myKeyboardRigisterCallback(_normalKeyShortPressCb,
                                _normalKeyShortReleaseCb,
                                _normalKeyLongPressCb,
                                _normalKeyLongReleaseCb);
    for (int i = 0; i < sizeof(s_a_normalKeyVal) / sizeof(uint32_t); i++) {
        myKeyboardRegisterKey(s_a_normalKeyVal[i]);
    }
    return 0;
}
static int GetStaInfo()
{
    if (sysparam_sta_conn_cfg_get(&connect) != SYSPARAM_ERR_NONE) {
    //    printf("get conn cfg fail\r\n");
        return -1;
    }

    if (strlen(connect.ssid) == 0 || strlen(connect.pwd) == 0) {
 //       printf("no conn info\r\n");
        return -1;
    }
    return 0;
}

static void WifiConn()
{
    //1. sta mac get
     if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(mac_addr)) {
        LOG(LOG_LVL_ERROR, "sta mac get failed!!!\r\n");
        return;
    }

    //1. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);
    sysparam_sta_mac_update((const uint8_t *)mac_addr);

    //2. wifi start
    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, WIFI_NO_POWERSAVE)){
        LOG(LOG_LVL_ERROR, "[%s]wifi sta start filed!!!\r\n", __func__);
    }
    if (strlen(connect.pwd) != 0) {
        if (0 == ln_psk_calc(connect.ssid, connect.pwd, connect.psk_value, sizeof (psk_value))) {
            // hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    wifi_sta_connect(&connect, &scan_cfg);
    return;
}

static uint16_t _lampSightBri2PwmBri(uint16_t sightBri)
{
    float pwmBri = 0.0f;
    float L = (sightBri-LIGHT_BRIGHT_MIN) * 100.0f / (LIGHT_BRIGHT_MAX-LIGHT_BRIGHT_MIN);

    if (L <= 7.999624) {
        pwmBri = L / 903.3;
    } else {
        pwmBri = (L+16)/116;
        pwmBri = pwmBri * pwmBri * pwmBri;
    }

    pwmBri = pwmBri > 1.0f ? 1.0f : pwmBri;

    return (uint16_t)(LIGHT_BRIGHT_MIN + pwmBri * (LIGHT_BRIGHT_MAX-LIGHT_BRIGHT_MIN) + 0.5f);
}
static int8_t _lampColor2Pwm(uint32_t ulPwm[4], myLampParam_t *pLampParam)
{ 
    float afPWM[3] = {1.0f, 1.0f, 1.0f};
    uint32_t tmpPwm = 0;
    uint16_t pwmBri = _lampSightBri2PwmBri(pLampParam->uwBri);

#if LIGHT_CCT_ENABLE
    uint16_t halfCCT = (LIGHT_COLOR_CCT_MAX-LIGHT_COLOR_CCT_MIN) / 2;
    uint16_t cctPoint2 = LIGHT_COLOR_CCT_MIN + (uint16_t)(halfCCT / LIGHT_PWM_OUTPUT_MAX_POWER_RATIO);
    uint16_t cctPoint1 = LIGHT_COLOR_CCT_MIN + (LIGHT_COLOR_CCT_MAX-cctPoint2); 
    uint16_t uwCCT = APP_RANGE(pLampParam->uwCCT, LIGHT_COLOR_CCT_MIN, LIGHT_COLOR_CCT_MAX);
    
    if (uwCCT <= cctPoint1) {
        afPWM[0] = (float)1.0f;
        afPWM[1] = (float)(uwCCT - LIGHT_COLOR_CCT_MIN) / (cctPoint2 - LIGHT_COLOR_CCT_MIN);
     } else if (uwCCT < cctPoint2) {
        afPWM[0] = (float)(LIGHT_COLOR_CCT_MAX - uwCCT) / (cctPoint2 - LIGHT_COLOR_CCT_MIN);
        afPWM[1] = (float)(uwCCT - LIGHT_COLOR_CCT_MIN) / (cctPoint2 - LIGHT_COLOR_CCT_MIN);
     } else {
        afPWM[0] = (float)(LIGHT_COLOR_CCT_MAX - uwCCT) / (cctPoint2 - LIGHT_COLOR_CCT_MIN);
        afPWM[1] = (float)1.0f;
     }
#endif //LIGHT_CCT_ENABLE

#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT || APP_DEV_TYPE_USED  == APP_DEV_TYPE_LAMP_NIGHT_PTJX)
    ulPwm[0] = (uwCCT == LIGHT_COLOR_CCT_MIN) ? LIGHT_PWM_MAX : ((uwCCT == LIGHT_COLOR_CCT_MAX) ? LIGHT_PWM_MIN : LIGHT_PWM_MAX);
    ulPwm[1] = (uwCCT == LIGHT_COLOR_CCT_MIN) ? LIGHT_PWM_MIN : ((uwCCT == LIGHT_COLOR_CCT_MAX) ? LIGHT_PWM_MAX : LIGHT_PWM_MAX);
#else
     tmpPwm = LIGHT_PWM_MIN + 
        (uint32_t)((LIGHT_PWM_MAX-LIGHT_PWM_MIN) * pwmBri * afPWM[0] / LIGHT_BRIGHT_MAX);
    ulPwm[0] = APP_RANGE(tmpPwm, LIGHT_PWM_MIN, LIGHT_PWM_MAX);
    tmpPwm = LIGHT_PWM_MIN + 
        (uint32_t)((LIGHT_PWM_MAX-LIGHT_PWM_MIN) * pwmBri * afPWM[1] / LIGHT_BRIGHT_MAX);
    ulPwm[1] = APP_RANGE(tmpPwm, LIGHT_PWM_MIN, LIGHT_PWM_MAX);
#endif    
    ulPwm[2] = 0;
    ulPwm[3] = 0;
    //my_hal_log_debug("bri %d pwmbri %d cct %d to pwm %d %d\r\n", pLampParam->uwBri, pwmBri, uwCCT, ulPwm[0], ulPwm[1]);

    return 0;
}
static void _lampPwmOutput(uint32_t ulPwm1, uint32_t ulPwm2, uint32_t ulPwm3, uint32_t ulPwm4)
{ 
    myLampParam_t lampParam;
    static uint32_t lastPwm1 = 0;
    static uint32_t lastPwm2 = 0;

    myLampStatusGet(gucLampId, &lampParam);
    ulPwm1 = APP_RANGE(ulPwm1, LIGHT_PWM_MIN, LIGHT_PWM_MAX);
    ulPwm2 = APP_RANGE(ulPwm2, LIGHT_PWM_MIN, LIGHT_PWM_MAX);

    if (lampParam.ucSwitch == 0) {
#ifdef LIGHT_PWM_OFF_LEVEL
        
        if ((ulPwm1 + ulPwm2) < LIGHT_PWM_OFF_LEVEL) {
#else
        if ((ulPwm1 + ulPwm2) <= LIGHT_PWM_MIN*2) {
#endif
            ulPwm1 = ulPwm2 = 0;
        }
    }

    if (ulPwm1 != lastPwm1) {
        lastPwm1 = ulPwm1;
        pwm_set_duty(PWM_CHA_1, ulPwm1);
        LOG(LOG_LVL_INFO, "pwm1 out %d\r\n", ulPwm1);
    }
    if (ulPwm2 != lastPwm2) {
        lastPwm2 = ulPwm2;
//      my_hal_log_debug("pwm2 out %d %d\r\n", ulPwm1, ulPwm2);
LOG(LOG_LVL_INFO, "pwm2 out %d\r\n", ulPwm2);
       // myHalPwmOutput(RL_PWM_C, ulPwm2);
        pwm_set_duty(PWM_CHA_1, ulPwm2);
    }
}

static void WifiReconn()
{
    /* 获取wifi信息 */
    if (GetStaInfo() != 0) {
    //    printf("no sta info\r\n");
        return;
    }

    WifiConn();
    return;
}

static void usr_app_task_entry(void *params)
{
    LN_UNUSED(params);

    wifi_manager_init();

    chip_mac_gen();
    sysparam_sta_mac_update((const uint8_t *)g_chip_wifi_mac);
    sysparam_softap_mac_update((const uint8_t *)g_chip_wifi_mac);
    ln_kv_ble_app_init();

    WifiReconn();
    MagicLinkSDKRun();
    OS_ThreadDelete(NULL);
}
/******************************************************************************
 Function    : rlTaskLampInit
 Description : reading lamp thread lamp init
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static int rlTaskLampInit(void* arg)
{    LOG(LOG_LVL_INFO,"LAMP INIT OK\n ");
    myLampParam_t *pLampParam = &s_stCurLampParam;

    pLampParam->minPwm = LIGHT_PWM_MIN;
    pLampParam->maxPwm = LIGHT_PWM_MAX;
    pLampParam->ucCtrlType = 0;
    pLampParam->ucSwitch = 0;
    pLampParam->uwBri = 65535;
    pLampParam->uwCCT = LIGHT_CCT_DEFAULT;
    pLampParam->ucSceneNo = g_stRlData.saveData.stLampSaveParam.ucSceneNo;
    pwm_init(10000,20,PWM_CHA_1,GPIO_B,GPIO_PIN_6);     //初始化PWM
    pwm_init(10000,20,PWM_CHA_2,GPIO_B,GPIO_PIN_7);     //初始化PWM
    pwm_start(PWM_CHA_1);
    pwm_start(PWM_CHA_2);  
    myLampInit(10, NULL, NULL, NULL);
    myLampCreateLamp(&gucLampId, 
                     pLampParam, 
                     _lampPwmOutput, 
                     _lampColor2Pwm, 
                     getMyDimmingCurve(LIGHT_PWM_CURVE), 
                     NULL,NULL);
  //  //my_hal_log_info("create lamp switch %d max pwm is %d\r\n", 
               //  pLampParam->ucSwitch, LIGHT_PWM_MAX);
  //  myLampRegisterDimmingStatusCtrlHook(gucLampId, _lampStateControlHook);
  //  myLampRegisterFlashEndHook(gucLampId, _lampBlinkEndHook);
    LOG(LOG_LVL_INFO,"LAMP INIT OK ");
    #if (APP_DEV_TYPE_USED != APP_DEV_TYPE_LAMP_NIGHT && APP_DEV_TYPE_USED  != APP_DEV_TYPE_LAMP_NIGHT_PTJX)
 //   s_resetWindowTimerHandle = xTimerCreate((const char*)"reset", (30000 / portTICK_RATE_MS), 0, NULL, _lampFactoryResetWindowTimeoutHandle);
  //  xTimerStart(s_resetWindowTimerHandle, 0);
    #else
    #endif
    return 0;
}
static void usr_app_light_task_entry(void *params)
{
       LOG(LOG_LVL_INFO,"LAMP INIT 445 ");
 // rlFlagSet(RL_FLAG_TASK_LAMP_RUNNING, 1);     
    if(0 > rlTaskLampInit(params)) {
        rlFlagSet(RL_FLAG_TASK_LAMP_RUNNING, 0);
    }
    while (rlFlagGet(RL_FLAG_TASK_LAMP_RUNNING)) {      
        vTaskDelay(10);
        myLampLoop();
    vTaskDelete(NULL);
    }
}
static void key_app_task_entry(void *params)
{

  //rlFlagSet(RL_FLAG_TASK_KEY_RUNNING, 1);
    if(0 > _rlTaskKeyInit()) {
       // rlFlagSet(RL_FLAG_TASK_KEY_RUNNING, 0);
    }

    while(1) {
        vTaskDelay(10);
        myKeyboardLoop();
    }
    vTaskDelete(NULL);
}
static void temp_cal_app_task_entry(void *params)
{
    LN_UNUSED(params);

    int8_t cap_comp = 0;
    uint16_t adc_val = 0;
    int16_t curr_adc = 0;
    uint8_t cnt = 0;

    if (NVDS_ERR_OK == ln_nvds_get_xtal_comp_val((uint8_t *)&cap_comp)) {
        if ((uint8_t)cap_comp == 0xFF) {
            cap_comp = 0;
        }
    }

    drv_adc_init();

    wifi_temp_cal_init(drv_adc_read(ADC_CH0), cap_comp);

    while (1)
    {
        OS_MsDelay(1000);

        adc_val = drv_adc_read(ADC_CH0);
        wifi_do_temp_cal_period(adc_val);

        curr_adc = (adc_val & 0xFFF);

        cnt++;
        if ((cnt % 60) == 0) {
            LOG(LOG_LVL_INFO, "adc raw: %4d, temp_IC: %4d\r\n", curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f));
            LOG(LOG_LVL_INFO, "Total:%d; Free:%ld;\r\n", OS_HeapSizeGet(), OS_GetFreeHeapSize());
        }
    }
}

void creat_usr_app_task(void)
{
    {
        ln_pm_sleep_mode_set(ACTIVE);
        ln_pm_always_clk_disable_select(CLK_G_I2S | CLK_G_WS2811 | CLK_G_SDIO | CLK_G_AES);
        ln_pm_lightsleep_clk_disable_select(CLK_G_SPI0 | CLK_G_SPI1 | CLK_G_I2C0 | CLK_G_TRNG);
    }

    if(OS_OK != OS_ThreadCreate(&g_usr_app_thread, "WifiUsrAPP", usr_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, USR_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }

    // ble_creat_usr_app_task();

#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_cal_thread, "TempAPP", temp_cal_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif

#if LAMP_TASK_EN && LAMP_TASK_EN==1
if(OS_OK!= OS_ThreadCreate(&g_lamp_thread,"LampApp",usr_app_light_task_entry,NULL,OS_PRIORITY_BELOW_NORMAL,LAMP_TASK_STACK_SIZE));
   LOG(LOG_LVL_INFO,"LAMP INIT TEST ");
#endif

#if KEY_TASK_EN && KEY_TASK_EN==1
    if(OS_OK != OS_ThreadCreate(&g_key_thread, "KeyApp", key_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, KEY_TASK_STACK_SIZE)) {
    }
#endif


    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }
}
