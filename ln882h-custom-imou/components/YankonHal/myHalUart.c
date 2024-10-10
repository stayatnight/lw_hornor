/******************************************************************************
* Version     : V100R001C01B001D001                                           *
* File        : myHalUart.c                                                   *
* Description :                                                               *
*               my hal UART 处理源文件                                         *
* Note        : (none)                                                        *
* Author      : kris li                                                       *
* Date:       : 2022-09-20                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "myHalUart.h"
#include "myFIFO.h"
#include "serial_api.h"
#include "serial_ex_api.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#define MY_HAL_UART_BUF_SIZE                            (256)

#define MY_HAL_UART0_TX                                 PA_14
#define MY_HAL_UART0_RX                                 PA_13
#define MY_HAL_UART1_TX                                 PA_3
#define MY_HAL_UART1_RX                                 PA_2
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct
{
    uint8_t rxBuf[MY_HAL_UART_BUF_SIZE];
    uint8_t txBuf[MY_HAL_UART_BUF_SIZE];
} _haluartBuf_t;

typedef struct
{
    myHalUartMode_t uartMode;
    uint8_t dmaTxBusy;
    serial_t serial;
    commsInterface_t interface;
    _haluartBuf_t halUartDefaultBuf;
    myHalUartBuf_t myHalUartBuf;
} myHalUartInfo_t;
/******************************************************************************
*                                Globals                                      *
******************************************************************************/
static myHalUartInfo_t myHalUartInfo[MY_HAL_UART_NUM];
/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/
extern int LOGUART_SetBaud(u32 BaudRate);
/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : uartRxISR
 Description : OPP串口UART硬件链路适配模块接收中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void uartRxISR(serial_t *sobj)
{
    uint8_t data = 0;
    commsInterface_t *pInterface = NULL;

    data = (uint8_t)serial_getc(sobj);

    if ( sobj == &myHalUartInfo[MY_HAL_UART_0].serial ) {
        pInterface = &myHalUartInfo[MY_HAL_UART_0].interface;
    } else if ( sobj == &myHalUartInfo[MY_HAL_UART_1].serial ) {
        pInterface = &myHalUartInfo[MY_HAL_UART_1].interface;
    } else {
        return;
    }
    
    if (myFIFOCntFree(&(pInterface->rxFIFO))) {
        myFIFOPutOne(data, &(pInterface->rxFIFO));
    }
}

