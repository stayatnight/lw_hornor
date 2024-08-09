#include "demo.h"
#include "demo_profile.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>

#include "magiclink.h"
#include "magiclink_netcfg.h"
#include "magiclink_log.h"

#define MY_PRDKEY "******5474b7459c9cc49eba30d93867"
#define MY_PRDSECRET "******561c5847fdb80a601004fd0016"

#define MY_LICENSE_KEY "******11a3f920e20b0e425d824d62bc0a16234e"
#define MY_LICENSE_SECRET "******23e8af0da99c406f62f4fb13b713a846a5cf5e6893098e2138d58aeb3e"

 /* ln882h固件分区分布，后续写入说明文档,并协调厂商给OTA升级留足空间
 * 分区     ||BOOT      ||PART_TAB  ||APP       ||OTA       ||CONF      ||USER      ||NVDS      ||KV       ||
 * 起始地址 ||0x00000000||0x00006000||0x00007000||0x00133000||0x001DD000||0x001E9000||0x001F9000||0x001FC000||
 * 分区大小 ||24KB      ||4KB       ||1200KB    ||680Kb     ||48 Kb     ||64Kb      ||12Kb      ||16Kb      ||
 */

#define CONFIG_FLASH_BASE_ADDR (0x001DD000) /* ln882hCONF分区存放SDK注册信息、秘钥等数据 */

