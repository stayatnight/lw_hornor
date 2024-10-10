/******************************************************************************
* Version     : OPP_IOTCTRL V100R001C01B001D001                               *
* File        : oppHalSpi.h                                                 *
* Description :                                                               *
*               OPPLE网络协议链路层SPI链路处理头文件                         *
* Note        : (none)                                                        *
* Author      : 智能控制研发部                                                *
* Date:       : 2017-10-19                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __OPP_HAL_SPI_H__
#define __OPP_HAL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "oppHal.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
#pragma pack(1)
typedef enum
{
    OPP_HAL_SPI_1,
    OPP_HAL_SPI_NUM
} oppHalSpi_t;
#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/
OPP_ERROR_CODE oppHalSpiInit(oppHalSpi_t spi, int hz, uint8_t nBits);
void oppHalSpiSendData(oppHalSpi_t spi, uint8_t *buf, uint32_t len);
uint32_t oppHalSpiRecieveData(oppHalSpi_t spi, uint8_t *buf, uint32_t len);
int oppHalSpiGetTxStatus(oppHalSpi_t spi);

#ifdef __cplusplus
}
#endif



#endif   /*__OPP_HAL_SPI_H__*/

