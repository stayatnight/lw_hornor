#ifndef __MAGICLINK_SDK_H__
#define __MAGICLINK_SDK_H__

#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "c" {
#endif

enum MagicLinkAttr {
    /* 配网参数 */
    MAGICLINK_ATTR_NETCFG_TIMEOUT = 0, /* 整数，单位为min，0表示配网永远不超时自动退出 */

    /* 传输参数 */
    MAGICLINK_ATTR_MQTT_BUF_SIZE = 100, /* 整数，为发送报文长度，如不设置默认为MAGICLINK_ATTR_MQTT_BUF_SIZE(2048), 可设置最大长度为32Kb */
    MAGICLINK_ATTR_CLOUD_HEARTBEAT_INTERVAL = 101, /* 整数，单位为秒，设备和设备云的心跳间隔，最小值：60s，最大值：300s，默认值：60s */

    /* OTA适配使用 */
    MAGICLINK_ATTR_OTA_REBOOT_METHOD = 200, /* 整数，0表示软重启，1表示硬重启 */
    MAGICLINK_ATTR_OTA_FS_PKG_PATH,
    MAGICLINK_ATTR_OTA_FS_INS_SCRIPT_PATH,
    MAGICLINK_ATTR_OTA_FLASH_MASTER_PKG_ADDR, /* 整数，固件主分区flash起始地址，必须为4K对齐 */
    MAGICLINK_ATTR_OTA_FLASH_SLAVE_PKG_ADDR, /* 整数，固件备份分区flash起始地址，必须为4K对齐 */
    MAGICLINK_ATTR_OTA_FLASH_MASTER_PKG_SIZE, /* 整数，OTA固件主分区flash存储空间大小，单位为字节，必须为4K的整数倍，建议该值设置要比实际固件大，考虑升级可扩展性 */
    MAGICLINK_ATTR_OTA_FLASH_SLAVE_PKG_SIZE, /* 整数，OTA固件备份分区flash存储空间大小，单位为字节，必须为4K的整数倍，建议该值设置要比实际固件大，考虑升级可扩展性 */
    MAGICLINK_ATTR_OTA_MCU_TRANS_TIMEOUT, /* 整数，模组给MCU发送MCU升级数据包的超时时间，单位为秒，默认值为30，建议实测发送需要的时间 */
    MAGICLINK_ATTR_OTA_MCU_INSTALL_TIMEOUT, /* 整数，MCU安装重启超时时长，单位为秒，默认值为30 */
    MAGICLINK_ATTR_OTA_DOWNLOAD_SINGLE_PKG_LEN, /* 整数，OTA分包下载单包长度，仅Linux系统设备使用，单位为KB，取值范围为4到512，默认值为64 */
    MAGICLINK_ATTR_OTA_MODULE_INSTALL_TIMEOUT, /* 整数，MODULE安装重启超时时长，单位为秒，默认值为30，取值范围为30到300 */

    /* 配置保存适配使用 */
    MAGICLINK_ATTR_CONFIG_PATH = 300,
    MAGICLINK_ATTR_CONFIG_BAK_PATH,
    MAGICLINK_ATTR_CONFIG_FLASH_BASE_ADDR, /* flash使用，48KB存储SDK数据起始地址 */
    MAGICLINK_ATTR_READONLY_CONFIG_PATH, /* 只读配置文件的存储地址，需设置为只读分区的路径 */

    /* unsigned int类型，接收到setProperty请求后自动上报的模式；0：上报setProperty下发属性所属服务下的所有属性状态；1：上报setProperty下发属性的状态；默认值：0 */
    MAGICLINK_ATTR_AUTO_REPORT_MODE = 400,

    /* 本地化信息，需要在设备已经完成云端注册时才能获取。 */
    MAGICLINK_ATTR_COUNTRYCODE = 450, /* char *类型，大小不超过4字节，参照ISO 3166-1 alpha-2 国家码，如中国国家码为"CN" */
    MAGICLINK_ATTR_LANGUAGECODE, /* char *类型，大小不超过16字节，参照ISO 639-1 语言码，如汉语的语言码是"zh" */
    MAGICLINK_ATTR_TIMEZONE, /* int 类型，参照UTC时间时区划分，如北京的时区是8 */

    MAGICLINK_ATTR_AUTO_REINIT = 500, /* unsigned int类型，执行reset时是否执行init：0：不执行init，1：执行init；默认值：1 */

