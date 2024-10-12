/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : appConfig.h                                                   *
* Description :                                                               *
*               app configuration                                             *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __APP_PLAT_CONFIG_H__
#define __APP_PLAT_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "appDefine.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
/* define system debug log level */
#define APP_LOG_LEVEL_DEFAULT                         APP_LOG_LEVEL_INFO

#define APP_MAGIC_LINK_USE                            (1)
/* define device type */
#define APP_DEV_TYPE_USED                             helloworld

#define IS_LAMP_DEV_TYPE_USED()                       ((APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING) ||\
                                                       (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_CEILING) ||\
                                                       (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE) ||\
                                                       (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT) ||\
                                                       (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_AURORA))


#if (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_READING) //台灯

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3357
#define APP_TASK_LAMP_USE                             (1)
#define APP_TASK_KEY_USE                              (1)
#define APP_TASK_FCT_USE                              (1)

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_CEILING) // 吸顶灯

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3383
#define APP_TASK_LAMP_USE                             (1)
#define APP_TASK_FCT_USE                              (1)
/* define device if cct support */
#define APP_DEV_CCT_SUPPORT                           (1)
/* define device power down type */
#define APP_DEV_PWR_TYPE_USED                         APP_DEV_PWR_TYPE_ONOFF|APP_DEV_PWR_TYPE_IO_PULSE

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_BEDSIDE) // 床头灯

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3383
#define APP_TASK_LAMP_SERIAL_USE                      (1)
/* define device if cct support */
#define APP_DEV_CCT_SUPPORT                           (1)

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_AURORA) // 氛围灯

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3357
#define APP_TASK_LAMP_SERIAL_USE                      (1)

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_AIRER_BASE) // 晾衣架

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3383
#define APP_TASK_LAMP_SERIAL_USE                      (1)

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_BATHBULLY_BASE) //浴霸

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3383
#define APP_TASK_LAMP_SERIAL_USE                      (1)

#elif (APP_DEV_TYPE_USED == APP_DEV_TYPE_LAMP_NIGHT) //小夜灯

/* define device hardware type */
#define APP_DEVICE_HARDWARE_TYPE_USED                 APP_DEVICE_HARDWARE_TYPE_BL3357
#define APP_TASK_LAMP_USE                             (1)
#define APP_TASK_KEY_USE                              (1)
#define APP_TASK_FCT_USE                              (1)

#endif
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /*__APP_PLAT_CONFIG_H__*/


