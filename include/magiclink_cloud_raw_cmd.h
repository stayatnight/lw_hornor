/**
 * Copyright (c) Honor Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: Magic Link header file.
 */

#ifndef __MAGICLINK_CLOUD_RAW_CMD_H__
#define __MAGICLINK_CLOUD_RAW_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 注册设备云响应回调
 * @param cmd 响应命令字
 * @param cb 回调函数
 * @return 0表示成功，-904001表示命令字不支持，-1表示其他类型失败
*/
int MagicLinkRegCloudAPICb(const char *cmd, int (*cb)(const char *errorCode, const char *cmd, const char *payload));

/**
 * 发送设备云命令请求
 * @param cmd 请求命令字
 * @param payload 请求内容
 * @return 0表示成功，-904001表示命令字不支持，-1表示其他类型失败
*/
int MagicLinkSendAPI2Cloud(const char *cmd, const char *payload);

#ifdef __cplusplus
}
#endif

#endif