    /* DFX日志收集使用 */
    MAGICLINK_ATTR_LOG_OUTPUT_PATH = 600, /* 字符类型，SDK日志落盘路径和日志打包文件保存路径。如果需要收集开发者的日志，开发者需要在这个路径中同时预留足够空间。 */
    MAGICLINK_ATTR_DFX_LOG_FILE_SIZE, /* 整数，SDK单个日志文件大小，单位为M。取值范围为1M到8M，默认值为1M */
    MAGICLINK_ATTR_DFX_LOG_FILE_NUM, /* 整数，SDK日志文件最大数量，取值范围为1-50，默认值为10 */
    MAGICLINK_ATTR_DFX_EXT_LOG_PKG_TIME, /* 整数，单位为秒，默认时间为30s，厂商日志收集的最长时间。
    SDK通知厂商进行日志收集后等待日志收集完成通知，如果等待超时，SDK不收集厂商日志，仅收集SDK日志 */

    /* 使用代理方式注册到设备云 */
    MAGICLINK_ATTR_CLOUD_PROXY = 700,

    MAGICLINK_ATTR_BUTT,
};

struct MagicLinkDataVal {
    unsigned int dataIdx;
    unsigned int dataLen;
    unsigned char *data;
};

int MagicLinkSetAttr(enum MagicLinkAttr attr, const void *value, unsigned int valueLen);

int MagicLinkGetAttr(enum MagicLinkAttr attr, void *buff, unsigned int buffLen, unsigned int *outLen);

/* 设备控制回调 */
struct MagicLinkCtrlFunc {
    /* 查询设备属性 */
    int (*getServiceProperty)(const char *service, const char *property,
        void **outBuf, unsigned int *outLen);

    /* 设置设备属性，返回-100表示正在执行中，执行完成后通过report上报状态信息 */
    int (*setServiceProperty)(const char *service, const char *property,
        const void *inBuf, unsigned int inLen);

    /* 执行动作，MagicLink等待动作执行结果 */
    int (*execAction)(const char *service, const char *action, unsigned int dataNum,
        const struct MagicLinkDataVal *dataArray);
};

/* 初始化 */
int MagicLinkInit(const char *devModelJson, const struct MagicLinkCtrlFunc *ctrlFunc);

/* 终止 */
int MagicLinkDeInit(void);

/* 恢复出厂 */
int MagicLinkReset(void);

/* 上报服务状态 */
int MagicLinkReportServiceStatus(const char *service);

/* 上报属性状态 */
int MagicLinkReportPropertyStatus(const char *service, unsigned int propNum, const unsigned int *propIndexArray);

/* 上报服务事件 */
int MagicLinkReportServiceEvent(const char *service, const char *eventId, unsigned int dataNum,
    const struct MagicLinkDataVal *dataArray);

/* MagicLink SDK状态 */
enum MagicLinkSDKStatus {
    /* 配网相关 */
    STATUS_NETCFG_ENTER = 100, /* 进入配网模式 */
    STATUS_NETCFG_RECV_INFO, /* 收到配网信息 */
    STATUS_NETCFG_TIMEOUT, /* 配网超时 */
    STATUS_NETCFG_STOP, /* 停止配网 */

    /* 连接WiFi相关 */
    STATUS_NET_CONNECTING = 200, /* 连接WiFi中 */
    STATUS_NET_CONNECT_SUCC, /* 连接WiFi成功 */
    STATUS_NET_CONNECT_FAIL, /* 连接WiFi失败 */

    /* 设备注册相关 */
    STATUS_REGISTER_RECV_INFO = 300, /* 收到注册信息 */
    STATUS_REGISTING, /* 设备注册中 */
    STATUS_REGISTER_SUCC, /* 设备注册成功 */
    STATUS_REGISTER_FAIL, /* 设备注册失败 */
    STATUS_EXT_REGISTER_SUCC, /* 组件注册成功 */
    STATUS_EXT_REGISTER_FAIL, /* 组件注册失败 */

    /* 设备注册相关，登录信息过期，刷新登录信息 */
    STATUS_SECRET_TIMEOUT = 400, /* 设备登录密钥过期 */
    STATUS_SECRET_REFRESHING, /* 设备登录密钥刷新中 */
    STATUS_SECRET_REFRESH_FAIL, /* 设备登录密钥刷新失败 */
    STATUS_SECRET_REFRESH_SUCC, /* 设备登录密钥刷新成功 */