static int GetDevSnFunc(void **data, unsigned int *len)
{
    char *tmp = "A2NM011309000573"; // SN需要和最终打印到设备上的SN保持一致，业界一般使用大写，建议使用大写
    unsigned int tmpLen = strlen(tmp) + 1; // 实际存储字符串的空间需要使用包含结束符的长度
    *len = tmpLen - 1; // 返回的长度为字符串实际的长度，不包含结束符

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetDevMacFunc(void **data, unsigned int *len)
{
    char *tmp = "test_my_mac";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetDevUdidFunc(void **data, unsigned int *len)
{
    /* 对于非android的设备，一般无UDID，该接口无需实现 */
    *len = 0;
    *data = NULL;

    return 0;
}

static char g_devName[64] = "test_my_DevName";
/* 设备名称的获取需从持久化保存单元获取， 全局变量g_devName仅作为示例 */
/* 设备重置（RESET）后，设备名的存储单元内容需更新为默认设备名称 */
static int GetDevNameFunc(void **data, unsigned int *len)
{
    unsigned int tmpLen = strlen(g_devName) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, g_devName);

    return 0;
}

/* 设备名称的获取需从持久化保存单元获取， 全局变量g_devName仅作为示例 */
/* 设备重置（RESET）后，设备名的存储单元内容需更新为默认设备名称 */
static int SetDevNameFunc(const void *data, unsigned int len)
{
    (void)memset(g_devName, 0, sizeof(g_devName));
    (void)memcpy(g_devName, data, len);

    printf("new devname[%s]\r\n", g_devName);
    return 0;
}

static int GetDevModelFunc(void **data, unsigned int *len)
{
    char *tmp = "test_DevModel";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

/* 获取设备子型号，可取值"0"~"35"，若无子型号，取默认值"0" */
static int GetDevSubDevTypeFunc(void **data, unsigned int *len)
{
    char *tmp = "0";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetDevFirmwareVersionFunc(void **data, unsigned int *len)
{
    char *tmp = "1.0.0.201";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetMCUVersionFunc(void **data, unsigned int *len)
{
    char *tmp = "";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetDevHardwareVersionVersionFunc(void **data, unsigned int *len)
{
    char *tmp = "HW_1.0.1";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

/* netInfo 设备网络信息相关回调 */
static int GetNetInfoSsidFunc(void **data, unsigned int *len)
{
    char *tmp = "testSsid123";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetNetInfoRssiFunc(void **data, unsigned int *len)
{
    int rssi = -36;
    *len = sizeof(rssi);

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &rssi, *len);

    return 0;
}

static int GetNetInfoIpFunc(void **data, unsigned int *len)
{
    char *tmp = "127.0.0.1";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static int GetNetInfoBssidFunc(void **data, unsigned int *len)
{
    char *tmp = "testBssid123";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len = tmpLen - 1;

    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);

    (void)strcpy(*data, tmp);

    return 0;
}

static struct MagicLinkTestLight {
    int on;
    char color[16];
    int lightMode;
    int volume;
    int brightness;
    int colorTemperature;
} g_light = {
    .on = 0,
    .color = "red",
    .lightMode = 1,
    .volume = 2,
    .brightness = 3,
    .colorTemperature = 4000
};

static int SetLightSwitchInt(const void *data, unsigned int len)
{
    g_light.on = *(int *)data;
    printf("set light switch succ\r\n");
    return 0;
}

static int GetLightSwitchInt(void **data, unsigned int *len)
{
    *len = sizeof(g_light.on);

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err \r\n");
        return -1;
    }

    (void)memset(*data, 0, *len);
    (void)memcpy(*data, &g_light.on, *len);

    printf("get light switch succ \r\n");
    return 0;
}

static int SetLightColorString(const void *data, unsigned int len)
{
    if (data == NULL || strlen(data) == 0 || len == 0) {
        printf("inv param\r\n");
        return -1;
    }

    char *color = (char *)data;
    (void)memset(g_light.color, 0, sizeof(g_light.color));

    (void)strcpy(g_light.color, color);

    printf("set light color\r\n");
    return 0;
}

static int GetLightColorString(void **data, unsigned int *len)
{
    unsigned int tmpLen = strlen(g_light.color) + 1;
    *len = tmpLen - 1;

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err \r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &g_light.color, tmpLen);

    printf("get light color\r\n");
    return 0;
}

static int SetLightModeInt(const void *data, unsigned int len)
{
    g_light.lightMode = *(int *)data;
    printf("set light mode\r\n");

    if (g_light.lightMode == 0) {
        g_light.brightness = 10;
        g_light.colorTemperature = 2000;
    }

    if (g_light.lightMode == 1) {
        g_light.brightness = 20;
        g_light.colorTemperature = 2500;
    }

    if (g_light.lightMode == 2) {
        g_light.brightness = 30;
        g_light.colorTemperature = 3000;
    }

    if (g_light.lightMode == 3) {
        g_light.brightness = 40;
        g_light.colorTemperature = 3500;
    }

    if (g_light.lightMode == 4) {
        g_light.brightness = 50;
        g_light.colorTemperature = 4000;
    }

    if (g_light.lightMode == 5) {
        g_light.brightness = 60;
        g_light.colorTemperature = 4500;
    }

    MagicLinkReportServiceStatus("brightness");
    MagicLinkReportServiceStatus("cct");
    return 0;
}

static int GetLightModeInt(void **data, unsigned int *len)
{
    *len = sizeof(g_light.lightMode);

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err \r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &g_light.lightMode, *len);

    printf("get light mode\r\n");
    return 0;
}

static int SetLightVolumeInt(const void *data, unsigned int len)
{
    g_light.volume = *(int *)data;
    printf("set light volume\r\n");
    return 0;
}

static int GetLightVolumeInt(void **data, unsigned int *len)
{
    *len = sizeof(g_light.volume);

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err \r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &g_light.volume, *len);

    printf("get light volume\r\n");
    return 0;
}

static int SetLightBrightnessInt(const void *data, unsigned int len)
{
    g_light.brightness = *(int *)data;
    printf("set light brightness\r\n");
    return 0;
}

static int GetLightBrightnessInt(void **data, unsigned int *len)
{
    *len = sizeof(g_light.brightness);

    *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err \r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &g_light.brightness, *len);

    printf("get light brightness\r\n");
    return 0;
}

static int SetLightCctInt(const void *data, unsigned int len)
{
    g_light.colorTemperature = *(int *)data;
    printf("set light cct\r\n");
    return 0;
}

static int GetLightCctInt(void **data, unsigned int *len)
{
    *len = sizeof(g_light.colorTemperature);

   *data = malloc(*len);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, *len);

    (void)memcpy(*data, &g_light.colorTemperature, *len);

    printf("get light cct\r\n");
    return 0;
}

struct TestControlFunc {
    const char *svcId;
    const char *propId;
    int (*setFunc)(const void *data, unsigned int len);
    int (*getFunc)(void **data, unsigned int *len);
};

