#include <stdarg.h>

#include "ln_types.h"
#include "ln_ate.h"
#include "ln_utils.h"
#include "ln_nvds.h"

#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "utils/reboot_trace/reboot_trace.h"

#include "proj_config.h"
#include "osal/osal.h"
#include "hal/hal_gpio.h"
#include "serial/serial.h"
#include "wifi.h"
#include "wifi_port.h"

#define ATE_CMD_FOUND                (0)
#define ATE_CMD_NOT_FOUND            (-1)
#define ATE_CMD_PRINT(fmt, ...)      ln_ty_ate_printf(fmt, ##__VA_ARGS__)

static uint8_t at_cmd_echo_en = 1; 

static Serial_t       g_at_serial;
static OS_Semaphore_t g_uart_rx_sem;

#define LN_AT_PRINTF_BUF_SIZE   (256)
static char s_at_log_buff[LN_AT_PRINTF_BUF_SIZE] = {0,};

#define LN_CMD_LINE_BUF_SIZE    (256)
static uint8_t sg_ate_cli_cmd_buff[LN_CMD_LINE_BUF_SIZE] = {0,};

typedef struct cmd_tbl_s
{
    char *name;        /* Command Name */
    int maxargs;       /* maximum number of arguments */
    int (*cmd)(struct cmd_tbl_s *, int, const char *);
    char *usage;       /* Usage message	(short) */
} cmd_tbl_t;


static bool is_ate_complete_cmd(char * cmd_str);
static void ln_ty_ate_printf(const char *format, ...);

static int do_ate0(cmd_tbl_t *cmd, int argc, const char *line)
{
    if (at_cmd_echo_en) {
        ln_ty_ate_printf("%s\r\n", line);
    }
    
    at_cmd_echo_en = 0;
    ln_ty_ate_printf("\r\nOK\r\n");
}

static int do_ate1(cmd_tbl_t *cmd, int argc, const char *line)
{
    if (at_cmd_echo_en) {
        ln_ty_ate_printf("%s\r\n", line);
    }
    
    at_cmd_echo_en = 1;
    ln_ty_ate_printf("\r\nOK\r\n");
}

static int do_ate_ok(cmd_tbl_t *cmd, int argc, const char *line)
{
    LOG(LOG_LVL_INFO, "raw data:\r\n%s\r\n", line);
    char *str = NULL;
    char *_line = NULL;

    //AT+ATE_OK=1\r\n
    //AT+ATE_OK?\r\n

    if ((str = strstr(line, "AT+ATE_OK")) > 0) {
        _line = str + strlen("AT+ATE_OK");
    } else {
        return -1;
    }
    
    if (at_cmd_echo_en) {
        ln_ty_ate_printf("%s\r\n", str);
    }
    
    is_ate_complete_cmd(_line);
    return 0;
}

static int do_pvtcmd(cmd_tbl_t *cmd, int argc, const char *line)
{
    LOG(LOG_LVL_INFO, "raw data:\r\n%s\r\n", line);
    char *str = NULL;
    char *_line = NULL;

    //AT+PVTCMD=XTAL_CAP,3\r\n
    //AT+PVTCMD=TX_POWER_GN,-3\r\n

    if ((str = strstr(line, "AT+PVTCMD=")) > 0) {
        _line = str + strlen("AT+PVTCMD=");
    } else {
        return -1;
    }
    
    if (at_cmd_echo_en) {
        ln_ty_ate_printf("%s\r\n", str);
    }
    if(0 != wifi_private_command((char *)_line)) {
        LOG(LOG_LVL_ERROR, "send pvtcmd failed!\r\n");
    }

    ln_ty_ate_printf("\r\nOK\r\n");

    return 0;
}

static int do_reboot(cmd_tbl_t *cmd, int argc, const char *line)
{
    LOG(LOG_LVL_INFO, "raw data:\r\n%s\r\n", line);
    char *str = NULL;

    //AT+RST\r\n
    //AT+REBOOT\r\n

    if (((str = strstr(line, "AT+RST"))    > 0) || \
        ((str = strstr(line, "AT+REBOOT")) > 0))
    {
        if (at_cmd_echo_en) {
            ln_ty_ate_printf("%s\r\n", str);
        }

        ln_ty_ate_printf("OK\r\n");
        OS_MsDelay(300);
        ln_chip_reboot();
        
    } else {
        return -1;
    }

    return 0;
}