/******************************************************************************
 Function    : uartTxISR
 Description : OPP串口UART硬件链路适配模块发送中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static void uartTxISR(serial_t *sobj)
{
    uint32_t nBytes = 0;
    commsInterface_t *pInterface = NULL;

    if ( sobj == &myHalUartInfo[MY_HAL_UART_0].serial ) {
        pInterface = &myHalUartInfo[MY_HAL_UART_0].interface;
    } else if ( sobj == &myHalUartInfo[MY_HAL_UART_1].serial ) {
        pInterface = &myHalUartInfo[MY_HAL_UART_1].interface;
    } else {
        serial_irq_set(sobj, TxIrq, 0);
        return;
    }

    nBytes = myFIFOCnt(&(pInterface->txFIFO));    
    if(nBytes == 0 && (pInterface->txFIFO.iIn != pInterface->txFIFO.iOut)) {
        myFIFOReset(&(pInterface->txFIFO));
    }
    else if(nBytes != 0 && (pInterface->txFIFO.iIn == pInterface->txFIFO.iOut) && nBytes != pInterface->txFIFO.maxBytes) {
        nBytes = 0;
        myFIFOReset(&(pInterface->txFIFO));
    }

    if (nBytes > 0) {
        uint8_t ucData;

        myFIFOGetOne(&ucData, &(pInterface->txFIFO));
        serial_putc(sobj, ucData);
    }
    else {      
        serial_irq_set(sobj, TxIrq, 0);
    }
}

/******************************************************************************
 Function    : uart0_irq
 Description : OPP串口UART irq proccess
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
static void uart0_irq(uint32_t id, SerialIrq event)
{
    serial_t    *sobj = (void*)id;

    if(event == RxIrq) {
        uartRxISR(sobj);
    }

    if(event == TxIrq){
        uartTxISR(sobj);
    }
}

/******************************************************************************
 Function    : uart1_irq
 Description : OPP串口UART irq proccess
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
static void uart1_irq(uint32_t id, SerialIrq event)
{
    serial_t    *sobj = (void*)id;

    if(event == RxIrq) {
        uartRxISR(sobj);
    }

    if(event == TxIrq){
        uartTxISR(sobj);
    }
}

/******************************************************************************
 Function    : uart0_send_string_done
 Description : OPP串口UART irq proccess
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
void uart0_send_string_done(uint32_t id)
{
    if ( id == (uint32_t)&myHalUartInfo[MY_HAL_UART_0].serial ) {
        myHalUartInfo[MY_HAL_UART_0].dmaTxBusy = 0;
    }  
}

/******************************************************************************
 Function    : uart1_send_string_done
 Description : OPP串口UART irq proccess
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
void uart1_send_string_done(uint32_t id)
{
    if ( id == (uint32_t)&myHalUartInfo[MY_HAL_UART_1].serial ) {
        myHalUartInfo[MY_HAL_UART_1].dmaTxBusy  = 0;
    }  
}

/******************************************************************************
 Function    : oppHalUartInit
 Description : OPP串口UART硬件链路适配模块初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
int myHalUartInit(myHalUart_t uart, uint32_t baudrate, myHalUartBuf_t *pstHalBuf, myHalUartMode_t mode)
{
    if(uart >= MY_HAL_UART_NUM || mode >= MY_HAL_UART_MODE_NUM) {
        return -1;
    }

    if (pstHalBuf) {
        myHalUartInfo[uart].myHalUartBuf.pucRxBuf = pstHalBuf->pucRxBuf;
        myHalUartInfo[uart].myHalUartBuf.ulRxSize = pstHalBuf->ulRxSize;
        myHalUartInfo[uart].myHalUartBuf.pucTxBuf = pstHalBuf->pucTxBuf;
        myHalUartInfo[uart].myHalUartBuf.ulTxSize = pstHalBuf->ulTxSize;
    }
    else {
        myHalUartInfo[uart].myHalUartBuf.pucRxBuf = myHalUartInfo[uart].halUartDefaultBuf.rxBuf;
        myHalUartInfo[uart].myHalUartBuf.ulRxSize = MY_HAL_UART_BUF_SIZE;
        myHalUartInfo[uart].myHalUartBuf.pucTxBuf = myHalUartInfo[uart].halUartDefaultBuf.txBuf;
        myHalUartInfo[uart].myHalUartBuf.ulTxSize = MY_HAL_UART_BUF_SIZE;
    }

    myHalUartInfo[uart].uartMode = mode;

    myFIFOInit(&(myHalUartInfo[uart].interface.rxFIFO), myHalUartInfo[uart].myHalUartBuf.pucRxBuf, myHalUartInfo[uart].myHalUartBuf.ulRxSize);

    if ( uart ==  MY_HAL_UART_0 ) {
        serial_t *pSerial = &myHalUartInfo[uart].serial;

        serial_init(pSerial, MY_HAL_UART0_TX, MY_HAL_UART0_RX);
        serial_baud(pSerial, baudrate);
        serial_format(pSerial, 8, ParityNone, 1);
        serial_irq_handler(pSerial, uart0_irq, (uint32_t)pSerial);
        serial_irq_set(pSerial, RxIrq, 1);
        serial_irq_set(pSerial, TxIrq, 0);
        serial_send_comp_handler(pSerial, (void*)uart0_send_string_done, (uint32_t)pSerial);
    } else if ( uart ==  MY_HAL_UART_1 ) {
        serial_t *pSerial = &myHalUartInfo[uart].serial;

        serial_init(pSerial, MY_HAL_UART1_TX, MY_HAL_UART1_RX);
        serial_baud(pSerial, baudrate);
        serial_format(pSerial, 8, ParityNone, 1);
        serial_irq_handler(pSerial, uart1_irq, (uint32_t)pSerial);
        serial_irq_set(pSerial, RxIrq, 1);
        serial_irq_set(pSerial, TxIrq, 0);
        serial_send_comp_handler(pSerial, (void*)uart1_send_string_done, (uint32_t)pSerial);
    }
    
    return 0;
}

/******************************************************************************
 Function    : oppHalUartIrqConfig
 Description : OPP串口UART IRQ初始化
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : MY_SUCCESS - 成功
               其它        - 失败码
 Other       : (none)
******************************************************************************/
//MY_ERROR_CODE oppHalUartIrqConfig(oppHalUart_t uart, uint8_t rxEn, uint8_t txEn)
//{
//    return MY_SUCCESS;
//}

