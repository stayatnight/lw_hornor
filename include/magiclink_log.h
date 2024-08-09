/*
 * Copyright (c) Honor Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Magic Link log header file.
 */

#ifndef __MAGICLINK_LOG_H__
#define __MAGICLINK_LOG_H__

#ifdef __cplusplus
extern "c" {
#endif

enum MagicLinkLogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_DATA,
    LOG_LEVEL_BUTT
};

int MagicLinkSetLogLevel(enum MagicLinkLogLevel level);

enum MagicLinkLogLevel MagicLinkGetLogLevel(void);

int MagicLinkRegLogFunc(void (*logFunc)(const char *func, int line, enum MagicLinkLogLevel level,
    const char *format, ...));

#ifdef __cplusplus
}
#endif

#endif