static const cmd_tbl_t sg_cmd_list[] = {
    {"ATE0",       1, do_ate0,       "Usage...",},
    {"ATE1",       1, do_ate1,       "Usage...",},
    {"AT+ATE_OK",  1, do_ate_ok,     "Usage...",},
    {"AT+PVTCMD",  1, do_pvtcmd,     "Usage...",},

    {"AT+RST",     1, do_reboot,     "Usage...",},
    {"AT+REBOOT",  1, do_reboot,     "Usage...",},
};

static const cmd_tbl_t *find_cmd (const char *cmd)
{
    const cmd_tbl_t *cmdtp = NULL;
    int i;

    for (i = 0; i < (sizeof(sg_cmd_list) / sizeof(sg_cmd_list[0])); i++) {
        cmdtp = &sg_cmd_list[i];
        if (strncasecmp(cmd, cmdtp->name, strlen(cmdtp->name)) == 0) {
            return cmdtp;   /* full match */
        }
    }
    return NULL;
}

static int ate_cmd_line_parser(const char *line)
{
    int ret = ATE_CMD_FOUND;
    cmd_tbl_t *cmdtp;

    /* Look up command in command table */
    if ((cmdtp = (cmd_tbl_t *)find_cmd(line)) == 0) {
        // LOG(LOG_LVL_ERROR, "Unknown command: %s\r\n", line);
        ret = ATE_CMD_NOT_FOUND;
    } else if (cmdtp->cmd) {
        (cmdtp->cmd)(cmdtp, 0, line);
    }
    return ret;
}

static bool is_ate_complete_cmd(char * cmd_str)
{
    int8_t xtal_cap = 0, tx_power = 0;
    int8_t tx_pwr_b = 0, tx_pwr_gn = 0;
    char *str = cmd_str;
    int8_t val = 0;

    if (strstr(str, "=1") > 0)
    {
        extern int ate_get_xtalcap_txpower_offset (int8_t *xtal_cap, int8_t *tx_power);
        extern int ate_get_xtalcap_calibration_info(void);
        extern int ate_get_txpower_calibration_info(void);

        extern int ate_is_calibrated_txpower_b(void);
        extern int ate_is_calibrated_txpower_gn(void);
        extern int ate_get_txpower_b_gn_offset (int8_t *tx_pwr_b, int8_t *tx_pwr_gn);

        ate_get_xtalcap_txpower_offset(&xtal_cap, &tx_power);
        ate_get_txpower_b_gn_offset(&tx_pwr_b, &tx_pwr_gn);

        /* 1. save to nvds(flash) */
        // 1.1 save XTAL_CAP
        if (ate_get_xtalcap_calibration_info()) {
            ln_nvds_set_xtal_comp_val(xtal_cap);
            LOG(LOG_LVL_INFO, "ATE_OK, xtal cap:%d\r\n", xtal_cap);
        }

        // 1.2 save TX_POWER
        if (ate_get_txpower_calibration_info()) {
            ln_nvds_set_tx_power_comp(tx_power);
            LOG(LOG_LVL_INFO, "ATE_OK, tx power:%d\r\n", tx_power);
        }
        if (ate_is_calibrated_txpower_b()) {
            ln_nvds_set_tx_power_b_comp(tx_pwr_b);
            LOG(LOG_LVL_INFO, "ATE_OK, tx power_b:%d\r\n", tx_pwr_b);
        }
        if (ate_is_calibrated_txpower_gn()) {
            ln_nvds_set_tx_power_gn_comp(tx_pwr_gn);
            LOG(LOG_LVL_INFO, "ATE_OK, tx power_gn:%d\r\n", tx_pwr_gn);
        }

        // 1.3 save ATE result
        // ('S'=ate_successful, 'F'=ate_failed)
        ln_nvds_set_ate_result('S');
    }
    else if (strstr(str, "?") > 0)
    {
        ln_nvds_get_xtal_comp_val((uint8_t *)&val);
        ln_ty_ate_printf("+XTAL_COMP:%d\r\n", val);
        ln_nvds_get_tx_power_comp((uint8_t *)&val);
        ln_ty_ate_printf("+TXPOWER_COMP:%d\r\n", val);
        ln_nvds_get_tx_power_b_comp((uint8_t *)&val);
        ln_ty_ate_printf("+TXPOWER_B_COMP:%d\r\n", val);
        ln_nvds_get_tx_power_gn_comp((uint8_t *)&val);
        ln_ty_ate_printf("+TXPOWER_GN_COMP:%d\r\n", val);
    } else {
        ln_nvds_set_ate_result('F');
    }

    /* 2.Response to instrument: OK\r\n */
    ln_ty_ate_printf("OK\r\n");

    return true;
}

