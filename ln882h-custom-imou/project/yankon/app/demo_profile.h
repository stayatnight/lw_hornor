/*
 * Copyright (c) Honor Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: magiclink sdk demo profile head file
 */

#ifndef __DEMO_PROFILE_H__
#define __DEMO_PROFILE_H__

#ifdef __cplusplus
extern "c" {
#endif

static const char *g_profile = "{\
\"prdID\": \"C1NL\",\
\"serviceList\": [\
{\
\"propertyList\": [\
{\
\"access\": 0,\
\"defaultValue\": \"C1NL\",\
\"description\": \"产品的唯一标识\",\
\"index\": 0,\
\"type\": \"string\",\
\"id\": \"prdID\",\
\"value\": \"C1NL\",\
\"maxLength\": 4\
},\
{\
\"access\": 0,\
\"defaultValue\": \"阳光博荣台灯S\",\
\"description\": \"产品名称\",\
\"index\": 1,\
\"type\": \"string\",\
\"id\": \"prdName\",\
\"value\": \"阳光博荣台灯S\"\
},\
{\
\"access\": 0,\
\"defaultValue\": \"A024\",\
\"description\": \"产品厂商唯一标识\",\
\"index\": 2,\
\"type\": \"string\",\
\"id\": \"manufactureID\",\
\"value\": \"A024\",\
\"maxLength\": 4\
},\
{\
\"access\": 0,\
\"defaultValue\": \"浙江阳光照明电器集团股份有限公司\",\
\"description\": \"产品厂商名称\",\
\"index\": 3,\
\"type\": \"string\",\
\"id\": \"manufactureName\",\
\"value\": \"浙江阳光照明电器集团股份有限公司\"\
},\
{\
\"access\": 0,\
\"defaultValue\": \"B000\",\
\"description\": \"产品类别唯一表示\",\
\"index\": 4,\
\"type\": \"string\",\
\"id\": \"prdTypeId\",\
\"value\": \"B000\",\
\"maxLength\": 4\
},\
{\
\"access\": 0,\
\"defaultValue\": \"照明\",\
\"description\": \"产品类别名称\",\
\"index\": 5,\
\"type\": \"string\",\
\"id\": \"prdTypeName\",\
\"value\": \"照明\"\
},\
{\
\"access\": 0,\
\"defaultValue\": \"WiFi\",\
\"description\": \"协议类型\",\
\"index\": 6,\
\"type\": \"string\",\
\"id\": \"protocolType\",\
\"value\": \"WiFi\",\
\"maxLength\": 4\
},\
{\
\"access\": 0,\
\"defaultValue\": 1,\
\"description\": \"是否需要配网：0,否;1,是\",\
\"index\": 7,\
\"type\": \"int\",\
\"id\": \"supportNetCfg\",\
\"value\": 1\
},\
{\
\"access\": 0,\
\"defaultValue\": 1,\
\"description\": \"是否支持注册到设备云：0,否;1,是\",\
\"index\": 8,\
\"type\": \"int\",\
\"id\": \"supportRegister\",\
\"value\": 1\
},\
{\
\"access\": 0,\
\"defaultValue\": 0,\
\"description\": \"pin码方式：0,默认pin码; 1,用户输入pin码\",\
\"index\": 9,\
\"type\": \"int\",\
\"id\": \"pinMode\",\
\"value\": 0\
},\
{\
\"access\": 0,\
\"defaultValue\": 2,\
\"description\": \"设备云认证设备的方式：0,PKI; 1,license; 2,固定密钥; 3,账号认证; 99,网关子设备\",\
\"index\": 10,\
\"type\": \"int\",\
\"id\": \"authMode\",\
\"value\": 2\
},\
{\
\"access\": 0,\
\"defaultValue\": 1,\
\"description\": \"是否支持云端控制：0,不支持; 1,支持\",\
\"index\": 11,\
\"type\": \"int\",\
\"id\": \"supportCloudCtrl\",\
\"value\": 1\
},\
{\
\"access\": 0,\
\"defaultValue\": \"\",\
\"description\": \"softAp配网使用的wlan名称\",\
\"index\": 12,\
\"type\": \"string\",\
\"id\": \"netCfgWlan\",\
\"value\": \"\"\
},\
{\
\"access\": 0,\
\"defaultValue\": \"0\",\
\"description\": \"Model ID\",\
\"index\": 13,\
\"type\": \"string\",\
\"id\": \"modelId\",\
\"value\": \"0\",\
\"maxLength\": 6\
},\
{\
\"access\": 0,\
\"defaultValue\": \"\",\
\"description\": \"offering code\",\
\"index\": 14,\
\"type\": \"string\",\
\"id\": \"offeringCode\",\
\"value\": \"\",\
\"maxLength\": 16\
},\
{\
\"access\": 0,\
\"defaultValue\": \"103\",\
\"description\": \"产品子类别标识\",\
\"index\": 15,\
\"type\": \"string\",\
\"id\": \"prdSubTypeId\",\
\"value\": \"103\",\
\"maxLength\": 3\
},\
{\
\"access\": 0,\
\"defaultValue\": \"台灯\",\
\"description\": \"产品子类别名称\",\
\"index\": 16,\
\"type\": \"string\",\
\"id\": \"prdSubTypeName\",\
\"value\": \"台灯\"\
},\
{\
\"access\": 0,\
\"defaultValue\": 0,\
\"description\": \"二维码：0,不涉及; 1,手机扫码; 2,设备扫码\",\
\"index\": 17,\
\"type\": \"int\",\
\"id\": \"qrScanMode\",\
\"value\": 0\
}\
],\
\"description\": \"产品基础信息\",\
\"index\": 0,\
\"id\": \"baseInfo\",\
\"title\": \"产品基础信息\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 0,\
\"description\": \"设备序列号\",\
\"id\": \"sn\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 64,\
\"defaultValue\": \"\",\
\"index\": 1,\
\"description\": \"设备udid\",\
\"id\": \"udid\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 17,\
\"defaultValue\": \"\",\
\"index\": 2,\
\"description\": \"设备mac\",\
\"id\": \"mac\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 7,\
\"maxlength\": 63,\
\"defaultValue\": \"useCloudName\",\
\"index\": 3,\
\"description\": \"设备名称\",\
\"id\": \"devName\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 15,\
\"defaultValue\": \"\",\
\"index\": 4,\
\"description\": \"设备型号\",\
\"id\": \"devModel\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 1,\
\"defaultValue\": \"\",\
\"index\": 5,\
\"description\": \"子设备型号\",\
\"id\": \"subDevType\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 6,\
\"description\": \"固件版本号\",\
\"id\": \"firmwareVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 7,\
\"description\": \"SDK版本号\",\
\"id\": \"sdkVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 8,\
\"description\": \"硬件版本号\",\
\"id\": \"hardwareVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 9,\
\"description\": \"MCU版本号\",\
\"id\": \"MCUVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
}\
],\
\"index\": 1,\
\"description\": \"设备基础信息\",\
\"id\": \"deviceInfo\",\
\"title\": \"设备基础信息\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 5,\
\"maxlength\": 32,\
\"defaultValue\": \"\",\
\"index\": 0,\
\"description\": \"WLAN名称\",\
\"id\": \"ssid\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 1,\
\"description\": \"信号强度\",\
\"id\": \"rssi\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"maxlength\": 15,\
\"defaultValue\": \"\",\
\"index\": 2,\
\"description\": \"ip地址\",\
\"id\": \"ip\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"maxlength\": 17,\
\"defaultValue\": \"\",\
\"index\": 3,\
\"description\": \"bssid\",\
\"id\": \"bssid\",\
\"type\": \"string\",\
\"value\": \"\"\
}\
],\
\"index\": 2,\
\"description\": \"网络信息\",\
\"id\": \"netInfo\",\
\"title\": \"网络信息\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 0,\
\"description\": \"0: 默认值, 100: 正在检测新版本, 200: 检测新版本完成, 300: 升级中, 400: 下载完成等待重启, 500: 超时, 600: 升级成功, -100: 升级失败\",\
\"id\": \"otaStatus\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 1,\
\"description\": \"检测新版本结果，检测成功0，其他为失败\",\
\"id\": \"checkResult\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 2,\
\"description\": \"预估升级时间(s)\",\
\"id\": \"downloadTimeout\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"defaultValue\": \"\",\
\"index\": 3,\
\"description\": \"新版本号\",\
\"id\": \"newVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"defaultValue\": \"\",\
\"index\": 4,\
\"description\": \"MCU新版本号\",\
\"id\": \"MCUNewVersion\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"defaultValue\": \"\",\
\"index\": 5,\
\"description\": \"新版本介绍\",\
\"id\": \"introduction\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 5,\
\"defaultValue\": \"\",\
\"index\": 6,\
\"description\": \"MCU新版本介绍\",\
\"id\": \"MCUIntroduction\",\
\"type\": \"string\",\
\"value\": \"\"\
},\
{\
\"access\": 7,\
\"defaultValue\": 0,\
\"index\": 7,\
\"description\": \"自动升级开关\",\
\"id\": \"autoOtaSwitch\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 8,\
\"description\": \"升级进度\",\
\"id\": \"otaProgress\",\
\"type\": \"int\",\
\"value\": 0\
},\
{\
\"access\": 5,\
\"defaultValue\": 0,\
\"index\": 9,\
\"description\": \"升级版本包大小\",\
\"id\": \"pkgSize\",\
\"type\": \"int\",\
\"value\": 0\
}\
],\
\"index\": 3,\
\"description\": \"升级服务\",\
\"actionList\": [\
{\
\"index\": 0,\
\"description\": \"检查版本更新\",\
\"id\": \"checkVersion\"\
},\
{\
\"index\": 1,\
\"description\": \"触发版本更新\",\
\"id\": \"otaAction\"\
}\
],\
\"id\": \"ota\",\
\"title\": \"升级服务\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 0,\
\"defaultValue\": 8,\
\"description\": \"开发者网站勾选本地定时后有此服务属性，网关本身定时器最大数量为8，开发者网站支持可修改范围[1,8]，非网关表示设备本身\",\
\"index\": 0,\
\"type\": \"int\",\
\"id\": \"timerMax\",\
\"value\": 8\
},\
{\
\"access\": 0,\
\"defaultValue\": 8,\
\"description\": \"开发者网站创建网关设备时勾选本地定时后有此服务属性，子设备的定时器最大数量为8，开发者网站支持可修改范围[1,8]，非网关设备无需关注此字段\",\
\"index\": 1,\
\"type\": \"int\",\
\"id\": \"subDevTimerMax\",\
\"value\": 8\
},\
{\
\"access\": 0,\
\"defaultValue\": 1,\
\"description\": \"网关本身倒计时最大数量，固定值为1\",\
\"index\": 2,\
\"type\": \"int\",\
\"id\": \"countdownMax\",\
\"value\": 1\
},\
{\
\"access\": 0,\
\"defaultValue\": 1,\
\"description\": \"子设备倒计时最大数量，固定值为1，非网关设备无需关注此字段\",\
\"index\": 3,\
\"type\": \"int\",\
\"id\": \"subDevCountdownMax\",\
\"value\": 1\
}\
],\
\"description\": \"定时/倒计时\",\
\"index\": 4,\
\"id\": \"timerCountdown\",\
\"title\": \"定时/倒计时\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 7,\
\"defaultValue\": 0,\
\"description\": \"\",\
\"index\": 0,\
\"type\": \"int\",\
\"valueList\": [\
{\
\"description\": \"关\",\
\"value\": 0\
},\
{\
\"description\": \"开\",\
\"value\": 1\
}\
],\
\"id\": \"On\",\
\"value\": 0\
},\
{\
\"access\": 7,\
\"defaultValue\": 1,\
\"maxValue\": 100,\
\"description\": \"亮度设置；变化就上报\",\
\"index\": 1,\
\"type\": \"int\",\
\"minValue\": 1,\
\"step\": 1,\
\"id\": \"Brightness\",\
\"value\": 1\
},\
{\
\"access\": 7,\
\"defaultValue\": 0,\
\"description\": \"\",\
\"index\": 2,\
\"type\": \"int\",\
\"valueList\": [\
{\
\"description\": \"会客模式\",\
\"value\": 0\
},\
{\
\"description\": \"阅读模式\",\
\"value\": 8\
},\
{\
\"description\": \"夜灯模式\",\
\"value\": 10\
},\
{\
\"description\": \"书写模式\",\
\"value\": 11\
}\
],\
\"id\": \"lightMode\",\
\"value\": 0\
}\
],\
\"description\": \"灯\",\
\"index\": 5,\
\"id\": \"light\",\
\"title\": \"灯\"\
},\
{\
\"propertyList\": [\
{\
\"access\": 3,\
\"defaultValue\": 0,\
\"index\": 0,\
\"type\": \"int\",\
\"valueList\": [\
{\
\"description\": \"无\",\
\"value\": 0\
},\
{\
\"description\": \"0.5s\",\
\"value\": 1\
},\
{\
\"description\": \"1.0s\",\
\"value\": 2\
},\
{\
\"description\": \"1.5s\",\
\"value\": 3\
},\
{\
\"description\": \"2.0s\",\
\"value\": 4\
},\
{\
\"description\": \"2.5s\",\
\"value\": 5\
},\
{\
\"description\": \"3.0s\",\
\"value\": 6\
}\
],\
\"id\": \"duration\",\
\"value\": 0\
}\
],\
\"description\": \"渐变\",\
\"index\": 6,\
\"id\": \"duration\",\
\"title\": \"渐变\"\
}\
],\
\"description\": \"阳光博荣台灯S\",\
\"title\": \"阳光博荣台灯S\"\
}";


#ifdef __cplusplus
}
#endif

#endif