static struct TestControlFunc g_testCtrlFunc[] = {
    { "deviceInfo", "sn", NULL, GetDevSnFunc },
    { "deviceInfo", "udid", NULL, GetDevUdidFunc },
    { "deviceInfo", "mac", NULL, GetDevMacFunc },
    { "deviceInfo", "devName", SetDevNameFunc, GetDevNameFunc },
    { "deviceInfo", "devModel", NULL, GetDevModelFunc },
    { "deviceInfo", "subDevType", NULL, GetDevSubDevTypeFunc },
    { "deviceInfo", "firmwareVersion", NULL, GetDevFirmwareVersionFunc },
    { "deviceInfo", "hardwareVersion", NULL, GetDevHardwareVersionVersionFunc },
    { "deviceInfo", "MCUVersion", NULL, GetMCUVersionFunc },
    { "netInfo", "ssid", NULL, GetNetInfoSsidFunc },
    { "netInfo", "rssi", NULL, GetNetInfoRssiFunc },
    { "netInfo", "ip", NULL, GetNetInfoIpFunc },
    { "netInfo", "bssid", NULL, GetNetInfoBssidFunc },
    { "dvService", "switch", SetLightSwitchInt, GetLightSwitchInt },
    { "dvService", "supportSinkSvc", SetLightSwitchInt, GetLightSwitchInt },
    { "dvService", "devSvcStatus", SetLightSwitchInt, GetLightSwitchInt },
    { "light", "switch", SetLightSwitchInt, GetLightSwitchInt },
    { "light", "color", SetLightColorString, GetLightColorString },
    { "lightMode", "mode", SetLightModeInt, GetLightModeInt },
    { "volume", "volume", SetLightVolumeInt, GetLightVolumeInt },
    { "switch", "on", SetLightSwitchInt, GetLightSwitchInt },
    { "brightness", "brightness", SetLightBrightnessInt, GetLightBrightnessInt },
    { "cct", "colorTemperature", SetLightCctInt, GetLightCctInt }
};

static int GetSvcProperty(const char *service, const char *property,
    void **outBuf, unsigned int *outLen)
{
    unsigned int num = sizeof(g_testCtrlFunc) / sizeof(struct TestControlFunc);
    for (unsigned int idx = 0; idx < num; idx++) {
        if (strcmp(service, g_testCtrlFunc[idx].svcId) != 0) {
            continue;
        }

        if (strcmp(property, g_testCtrlFunc[idx].propId) != 0) {
            continue;
        }

        if (g_testCtrlFunc[idx].getFunc == NULL) {
            printf("[%s|%s] no get func\r\n", service, property);
            return -1;
        }

        return g_testCtrlFunc[idx].getFunc(outBuf, outLen);
    }

    return -1;
}

static int SetSvcProperty(const char *service, const char *property,
    const void *inBuf, unsigned int inLen)
{
    unsigned int num = sizeof(g_testCtrlFunc) / sizeof(struct TestControlFunc);
    for (unsigned int idx = 0; idx < num; idx++) {
        if (strcmp(service, g_testCtrlFunc[idx].svcId) != 0) {
            continue;
        }

        if (strcmp(property, g_testCtrlFunc[idx].propId) != 0) {
            continue;
        }

        if (g_testCtrlFunc[idx].setFunc == NULL) {
           printf("[%s|%s] no set func\r\n", service, property);
            return -1;
        }

        return g_testCtrlFunc[idx].setFunc(inBuf, inLen);
    }

    return -1;
}

static int ExectionTestAction1(unsigned int dataNum, const struct MagicLinkDataVal *dataArray)
{
    if (dataNum == 0 || dataArray == NULL) {
        printf("no data\r\n");
        return 0;
    }
    for (int i = 0;i < dataNum; i++) {
        printf("%d, %s, %d", dataArray[0].dataIdx,
            (char *)dataArray[i].data, *(int *)dataArray[i].data);
    }
    return 0;
}

struct TestActionFunc {
    const char *svcId;
    const char *actionId;
    int (*action)(unsigned int dataNum, const struct MagicLinkDataVal *dataArray);
};

static struct TestActionFunc g_testActionFunc[] = {
    { "testAction", "testAction1", ExectionTestAction1 }
};

static int Action(const char *service, const char *action,
    unsigned int dataNum, const struct MagicLinkDataVal *dataArray)
{
    printf("service[%s], action[%s]\r\n", service, action);
    unsigned int num = sizeof(g_testActionFunc) / sizeof(struct TestActionFunc);
    for (unsigned int idx = 0; idx < num; idx++) {
        if (strcmp(service, g_testActionFunc[idx].svcId) != 0) {
            continue;
        }

        if (strcmp(action, g_testActionFunc[idx].actionId) != 0) {
            continue;
        }

        if (g_testActionFunc[idx].action == NULL) {
            printf("no exec func\r\n");
            return -1;
        }

        return g_testActionFunc[idx].action(dataNum, dataArray);
    }

    printf("no action[%s][%s]\r\n", service, action);
    return -1;
}

