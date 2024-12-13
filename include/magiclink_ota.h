#ifndef __MAGICLINK_SDK_OTA_H__
#define __MAGICLINK_SDK_OTA_H__

#ifdef __cplusplus
extern "c" {
#endif

typedef enum {
    OTA_PACKAGE_BUF_FIRST,
    OTA_PACKAGE_BUF_MIDDLE,
    OTA_PACKAGE_BUF_LAST,
    OTA_PACKAGE_BUF_BUTT
} OtaPackageBufType;

typedef struct MagicLinkOtaFunc {
    /* 存储版本包回调 */
    int (*writeOtaPkg)(const void *position, unsigned int offset, const void *data, unsigned int len,
        OtaPackageBufType type);
    /* 执行升级动作 */
    int (*execOta)(void);

    /* 获取版本包储存空间大小 */
    int (*getOtaPkgSaveSize)(unsigned int *size);
} MagicLinkOtaFunc;

/* 注册OTA升级回调 */
int MagicLinkRegOtaFunc(const MagicLinkOtaFunc *otaFunc);

typedef struct MagicLinkMcuOtaFunc {
    /* 开始升级 */
    int (*mcuOtaStart)(const char *version, const char *pkgName, unsigned int pkgSize,
        const unsigned char *sha256, unsigned int sha256Len);

    /* 发送版本包 */
    int (*mcuOtaSendPkg)(unsigned int offset, const unsigned char *pkgData, unsigned int pkgDataLen);

    /* 升级完成：版本包发送完成 */
    int (*mcuOtaEnd)(void);

    /* 通知MCU OTA过程异常 */
    void (*notifyMcuOtaException)(int status, const char *str);
} MagicLinkMcuOtaFunc;

/* 注册MCU升级回调 */
int MagicLinkRegMcuOtaFunc(const MagicLinkMcuOtaFunc *mcuRegFunc);

/* MCU通知SDK升级状态 */
int MagicLinkSetMcuOtaStatus(int status, const char *data);

#ifdef __cplusplus
}
#endif

#endif