/******************************************************************************
 Function    : myHalUartSend
 Description : OPP串口USART 硬件链路适配模块发送函数
 Note        : (none)
 Input Para  : pucData      - 发送数据包地址(从网络层开始)
               ulDataLen    - 发送数据包长度
 Output Para : (none)
 Return      : 实际发送字节数
 Other       : (none)
******************************************************************************/
int myHalUartSend(myHalUart_t uart, uint8_t* pucData, uint32_t ulDataLen)
{
    if(ulDataLen == 0 || !pucData || uart >= MY_HAL_UART_NUM) {
        return -1;
    }

    if ( ulDataLen >  myHalUartInfo[uart].myHalUartBuf.ulTxSize) {
        return -2;
    }

    if(myHalUartInfo[uart].dmaTxBusy) {
        return 0;
    }

    myHalUartInfo[uart].dmaTxBusy = 1;

    memcpy(myHalUartInfo[uart].myHalUartBuf.pucTxBuf, pucData, ulDataLen);
    serial_send_stream_dma(&myHalUartInfo[uart].serial, myHalUartInfo[uart].myHalUartBuf.pucTxBuf, ulDataLen);
    
    return(ulDataLen);
}

/******************************************************************************
 Function    : myHalUartRecv
 Description : OPP串口Output UART硬件链路适配模块接收函数
 Note        : (none)
 Input Para  : data - 串口接收的数据，每次一个字节
 Output Para : (none)
 Return      : 实际接收到的字节数
 Other       : (none)
******************************************************************************/
int myHalUartRecv(myHalUart_t uart, uint8_t *pucData, uint32_t ulDataLen)
{   
    commsInterface_t *pInterface = &myHalUartInfo[uart].interface;
    uint32_t nBytes = myFIFOCnt(&(pInterface->rxFIFO));

    if(!pucData || uart >= MY_HAL_UART_NUM) {
        return -1;
    }

    if (nBytes == 0 || ulDataLen == 0) {
        return 0;
    }

    serial_irq_set(&myHalUartInfo[uart].serial, RxIrq, 0);
    if(nBytes == 0 && (pInterface->rxFIFO.iIn != pInterface->rxFIFO.iOut)) {
        myFIFOReset(&(pInterface->rxFIFO));
    }
    else if(nBytes != 0 && (pInterface->rxFIFO.iIn == pInterface->rxFIFO.iOut) && nBytes != pInterface->rxFIFO.maxBytes) {
        nBytes = 0;
        myFIFOReset(&(pInterface->rxFIFO));
    }

    if(nBytes > 0) {
        nBytes = (nBytes > ulDataLen) ? ulDataLen : nBytes;
        myFIFOGet(pucData, nBytes, &(pInterface->rxFIFO));
    }
    serial_irq_set(&myHalUartInfo[uart].serial, RxIrq, 1);

    return nBytes;
}

/******************************************************************************
 Function    : oppHalUartGetBusy
 Description : OPP串口USART2 busy
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : 1-- busy; 0-- idle
 Other       : (none)
******************************************************************************/
//uint8_t oppHalUartGetBusy(oppHalUart_t uart)
//{
//    return 0;
//}

/******************************************************************************
 Function    : oppHalUartGetRecvLen
 Description : OPP串口 UART硬件链路适配模块接收函数
 Note        : (none)
 Input Para  : data - 串口接收的数据，每次一个字节
 Output Para : (none)
 Return      : 实际接收到的字节数
 Other       : (none)
******************************************************************************/
uint32_t myHalUartGetRecvLen(myHalUart_t uart)
{ 
    uint32_t nBytes = 0;
    commsInterface_t *pInterface = &myHalUartInfo[uart].interface;
    if(uart >= MY_HAL_UART_NUM) {
        return 0;
    }

    nBytes = myFIFOCnt(&(pInterface->rxFIFO));
    
    return(nBytes);
}

/******************************************************************************
 Function    : oppHalUartRxISR
 Description : OPP串口UART硬件链路适配模块接收中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
//void oppHalUartRxISR(oppHalUart_t uart)
//{
//
//}


/******************************************************************************
 Function    : oppHalUartIdleISR
 Description : OPP串口UART硬件链路适配模块接收完成中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
//void oppHalUartIdleISR(oppHalUart_t uart)
//{
//}

/******************************************************************************
 Function    : oppHalUSARTTxISR
 Description : OPP串口UART硬件链路适配模块发送中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
//void oppHalUartTxISR(oppHalUart_t uart)
//{
//}

/******************************************************************************
 Function    : oppHalUartTcISR
 Description : OPP串口output UART硬件链路适配模块发送中断处理
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
//void oppHalUartTcISR(oppHalUart_t uart)
//{
//}


/******************************************************************************
 Function    : oppHalUartPrintf
 Description : opp hal UART打印
 Note        : (none)
 Input Para  : fmt -- 格式
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
//uint32_t oppHalUartPrintf(char* fmt, ...)
//{
//    static char buf[1024];
//    va_list ap;
//
//    va_start(ap, fmt);
//    vsprintf(buf, fmt, ap);
//    oppHalUartSend(MY_HAL_UART_1, buf, strlen((const char*)buf));
//    va_end(ap);
//
//    return 0;
//}

#ifdef __cplusplus
}
#endif

