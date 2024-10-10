/******************************************************************************
* Version     : OPP_IOTCTRL V100R001C01B001D001                               *
* File        : oppHalSpi.c                                                 *
* Description :                                                               *
*               OPPLE网络协议链路层SPI链路处理源文件                         *
* Note        : (none)                                                        *
* Author      : 智能控制研发部                                                *
* Date:       : 2017-10-19                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "oppHalSpi.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define SPI1_MOSI  PA_23
#define SPI1_MISO  PA_22
#define SPI1_SCLK  PA_18
#define SPI1_CS    PA_19

#define SCLK_FREQ           6000000
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
static spi_t spi_master;
volatile int MasterTxDone;
volatile int MasterRxDone;
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : master_tr_done_callback
 Description : OPP SPI master trans done callback
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void master_tr_done_callback(void *pdata, SpiIrq event)
{
    switch(event){
    case SpiRxIrq:
        MasterRxDone = 1;
        break;
    case SpiTxIrq:
        MasterTxDone = 1;
        break;
    default:
        DBG_8195A("unknown interrput evnent!\n");
    }
}

/******************************************************************************
 Function    : oppHalSpiInit
 Description : OPP SPI硬件链路适配模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : OPP_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
OPP_ERROR_CODE oppHalSpiInit(oppHalSpi_t spi, int hz, uint8_t nBits)
{
    if ( spi >= OPP_HAL_SPI_NUM ) return OPP_FAILURE;
    if ( spi == OPP_HAL_SPI_1 ) {
        spi_master.spi_idx = MBED_SPI1;
        spi_init(&spi_master, SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CS);
    }  
    spi_frequency(&spi_master, hz);
    spi_format(&spi_master, nBits, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_MIDDLE) , 0);
    spi_irq_hook(&spi_master,(spi_irq_handler) master_tr_done_callback, (uint32_t)&spi_master);
    MasterTxDone = 1;
    return OPP_SUCCESS;
}

/******************************************************************************
 Function    : oppHalSpiSendData
 Description : hal spi 发送
 Note        : (none)
 Input Para  : SendByte -- 待发送数据
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppHalSpiSendData(oppHalSpi_t spi, uint8_t *buf, uint32_t len)
{
    if ( spi >= OPP_HAL_SPI_NUM ) return;
    spi_master_write_stream_dma(&spi_master, buf, len);
}

/******************************************************************************
 Function    : oppHalSpiRecieveData
 Description : hal spi 接收
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : 接收到的数据
 Other       : (none)
******************************************************************************/
uint32_t oppHalSpiRecieveData(oppHalSpi_t spi, uint8_t *buf, uint32_t len)
{
    return 0;
}

/******************************************************************************
 Function    : oppHalSpiGetTxStatus
 Description : hal spi get tx status
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
int oppHalSpiGetTxStatus(oppHalSpi_t spi)
{
    if ( spi >= OPP_HAL_SPI_NUM ) return 0;
    return MasterTxDone;
}

#ifdef __cplusplus
}
#endif

