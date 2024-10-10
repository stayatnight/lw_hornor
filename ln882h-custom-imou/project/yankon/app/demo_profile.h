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
\"prdID\": \"0501\",\
\"title\": \"智能学习台灯-ln882h模组\",\
\"description\": \"智能学习台灯-ln882h模组\",\
\"serviceList\": [{\
\"index\": 0,\
\"id\": \"baseInfo\",\
\"title\": \"产品基础信息\",\
\"description\": \"产品基础信息\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"prdID\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品的唯一标识\",\
\"value\": \"0501\",\
\"defaultValue\": \"0501\"\
},\
{\
\"index\": 1,\
\"id\": \"prdName\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品名称\",\
\"value\": \"@prdName\",\
\"defaultValue\": \"@prdName\"\
},\
{\
\"index\": 2,\
\"id\": \"manufactureID\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品厂商唯一标识\",\
\"value\": \"A001\",\
\"defaultValue\": \"A001\"\
},\
{\
\"index\": 3,\
\"id\": \"manufactureName\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品厂商名称\",\
\"value\": \"@manufactureName\",\
\"defaultValue\": \"@manufactureName\"\
},\
{\
\"index\": 4,\
\"id\": \"prdTypeId\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品类别唯一表示\",\
\"value\": \"B000\",\
\"defaultValue\": \"B000\"\
},\
{\
\"index\": 5,\
\"id\": \"prdTypeName\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品类别名称\",\
\"value\": \"照明\",\
\"defaultValue\": \"照明\"\
},\
{\
\"index\": 6,\
\"id\": \"prdIcon\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品图标\",\
\"value\": \"/icon\",\
\"defaultValue\": \"/icon\"\
},\
{\
\"index\": 7,\
\"id\": \"uiType\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"UI类型\",\
\"value\": \"H5\",\
\"defaultValue\": \"H5\"\
},\
{\
\"index\": 8,\
\"id\": \"protocolType\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"协议类型\",\
\"value\": \"WiFi\",\
\"defaultValue\": \"WiFi\"\
},\
{\
\"index\": 9,\
\"id\": \"supportNetCfg\",\
\"access\": 0,\
\"type\": \"int\",\
\"description\": \"是否需要配网\",\
\"value\": 1,\
\"defaultValue\": 1\
},\
{\
\"index\": 10,\
\"id\": \"supportRegister\",\
\"access\": 0,\
\"type\": \"int\",\
\"description\": \"是否支持注册到设备云\",\
\"value\": 1,\
\"defaultValue\": 1\
},\
{\
\"index\": 11,\
\"id\": \"pinMode\",\
\"access\": 0,\
\"type\": \"int\",\
\"description\": \"pin码方式：0,默认pin码; 1,输入pin码; 2,扫码\",\
\"value\": 1,\
\"defaultValue\": 1\
},\
{\
\"index\": 12,\
\"id\": \"authMode\",\
\"access\": 0,\
\"type\": \"int\",\
\"description\": \"设备云认证设备的方式：0,PKI; 1,license; 2,固定密钥; 3,账号认证\",\
\"value\": 1,\
\"defaultValue\": 1\
},\
{\
\"index\": 13,\
\"id\": \"supportCloudCtrl\",\
\"access\": 0,\
\"type\": \"int\",\
\"description\": \"是否支持云端控制：0,不支持; 1,支持\",\
\"value\": 1,\
\"defaultValue\": 1\
},\
{\
\"index\": 14,\
\"id\": \"netCfgWlan\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"softAp配网使用的wlan名称\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 15,\
\"id\": \"pluginName\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"插件名\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 16,\
\"id\": \"modelId\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"Model ID\",\
\"value\": \"\",\
\"defaultValue\": \"0\"\
},\
{\
\"index\": 17,\
\"id\": \"offeringCode\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"offering code\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 18,\
\"id\": \"prdSubTypeId\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品子类别标识\",\
\"value\": \"103\",\
\"defaultValue\": \"103\"\
},\
{\
\"index\": 19,\
\"id\": \"prdSubTypeName\",\
\"access\": 0,\
\"type\": \"string\",\
\"description\": \"产品子类别名称\",\
\"value\": \"@prdSubTypeName\",\
\"defaultValue\": \"@prdSubTypeName\"\
}\
]\
},\
{\
\"index\": 1,\
\"id\": \"deviceInfo\",\
\"title\": \"设备基础信息\",\
\"description\": \"设备基础信息\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"sn\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"设备序列号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 1,\
\"id\": \"udid\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"设备udid\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 2,\
\"id\": \"mac\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"设备mac\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 3,\
\"id\": \"devName\",\
\"access\": 7,\
\"type\": \"string\",\
\"description\": \"设备名称\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 4,\
\"id\": \"devModel\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"设备型号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 5,\
\"id\": \"subDevType\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"子设备型号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 6,\
\"id\": \"firmwareVersion\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"固件版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 7,\
\"id\": \"sdkVersion\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"SDK版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 8,\
\"id\": \"hardwareVersion\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"硬件版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 9,\
\"id\": \"MCUVersion\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"MCU版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
}\
]\
},\
{\
\"index\": 2,\
\"id\": \"netInfo\",\
\"title\": \"网络信息\",\
\"description\": \"网络信息\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"ssid\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"WLAN名称\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 1,\
\"id\": \"rssi\",\
\"access\": 1,\
\"type\": \"int\",\
\"description\": \"信号强度\",\
\"value\": 0,\
\"defaultValue\": 0\
},\
{\
\"index\": 2,\
\"id\": \"ip\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"ip地址\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 3,\
\"id\": \"bssid\",\
\"access\": 1,\
\"type\": \"string\",\
\"description\": \"bssid\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
}\
]\
},\
{\
\"index\": 3,\
\"id\": \"ota\",\
\"title\": \"升级服务\",\
\"description\": \"升级服务\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"otaStatus\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"0: 默认值, 100: 正在检测新版本, 200: 检测新版本完成, 300: 升级中, 400: 下载完成等待重启, 500: 超时, 600: 升级成功, -100: 升级失败\",\
\"value\": 0,\
\"defaultValue\": 0\
},\
{\
\"index\": 1,\
\"id\": \"checkResult\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"检测新版本结果，检测成功0，其他为失败\",\
\"value\": 0,\
\"defaultValue\": 0\
},\
{\
\"index\": 2,\
\"id\": \"downloadTimeout\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"预估升级时间(s)\",\
\"value\": 0,\
\"defaultValue\": 0\
},\
{\
\"index\": 3,\
\"id\": \"newVersion\",\
\"access\": 5,\
\"type\": \"string\",\
\"description\": \"新版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 4,\
\"id\": \"MCUNewVersion\",\
\"access\": 5,\
\"type\": \"string\",\
\"description\": \"MCU新版本号\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 5,\
\"id\": \"introduction\",\
\"access\": 5,\
\"type\": \"string\",\
\"description\": \"新版本介绍\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 6,\
\"id\": \"MCUIntroduction\",\
\"access\": 5,\
\"type\": \"string\",\
\"description\": \"MCU新版本介绍\",\
\"value\": \"\",\
\"defaultValue\": \"\"\
},\
{\
\"index\": 7,\
\"id\": \"autoOtaSwitch\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"自动升级开关\",\
\"value\": 0,\
\"defaultValue\": 0\
},\
{\
\"index\": 8,\
\"id\": \"otaProgress\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"升级进度\",\
\"value\": 0,\
\"defaultValue\": 0\
}\
],\
\"actionList\": [{\
\"index\": 0,\
\"id\": \"checkVersion\",\
\"description\": \"检查版本更新\"\
},\
{\
\"index\": 1,\
\"id\": \"otaAction\",\
\"description\": \"触发版本更新\"\
}\
]\
},\
{\
\"index\": 1001,\
\"id\": \"switch\",\
\"title\": \"电源开关\",\
\"description\": \"电源开关\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"on\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"电源开关状态；变化就上报\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"valueList\": [{\
\"value\": 0,\
\"description\": \"关\"\
},\
{\
\"value\": 1,\
\"description\": \"开\"\
}\
]\
}]\
},\
{\
\"index\": 1002,\
\"id\": \"brightness\",\
\"title\": \"亮度\",\
\"description\": \"亮度设置；变化就上报\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"brightness\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"亮度设置；变化就上报\",\
\"value\": 1,\
\"defaultValue\": 1,\
\"step\": 1,\
\"minValue\": 1,\
\"maxValue\": 100\
}]\
},\
{\
\"index\": 1003,\
\"id\": \"lightMode\",\
\"title\": \"灯光模式\",\
\"description\": \"灯光模式\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"mode\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"colourMode和lightMode需要同时上报\\n1灯的模式为一键快捷方式，当设置某一模式时，由设备端自行配置包括诸如开关、亮度、色温等参数；\\n2设备完成开关、亮度、色温等参数，需立即上报给APP。\\n变化就上报；\\n3 智能感光：通过判断当前光线环境来亮度。\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"valueList\": [{\
\"value\": 0,\
\"description\": \"阅读模式\"\
},\
{\
\"value\": 1,\
\"description\": \"休闲模式\"\
},\
{\
\"value\": 2,\
\"description\": \"平板模式\"\
},\
{\
\"value\": 3,\
\"description\": \"专业模式\"\
},\
{\
\"value\": 4,\
\"description\": \"智能感光\"\
}\
]\
}]\
},\
{\
\"index\": 1007,\
\"id\": \"tomatoClk\",\
\"title\": \"番茄钟\",\
\"description\": \"番茄钟\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"status\",\
\"access\": 3,\
\"type\": \"int\",\
\"description\": \"番茄钟当前状态\\n变化就上报\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"valueList\": [{\
\"value\": 0,\
\"description\": \"工作中\"\
},\
{\
\"value\": 1,\
\"description\": \"休息中\"\
}\
]\
},\
{\
\"index\": 1,\
\"id\": \"enable\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"0表示番茄钟关闭，1表示番茄钟开启\\n变化就上报\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"valueList\": [{\
\"value\": 0,\
\"description\": \"关\"\
},\
{\
\"value\": 1,\
\"description\": \"开\"\
}\
]\
},\
{\
\"index\": 2,\
\"id\": \"num\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"循环次数\\n变化就上报\",\
\"value\": 1,\
\"defaultValue\": 1,\
\"step\": 1,\
\"minValue\": 1,\
\"maxValue\": 6\
}\
]\
},\
{\
\"index\": 1008,\
\"id\": \"volume\",\
\"title\": \"声音设置\",\
\"description\": \"声音设置；\\n变化就上报\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"volume\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"声音设置\\n变化就上报\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"step\": 1,\
\"minValue\": 0,\
\"maxValue\": 100\
}]\
},\
{\
\"index\": 1009,\
\"id\": \"dvService\",\
\"title\": \"设备虚拟化\",\
\"description\": \"设备虚拟化\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"status\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"开关状态\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"valueList\": [{\
\"value\": 0,\
\"description\": \"关\"\
},\
{\
\"value\": 1,\
\"description\": \"开\"\
}\
]\
},\
{\
\"index\": 1,\
\"id\": \"supportSinkSvc\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"按位表示不同能力，由低到高：1<<0：Camera;1<<1：Mic;1<<2：Speaker剩余位预留;设备侧能力支持:如果0，表示能力不支持;如果是1，表示能力支持;0与1按位表达\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"minValue\": 0,\
\"maxValue\": 4294967295\
},\
{\
\"index\": 2,\
\"id\": \"devSvcStatus\",\
\"access\": 5,\
\"type\": \"int\",\
\"description\": \"按位表示不同能力，由低到高：1<<0：Camera;1<<1：Mic;1<<2：Speaker剩余位预留;如果0，表示能力空闲;如果是1，表示能力被占用。0与1按位表达\",\
\"value\": 0,\
\"defaultValue\": 0,\
\"minValue\": 0,\
\"maxValue\": 4294967295\
}\
]\
},\
{\
\"index\": 1010,\
\"id\": \"cct\",\
\"title\": \"色温设置\",\
\"description\": \"色温设置\",\
\"propertyList\": [{\
\"index\": 0,\
\"id\": \"colorTemperature\",\
\"access\": 7,\
\"type\": \"int\",\
\"description\": \"灯的色温（2000~4000暖色， 4001~6500冷色）变化就上报\",\
\"value\": 2000,\
\"defaultValue\": 2000,\
\"step\": 1,\
\"minValue\": 2000,\
\"maxValue\": 6500\
}]\
}\
],\
\"quickmenu\": [{\
\"switchInfo\": {\
\"path\": \"switch.on\",\
\"off\": 0,\
\"on\": 1\
},\
\"content\": [{\
\"text\": \"亮度:{brightness.brightness%1}%\"\
}]\
}]\
}";

#ifdef __cplusplus
}
#endif

#endif
