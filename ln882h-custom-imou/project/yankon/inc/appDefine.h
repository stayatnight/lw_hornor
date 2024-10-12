/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : appDefine.h                                                   *
* Description :                                                               *
*               app define header file                                        *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-07                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __APP_PLAT_DEFINE_H__
#define __APP_PLAT_DEFINE_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/

/******************************************************************************
*                                Defines                                      *
******************************************************************************/ 
//******************************Global Define*******************************//
#define INVALID_U8_VAL                                     (0xff)

#define INVALID_U16_VAL                                    (0xffff)

#define INVALID_U32_VAL                                    (0xffffffff)
       
#define VALID_VALUE                                        (1)

#define IS_VALID_VALUE(a)                                  (VALID_VALUE == (a))

#define IS_INVALID_VALUE(a)                                ((0 == (a)) || (INVALID_U8_VAL == (a)) \
                                                           || (INVALID_U16_VAL == (a)) || (INVALID_U32_VAL == (a)))

#define APP_MIN_VAL(a, b)                                  (((a) > (b)) ? (b) : (a))

#define APP_MAX_VAL(a, b)                                  (((a) < (b)) ? (b) : (a))

#define APP_RANGE(a, mn, mx)                               ((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))

#define APP_SELECT(s, a, b)                                ((s) ? (a) : (b))

#define APP_FOR_EACH(num, func, arg)                       for(int i=0; i<(num); i++)func(arg[i])

#define APP_NTOHL(x)                                       ((((x) & 0xff000000) >> 24) \
                                                           | (((x) & 0x00ff0000) >> 8) \
                                                           | (((x) & 0x0000ff00) << 8) \
                                                           | (((x) & 0x000000ff) << 24))

#define APP_NTOHS(x)                                       ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))


#define APP_HTONL(x)                                       (APP_NTOHL((x)))

#define APP_HTONS(x)                                       (APP_NTOHS((x)))

#define APP_SWAP_L(x)                                      do {x = APP_NTOHL((x));} while(0)

#define APP_SWAP_S(x)                                      do {x = APP_NTOHS((x));} while(0)

#define swap_u16(x)                                        APP_NTOHS(x)

#define swap_u32(x)                                        APP_NTOHL(x)

#define Break_uint32(var,ByteNum)                          (uint8_t)((uint32_t)(((var) >> ((ByteNum) * 8)) & 0x00FF))

#define Build_uint32(Byte3, Byte2, Byte1, Byte0)           ((uint32_t)((uint32_t)((Byte0) & 0x00FF)    \
                                                           + ((uint32_t)((Byte1) & 0x00FF)<<8)        \
                                                           + ((uint32_t)((Byte2) & 0x00FF)<<16)       \
                                                           + ((uint32_t)((Byte3) & 0x00FF)<<24)))

 #define Build_uint16(loByte, hiByte)                      ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

 #define Build_uint8(hiByte, loByte)                       ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

 #define Hi_uint16(a)                                      (((a) >> 8) & 0xFF)

 #define Lo_uint16(a)                                      ((a) & 0xFF)

 #define Hi_Uint8(a)                                       (((a) >> 4) & 0x0F) 

 #define Lo_Uint8(a)                                       ((a) & 0x0F)

#define OffsetOf(TYPE, MEMBER)                             ((uint32_t)(&(((TYPE*)0)->MEMBER)))

#define OffsetOfNext(TYPE, MEMBER)                         (sizeof(((TYPE*)0)->MEMBER) + ((uint32_t)(&(((TYPE*)0)->MEMBER))))

#define APP_ARRAY_SIZE(x)                                  (sizeof(x) / sizeof(x[0]))

#define UNREFERENCED_PARAMETER(P)                          do{if(P){;}}while(0)

// hardware model define
#define APP_DEVICE_HARDWARE_TYPE_BL3357                    0
#define APP_DEVICE_HARDWARE_TYPE_BL3383                    1

//log level define
#define APP_LOG_LEVEL_DEBUG                                (0)
#define APP_LOG_LEVEL_INFO                                 (1)
#define APP_LOG_LEVEL_NOTICE                               (2)
#define APP_LOG_LEVEL_WARNING                              (3)
#define APP_LOG_LEVEL_ERROR                                (4)

//device type define
#define APP_DEV_TYPE_LAMP_READING                          0x00
#define APP_DEV_TYPE_LAMP_CEILING                          0x01
#define APP_DEV_TYPE_LAMP_BEDSIDE                          0x02
#define APP_DEV_TYPE_LAMP_AURORA                           0x03
#define APP_DEV_TYPE_LAMP_NIGHT                            0x04
#define APP_DEV_TYPE_LAMP_READING_L                        0x05
#define APP_DEV_TYPE_LAMP_NIGHT_PTJX                       0x06
#define APP_DEV_TYPE_AIRER_BASE                            0x10
#define APP_DEV_TYPE_BATHBULLY_BASE                        0x20

#define APP_DEV_PWR_TYPE_IO_LEVEL                          0x00000001
#define APP_DEV_PWR_TYPE_IO_PULSE                          0x00000002
#define APP_DEV_PWR_TYPE_ONOFF                             0x00000004
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef float FLOAT;
typedef double DOUBLE;

typedef enum
{
    False = 0x00,
    True
}Bool;

/*APP 错误码(失败码/原因码)定义*/
typedef enum
{
    APP_SUCCESS = 0, 
    APP_FAILURE, 
    APP_OVERFLOW, 
    APP_NO_PRIVILEGE, 
    APP_EXISTED, 
    /*后续继续扩展*/
    APP_UNKNOWN
}APP_ERROR_CODE;

/*OPPLE时间类型定义*/
typedef struct
{
    unsigned short uwYear;                                    /*年份(如: 2013)*/
    unsigned char  ucMon;                                     /*月份(1~12)*/
    unsigned char  ucDay;                                     /*日期(1~31)*/
    unsigned char  ucWDay;                                    /*星期几(1~7)*/
    unsigned char  ucHour;                                    /*时(0~23)*/
    unsigned char  ucMin;                                     /*分(0~59)*/
    unsigned char  ucSec;                                     /*秒(0~59)*/
}ST_MY_TIME;

/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /*__APP_PLAT_DEFINE_H__*/