static struct MagicLinkCtrlFunc ctrlFunc = {
    .getServiceProperty = GetSvcProperty,
    .setServiceProperty = SetSvcProperty,
    .execAction = Action
};

static void MyRecvStatus(enum MagicLinkSDKStatus status)
{
    printf("current status is %d\r\n", status);
    switch (status) {
        case STATUS_NETCFG_ENTER:
            printf("STATUS_NETCFG_ENTER\r\n");
            break;
        case STATUS_REGISTER_RECV_INFO:
            printf("STATUS_REGISTER_RECV_INFO\r\n");
            break;
        case STATUS_LOGIN_ONLINE: {
            struct MagicLinkDataVal eventData = { 0, strlen("test event msg"), (unsigned char *)"test event msg" };

            if (MagicLinkReportServiceEvent("event", "testEventMsg", 1, &eventData) != 0) {
                printf("rpt event msg fail\r\n");
                return;
            }
            printf("rpt event msg\r\n");
            break;
        }

        default:
            break;
    }
    return;
}

static int MyGetPrdKey(char *key, unsigned int keyBufLen, char *secret, unsigned int secretBufLen)
{
    (void)memcpy(key, MY_PRDKEY, strlen(MY_PRDKEY));

    (void)memcpy(secret, MY_PRDSECRET, strlen(MY_PRDSECRET));

    return 0;
}

static int MyGetLicense(char *key, unsigned int keyBufLen, char *secret, unsigned int secretBufLen)
{
    (void)memcpy(key, MY_LICENSE_KEY, strlen(MY_LICENSE_KEY));

    (void)memcpy(secret, MY_LICENSE_SECRET, strlen(MY_LICENSE_SECRET));

    return 0;
}

static int MyGetHardwareRootKey(unsigned char *rootKeyBuf, unsigned int rootKeyBufLen)
{
    unsigned char rootKey[] = {
        0xb0, 0x39, 0x8f, 0xc4, 0x03, 0x96, 0x17, 0xf4, 0x47, 0x56, 0x34, 0xc7, 0xe7, 0xa4, 0xc7, 0x5c,
        0x61, 0x2f, 0x24, 0xb5, 0xeb, 0x5c, 0xb6, 0x26, 0x68, 0xf8, 0xb4, 0x23, 0x0d, 0x38, 0x17, 0x7e,
    };

    (void)memcpy(rootKeyBuf, rootKey, sizeof(rootKey));

    return sizeof(rootKey);
}

static int GetPin(char *pinBuf, unsigned int pinBufLen)
{
    char *pin ="123456";

    (void)memset(pinBuf, 0, pinBufLen);
    (void)memcpy(pinBuf, pin, strlen(pin));

    printf("GEN PIN(%s)\r\n", pinBuf);
    return strlen(pinBuf);
}

void MagicLinkSDKRun()
{
    printf("MEM enter MagicLinkSDKRun [%d]\r\n", xPortGetFreeHeapSize());

    unsigned int timeout = 10;
    if (MagicLinkSetAttr(MAGICLINK_ATTR_NETCFG_TIMEOUT, &timeout, sizeof(timeout)) != 0) {
        return;
    }

    unsigned int cfgBaseAddr = CONFIG_FLASH_BASE_ADDR;
    if (MagicLinkSetAttr(MAGICLINK_ATTR_CONFIG_FLASH_BASE_ADDR, &cfgBaseAddr, sizeof(cfgBaseAddr)) != 0) {
        printf("set cfg addr fail\r\n");
        return;
    }

    if (MagicLinkRegGetPin(&GetPin) != 0) {
        printf("reg pin fail\r\n");
        return;
    }

    if (MagicLinkRegHardwareRootKey(MyGetHardwareRootKey) != 0) {
        printf("register MyGetHardwareRootKey fail\r\n");
        return;
    }

    if (MagicLinkInit(g_profile, &ctrlFunc) != 0) {
        printf("MagicLinkInit fail\r\n");
        return;
    }

    if (MagicLinkRegRecvStatus(MyRecvStatus) != 0) {
        return;
    }

    if (MagicLinkRegGetPrdKey(MyGetPrdKey) != 0) {
        return;
    }

    if (MagicLinkRegGetLicense(MyGetLicense) != 0) {
        return;
    }

    /* 设备未注册到设备云，启动配网 */
    if (MagicLinkIsRegistered() == false) {
        if (MagicLinkStartNetCfg() != 0) {
            return;
        }
    }

    printf("MEM Start demo [%d]\r\n", xPortGetFreeHeapSize());
    return;
}