static void ate_cli_thr_entry(void *arg)
{
    int len = 0;
    uint8_t ch;
    uint16_t counter = 0;

    Serial_t *port = (Serial_t *)arg;

    while(1)
    {
        if (OS_OK == OS_SemaphoreWait(&g_uart_rx_sem, OS_WAIT_FOREVER))
        {
            while (!fifo_isempty(&port->rxfifo))
            {
                len = serial_read(port, &ch, 1);

                if ((len > 0) && (isprint(ch) || (ch == '\n') || (ch == '\r')))
                {
                    sg_ate_cli_cmd_buff[counter++] = ch;
                    if (counter >= LN_CMD_LINE_BUF_SIZE) {
                        counter = 0;
                        continue;
                    }

                    if (ch == '\n')
                    {
                        sg_ate_cli_cmd_buff[counter]   = '\0';

                        sg_ate_cli_cmd_buff[counter-1] = '\0'; //rm \n
                        sg_ate_cli_cmd_buff[counter-2] = '\0'; //rm \r

                        if (ATE_CMD_NOT_FOUND == ate_cmd_line_parser((const char *)sg_ate_cli_cmd_buff)) {
                            LOG(LOG_LVL_INFO, "[%s:%d]ate_cli unknow command!\r\n", __func__, __LINE__);
                        }

                        counter = 0;
                        memset(sg_ate_cli_cmd_buff, 0x0, sizeof(sg_ate_cli_cmd_buff));
                    }
                }
            }
        }
    }
}

static size_t ln_at_vprintf(const char *format, va_list args)
{
    uint32_t len = 0;
    int      ret = 0;

    len = vsnprintf(s_at_log_buff, sizeof(s_at_log_buff), format, args);
    ret = serial_write(&g_at_serial, (const void *)s_at_log_buff, len);
    return ret;
}

static void ln_ty_ate_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    ln_at_vprintf(format, args);
    va_end(args);
}

static void ln_serial_rx_callbcak(void) {
    OS_SemaphoreRelease(&g_uart_rx_sem);
}

int ate_init(void)
{
#define ATE_CLI_BAUDRATE       (115200)
#define ATE_CLI_THR_PRI        OS_PRIORITY_NORMAL
#define ATE_CLI_THR_STACK_SIZE (4*512)

    static OS_Thread_t ate_cli_thr = { 0 };

    wlib_pvtcmd_output_cb_set(ln_at_vprintf);

    Serial_t *fd = NULL;
    fd = &g_at_serial;

    /* serial init */
    serial_init(fd, SER_PORT_UART1, ATE_CLI_BAUDRATE, ln_serial_rx_callbcak);
    /* semaphore init */
    if (0 != OS_SemaphoreCreate(&g_uart_rx_sem, 0, 2000)) {
        return -1;
    }
    /* CLI task init */
    if (0 != OS_ThreadCreate(&ate_cli_thr, "ATE_CLI", ate_cli_thr_entry, fd, ATE_CLI_THR_PRI, ATE_CLI_THR_STACK_SIZE)) {
        return -1;
    }

    return 0;
}

bool wifi_module_test_ok_already(void)
{
    uint8_t result = 0; 
    ln_nvds_get_ate_result(&result);

    // ('S'=ate_successful, 'F'=ate_failed, others = not test!!!)
    if ("S" == result) {
        return true;
    } else {
        return false;  
    }
}