    /* 登录设备云相关 */
    STATUS_LOGIN_ONLINE = 500, /* 设备登录设备云成功 */
    STATUS_LOGIN_OFFLINE, /* 设备登录设备云失败 */
    STATUS_LOGOUT, /* 设备登出设备云 */

    /* 设备删除相关 */
    STATUS_DEVICE_NOT_IN_CLOUD = 600, /* 设备云端注册数据被删除 */
    STATUS_DEVICE_DELETE, /* 设备删除 */
    STATUS_DEVICE_RESET, /* 设备恢复出厂 */

    /* 设备升级相关 */
    STATUS_OTA_CHKING_VERSION = 700, /* 固件检测版本中 */
    STATUS_OTA_CHK_VERSION_DONE, /* 固件检测版本完成 */
    STATUS_OTA_UPGRADING, /* 固件升级中 */
    STATUS_OTA_DOWNLOAD_COMPLETE, /* 固件下载升级包成功 */
    STATUS_OTA_DOWNLOAD_TIMEOUT, /* 固件下载升级包超时 */
    STATUS_OTA_CHECK_SHA256_FAIL, /* 固件升级包校验失败 */
    STATUS_OTA_UPGRADE_SUCC, /* 固件升级成功 */
    STATUS_OTA_UPGRADE_FAIL, /* 固件升级失败 */
    STATUS_OTA_MCU_INSTALL_TIMEOUT, /* MCU安装超时 */

    /* 设备异常相关 */
    STATUS_SYSTEM_ABNORMAL = 1000, /* 系统异常，SDK恢复出厂 */

    STATUS_BUTT
};

/* 查询MagicLink SDK状态 */
enum MagicLinkSDKStatus MagicLinkGetStatus(void);

/* 注册MagicLink SDK状态回调 */
int MagicLinkRegRecvStatus(void (*recvStatus)(enum MagicLinkSDKStatus status));

/* 注册获取PIN码回调, 回调返回值为pin码的长度 */
int MagicLinkRegGetPin(int (*getPin)(char *pinBuf, unsigned int pinBufLen));

/* 注册获取license回调 */
int MagicLinkRegGetLicense(int (*getLicense)(char *keyBuf, unsigned int keyBufLen,
    char *secret, unsigned int secretBufLen));

/* 注册获取prdKey回调 */
int MagicLinkRegGetPrdKey(int (*getPrdKey)(char *keyBuf, unsigned int keyBufLen,
    char *secret, unsigned int secretBufLen));

/* 注册硬件根秘钥回调，硬件根秘钥长度为[32,64]，回调函数返回硬件根秘钥的长度 */
int MagicLinkRegHardwareRootKey(int (*getHardwareRootKey)(unsigned char *rootKey, unsigned int rootKeyBufLen));

/* 获取MagicLinkSDK版本号，建议入参的缓冲区长度为32字节 */
int MagicLinkGetSDKVersion(char *verBuf, unsigned int verBufLen);

/* 获取本地时间 */
int MagicLinkGetLocalTime(struct tm *time);

/* 获取UTC时间 */
int MagicLinkGetUTCTime(struct tm *time);

/* 设备是否已经注册到荣耀设备云 */
bool MagicLinkIsRegistered(void);

/* 获取设备devId，buf长度不小于48 */
int MagicLinkGetDeviceId(char *devIdBuf, unsigned int devIdBufLen);

/* 注册接收设备绑定时间回调 */
int MagicLinkRegRecvDevBindTime(int (*recvDevBindTime)(long long localTime));

/*
 获取本地化信息，包含国家码语言码和时区
 返回值：0表示注册成功，其他表示注册失败；
 参数：getLocale，本地话信息回调，country，国家码，如CN；lang，语言码，如zh；
 timeZone, UTC时间时区，如北京的时区是8；
 countryBufLen, 国家码buffer长度，最大长度5字节；langBufLen，语言码buffer长度，最大长度17字节；
 */
int MagicLinkRegGetLocale(int (*getLocale)(char *country, unsigned int countryBufLen,
    char *lang, unsigned int langBufLen, int *timeZone));

#ifdef __cplusplus
}
#endif

#endif
